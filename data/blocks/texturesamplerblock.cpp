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

#include "texturesamplerblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

namespace ysm
{
    TextureSamplerBlock::TextureSamplerBlock(Pipeline* parent) : TextureBaseBlock(parent, block_type, "Texture Sampler")
	{

	}

    void TextureSamplerBlock::createProperties()
    {
        TextureBaseBlock::createProperties();
    }

    bool TextureSamplerBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Texture, PortType::Texture_Sampler);
			conPoints << qMakePair(BlockType::TextureView, PortType::Texture_Sampler);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Texture Sampler output must be connected to a Texture or Texture View block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }
}
