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

#ifndef GLSLDOCUMENTLISTPROPERTY_H
#define GLSLDOCUMENTLISTPROPERTY_H

#include "property.h"
#include "data/iblock.h"
#include "../types/glsldocumentlist.h"

namespace ysm
{
	//! \brief Property holding a list of GLSL documents.
    class GLSLDocumentListProperty : public Property<GLSLDocumentList, PropertyType::GLSLDocumentList>
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param id The property id.
		 * \param name The property name.
		 * \param isReadOnly True if readonly.
		 */
		explicit GLSLDocumentListProperty(const PropertyID id = PropertyID::Shader_Code, const QString& name  = "Code", bool isReadOnly = false);

		/*!
		 * \brief Serialize the property.
		 * \param xmlElement The target XML element.
		 * \param ctx The context.
		 */
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;

		/*!
		 * \brief Serialize the property.
		 * \param xmlElement The source XML element.
		 * \param ctx The context.
		 */
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;
	};

	//! \brief To string conversion.
	template<> QString Property<GLSLDocumentList, PropertyType::GLSLDocumentList>::toString() const;

	//! \brief From string conversion.
	template<> bool Property<GLSLDocumentList, PropertyType::GLSLDocumentList>::fromString(const QString& string);

}

#endif // GLSLDOCUMENTLISTPROPERTY_H
