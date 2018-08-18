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

#ifndef TEXTURESAMPLERBLOCK_H
#define TEXTURESAMPLERBLOCK_H

#include "texturebaseblock.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Texture sampler block
	 */
    class TextureSamplerBlock : public TextureBaseBlock
	{
        Q_OBJECT
	public:
		static const BlockType block_type{BlockType::TextureSampler};

	public:
		// Construction
		explicit TextureSamplerBlock(Pipeline* parent);

    protected:
        void createProperties() override;

    public:
        bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

    private:
        // Properties
        FloatProperty* _anisotropy{nullptr};

	};
}

#endif
