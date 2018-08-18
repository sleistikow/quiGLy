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

#include "geometryshaderblock.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"

namespace ysm
{
	GeometryShaderBlock::GeometryShaderBlock(Pipeline* parent) : ShaderBlock(parent, block_type, "Geometry Shader")
	{

	}

    Port* GeometryShaderBlock::getTransformFeedbackOutPort()
	{
		return _transformFeedbackOutPort;
	}

	void GeometryShaderBlock::createPorts()
	{
		ShaderBlock::createPorts();

        _transformFeedbackOutPort = _ports->newPort(PortType::Shader_TransformFeedback, PortDirection::Out, "Transform Feedback Out");
	}

    bool GeometryShaderBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Rasterization, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
                denialReason = "Geometry Shader output must be connected to a Rasterization block";
				return false;
			}

			return true;
        }

        if (src == _transformFeedbackOutPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::TransformFeedback, PortType::GenericIn);

            if (!checkConnectionPoints(dest, conPoints))
            {
                denialReason = "Geometry Shader Transform Feedback output must be connected to a Transform Feedback block";
                return false;
            }

            return true;
        }

		if (src == _ssboOutPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Buffer, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Shader SSBO output must be connected to Buffer Data-In";
				return false;
			}

			return true;
		}


        // Nope, we don't like this connection
        return false;
    }
}
