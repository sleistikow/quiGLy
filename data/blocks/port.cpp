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

#include "port.h"
#include "block.h"
#include "blocklist.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "connectionlist.h"
#include "data/common/dataexceptions.h"
#include "data/common/utils.h"
#include "data/pipeline/visitors/pipelinevisitor.h"

namespace ysm
{
	Port::Port(Pipeline* pipeline, Block* parent, const PortType type, const PortDirection dir, const QString& name) : PipelineItem(pipeline, name), _block{parent}, _connections{new ConnectionList{pipeline, this}}, _type{type}, _direction{dir}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};

		if (type == PortType::Undefined)
			throw std::invalid_argument{"type may not be PortType::Undefined"};
	}

	Port::~Port()
	{
		disconnectFromAll();

		delete _connections;
	}

	ConnectionList* Port::getConnectionList()
	{
		return _connections;
	}

	QVector<Connection*> Port::assembleConnections(const PortDirection dir, const BlockType blockType) const
	{
		QVector<Connection*> cons;

		if (dir == PortDirection::Dry)
			throw std::invalid_argument{"dir may not be PortDirection::Dry"};		

		if ((static_cast<int>(dir) & static_cast<int>(PortDirection::Out)) != 0)
		{
			if (blockType != BlockType::Undefined)
			{
				// Only add connections which target to a block of type blockType
				cons.append(_connections->objects([blockType](Connection* con)
				{
					if (con->getDest() && con->getDest()->getType() == blockType)
						return true;

					return false;
				}
				));
			}
			else
				cons.append(_connections->objects());
		}

		if ((static_cast<int>(dir) & static_cast<int>(PortDirection::In)) != 0)
		{
			// Iterate over all blocks in the pipeline
			for (auto block : *_block->getPipeline()->getBlockList())
			{
				if (block == _block)
					continue;

				// Iterate over all ports of the current block
				for (auto port : *block->getPortList())
				{
					// Iterate over all connections of the current port
					for (auto con : *port->getConnectionList())
					{
						// Connection targets to us
						if (con->getDestPort() == this)
						{
							if (blockType != BlockType::Undefined)
							{
								// Only add connections which stem from a block of type blockType
								if (con->getSource() && con->getSource()->getType() == blockType)
									cons << con;
							}
							else
								cons << con;
						}
					}
				}
			}
		}

		return cons;
	}

	Connection* Port::connectTo(Port* dest)
	{
		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		// Do not connect to self
		if (dest == this)
			throw PortException{this, "Cannot connect a port to itself"};

		// Port must accept connection direction
		if (!isOut() || !dest->isIn())
			throw PortException{this, "The participating ports do not accept the respective connection directions"};

		// Check if connection already exists
		if (_connections->findConnection(dest))
			throw PortException{this, "The connection already exists"};

		QString denialReason;

		if (!_block->canAcceptConnection(this, dest, denialReason))
			throw PortException(this, denialReason);

		return _connections->addConnectionTo(dest);
	}

	void Port::disconnectFrom(Port* dest, bool keepObject)
	{
		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		_connections->removeConnectionTo(dest, keepObject);
	}

	PortDirection Port::getDirection() const
	{
		return _direction;
	}

	void Port::setDirection(const PortDirection dir)
	{
		_direction = dir;
	}

	QVector<IConnection*> Port::getInConnections() const
	{
		return TypeConversion::convertVectorToVector<IConnection*>(assembleConnections(PortDirection::In));
	}

	QVector<IConnection*> Port::getOutConnections() const
	{
		return _connections->objects<IConnection>();
	}

	Block* Port::getBlock()
	{
		return _block;
	}

	PortType Port::getType() const
	{
		return _type;
	}

	bool Port::isIn() const
	{
		return ((static_cast<int>(_direction) & static_cast<int>(PortDirection::In)) != 0);
	}

	bool Port::isOut() const
	{
		return ((static_cast<int>(_direction) & static_cast<int>(PortDirection::Out)) != 0);
	}

	unsigned int Port::getMinimumVersion() const
	{
		return _minimumVersion;
	}

	void Port::setMinimumVersion(const unsigned int minVersion)
	{
		_minimumVersion = minVersion;
	}

	bool Port::isSupported() const
	{
		return PipelineItem::isSupported() && _block->isSupported();
	}

	void Port::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		PipelineItem::serialize(xmlElement, ctx);

		xmlElement->setAttribute("type", static_cast<int>(_type));

		QDomElement elem = ctx->createElement("Connections");

		xmlElement->appendChild(elem);
		_connections->serialize(&elem, ctx);
	}

	void Port::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		PipelineItem::deserialize(xmlElement, ctx);

		QDomElement elem = xmlElement->firstChildElement("Connections");

		if (!elem.isNull())
			_connections->deserialize(&elem, ctx);
	}

	void Port::disconnectFromAll()
	{
		// Remove any connections to or from this port (so no orphaned connections exist when a port is deleted)
		for (Connection* con : *_connections)
			disconnectFrom(con->getDestPort());

		// Get all incoming connections and remove them
		QVector<Connection*> inCons = assembleConnections(PortDirection::In);
		QVector<Port*> inPorts;

		for (Connection* con : inCons)
			inPorts << con->getSourcePort();

		for (Port* port : inPorts)
			port->disconnectFrom(this);
	}

	bool Port::isConnectedTo(const BlockType blockType, const PortType portType, const PortDirection dir) const
	{
		QVector<Connection*> cons = assembleConnections(dir, blockType);

		for (Connection* con : cons)
		{
			// If we are the source port, the target port type must be of portType and vice versa
			if ((con->getSourcePort() == this && con->getDestPort()->getType() == portType) || (con->getDestPort() == this && con->getSourcePort()->getType() == portType))
				return true;
		}

		return false;
	}

	unsigned int Port::getConnectionCount() const
	{
		return assembleConnections(PortDirection::Both).size();
	}

	bool Port::takeVisitor(PipelineVisitor* visitor)
	{
		if (!visitor)
			throw std::invalid_argument{"visitor may not be null"};

		if (!visitor->portVisited(this))
			return false;

		for (Connection* con : *_connections)
		{
			if (!con->takeVisitor(visitor))
				return false;
		}

		return true;
	}

	bool Port::canRename() const
	{
		return false;
	}
}
