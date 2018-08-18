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

#include "mixerlayoutproperty.h"
#include "data/blocks/connection.h"
#include "data/ipipelineitem.h"

namespace ysm
{
	MixerLayoutProperty::MixerLayoutProperty(const PropertyID id, const QString& name, bool isReadOnly) : Property<MixerLayout, PropertyType::MixerLayout>(id, name, isReadOnly)
	{

	}

	void MixerLayoutProperty::removeConnectionEntries(const Connection* con)
	{
		valueChanging();
		_value.removeConnectionEntries(con);
		valueChanged();
	}

	void MixerLayoutProperty::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		base_type::serialize(xmlElement, ctx);

		_value.serialize(xmlElement, ctx);
	}

	void MixerLayoutProperty::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		if (ctx->isProcessingDeferredElements())
			_value.deserialize(xmlElement, ctx);
		else
			//Must be deserialized after connections but before VAO layout.
			ctx->deferElement(this, *xmlElement, SerializationContext::DeferringPriority::Normal);
	}

	// String representation member specialization

	template<> QString Property<MixerLayout, PropertyType::MixerLayout>::toString() const
	{
		// This property has no string representation
		return "";
	}

	template<> bool Property<MixerLayout, PropertyType::MixerLayout>::fromString(const QString& string)
	{
		// This property has no string representation
		Q_UNUSED(string);
		return true;
	}
}
