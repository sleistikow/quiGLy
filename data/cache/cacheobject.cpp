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

#include "cacheobject.h"

namespace ysm
{
	CacheObject::CacheObject(CachePool* parent, const Key& key) : _cachePool{parent}, _key{key}
	{

	}

	CacheObject::~CacheObject()
	{
		delete _data;
	}

	const CacheObject::Key&CacheObject::getKey() const
	{
		return _key;
	}

	void CacheObject::attachData(CacheObject::CacheObjectData* data)
	{
		// If the object already contains some data, delete it
		if (_data)
			delete _data;

		_data = data;
	}

	void CacheObject::registerOwner(const ICacheable* owner)
	{
		if (!_owners.contains(owner))
			_owners << owner;
	}

	void CacheObject::unregisterOwner(const ICacheable* owner)
	{
		_owners.removeAll(owner);
	}

	bool CacheObject::isOwner(const ICacheable* owner) const
	{
		return _owners.contains(owner);
	}

	bool CacheObject::isOrphaned() const
	{
		return _owners.isEmpty();
	}
}
