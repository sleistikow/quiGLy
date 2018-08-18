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

#ifndef TEXTUREVIEWBLOCK_H
#define TEXTUREVIEWBLOCK_H


#include "block.h"
#include "texturebaseblock.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Texture block
	 */
	class TextureViewBlock : public TextureBaseBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::TextureView};

	public:
		// Construction
		explicit TextureViewBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the minimum level
		 */
		UIntProperty* getMinimumLevel();

		/**
		 * @brief Gets the level count
		 */
		UIntProperty* getLevelCount();

		/**
		 * @brief Gets the minimum layer
		 */
		UIntProperty* getMinimumLayer();

		/**
		 * @brief Gets the layer count
		 */
		UIntProperty* getLayerCount();

		/**
		 * @brief Gets the internally used format
		 */
		EnumProperty* getInternalFormat();

		/**
		 * @brief Gets the texture target
		 */
		EnumProperty* getTextureTarget();

		// Port access
		/**
		 * @brief Gets the texture in-port
		 */
		Port* getTexturePort();

		/**
		 * @brief Gets the FBO in-port
		 */
		Port* getFrameBufferObjectPort();

		/**
		 * @brief Gets the sampler in-port
		 */
		Port* getSamplerPort();

		/**
		 * @brief Gets the single out-port
		 */
		Port* getGenericOutPort();

	public:
		unsigned int getMinimumVersion() const override;

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	protected slots:
		void prepareConnection(Connection* con) override;

	private:
		// Properties
		UIntProperty* _minLevel{nullptr};
		UIntProperty* _numLevels{nullptr};
		UIntProperty* _minLayer{nullptr};
		UIntProperty* _numLayers{nullptr};
		EnumProperty* _textureTarget{nullptr};
		EnumProperty* _internalFormat{nullptr};

		// Ports
		Port* _texturePort{nullptr};
		Port* _frameBufferObjectInPort{nullptr};
		Port* _samplerPort{nullptr};
		Port* _outPort{nullptr};
	};
}

#endif // TEXTUREVIEWBLOCK_H
