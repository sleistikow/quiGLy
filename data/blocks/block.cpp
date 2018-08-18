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

#include "block.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "connectionlist.h"
#include "data/pipeline/visitors/pipelinevisitor.h"

namespace ysm
{
	Block::Block(Pipeline* parent, const BlockType type, const QString& name) : PipelineItem(parent, name), _type{type}, _ports{new PortList{parent, this}}
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};
	}

	Block::~Block()
	{
		disableConnectionEvents();

		delete _ports;
	}

	void Block::initBlock()
	{
		PipelineItem::initPipelineItem();

		createPorts();

		connect(this, SIGNAL(blockConnected(Connection*)), this, SLOT(prepareConnection(Connection*)));
	}

	bool Block::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!src)
			throw std::invalid_argument{"src may not be null"};

		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		denialReason = "";

		// The source port must belong to this block
		if (src->getBlock() != this)
		{
			denialReason = "The source port must belong to the current block";
			return false;
		}

		// Do not connect to self
		if (dest->getBlock() == this)
		{
			denialReason = "Cannot connect a block to itself";
			return false;
		}

		// Check port directions
		if (!src->isOut())
		{
			denialReason = "The source port is not an outgoing port";
			return false;
		}

		if (!dest->isIn())
		{
			denialReason = "The target port is not an incoming port";
			return false;
		}

		return true;
	}

	PortList* Block::getPortList()
	{
		return _ports;
	}

	bool Block::takeVisitor(PipelineVisitor* visitor)
	{
		if (!visitor)
			throw std::invalid_argument{"visitor may not be null"};

		if (!visitor->blockVisited(this))
			return false;

		for (Port* port : *_ports)
		{
			if (!port->takeVisitor(visitor))
				return false;
		}

		return true;
	}

	BlockType Block::getType() const
	{
		return _type;
	}

	QVector<IConnection*> Block::getInConnections() const
	{
		QVector<IConnection*> cons;

		// Get all (incoming) connections from all ports
		for (auto it = _ports->cbegin(); it != _ports->cend(); ++it)
			cons.append((*it)->getInConnections());

		return cons;
	}

	QVector<IConnection*> Block::getOutConnections() const
	{
		QVector<IConnection*> cons;

		// Get all (outgoing) connections from all ports
		for (auto it = _ports->cbegin(); it != _ports->cend(); ++it)
			cons.append((*it)->getOutConnections());

		return cons;
	}

	QVector<IPort*> Block::getInPorts() const
	{
		// Get all ports that accept incoming connections
		return _ports->objects<IPort>(
			[](const Port* port)
			{
				return ((static_cast<int>(port->getDirection()) & static_cast<int>(PortDirection::In)) != 0);
			}
		);
	}

	QVector<IPort*> Block::getOutPorts() const
	{
		// Get all ports that accept outgoing connections
		return _ports->objects<IPort>(
			[](const Port* port)
			{
				return ((static_cast<int>(port->getDirection()) & static_cast<int>(PortDirection::Out)) != 0);
			}
		);
	}

	IPort* Block::getPort(PortType type)
	{
		return _ports->findPort(type);
	}

	void Block::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		PipelineItem::serialize(xmlElement, ctx);

		xmlElement->setAttribute("type", static_cast<int>(_type));

		QDomElement elem = ctx->createElement("Ports");

		xmlElement->appendChild(elem);
		_ports->serialize(&elem, ctx);
	}

	void Block::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		PipelineItem::deserialize(xmlElement, ctx);

		QDomElement elem = xmlElement->firstChildElement("Ports");

		if (!elem.isNull())
			_ports->deserialize(&elem, ctx);
	}

	void Block::createPorts()
	{
		_ports->clear();
	}

	bool Block::checkConnectionPoints(IPort* port, const ConnectionPoints& validPoints)
	{
		if (!port)
			throw std::invalid_argument{"port may not be null"};

		for (auto conPoint : validPoints)
		{
			// Does the port belong to a valid block type?
			if (port->getBlock() && port->getBlock()->getType() == conPoint.first)
			{
				// Block type matches, now check the validity of the port type
				if (port->getType() == conPoint.second)
					return true;
			}
		}

		return false;
	}

	void Block::prepareConnection(Connection* con)
	{
		Q_UNUSED(con);
	}

	void Block::disableConnectionEvents()
	{
		// Disable connection signals for connections from/to this block
		auto disableEvents = [](QVector<Connection*> cons, int s){
			for (Connection* con : cons)
			{
				int receivers = con->getConnectionEventsReceivers() & ~s;
				con->setConnectionEventsReceivers(receivers);
			}
		};

		for (Port* port : *_ports)
		{
			disableEvents(port->assembleConnections(PortDirection::Out), Connection::SignalReceiver_Source);
			disableEvents(port->assembleConnections(PortDirection::In), Connection::SignalReceiver_Target);
		}
	}
}
