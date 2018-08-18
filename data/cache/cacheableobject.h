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

#ifndef CACHEABLEOBJECT_H
#define CACHEABLEOBJECT_H

#include "icacheable.h"
#include "data/pipeline/pipelinemanager.h"

namespace ysm
{
	class CacheableObject : public ICacheable
	{
	public:
		// Construction
		explicit CacheableObject(CachePool* pool);
		explicit CacheableObject(Pipeline* pipeline);
		virtual ~CacheableObject();

	protected:
		/**
		 * @brief Retrieves the cached data for this object
		 */
		template<typename T>
		const T* getCachedData();

		/**
		 * @brief Removes this object from the global cache
		 */
		void removeFromCache();

	private:

		CachePool* _pool{nullptr};
	};

	// Template member functions

	template<typename T>
	const T* CacheableObject::getCachedData()
	{
		// Get the cache object for this data source
		CacheObject* cacheObject = _pool->getCacheObject(this);

		if (cacheObject)
		{
			// Retrieve the actual data
			return cacheObject->getData<T>();
		}

		return nullptr;
	}
}

#endif
