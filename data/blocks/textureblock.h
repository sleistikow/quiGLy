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

#ifndef TEXTUREBLOCK_H
#define TEXTUREBLOCK_H

#include "texturebaseblock.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Texture block
	 */
	class TextureBlock : public TextureBaseBlock
	{
		Q_OBJECT

	public:
		// Types
		/**
		 * @brief The texture Target type
		 */
		enum TextureModeType
		{
			TextureMode_DepthComponent,
			TextureMode_StencilIndex,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getTextureModeTypeNames();

		/**
		 * @brief The SwizzleRGBA Type
		 */
		enum SwizzleType
		{
			Swizzle_Red,
			Swizzle_Green,
			Swizzle_Blue,
			Swizzle_Alpha,
			Swizzle_Zero,
			Swizzle_One,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getSwizzleTypeNames();

		// Property access
		/**
		 * @brief Gets the Mipmapping
		 */
		BoolProperty* getMipmaps();

		/**
		 * @brief Gets the Baselevel
		 */
		IntProperty* getBaseLevel();

		/**
		 * @brief Gets the Baselevel
		 */
		IntProperty* getMaxLevel();

		/**
		 * @brief Determines, whether to use size of the framebuffer
		 */
		BoolProperty* getAutoSize();

	public:
		static const BlockType block_type{BlockType::Texture};

	public:
		// Construction
		explicit TextureBlock(Pipeline* parent);

	public:

		// Port access
		/**
		 * @brief Gets the single in-port
		 */
		Port* getDataInPort();

		/**
		 * @brief Gets the single in-port
		 */
		Port* getSamplerPort();

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	protected slots:
		void prepareConnection(Connection* con) override;

	private slots:
		/**
		 * @brief Called when a Connection has been established
		 */
		void onConnectionEstablished(Connection* con);
		/**
		 * @brief Called when a Connection to or from Textureblock has been removed
		 */
		void onConnectionRemoved(Connection* con);

	private:
		// Properties
		EnumProperty* _target{nullptr};
		BoolProperty* _mipmaps{nullptr};
		UIntProperty* _width{nullptr};
		UIntProperty* _height{nullptr};
		BoolProperty* _autoSize{nullptr};
		EnumProperty* _depthStencilTextureMode{nullptr};
		IntProperty* _baseLevel{nullptr};
		IntProperty* _maxLevel{nullptr};
		EnumProperty* _swizzleR{nullptr};
		EnumProperty* _swizzleG{nullptr};
		EnumProperty* _swizzleB{nullptr};
		EnumProperty* _swizzleA{nullptr};

		// Ports
		Port* _dataInPort{nullptr};
		Port* _samplerPort{nullptr};
	};
}

#endif
