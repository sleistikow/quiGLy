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

#include "transformfeedbackblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "data/properties/propertylist.h"
#include "data/properties/varyingsproperty.h"

namespace ysm
{
	TransformFeedbackBlock::TransformFeedbackBlock(Pipeline *parent) : Block(parent, block_type, "Transform Feedback")
	{

	}

	QMap<int, QString> TransformFeedbackBlock::getBufferModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[BufferMode_Interleaved] = "Interleaved";
		enumNames[BufferMode_Separate] = "Separate";

		return enumNames;
	}

	Port* TransformFeedbackBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* TransformFeedbackBlock::getGenericOutPort()
	{
		return _outPort;
	}


	EnumProperty* TransformFeedbackBlock::getBufferMode()
	{
		return _bufferMode;
	}

	VaryingsProperty* TransformFeedbackBlock::getVaryings()
	{
		return _varyings;
	}

	void TransformFeedbackBlock::createProperties()
	{
		Block::createProperties();

		_bufferMode = _properties->newProperty<EnumProperty>(PropertyID::TransformFeedback_BufferMode, "Buffer Mode");
		*_bufferMode = BufferMode_Interleaved;

		_varyings = _properties->newProperty<VaryingsProperty>(PropertyID::Varyings, "Varyings");
	}

	void TransformFeedbackBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "InPort");

		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "OutPort");
	}

	void TransformFeedbackBlock::prepareConnection(Connection* con)
	{
		// If we're connecting to a Shader, add a texture unit property to the connection
		if (con->getSourcePort()->getType() == PortType::GenericOut && con->getDestPort()->getType() == PortType::Data_In)
		{
			UIntProperty* index = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::TransformFeedback_Index, "Binding Index");
			*index = 0;
		}
	}

    bool TransformFeedbackBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Buffer, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Transform Feedback output must be connected to a Buffer block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }
}
