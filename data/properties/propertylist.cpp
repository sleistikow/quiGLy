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

#include "propertylist.h"
#include "property.h"
#include "propertytype.h"
#include "propertyid.h"
#include "data/pipeline/pipelinemanager.h"

namespace ysm
{
	IProperty* PropertyList::addProperty(IProperty* prop)
	{
		if (!prop)
			throw std::invalid_argument{"prop may not be null"};

		if (prop->getType() == PropertyType::Undefined)
			throw std::invalid_argument{"The property type may not be PropertyType::Undefined"};

		if (prop->getID() == PropertyID::None)
			throw std::invalid_argument{"The property type may not be PropertyID::None"};

		IProperty* propEx = findProperty(prop->getID());

		// Return already existing property and delete passed one
		if (propEx)
		{
			delete prop;
			prop = propEx;
		}
		else
			append(prop);

		return prop;
	}

	IProperty* PropertyList::newProperty(const PropertyID id, const PropertyType type, const QString& name, bool isReadOnly)
	{
		if (id == PropertyID::None)
			throw std::invalid_argument{"The property id may not be PropertyID::None"};

		IProperty* prop = findProperty(id);

		// Only create new property if one with given id doesn't exist yet
		if (!prop)
		{
			// PropertyType can not be Undefined, if the property does not exist
			if (type == PropertyType::Undefined)
				throw std::invalid_argument{"The property type may only be PropertyType::Undefined, if the property has been created before"};

			prop = _pipeline->getManager()->getObjectFactory()->createProperty(type, id, name, isReadOnly);

			if (!prop)
				throw std::runtime_error{QString("Failed to create a property of type %1 with ID %2").arg(static_cast<int>(type)).arg(static_cast<int>(id)).toStdString()};

			append(prop);
		}
		else
		{
			// TODO: Why is read only flag adjusted on existing properties?
			// Furthermore, a default parameter is given for isReadOnly, why should it override current state?
			PropertyBase* propBase = dynamic_cast<PropertyBase*>(prop);
			propBase->setReadOnly(isReadOnly);
		}

		return prop;
	}

	IProperty* PropertyList::findProperty(const PropertyID id) const
	{
		if (id == PropertyID::None)
			throw std::invalid_argument{"The property type may not be PropertyID::None"};

		return find([id](const IProperty* prop) { return (prop->getID() == id); });
	}

	void PropertyList::append(const IProperty* prop)
	{
		// Need to access _owner of PropertyBase
		IProperty* propNC = const_cast<IProperty*>(prop);
		PropertyBase* propBase = dynamic_cast<PropertyBase*>(propNC);

		if (propBase)
		{
			propBase->_owner = _owner;
			propBase->_privateOwner = _privateOwner;
		}

		ObjectVector<IProperty>::append(prop);
	}

	void PropertyList::insert(const int i, const IProperty* prop)
	{
		// Need to access _owner of PropertyBase
		IProperty* propNC = const_cast<IProperty*>(prop);
		PropertyBase* propBase = dynamic_cast<PropertyBase*>(propNC);

		if (propBase)
		{
			propBase->_owner = _owner;
			propBase->_privateOwner = _privateOwner;
		}

		ObjectVector<IProperty>::insert(i, prop);
	}

	void PropertyList::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		for (auto it = cbegin(); it != cend(); ++it)
		{
			PropertyBase* propBase = dynamic_cast<PropertyBase*>(*it);

			if (propBase && !propBase->isSerializable())
				continue;

			QDomElement elem = ctx->createElement(_xmlElementName, *it);

			xmlElement->appendChild(elem);
			(*it)->serialize(&elem, ctx);
		}
	}

	void PropertyList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "id"))
				continue;

			PropertyID id = static_cast<PropertyID>(elem.attribute("id").toInt());
			IProperty* prop = newProperty(id);

			try
			{
				prop->deserialize(&elem, ctx);
			}
			catch (...)
			{
				QString msg = QString("Failed to deserialize property #%1 of type %2").arg(static_cast<int>(id));
				ctx->addMessage(msg);
			}
		}
	}
}
