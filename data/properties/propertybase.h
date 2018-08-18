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

#ifndef PROPERTYBASE_H
#define PROPERTYBASE_H

#include <QObject>

#include "data/iproperty.h"

namespace ysm
{
	class IPipelineItem;
	class IPipelineItemPrivate;

	/**
	 * @brief Base class for properties (never used directly)
	 */
	class PropertyBase : public IProperty
	{
		// PropertyList will set _owner
		friend class PropertyList;

	public: // IProperty
		IPipelineItem* getOwner() const override;

	public: // IVersionable

		unsigned int getVersion() const override;

		unsigned int getMinimumVersion() const override;
		unsigned int getDeprecatedVersion() const override;
		unsigned int getMaximumVersion() const override;

		void setMinimumVersion(const unsigned int minVersion);
		void setDeprecatedVersion(const unsigned int depVersion);
		void setMaximumVersion(const unsigned int maxVersion);

	public:
		// Property flags
		/**
		 * @brief Sets whether the property is read only
		 */
		virtual void setReadOnly(const bool readOnly = true) = 0;

		// Serialization
		/**
		 * @brief Sets whether this property should be serialized
		 */
		void setSerializable(const bool isSerializable);

		/**
		 * @brief Checks whether this property is serializable
		 */
		bool isSerializable() const;				

	protected:
		explicit PropertyBase();

	protected:
		// Always points at the same instance, but prevents reinterpret casts
		IPipelineItem* _owner{nullptr};
		IPipelineItemPrivate* _privateOwner{nullptr};

		unsigned int _minimumVersion{DEFAULT_MINIMUM_VERSION};
		unsigned int _deprecatedVersion{0};
		unsigned int _maximumVersion{0};

		bool _isSerializable{true};
	};
}

#endif
