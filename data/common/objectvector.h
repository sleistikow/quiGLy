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

#ifndef OBJECTVECTOR_H
#define OBJECTVECTOR_H

#include <QVector>
#include <memory>
#include <functional>

#include "data/iserializable.h"
#include "data/common/serializationcontext.h"

namespace ysm
{
	/**
	 * @brief Managed object vector; closely resembles the basic QVector interface
	 * @see QVector
	 */
	template<typename T>
	class ObjectVector : public ISerializable
	{
	public:
		using object_type = T*;
		using vector_type = QVector<object_type>;

	public:
		explicit ObjectVector() { }
		virtual ~ObjectVector() { clear(); }

	public:
		// General attributes
		int size() const;
		bool isEmpty() const;

		// Item access
		T* at(int i);
		const T* at(int i) const;

		int indexOf(const T* t, const int from = 0) const;
		bool contains(const T* t) const;

		/**
		 * @brief Finds the (first) object matching the given predicate @p pred
		 * @arg I The object class to cast the result to
		 * @param pred The predicate to test against
		 */
		template<typename I = T>
		I* find(const std::function<bool(T*)> pred) const;

		/**
		 * @brief Returns a vector of objects cast to type @p I
		 * @arg I The object class to cast the result to
		 * @param pred An optional predicate to test objects against before adding them
		 */
		template<typename I = T>
		QVector<I*> objects(const std::function<bool(T*)> pred = nullptr) const;

		// Item modifications
		virtual void append(const T* t);
		virtual void insert(const int i, const T* t);
		virtual void remove(const int i, bool deleteObj = true);
		virtual bool remove(const T* t, bool deleteObj = true);
		virtual void clear();

	public:
		// Item access
		T& operator[](const int i);
		const T& operator[](const int i) const;

		// Item modifications
		ObjectVector<T>& operator +=(const T& t);
		ObjectVector<T>& operator <<(const T& t);

	public:
		// STL iterators
		using iterator = typename vector_type::iterator;
		using const_iterator = typename vector_type::const_iterator;

		iterator begin();
		const_iterator cbegin() const;
		iterator end();
		const_iterator cend() const;

	public:
		// ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	protected:
		vector_type _objects;

		QString _xmlElementName;
	};

	// Template member functions

	template<typename T>
	int ObjectVector<T>::size() const
	{
		return (int)_objects.size();
	}

	template<typename T>
	bool ObjectVector<T>::isEmpty() const
	{
		return _objects.empty();
	}

	template<typename T>
	T* ObjectVector<T>::at(const int i)
	{
		return _objects[i];
	}

	template<typename T>
	const T* ObjectVector<T>::at(const int i) const
	{
		return _objects[i];
	}

	template<typename T>
	int ObjectVector<T>::indexOf(const T* t, const int from) const
	{
		for (int i = from; i < size(); ++i)
		{
			if (_objects[i] == t)
				return i;
		}

		return -1;
	}

	template<typename T>
	bool ObjectVector<T>::contains(const T* t) const
	{
		return (indexOf(t) != -1);
	}

	template<typename T>
	void ObjectVector<T>::append(const T* t)
	{
		if (!contains(t))
			_objects.append(const_cast<T*>(t));
	}

	template<typename T>
	void ObjectVector<T>::insert(const int i, const T* t)
	{
		if (!contains(t))
			_objects.insert(i, const_cast<T*>(t));
	}

	template<typename T>
	void ObjectVector<T>::remove(const int i, bool deleteObj)
	{
		T* obj = _objects[i];

		_objects.remove(i);

		if (deleteObj)
			delete obj;
	}

	template<typename T>
	bool ObjectVector<T>::remove(const T* t, bool deleteObj)
	{
		int idx = indexOf(t);

		if (idx != -1)
		{
			remove(idx, deleteObj);
			return true;
		}

		return false;
	}

	template<typename T>
	void ObjectVector<T>::clear()
	{
		vector_type objs = _objects;

		while (size())
		{
			T* obj = _objects[0];
			_objects.removeAt(0);

			delete obj;
		}
	}

	template<typename T>
	T& ObjectVector<T>::operator[](const int i)
	{
		return at(i);
	}

	template<typename T>
	const T& ObjectVector<T>::operator[](const int i) const
	{
		return at(i);
	}

	template<typename T>
	ObjectVector<T>& ObjectVector<T>::operator +=(const T& t)
	{
		append(t);
		return *this;
	}

	template<typename T>
	ObjectVector<T>& ObjectVector<T>::operator <<(const T& t)
	{
		append(t);
		return *this;
	}

	template<typename T>
	typename ObjectVector<T>::iterator ObjectVector<T>::begin()
	{
		return _objects.begin();
	}

	template<typename T>
	typename ObjectVector<T>::const_iterator ObjectVector<T>::cbegin() const
	{
		return _objects.cbegin();
	}

	template<typename T>
	typename ObjectVector<T>::iterator ObjectVector<T>::end()
	{
		return _objects.end();
	}

	template<typename T>
	typename ObjectVector<T>::const_iterator ObjectVector<T>::cend() const
	{
		return _objects.cend();
	}

	template<typename T>
	void ObjectVector<T>::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		for (auto it = cbegin(); it != cend(); ++it)
		{
			QDomElement elem = ctx->createElement(_xmlElementName, *it);

			xmlElement->appendChild(elem);
			(*it)->serialize(&elem, ctx);
		}
	}

	template<typename T>
	void ObjectVector<T>::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// No default implementation
		Q_UNUSED(xmlElement);
		Q_UNUSED(ctx);
	}

	template<typename T>
	template<typename I>
	I* ObjectVector<T>::find(const std::function<bool(T*)> pred) const
	{
		for (const object_type& obj : _objects)
		{
			if (pred(obj))
				return obj;
		}

		return nullptr;
	}

	template<typename T>
	template<typename I>
	QVector<I*> ObjectVector<T>::objects(const std::function<bool(T*)> pred) const
	{
		QVector<I*> vec;

		for (const object_type& obj : _objects)
		{
			if (pred && !pred(obj))
				continue;

			vec << obj;
		}

		return vec;
	}
}

#endif
