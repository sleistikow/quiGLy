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
#include "data/common/dataexceptions.h"

namespace ysm
{
	ConnectionList::ConnectionList(Pipeline* pipeline, Port* parent) : PipelineItemList(pipeline), _port{parent}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};

		_xmlElementName = "Connection";
	}

	void ConnectionList::addConnection(const Connection* con)
	{
		if (!con)
			throw std::invalid_argument{"con may not be null"};

		append(con);
	}

	Connection* ConnectionList::addConnectionTo(Port* dest)
	{
		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		Connection* con = new Connection{_pipeline, _port, dest};

		if (!con)
			throw std::runtime_error{"Out of memory"};

		append(con);

		// The connection has been established
		con->connected();

		return con;
	}

	void ConnectionList::removeConnectionTo(Port* dest, bool keepObject)
	{
		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		Connection* con = findConnection(dest);

		if (!con)
			throw PortException{_port, "No connection to the given destination port"};

		// The connection has been removed
		con->disconnected();

		remove(con, !keepObject);
	}

	Connection* ConnectionList::findConnection(const Port* dest) const
	{
		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		for (auto it = cbegin(); it != cend(); ++it)
		{
			if ((*it)->getDestPort() == dest)
				return (*it);
		}

		return nullptr;
	}

	void ConnectionList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		if (ctx->isProcessingDeferredElements())
		{
			clear();

			for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
			{
				if (!ctx->assertAttributes(&elem, QStringList() << "destination" << "id"))
					continue;

				unsigned int destPortID = elem.attribute("destination").toUInt();
				PipelineItemID id = elem.attribute("id").toInt();
				Port* destPort = dynamic_cast<Port*>(ctx->findObject(destPortID));

				if (!destPort)
				{
					QString msg = QString("The destination (port #%2) of connection #%1 doesn't exist").arg(id).arg(destPortID);
					ctx->addMessage(msg);
					continue;
				}

				Connection* con = addConnectionTo(destPort);

				id = ctx->setObjectID(con, id);
				con->setID(id);

				// Prevent memory leaks in case of deserialization failure
				try
				{
					con->deserialize(&elem, ctx);
				}
				catch (...)
				{
					QString msg = QString("Failed to deserialize connection #%1").arg(id);
					ctx->addMessage(msg);

					remove(con);
				}
			}
		}
		else
			ctx->deferElement(this, *xmlElement, SerializationContext::DeferringPriority::Highest);
	}
}
