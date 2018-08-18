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

#include "varyingsproperty.h"

namespace ysm
{
	VaryingsProperty::VaryingsProperty(const PropertyID id, const QString& name, bool isReadOnly) : Property<Varyings, PropertyType::Varyings>(id, name, isReadOnly)
	{

	}

	void VaryingsProperty::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		base_type::serialize(xmlElement, ctx);

		_value.serialize(xmlElement, ctx);
	}

	void VaryingsProperty::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		if (ctx->isProcessingDeferredElements())
			_value.deserialize(xmlElement, ctx);
		else
			ctx->deferElement(this, *xmlElement, SerializationContext::DeferringPriority::Normal);
	}

	template<> QString Property<Varyings, PropertyType::Varyings>::toString() const
	{
		// This property has no string representation
		return "";
	}

	template<> bool Property<Varyings, PropertyType::Varyings>::fromString(const QString& string)
	{
		// This property has no string representation
		Q_UNUSED(string);
		return true;
	}
}
