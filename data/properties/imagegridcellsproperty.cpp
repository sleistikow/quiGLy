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

#include "imagegridcellsproperty.h"

namespace ysm
{
	ImageGridCellsProperty::ImageGridCellsProperty(const PropertyID id, const QString& name, bool isReadOnly) : Property<ImageDataSource::ImageGridCells, PropertyType::ImageGridCells>(id, name, isReadOnly)
	{

	}

	void ImageGridCellsProperty::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		// This property is never serialized
		Q_UNUSED(xmlElement);
		Q_UNUSED(ctx);
	}

	void ImageGridCellsProperty::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// This property is never deserialized
		Q_UNUSED(xmlElement);
		Q_UNUSED(ctx);
	}

	template<> QString Property<ImageDataSource::ImageGridCells, PropertyType::ImageGridCells>::toString() const
	{
		// This property has no string representation
		return "";
	}

	template<> bool Property<ImageDataSource::ImageGridCells, PropertyType::ImageGridCells>::fromString(const QString& string)
	{
		// This property has no string representation
		Q_UNUSED(string);
		return true;
	}
}
