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

#include "vertexpullerblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "bufferblock.h"
#include "data/properties/propertylist.h"

#include <limits>
#include <QtMath>
#include <QOpenGLFunctions>

namespace ysm
{
	VertexPullerBlock::VertexPullerBlock(Pipeline* parent) : Block(parent, block_type, "Vertex Puller")
	{

	}

	UIntProperty* VertexPullerBlock::getRestartIndex()
	{
		return _restartIndex;
	}

	UIntProperty* VertexPullerBlock::getElementCount() const
	{
		return _elementCount;
	}

	Port* VertexPullerBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* VertexPullerBlock::getGenericOutPort()
	{
		return _outPort;
	}

	Port* VertexPullerBlock::getIboPort()
	{
		return _iboPort;
	}

	void VertexPullerBlock::createProperties()
	{
		Block::createProperties();

		// Restart index
		_restartIndex = _properties->newProperty<UIntProperty>(PropertyID::VertexPuller_RestartIndex, "Restart Index");
		*_restartIndex = std::numeric_limits<unsigned int>::max();

		_elementCount = _properties->newProperty<UIntProperty>(PropertyID::VertexPuller_ElementCount, "Element Count", true);
		_elementCount->setSerializable(false);
		_elementCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = calculateElementCount(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return !clear; });
	}

	void VertexPullerBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
		_iboPort = _ports->newPort(PortType::VertexPuller_IndexList, PortDirection::In, "Index List");
	}

	bool VertexPullerBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _outPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Vertex Puller output must be connected to a Vertex Shader block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	unsigned int VertexPullerBlock::calculateElementCount() const
	{
		// IBO connected?
		QVector<Connection*> inCons = _iboPort->assembleConnections(PortDirection::In);

		if (inCons.size() == 1)
		{
			Block* block = inCons[0]->getSource();

			if (block->getType() == BlockType::Buffer)
			{
				// Our IBO must be a buffer
				BufferBlock* buffer = dynamic_cast<BufferBlock*>(block);

				if (!buffer)
					throw BlockException{this, "Thought I had a connection to a buffer, but I was wrong"};

				unsigned int bufSize = *buffer->getBufferSize();
				float elemCount =  qFloor(bufSize / sizeof(GLuint));

				return static_cast<int>(elemCount);
			}
		}

		return 0;
	}
}
