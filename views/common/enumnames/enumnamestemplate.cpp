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

#include "enumnamestemplate.h"

#include "bufferblockhelper.h"
#include "datatypehelper.h"

#include "data/blocks/fragmenttestsblock.h"
#include "data/blocks/meshgeneratorblock.h"
#include "data/blocks/rasterizationblock.h"
#include "data/blocks/imageloaderblock.h"
#include "data/blocks/texturebaseblock.h"
#include "data/blocks/textureblock.h"
#include "data/blocks/renderbufferblock.h"
#include "data/blocks/transformfeedbackblock.h"
#include "data/blocks/framebufferobjectblock.h"
#include "data/blocks/codegeneratorblock.h"
#include "data/blocks/arraydatasourceblock.h"

#include "data/rendercommands/drawrendercommand.h"

using namespace ysm;

//Macros that can be used to return the property value names.
#define STATIC(propertyID, propertyName) case PropertyID::propertyID: return propertyName##Helper::getUntypedNames();

EnumPropertyHelper::EnumPropertyHelper() { }

QString EnumPropertyHelper::getName(const EnumProperty* enumProperty)
{
	//Return the current value's name.
	return getNames(enumProperty).value(*enumProperty);
}

QMap<int, QString> EnumPropertyHelper::getNames(const EnumProperty* enumProperty)
{
	//Switch over the property identifier.
	switch (enumProperty->getID())
	{
		STATIC(Buffer_UsageFrequency, BufferUsageFrequency);
		STATIC(Buffer_UsageAccess, BufferUsageAccess);
		STATIC(Data_OutputType, DataType);

	// Fragment Tests
	case PropertyID::FragmentTests_DepthFunc:
	case PropertyID::FragmentTests_StencilFuncFront:
	case PropertyID::FragmentTests_StencilFuncBack:
		return FragmentTestsBlock::getTestFuncNames();
	case PropertyID::FragmentTests_StencilOpSFailFront:
	case PropertyID::FragmentTests_StencilOpDpFailFront:
	case PropertyID::FragmentTests_StencilOpDpPassFront:
	case PropertyID::FragmentTests_StencilOpSFailBack:
	case PropertyID::FragmentTests_StencilOpDpFailBack:
	case PropertyID::FragmentTests_StencilOpDpPassBack:
		return FragmentTestsBlock::getStencilOperationNames();


	case PropertyID::FragmentTests_BlendFuncSourceColor:
	case PropertyID::FragmentTests_BlendFuncSourceAlpha:
	case PropertyID::FragmentTests_BlendFuncDestinationColor:
	case PropertyID::FragmentTests_BlendFuncDestinationAlpha:
		return FragmentTestsBlock::getBlendFuncNames();

	case PropertyID::FragmentTests_BlendEquation:
		return FragmentTestsBlock::getBlendEquationNames();

	// Mesh Generator
	case PropertyID::Mesh_Type:
		return MeshGeneratorBlock::getGeometryTypeNames();

	// Rasterization
	case PropertyID::Rasterization_CullFaceMode:
		return RasterizationBlock::getCullFaceModeNames();

	case PropertyID::Rasterization_PolygonMode:
		return RasterizationBlock::getPolygonModeNames();

	case PropertyID::Rasterization_FrontFace:
		return RasterizationBlock::getFrontFaceNames();

	// Texture
	case PropertyID::Img_PixelDataFormat:
		return ImageLoaderBlock::getPixelDataFormatNames();
	case PropertyID::Img_PixelDataType:
		return ImageLoaderBlock::getPixelDataTypeNames();
	case PropertyID::Texture_TargetType:
		return TextureBaseBlock::getTargetTypeNames();
	case PropertyID::Texture_DepthStencilMode:
		return TextureBlock::getTextureModeTypeNames();
	case PropertyID::Texture_SwizzleRed:
	case PropertyID::Texture_SwizzleGreen:
	case PropertyID::Texture_SwizzleBlue:
	case PropertyID::Texture_SwizzleAlpha:
		return TextureBlock::getSwizzleTypeNames();

	case PropertyID::TextureBase_MinFilter:
		return TextureBaseBlock::getMinificationFilteringTypeNames();
	case PropertyID::TextureBase_MagFilter:
		return TextureBaseBlock::getMagnificationFilteringTypeNames();

	case PropertyID::TextureBase_WrapModeS:
	case PropertyID::TextureBase_WrapModeT:
	case PropertyID::TextureBase_WrapModeR:
		return TextureBaseBlock::getWrapModeNames();

	case PropertyID::TextureBase_InternalFormat:
		return TextureBaseBlock::getInternalFormatNames(enumProperty);
	case PropertyID::RenderBuffer_InternalFormat:
		return RenderBufferBlock::getInternalFormatNames();

	case PropertyID::TextureBase_CompareFunc:
		return TextureBaseBlock::getCompareFuncTypeNames();
	case PropertyID::Sampler_Type:
		return TextureBaseBlock::getSamplerTypeNames();

	// Transform Feedback
	case PropertyID::TransformFeedback_BufferMode:
		return TransformFeedbackBlock::getBufferModeNames();

	// FrameBufferObject
	case PropertyID::FrameBufferObject_Attachment:
		return FrameBufferObjectBlock::getAttachmentTypeNames();
	case PropertyID::FrameBufferObject_CubeMapSide:
		return TextureBaseBlock::getCubeMapSideNames();

	// Code Generator
	case PropertyID::CodeGenerator_Language:
		return CodeGeneratorBlock::getLanguageNames();
	case PropertyID::CodeGenerator_API:
		return CodeGeneratorBlock::getAPINames();

	// Draw Command
	case PropertyID::Draw_DrawMode:
		return DrawRenderCommand::getDrawModeNames();

	case PropertyID::Draw_PrimitiveMode:
		return DrawRenderCommand::getPrimitiveModeNames();


	case PropertyID::Array_DataType:
		return ArrayDataSourceBlock::getDataTypeNames();

	default:
		return QMap<int, QString>();
	}
}
