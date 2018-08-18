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

#include "tessellationprimitivegeneratorblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	TessellationPrimitiveGeneratorBlock::TessellationPrimitiveGeneratorBlock(Pipeline* parent) : Block(parent, block_type, "Tessellation Primitive Generator")
    {

    }

	unsigned int TessellationPrimitiveGeneratorBlock::getMinimumVersion() const
	{
		return 400;
	}

    UIntProperty* TessellationPrimitiveGeneratorBlock::getPatchVertices()
    {
        return _patchVertices;
    }

    FloatDataProperty* TessellationPrimitiveGeneratorBlock::getDefaultInnerLevels()
    {
        return _patchDefaultInnerLevels;
    }

    FloatDataProperty* TessellationPrimitiveGeneratorBlock::getDefaultOuterLevels()
    {
        return _patchDefaultOuterLevels;
    }

    Port* TessellationPrimitiveGeneratorBlock::getGenericInPort()
    {
        return _inPort;
    }

    Port* TessellationPrimitiveGeneratorBlock::getGenericOutPort()
    {
        return _outPort;
    }

    void TessellationPrimitiveGeneratorBlock::createProperties()
    {
        Block::createProperties();

        _patchVertices = _properties->newProperty<UIntProperty>(PropertyID::TessellationPrimitiveGenerator_PatchVertices, "Patch Vertices");
        *_patchVertices = 0;

        _patchDefaultInnerLevels = _properties->newProperty<FloatDataProperty>(PropertyID::TessellationPrimitiveGenerator_PatchDefaultInnerLevel, "Default Patch Inner Levels");
        *_patchDefaultInnerLevels = QVector<float>{};

        _patchDefaultOuterLevels = _properties->newProperty<FloatDataProperty>(PropertyID::TessellationPrimitiveGenerator_PatchDefaultOuterLevel, "Default Patch Outer Levels");
        *_patchDefaultOuterLevels = QVector<float>{};
    }

    void TessellationPrimitiveGeneratorBlock::createPorts()
    {
        Block::createPorts();

        _inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
        _outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
    }

    bool TessellationPrimitiveGeneratorBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Tessellation Primitive Generator output must be connected to a Tessellation Evaluation block";
				return false;
			}

            return true;
        }

        // Nope, we don't like this connection
        return false;
    }
}
