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

#ifndef CACHEOBJECT_H
#define CACHEOBJECT_H

#include <QByteArray>
#include <QList>
#include <stdexcept>

namespace ysm
{
	class CachePool;
	class ICacheable;

	/**
	 * @brief Wrapper class for cached objects (managed by a CachePool)
	 */
	class CacheObject
	{
	public:
		using Key = QString;

		/**
		 * @brief Simple base for the actual cached data
		 */
		struct CacheObjectData
		{
			CacheObjectData() { }
			virtual ~CacheObjectData() { }
		};

	public:
		// Construction
		explicit CacheObject(CachePool* parent, const Key& key);
		virtual ~CacheObject();

	public:
		// General attributes
		/**
		 * @brief Retrieves the key of this object
		 */
		const Key& getKey() const;

		// Data handling
		/**
		 * @brief Retrieves the data stored in this object
		 */
		template<typename T>
		const T* getData() const;

		/**
		 * @brief Attaches the provided @p data to this object
		 * Note that the data has to be created on the heap; the data will be owned by this object.
		 */
		void attachData(CacheObjectData* data);

		// Owner handling
		/**
		 * @brief Registers a new owner
		 */
		void registerOwner(const ICacheable* owner);

		/**
		 * @brief Removes an existing owner
		 */
		void unregisterOwner(const ICacheable* owner);

		/**
		 * @brief Checks whether @p owner is an owner of this cache object
		 */
		bool isOwner(const ICacheable* owner) const;

		/**
		 * @brief Checks whether this cache object has at least one owner
		 */
		bool isOrphaned() const;

	private:
		CachePool* _cachePool{nullptr};

		Key _key;
		CacheObjectData* _data{nullptr};

		QList<const ICacheable*> _owners;
	};

	// Template member functions

	template<typename T>
	const T* CacheObject::getData() const
	{
		static_assert(std::is_base_of<CacheObjectData, T>::value, "T must be derived from CacheObjectData");

		T* data = dynamic_cast<T*>(_data);

		if (!data)
			throw std::runtime_error{"Trying to request cached data of the wrong type"};

		return data;
	}
}

#endif
