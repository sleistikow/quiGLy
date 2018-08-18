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

#ifndef IPIPELINEITEM_H
#define IPIPELINEITEM_H

#include <QStringList>
#include <QVector>
#include <stdexcept>

#include "iversionable.h"
#include "iserializable.h"

#include "commands/ichangeable.h"

#include "properties/propertytype.h"
#include "properties/propertyid.h"
#include "pipeline/pipelineitemstatus.h"

namespace ysm
{
	class IPipeline;
	class IProperty;

	using PipelineItemID = unsigned int;

	/// @brief Interface for all pipeline items.
	class IPipelineItem : public IVersionable, public ISerializable, public IChangeable
	{
	public:

		/// @brief Destruct instance.
		virtual ~IPipelineItem() { }

	public:

		/**
		 * @brief Gets the pipeline this item belongs to.
		 * @return The pipeline.
		 */
		virtual IPipeline* getPipeline() const = 0;

		/**
		 * @brief Gets the unique ID of this item.
		 * @return The ID.
		 */
		virtual PipelineItemID getID() const = 0;

		/**
		 * @brief Gets the name of this item.
		 * @return The name.
		 */
		virtual QString getName() const = 0;

		/**
		 * @brief Sets the name of this item.
		 * @param name The name.
		 */
		virtual void setName(const QString& name) = 0;

		/**
		 * @brief Gets the status of this item.
		 * @return The status.
		 */
		virtual PipelineItemStatus getStatus() const = 0;

		/**
		 * @brief Sets the status of this item.
		 * @param status The status.
		 * @param msg The status message.
		 */
		virtual void setStatus(const PipelineItemStatus status, const QString& msg = "") = 0;

		/**
		 * @brief Determines, whether the item is overriding its status.
		 * @return True, if overriding the status.
		 */
		virtual bool isOverridingStatus() const = 0;

	public:

		/**
		 * @brief Gets all properties this item has.
		 * @return List of properties.
		 */
		virtual QVector<IProperty*> getProperties() const = 0;				

		/**
		 * @brief Gets the property with the given ID.
		 * @param id The property's ID.
		 * @return The property.
		 */
		template<typename T> T* getProperty(const PropertyID id) const
		{ return dynamic_cast<T*>(getPropertyHelper(id)); }

		/**
		 * @brief Adds a new property.
		 * If a property with the given ID already exists, this property will be returned.
		 * If the property could not be added, an exception is thrown.
		 * @param id The property's ID.
		 * @param value The property's initial value.
		 * @param name The property's name.
		 * @return The property.
		 */
		template<typename T, typename A>
		T* addProperty(const PropertyID id, const A& value = A{}, const QString& name = "")
		{
			//Validate property id.
			if (id == PropertyID::None)
				throw std::invalid_argument{"Invalid property ID."};

			//Create property, ensure it's valid.
			T* prop = dynamic_cast<T*>(addPropertyHelper(T::property_type, id, name));
			if (!prop)
				throw std::runtime_error{"Property could not be created."};

			//Set property value.
			prop->setValue(value);
			return prop;
		}

		/**
		 * @brief Deletes a property.
		 * If the property could not be deleted, an exception is thrown.
		 * @param prop The property.
		 */
		virtual void deleteProperty(const IProperty* prop) = 0;

		//! @brief Removes all properties from this item.
		virtual void clearProperties() = 0;

		//! @brief Suppress update notifications, before updating multiple properties.
		virtual void beginPropertyUpdate() = 0;

		/**
		 * @brief Reactivate update notifications, after updating multiple properties.
		 * @param applyChanges True, if changes should be applied.
		 */
		virtual void endPropertyUpdate(bool applyChanges = true) = 0;

	protected:

		//! @brief Initialize new instance.
		IPipelineItem() { }

		/**
		 * @brief Helper function for retrieving properties.
		 * @param id The property's ID.
		 * @return The property with the given id (or null).
		 */
		virtual IProperty* getPropertyHelper(const PropertyID id) const = 0;

		/**
		 * @brief Helper function for adding new properties.
		 * If no property could be created, an exception is thrown.
		 * @param type The property's type.
		 * @param id The property's ID.
		 * @param name The property's name.
		 * @return The newly created property.
		 */
		virtual IProperty* addPropertyHelper(const PropertyType type, const PropertyID id, const QString& name) = 0;
	};
}

#endif
