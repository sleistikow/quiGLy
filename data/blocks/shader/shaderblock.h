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

#ifndef SHADERBLOCK_H
#define SHADERBLOCK_H

#include "data/blocks/block.h"
#include "data/properties/property.h"
#include "data/properties/glsldocumentlistproperty.h"

namespace ysm
{

	/**
	 * @brief Programmable block representing the a shader stage
	 */
	class ShaderBlock : public Block
	{
		Q_OBJECT

	public:
		explicit ShaderBlock(Pipeline* parent, BlockType block_type, QString name);

		/**
		 * @brief getShaderCode Gets the shader code
		 */
        GLSLDocumentListProperty *getShaderCode();

		/**
		 * @brief Gets the generic in-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the generic out-port
		 */
		Port* getGenericOutPort();

		/**
		 * @brief gets the in-port, which is connected to textures
		 */
		Port* getTextureInPort();

		/**
		 * @brief gets the in-port, which is connected to uniforms
		 */
		Port* getUniformInPort();

		/**
		 * @brief gets the in-port, which is connected to UBO
		 */
		Port* getUboInPort();
		/**
		 * @brief gets the in-port, which is connected to SSBO
		 */
		Port* getSsboInPort();
		/**
		 * @brief gets the out-port, which is connected to SSBO
		 */
		Port* getSsboOutPort();
		/**
		 * @brief gets the in-port, which is connected to Atomic Counter
		 */
		Port* getAtomicCountersInPort();

	protected:
		void createProperties();
		void createPorts();

    protected:
        GLSLDocumentListProperty* _shaderCode{nullptr};

		Port* _textureInPort{nullptr};
		Port* _uniformInPort{nullptr};
		Port* _uboInPort{nullptr};
		Port* _ssboInPort{nullptr};
		Port* _ssboOutPort{nullptr};
		Port* _atomicCounterInPort{nullptr};
		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
	};
}
#endif // SHADERBLOCK_H
