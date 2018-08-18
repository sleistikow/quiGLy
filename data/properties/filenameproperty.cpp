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

#include "filenameproperty.h"

namespace ysm
{
	FilenameProperty::FilenameProperty(const PropertyID id, const QString& name, bool isReadOnly) : Property<QString, PropertyType::Filename>(id, name, isReadOnly)
	{

	}

	void FilenameProperty::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		// Serialization needs to be adjusted to support assets.
		// Set default XML parameters.
		xmlElement->setAttribute("type", static_cast<int>(_type));
		xmlElement->setAttribute("id", static_cast<int>(_id));
		xmlElement->setAttribute("name", _name);

		// If file is specified, adjust the path and store it.
		QString filename = getValue();
		if(!filename.isEmpty())
		{
			// Create the XML element.
			QString adjustedPath = ctx->resolveFilename(filename);
			QDomText elementContent = ctx->createTextElement(adjustedPath);

			// Append the child element.
			xmlElement->appendChild(elementContent);
		}
	}

	void FilenameProperty::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// Deserialization needs to be adjusted to support assets.
		// Get default XML parameters.
		_name = xmlElement->attribute("name", _name);

		// Retrieve the path and adjust it.
		QString sourceFile = xmlElement->text();
		if(!sourceFile.isEmpty())
			setValue(ctx->resolveFilename(sourceFile));
	}
}
