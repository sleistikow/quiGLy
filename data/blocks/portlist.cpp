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

#include "portlist.h"
#include "port.h"

namespace ysm
{
	PortList::PortList(Pipeline* pipeline, Block* parent) : PipelineItemList(pipeline), _block{parent}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};

		_xmlElementName = "Port";
	}

	Port* PortList::addPort(Port* port)
	{
		if (!port)
			throw std::invalid_argument{"port may not be null"};

		if (port->getType() == PortType::Undefined)
			throw std::invalid_argument{"The port type may not be PortType::Undefined"};

		Port* portEx = findPort(port->getType());

		// Return already existing port and delete passed one
		if (portEx)
		{
			delete port;
			port = portEx;
		}
		else
			append(port);

		return port;
	}

	Port* PortList::newPort(const PortType type, const PortDirection dir, const QString& name)
	{
		if (type == PortType::Undefined)
			throw std::invalid_argument{"type may not be PortType::Undefined"};

		Port* port = findPort(type);

		// Only create new port if one of given type doesn't exist yet
		if (!port)
		{
			port = new Port{_pipeline, _block, type, dir, name};

			if (!port)
				throw std::runtime_error{QString("Failed to create a port of type %1").arg(static_cast<int>(type)).toStdString()};

			append(port);
		}

		return port;
	}

	Port* PortList::findPort(const PortType type) const
	{
		if (type == PortType::Undefined)
			throw std::invalid_argument{"type may not be PortType::Undefined"};

		return find([type](const Port* port) { return (port->getType() == type); });
	}

	QVector<Port*> PortList::findPorts(const PortDirection dir) const
	{
		if (dir == PortDirection::Dry)
			throw std::invalid_argument{"dir may not be PortDirection::Dry"};

		// Get all ports that accept connection directions of type dir
		return objects([dir](const Port* port) { return ((static_cast<int>(port->getDirection()) & static_cast<int>(dir)) != 0); });
	}

	void PortList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "type" << "id"))
				continue;

			PortType type = static_cast<PortType>(elem.attribute("type").toInt());
			PipelineItemID id = elem.attribute("id").toInt();
			Port* port = findPort(type);

			if (!port)
			{
				QString msg = QString("A port (#%2) of type %1 is missing in its block").arg(static_cast<int>(type)).arg(id);
				ctx->addMessage(msg);
				continue;
			}

			id = ctx->setObjectID(port, id);
			port->setID(id);

			try
			{
				port->deserialize(&elem, ctx);
			}
			catch (...)
			{
				QString msg = QString("Failed to deserialize port #%1 of type %2").arg(id).arg(static_cast<int>(type));
				ctx->addMessage(msg);
			}
		}
	}
}
