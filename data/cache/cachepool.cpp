/***********************************************************************************
 *                                                                                 *
 * quiGLy - quick GL prototyping                                                   *
 *                                                                                 *
 * Copyright (C) 2015-2018 University of Muenster, Germany.                        *
 * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
 * For a list of authors please refer to the file "CREDITS.txt".                   *
 *                                                                                 *
 * This file is part of the quiGLy software package. quiGLy is free software:      *
 * you can redistribute it and/or modify it under the terms of the GNU General     *
 * Public License version 2 as published by the Free Software Foundation.          *
 *                                                                                 *
 * quiGLy is distributed in the hope that it will be useful, but WITHOUT ANY       *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License in the file   *
 * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                                 *
 * For non-commercial academic use see the license exception specified in the file *
 * "LICENSE-academic.txt". To get information about commercial licensing please    *
 * contact the authors.                                                            *
 *                                                                                 *
 ***********************************************************************************/

#include "cachepool.h"

#include <QVector>

namespace ysm
{
	CachePool::CachePool()
	{

	}

	CachePool::~CachePool()
	{
		clearCache();
	}

	CacheObject* CachePool::getCacheObject(ICacheable* owner)
	{
		if (!owner)
			throw std::invalid_argument{"owner may not be null"};

		CacheObject::Key key = owner->getCacheKey();

		// Check if we need to create a new cache object
		if (!_cacheObjects.contains(key))
		{
			CacheObject* co = new CacheObject{this, key};

			if (!co)
				throw std::runtime_error{"Out of memory"};

			// Get the actual data from the owner
			CacheObject::CacheObjectData* data = owner->createCacheData();

			if (data)
			{
				co->attachData(data);
				_cacheObjects[key] = co;
			}
			else
			{
				// No data could be created, do not cache anything
				delete co;
				return nullptr;
			}
		}

		// At this point, a cached object should always exist
		if (!_cacheObjects.contains(key))
			throw std::runtime_error{"No object could be created in the cache"};

		CacheObject* cacheObject = _cacheObjects[key];

		cacheObject->registerOwner(owner);

		// Remove owner from any other cache objects (an object can only be cached once)
		for (CacheObject* co : _cacheObjects)
		{
			if (co != cacheObject)
				co->unregisterOwner(owner);
		}

		// Clean up the cache
		purgeCache();

		return cacheObject;
	}

	bool CachePool::containsCacheObject(ICacheable* owner) const
	{
		if (!owner)
			throw std::invalid_argument{"owner may not be null"};

		CacheObject::Key key = owner->getCacheKey();
		return _cacheObjects.contains(key);
	}

	void CachePool::unregisterOwnerFromAll(const ICacheable* owner)
	{
		if (!owner)
			throw std::invalid_argument{"owner may not be null"};

		for (CacheObject* co : _cacheObjects)
			co->unregisterOwner(owner);

		// Clean up the cache
		purgeCache();
	}

	void CachePool::purgeCache()
	{
		// Remove any orphaned objects
		QVector<CacheObject::Key> orphanedObjects;

		for (auto it = _cacheObjects.begin(); it != _cacheObjects.end(); ++it)
		{
			CacheObject* co = it.value();

			if (co->isOrphaned())
				orphanedObjects << it.key();
		}

		for (CacheObject::Key key : orphanedObjects)
		{
			CacheObject* co = _cacheObjects[key];

			_cacheObjects.remove(key);
			delete co;
		}
	}

	void CachePool::clearCache()
	{
		qDeleteAll(_cacheObjects);
		_cacheObjects.clear();
	}
}
