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

#ifndef VAOLAYOUTPROPERTY_H
#define VAOLAYOUTPROPERTY_H

#include "property.h"
#include "data/types/vaolayout.h"

namespace ysm
{
	/**
	 * @brief Property to represent the VAO layout (glAttribPointer parameters)
	 */
	class VaoLayoutProperty : public Property<VaoLayout, PropertyType::VaoLayout>
	{
	public:
		explicit VaoLayoutProperty(const PropertyID id = PropertyID::Vao_Layout, const QString& name  = "VAO", bool isReadOnly = false);

		/**
		 * @brief Removes all entries, which use the bufferconnection @p con
		 */
		void removeConnectionEntries(Connection* con);

		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;
	};

	template<> QString Property<VaoLayout, PropertyType::VaoLayout>::toString() const;
	template<> bool Property<VaoLayout, PropertyType::VaoLayout>::fromString(const QString& string);
}
#endif // VAOLAYOUTPROPERTY_H
