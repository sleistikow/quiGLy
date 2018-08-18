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

#ifndef IPROPERTY_H
#define IPROPERTY_H

#include <QString>

#include "iversionable.h"
#include "iserializable.h"

#include "commands/itraceable.h"

#include "properties/propertytype.h"
#include "properties/propertyid.h"

namespace ysm
{
	class IProperty;
	class IPipelineItem;

	/// @brief Base interface for properties.
	class IProperty : public IVersionable, public ISerializable, public ITraceable
	{
	public:

		/// @brief Destruct instance.
		virtual ~IProperty() { }

	public:

		/**
		 * @brief Gets the type of this property.
		 * @return The property's type.
		 */
		virtual PropertyType getType() const = 0;

		/**
		 * @brief Gets the (item-unique) ID of this property.
		 * @return The property's ID.
		 */
		virtual PropertyID getID() const = 0;

		/**
		 * @brief Gets the name of this property.
		 * @return The property's name.
		 */
		virtual QString getName() const = 0;

		/**
		 * @brief Retrieves the owning pipeline item of this property (if any).
		 * @return The pipeline item.
		 */
		virtual IPipelineItem* getOwner() const = 0;

		/**
		 * @brief Copies the value of another property.
		 * @param source The source property.
		 */
		virtual void copyProperty(const IProperty* source) = 0;

		/**
		 * @brief Creates a clone of this property.
		 * @return The new property.
		 */
		virtual IProperty* cloneProperty() const = 0;

		/**
		 * @brief Checks if this is a read-only property.
		 * @return True, if this property is read-only.
		 */
		virtual bool isReadOnly() const = 0;

		/**
		 * @brief Converts the value of this property to a string.
		 * @return String representation of this property.
		 */
		virtual QString toString() const = 0;

		/**
		 * @brief Tries to set this property's value based on the given data string.
		 * @param data The string that holds the data.
		 * @return True, if the property value could be set from the data string.
		 */
		virtual bool fromString(const QString& data) = 0;

		/**
		 * @brief Compares two properties for equality.
		 * @param prop The other property.
		 * @return True, if both are equal.
		 */
		virtual bool isEqual(const IProperty* prop) const = 0;

	protected:

		/// @brief Initialize new instance.
		IProperty() { }
	};
}

#endif
