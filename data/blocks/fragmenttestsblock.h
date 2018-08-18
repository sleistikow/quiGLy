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

#ifndef FRAGMENTTESTSBLOCK_H
#define FRAGMENTTESTSBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Fixed function block, to store OpenGl state information regarding fragment tests
	 */
	class FragmentTestsBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::FragmentTests};

		explicit FragmentTestsBlock(Pipeline* parent);

		/**
		 * @brief The different function values for culling and stenciltests
		 */
		enum TestFunc
		{
			TestFunc_Never,
			TestFunc_Less,
			TestFunc_Equal,
			TestFunc_LEqual,
			TestFunc_Greater,
			TestFunc_NotEqual,
			TestFunc_GEqual,
			TestFunc_Always,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getTestFuncNames();

		/**
		 * @brief The blendfunc values
		 */
		enum BlendFunc
		{
			BlendFunc_Zero,
			BlendFunc_One,
			BlendFunc_SourceColor,
			BlendFunc_OneMinusSourceColor,
			BlendFunc_DestinationColor,
			BlendFunc_OneMinusDestinationColor,
			BlendFunc_SourceAlpha,
			BlendFunc_OneMinusSourceAlpha,
			BlendFunc_DestinationAlpha,
			BlendFunc_OneMinusDestinationAlpha,
			BlendFunc_ConstantColor,
			BlendFunc_OneMinusConstantColor,
			BlendFunc_ConstantAlpha,
			BlendFunc_OneMinusConstantAlpha,
			BlendFunc_SourceAlphaSaturate,
			BlendFunc_Source1Color,
			BlendFunc_OneMinusSource1Color,
			BlendFunc_Source1Alpha,
			BlendFunc_OneMinusSource1Alpha,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getBlendFuncNames();

		/**
		 * @brief The blendequation values
		 */
		enum BlendEquation
		{
			BlendEqu_Add,
			BlendEqu_Sub,
			BlendEqu_RevSub,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getBlendEquationNames();

		/**
		 * @brief The Stenciloperation values
		 */
		enum StencilOperation
		{
			StencilOp_Keep,
			StencilOp_Zero,
			StencilOp_Replace,
			StencilOp_Incr,
			StencilOp_IncrWrap,
			StencilOp_Decr,
			StencilOp_DecrWrap,
			StencilOp_Invert,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getStencilOperationNames();
		/**
		 * @brief Gets the generic in-port
		 * @return in-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the generic out-port
		 * @return out-port
		 */
		Port* getGenericOutPort();

		/**
		 * @brief Gets the DepthTestEnabled property
		 */
		BoolProperty* getDepthTestEnabled();
		/**
		 * @brief Gets the DepthMask property
		 */
		BoolProperty* getDepthMask();
		/**
		 * @brief Gets the blendingEnabled
		 */
		BoolProperty* getBlendingEnabled();
		/**
		 * @brief Gets the StencilEnabled
		 */
		BoolProperty* getStencilEnabled();
		/**
		 * @brief Gets the StencilMask
		 */
		UIntProperty* getStencilMask();
		/**
		 * @brief Gets the StencilRef
		 */
		IntProperty* getStencilRef();
		/**
		 * @brief Gets the scissorTestEnabled
		 */
		BoolProperty* getScissorTestEnabled();
		/**
		 * @brief gets the lower-left corner of rectangle for scissortest
		 */
		Vec2Property* getLowerLeftPosition();
		/**
		 * @brief gets the width of rectangle
		 */
		IntProperty* getRectangleWidth();
		/**
		 * @brief gets the height of rectangle
		 */
		IntProperty* getRectangleHeight();

		/**
		 * @brief Gets the colorMaskRed
		 */
		BoolProperty* getColorMaskRed();/**
		 * @brief Gets the colormaskgreen
		 */
		BoolProperty* getColorMaskGreen();/**
		 * @brief Gets the colormaskblue
		 */
		BoolProperty* getColorMaskBlue();
		/**
		 * @brief Gets the colormaskalpha
		 */
		BoolProperty* getColorMaskAlpha();

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() Q_DECL_OVERRIDE;
		void createPorts() Q_DECL_OVERRIDE;

	private:
		BoolProperty* _depthTestEnabled{nullptr};
		EnumProperty* _depthFunc{nullptr};
		BoolProperty* _depthMask{nullptr};
		//Blending properties
		BoolProperty* _blendingEnabled{nullptr};
		EnumProperty* _blendFuncSourceColor{nullptr};
		EnumProperty* _blendFuncSourceAlpha{nullptr};
		EnumProperty* _blendFuncDestinationColor{nullptr};
		EnumProperty* _blendFuncDestinationAlpha{nullptr};
		EnumProperty* _blendEquation{nullptr};
		//Stenciltest properties
		BoolProperty* _stencilEnabled{nullptr};
		EnumProperty* _stencilFuncFront{nullptr};
		EnumProperty* _stencilFuncBack{nullptr};
		EnumProperty* _stencilOpSFailFront{nullptr};
		EnumProperty* _stencilOpDpFailFront{nullptr};
		EnumProperty* _stencilOpDpPassFront{nullptr};
		EnumProperty* _stencilOpSFailBack{nullptr};
		EnumProperty* _stencilOpDpFailBack{nullptr};
		EnumProperty* _stencilOpDpPassBack{nullptr};
		UIntProperty* _stencilMask{nullptr};
		IntProperty* _stencilRef{nullptr};

		//Scissortest properties
		BoolProperty* _scissorTestEnabled{nullptr};
		Vec2Property* _lowerLeftPosition{nullptr};
		IntProperty* _rectangleWidth{nullptr};
		IntProperty* _rectangleHeight{nullptr};

		//Colormask properties
		BoolProperty* _colorMaskRed{nullptr};
		BoolProperty* _colorMaskGreen{nullptr};
		BoolProperty* _colorMaskBlue{nullptr};
		BoolProperty* _colorMaskAlpha{nullptr};

		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
	};
}
#endif // FRAGMENTTESTSBLOCK_H
