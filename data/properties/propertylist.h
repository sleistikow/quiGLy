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

#ifndef PROPERTYLIST_H
#define PROPERTYLIST_H

#include "data/common/objectvector.h"
#include "data/iproperty.h"

namespace ysm
{
	class IPipelineItem;
	class IPipelineItemPrivate;

	class Pipeline;
	template<typename T> class PipelineItem;

	class PropertyList : public ObjectVector<IProperty>
	{
	public:
		// Construction
		template<typename T>
		explicit PropertyList(Pipeline* pipeline, PipelineItem<T>* owner = nullptr) : _pipeline{pipeline}, _owner{owner}, _privateOwner(owner)
		{
			_xmlElementName = "Property";
		}

	public:
		/**
		 * @brief Adds an existing property to the list
		 * If a property with the same ID already exists, the passed property will be deleted and the existing one
		 * will be returned.
		 */
		IProperty* addProperty(IProperty* prop);

		/**
		 * @brief Creates a new property of class @p T
		 * If no property could be created, an exception is thrown.
		 * @arg T The property class
		 * @return The newly created property
		 */
		template<typename T>
		T* newProperty(const PropertyID id = PropertyID::None, const QString& name = "", bool isReadOnly = false);

		/**
		 * @brief Creates a new property of type @p type and adds it to the list
		 * If no property could be created, an exception is thrown.
		 * @return The newly created property
		 */
		IProperty* newProperty(const PropertyID id, const PropertyType type = PropertyType::Undefined, const QString& name = "", bool isReadOnly = false);

		/**
		 * @brief Finds a property with the given ID
		 * @return The property with ID @p id if one exists
		 */
		IProperty* findProperty(const PropertyID id) const;

	public:
		void append(const IProperty* prop) override;
		void insert(const int i, const IProperty* prop) override;

	public:
		// ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		Pipeline* _pipeline{nullptr};
		IPipelineItem* _owner{nullptr};
		IPipelineItemPrivate* _privateOwner{nullptr};
	};

	// Template member functions

	template<typename T>
	T* PropertyList::newProperty(const PropertyID id, const QString& name, bool isReadOnly)
	{
		static_assert(std::is_base_of<IProperty, T>::value, "T must be derived from IProperty");

		if (id == PropertyID::None)
			throw std::invalid_argument{"id may not be PropertyID::None"};

		IProperty* propEx = findProperty(id);

		if (propEx)
		{
			T* prop = dynamic_cast<T*>(propEx);
			prop->setReadOnly(isReadOnly);

			return dynamic_cast<T*>(propEx);
		}

		T* prop = new T(id, name, isReadOnly);

		if (!prop)
			throw std::runtime_error{"Out of memory"};

		append(prop);
		return prop;
	}
}

#endif
