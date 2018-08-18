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

#include "fragmenttestsblockevaluator.h"
#include "opengl/glconfiguration.h"

#include "data/blocks/fragmenttestsblock.h"

namespace ysm
{

	FragmentTestsBlockEvaluator::FragmentTestsBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> FragmentTestsBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::FragmentTests;
	}

	bool FragmentTestsBlockEvaluator::isRendertimeEvaluated() const
	{
		return true;
	}

	void FragmentTestsBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		initializeDepthTest(block);
		initializeBlending(block);
		initializeStencilTest(block);
		initializeScissorTest(block);
		initializeDepthMask(block);
		initializeColorMask(block);
	}

	void FragmentTestsBlockEvaluator::initializeDepthTest(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool depthTestEnabled = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_DepthTest);
		if(depthTestEnabled)
		{
			f->glEnable(GL_DEPTH_TEST);

			int depthTestFunc = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_DepthFunc);
			switch (depthTestFunc) {
			case FragmentTestsBlock::TestFunc_Always: f->glDepthFunc(GL_ALWAYS);
				break;
			case FragmentTestsBlock::TestFunc_Equal: f->glDepthFunc(GL_EQUAL);
				break;
			case FragmentTestsBlock::TestFunc_GEqual: f->glDepthFunc(GL_GEQUAL);
				break;
			case FragmentTestsBlock::TestFunc_Greater: f->glDepthFunc(GL_GREATER);
				break;
			case FragmentTestsBlock::TestFunc_LEqual: f->glDepthFunc(GL_LEQUAL);
				break;
			case FragmentTestsBlock::TestFunc_Never: f->glDepthFunc(GL_NEVER);
				break;
			case FragmentTestsBlock::TestFunc_NotEqual: f->glDepthFunc(GL_NOTEQUAL);
				break;
			default: f->glDepthFunc(GL_LESS);
				break;
			}
		}
		else
		{
			f->glDisable(GL_DEPTH_TEST);
		}
	}

	void FragmentTestsBlockEvaluator::initializeBlending(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool blendingEnabled = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_Blending);
		if(blendingEnabled)
		{
			f->glEnable(GL_BLEND);

			//set BlendEquation
			int blendEquation = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendEquation);
			switch (blendEquation) {
			case FragmentTestsBlock::BlendEqu_RevSub:
				f->glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
				break;
			case FragmentTestsBlock::BlendEqu_Sub:
				f->glBlendEquation(GL_FUNC_SUBTRACT);
				break;
			default:
				f->glBlendEquation(GL_FUNC_ADD);
				break;
			}

			//lambda to initialize params
			auto switchLambda = [] (const int blendParam) -> int
			{
				switch (blendParam) {
				case FragmentTestsBlock::BlendFunc_ConstantAlpha: return GL_CONSTANT_ALPHA;
				case FragmentTestsBlock::BlendFunc_ConstantColor: return GL_CONSTANT_COLOR;
				case FragmentTestsBlock::BlendFunc_DestinationAlpha: return GL_DST_ALPHA;
				case FragmentTestsBlock::BlendFunc_DestinationColor: return GL_DST_COLOR;
				case FragmentTestsBlock::BlendFunc_Zero: return GL_ZERO;
				case FragmentTestsBlock::BlendFunc_OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
				case FragmentTestsBlock::BlendFunc_OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
				case FragmentTestsBlock::BlendFunc_OneMinusDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
				case FragmentTestsBlock::BlendFunc_OneMinusDestinationColor: return GL_ONE_MINUS_DST_COLOR;
				case FragmentTestsBlock::BlendFunc_OneMinusSource1Alpha: return GL_ONE_MINUS_SRC1_ALPHA;
				case FragmentTestsBlock::BlendFunc_OneMinusSource1Color: return GL_ONE_MINUS_SRC1_COLOR;
				case FragmentTestsBlock::BlendFunc_OneMinusSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
				case FragmentTestsBlock::BlendFunc_OneMinusSourceColor: return GL_ONE_MINUS_SRC_COLOR;
				case FragmentTestsBlock::BlendFunc_Source1Alpha: return GL_SRC1_ALPHA;
				case FragmentTestsBlock::BlendFunc_Source1Color: return GL_SRC1_COLOR;
				case FragmentTestsBlock::BlendFunc_SourceAlpha: return GL_SRC_ALPHA;
				case FragmentTestsBlock::BlendFunc_SourceAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
				case FragmentTestsBlock::BlendFunc_SourceColor: return GL_SRC_COLOR;
				default: return GL_ONE;
				}
			};

			//srcColor
			int blendFunc = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceColor);
			int srcColor = switchLambda(blendFunc);

			//dstColor
			blendFunc = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationColor);
			int dstColor = switchLambda(blendFunc);

			//srcAlpha
			blendFunc = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceAlpha);
			int srcAlpha = switchLambda(blendFunc);

			//dstAlpha
			blendFunc = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationAlpha);
			int dstAlpha = switchLambda(blendFunc);

			//finally call blend function
			f->glBlendFuncSeparate(srcColor, dstColor, srcAlpha, dstAlpha);
		}
		else
		{
			f->glDisable(GL_BLEND);
		}
	}

	void FragmentTestsBlockEvaluator::initializeStencilTest(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool stencilTestEnabled = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_StencilTest);
		if(stencilTestEnabled)
		{
			f->glEnable(GL_STENCIL_TEST);

			//set StencilFuncSeparate
			int stencilFuncFront = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncFront);
			switch (stencilFuncFront) {
			case FragmentTestsBlock::TestFunc_Always: stencilFuncFront = GL_ALWAYS;
					break;
			case FragmentTestsBlock::TestFunc_Equal: stencilFuncFront = GL_EQUAL;
					break;
			case FragmentTestsBlock::TestFunc_GEqual: stencilFuncFront = GL_GEQUAL;
					break;
			case FragmentTestsBlock::TestFunc_Greater: stencilFuncFront = GL_GREATER;
					break;
			case FragmentTestsBlock::TestFunc_LEqual: stencilFuncFront = GL_LEQUAL;
					break;
			case FragmentTestsBlock::TestFunc_Never: stencilFuncFront = GL_NEVER;
					break;
			case FragmentTestsBlock::TestFunc_NotEqual: stencilFuncFront = GL_NOTEQUAL;
					break;
			default: stencilFuncFront = GL_LESS;
					break;
			}

			int stencilFuncBack = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncBack);
			switch (stencilFuncBack) {
			case FragmentTestsBlock::TestFunc_Always: stencilFuncBack = GL_ALWAYS;
					break;
			case FragmentTestsBlock::TestFunc_Equal: stencilFuncBack = GL_EQUAL;
					break;
			case FragmentTestsBlock::TestFunc_GEqual: stencilFuncBack = GL_GEQUAL;
					break;
			case FragmentTestsBlock::TestFunc_Greater: stencilFuncBack = GL_GREATER;
					break;
			case FragmentTestsBlock::TestFunc_LEqual: stencilFuncBack = GL_LEQUAL;
					break;
			case FragmentTestsBlock::TestFunc_Never: stencilFuncBack = GL_NEVER;
					break;
			case FragmentTestsBlock::TestFunc_NotEqual: stencilFuncBack = GL_NOTEQUAL;
					break;
			default: stencilFuncBack = GL_LESS;
					break;
			}
			unsigned int mask = *block->getProperty<UIntProperty>(PropertyID::FragmentTests_StencilMask);
			int ref = *block->getProperty<IntProperty>(PropertyID::FragmentTests_StencilRef);
			if(stencilFuncBack == stencilFuncFront)
				f->glStencilFunc(stencilFuncBack, ref, mask);
			else
			{
				f->glStencilFuncSeparate(GL_FRONT, stencilFuncFront, ref, mask);
				f->glStencilFuncSeparate(GL_BACK, stencilFuncBack, ref, mask);
			}

			//set StencilOpSeparate
			int sfailFront = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailFront);
			int dpfailFront = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailFront);
			int dppassFront = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassFront);
			int sfailBack = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailBack);
			int dpfailBack = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailBack);
			int dppassBack = *block->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassBack);
			auto switchLambda = [] (const int stencilOpParam) -> int
			{
				switch (stencilOpParam) {
				case FragmentTestsBlock::StencilOp_Replace: return GL_REPLACE;
				case FragmentTestsBlock::StencilOp_Zero: return GL_ZERO;
				case FragmentTestsBlock::StencilOp_Incr: return GL_INCR;
				case FragmentTestsBlock::StencilOp_IncrWrap: return GL_INCR_WRAP;
				case FragmentTestsBlock::StencilOp_Decr: return GL_DECR;
				case FragmentTestsBlock::StencilOp_DecrWrap: return GL_DECR_WRAP;
				case FragmentTestsBlock::StencilOp_Invert: return GL_INVERT;
				default: return GL_KEEP;
				}
			};
			f->glStencilOpSeparate(GL_FRONT, switchLambda(sfailFront),switchLambda(dpfailFront), switchLambda(dppassFront));
			f->glStencilOpSeparate(GL_BACK, switchLambda(sfailBack),switchLambda(dpfailBack), switchLambda(dppassBack));
		}
		else
		{
				f->glDisable(GL_STENCIL_TEST);
		}
	}

	void FragmentTestsBlockEvaluator::initializeScissorTest(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool scissorTestEnabled = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_ScissorTest);
		if(scissorTestEnabled)
		{
			f->glEnable(GL_SCISSOR_TEST);
			int recWidth = *block->getProperty<IntProperty>(PropertyID::FragmentTests_RectangleWidth);
			int recHeight = *block->getProperty<IntProperty>(PropertyID::FragmentTests_RectangleHeight);
			QVector2D lowerLeftPosition = *block->getProperty<Vec2Property>(PropertyID::FragmentTests_LowerLeftPosition);
			f->glScissor(lowerLeftPosition.x(), lowerLeftPosition.y(), recWidth, recHeight);
		}
		else
		{
			f->glDisable(GL_SCISSOR_TEST);
		}
	}

	void FragmentTestsBlockEvaluator::initializeDepthMask(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool depthMask = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_DepthMask);
		if(depthMask)
			f->glDepthMask(GL_TRUE);
		else
			f->glDepthMask(GL_FALSE);
	}

	void FragmentTestsBlockEvaluator::initializeColorMask(IBlock* block)
	{
		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		bool red = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskRed);
		bool green = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskGreen);
		bool blue = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskBlue);
		bool alpha = *block->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskAlpha);
		f->glColorMask(red, green, blue, alpha);
	}

}
