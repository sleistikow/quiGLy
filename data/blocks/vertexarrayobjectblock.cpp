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

#include "vertexarrayobjectblock.h"
#include "port.h"
#include "portlist.h"
#include "data/properties/propertylist.h"
#include "data/properties/vaolayoutproperty.h"

namespace ysm
{
	VertexArrayObjectBlock::VertexArrayObjectBlock(Pipeline *parent) : Block(parent, block_type, "Vertex Array Object")
	{

	}

	void VertexArrayObjectBlock::initBlock()
	{
		Block::initBlock();

		//Connect to self to receive disconnection events
		connect(this, SIGNAL(blockDisconnected(Connection*)), this, SLOT(onConnectionRemoved(Connection*)));
	}

	VaoLayoutProperty* VertexArrayObjectBlock::getVaoLayout()
	{
		return _vaoLayout;
	}	

	void VertexArrayObjectBlock::createProperties()
	{
		Block::createProperties();

		_vaoLayout = _properties->newProperty<VaoLayoutProperty>(PropertyID::Vao_Layout, "Layout");
	}

	void VertexArrayObjectBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::Data_In, PortDirection::In, "Data In");
		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

    bool VertexArrayObjectBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::VertexPuller, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "VAO output must be connected to a Vertex Puller block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }

	void VertexArrayObjectBlock::onConnectionRemoved(Connection* con)
	{
		_vaoLayout->removeConnectionEntries(con);
	}

	Port* VertexArrayObjectBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* VertexArrayObjectBlock::getGenericOutPort()
	{
		return _outPort;
	}
}
