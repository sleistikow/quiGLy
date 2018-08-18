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

#ifndef CACHEPOOL_H
#define CACHEPOOL_H

#include "cacheobject.h"
#include "icacheable.h"

#include <QMap>

namespace ysm
{
	/**
	 * @brief A cache pool (contains and manages cached objects)
	 */
	class CachePool
	{
	public:
		// Construction
		explicit CachePool();
		virtual ~CachePool();

	public:
		/**
		 * @brief Retrieves the cached object for @p owner
		 * If no such object exists, a new one is created and data is requested from @p owner.
		 * If the key of @p owner has changed, it will also be removed from any previous cached objects as the owner.
		 */
		CacheObject* getCacheObject(ICacheable* owner);

		/**
		 * @brief Checks if a cache object has already been created for the given @p owner
		 */
		bool containsCacheObject(ICacheable* owner) const;

		/**
		 * @brief Removes the provided @p owner from all cache objects
		 */
		void unregisterOwnerFromAll(const ICacheable* owner);

		/**
		 * @brief Purges the cache (removes orphaned objects etc.)
		 */
		void purgeCache();

		/**
		 * @brief Clears the entire cache
		 */
		void clearCache();

	private:
		QMap<CacheObject::Key, CacheObject*> _cacheObjects;
	};
}

#endif
