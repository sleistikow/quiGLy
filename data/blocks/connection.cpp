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

#include "connection.h"
#include "connectionlist.h"
#include "port.h"
#include "block.h"
#include "data/pipeline/visitors/pipelinevisitor.h"

namespace ysm
{
	Connection::Connection(Pipeline* parent, Port* src, Port* dest) : PipelineItem(parent), _sourcePort{src}, _destPort{dest}
	{
		if (!src)
			throw std::invalid_argument{"src may not be null"};

		if (!dest)
			throw std::invalid_argument{"dest may not be null"};
	}

	void Connection::connected()
	{
		// Signal both involved blocks about the new connection
		if (_connectionSignalReceivers & SignalReceiver_Source)
			emit _sourcePort->getBlock()->blockConnected(this);

		if (_connectionSignalReceivers & SignalReceiver_Target)
			emit _destPort->getBlock()->blockConnected(this);
	}

	void Connection::disconnected()
	{
		// Signal both involved blocks about the removed connection
		if (_connectionSignalReceivers & SignalReceiver_Source)
			emit _sourcePort->getBlock()->blockDisconnected(this);

		if (_connectionSignalReceivers & SignalReceiver_Target)
			emit _destPort->getBlock()->blockDisconnected(this);
	}

	int Connection::getConnectionEventsReceivers() const
	{
		return _connectionSignalReceivers;
	}

	void Connection::setConnectionEventsReceivers(int receivers)
	{
		_connectionSignalReceivers = receivers;
	}

	bool Connection::takeVisitor(PipelineVisitor* visitor)
	{
		if (!visitor)
			throw std::invalid_argument{"visitor may not be null"};

		return visitor->connectionVisited(this);
	}

	Port* Connection::getSourcePort()
	{
		return _sourcePort;
	}

	Port* Connection::getDestPort()
	{
		return _destPort;
	}

	Block* Connection::getSource()
	{
		if (_sourcePort)
			return _sourcePort->getBlock();

		return nullptr;
	}

	Block* Connection::getDest()
	{
		if (_destPort)
			return _destPort->getBlock();

		return nullptr;
	}

	void Connection::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		PipelineItem::serialize(xmlElement, ctx);

		if (_destPort)
			xmlElement->setAttribute("destination", ctx->getObjectID(_destPort));
	}

	void Connection::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		PipelineItem::deserialize(xmlElement, ctx);
	}

	bool Connection::canRename() const
	{
		return false;
	}
}
