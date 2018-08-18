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

#include "evaluationutils.h"

#include "data/blocks/bufferblock.h"
#include "data/blocks/framebufferobjectblock.h"
#include "data/blocks/texturebaseblock.h"
#include "data/blocks/transformfeedbackblock.h"
#include "data/blocks/shader/shaderblock.h"
#include "data/blocks/textureblock.h"
#include "data/blocks/imageloaderblock.h"

namespace ysm
{

EvaluationUtils::EvaluationUtils()
{
}

QString EvaluationUtils::mapTextureTargetToString(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Target_1D:				return "GL_TEXTURE_1D";
	case TextureBaseBlock::Target_1DArray:			return "GL_TEXTURE_1D_ARRAY";
	case TextureBaseBlock::Target_2D:				return "GL_TEXTURE_2D";
	case TextureBaseBlock::Target_2DArray:			return "GL_TEXTURE_2D_ARRAY";
	case TextureBaseBlock::Target_2DRect:			return "GL_TEXTURE_RECTANGLE";
	case TextureBaseBlock::Target_2DMS:				return "GL_TEXTURE_2D_MULTISAMPLE";
	case TextureBaseBlock::Target_2DMSArray:		return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";
	case TextureBaseBlock::Target_3D:				return "GL_TEXTURE_3D";
	case TextureBaseBlock::Target_CubeMap:			return "GL_TEXTURE_CUBE_MAP";
	case TextureBaseBlock::Target_CubeArray:		return "GL_TEXTURE_CUBE_MAP_ARRAY";
	case TextureBaseBlock::Target_Buffer:			return "GL_TEXTURE_BUFFER";
	case TextureBaseBlock::Target_CubeMapNegX:		return "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
	case TextureBaseBlock::Target_CubeMapPosX:		return "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
	case TextureBaseBlock::Target_CubeMapNegY:		return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
	case TextureBaseBlock::Target_CubeMapPosY:		return "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
	case TextureBaseBlock::Target_CubeMapNegZ:		return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";
	case TextureBaseBlock::Target_CubeMapPosZ:		return "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
	case TextureBaseBlock::Target_Proxy1D:			return "GL_PROXY_TEXTURE_1D";
	case TextureBaseBlock::Target_Proxy1DArray:		return "GL_PROXY_TEXTURE_1D_ARRAY";
	case TextureBaseBlock::Target_Proxy2D:			return "GL_PROXY_TEXTURE_2D";
	case TextureBaseBlock::Target_Proxy2DRect:		return "GL_PROXY_TEXTURE_RECTANGLE";
	case TextureBaseBlock::Target_ProxyCubeMap:		return "GL_PROXY_TEXTURE_CUBE_MAP";
	default:										return ""; // Should not happen
	}
}

QString EvaluationUtils::mapFilterTypeToString(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Filter_Nearest:				return "GL_NEAREST";
	case TextureBaseBlock::Filter_NearestMipmapNearest:	return "GL_NEAREST_MIPMAP_NEAREST";
	case TextureBaseBlock::Filter_NearestMipmapLinear:	return "GL_NEAREST_MIPMAP_LINEAR";
	case TextureBaseBlock::Filter_Linear:				return "GL_LINEAR";
	case TextureBaseBlock::Filter_LinearMipmapNearest:  return "GL_LINEAR_MIPMAP_NEAREST";
	case TextureBaseBlock::Filter_LinearMipmapLinear:	return "GL_LINEAR_MIPMAP_LINEAR";
	default:											return ""; // Should not happen
	}
}

QString EvaluationUtils::mapWrapModeToString(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Wrap_ClampToBorder:	return "GL_CLAMP_TO_BORDER";
	case TextureBaseBlock::Wrap_ClampToEdge:	return "GL_CLAMP_TO_EDGE";
	case TextureBaseBlock::Wrap_Mirror:			return "GL_MIRRORED_REPEAT";
	case TextureBaseBlock::Wrap_Repeat:			return "GL_REPEAT";
	default:									return ""; // Should not happen
	}
}

QString EvaluationUtils::mapUsagePatternToString(int frequency, int access)
{
	switch(frequency)
	{
	case BufferBlock::Usage_Stream:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return "GL_STREAM_DRAW";
		case BufferBlock::Usage_Read: return "GL_STREAM_READ";
		case BufferBlock::Usage_Copy: return "GL_STREAM_COPY";
		}
	case BufferBlock::Usage_Static:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return "GL_STATIC_DRAW";
		case BufferBlock::Usage_Read: return "GL_STATIC_READ";
		case BufferBlock::Usage_Copy: return "GL_STATIC_COPY";
		}
	case BufferBlock::Usage_Dynamic:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return "GL_DYNAMIC_DRAW";
		case BufferBlock::Usage_Read: return "GL_DYNAMIC_READ";
		case BufferBlock::Usage_Copy: return "GL_DYNAMIC_COPY";
		}
	}

	// Should not happen
	return "";
}

QString EvaluationUtils::mapDataTypeToString(GLDataType value)
{
	switch (value)
	{
	case GLDataType::Byte:		return "GL_BYTE";
	case GLDataType::UByte:		return "GL_UNSIGNED_BYTE";
	case GLDataType::Short:		return "GL_SHORT";
	case GLDataType::UShort:	return "GL_UNSIGNED_SHORT";
	case GLDataType::Int:		return "GL_INT";
	case GLDataType::UInt:		return "GL_UNSIGNED_INT";
	case GLDataType::HalfFloat:	return "GL_HALF_FLOAT";
	case GLDataType::Float:		return "GL_FLOAT";
	case GLDataType::Double:	return "GL_DOUBLE";
	case GLDataType::Fixed:		return "GL_FIXED";
	default:					return ""; // Should not happen
	}
}

QString EvaluationUtils::mapAttachmentToString(int value, unsigned int increment)
{
	switch (value)
	{
	case FrameBufferObjectBlock::Attachment_Color0:		return "GL_COLOR_ATTACHMENT0 + " + QString::number(increment);
	case FrameBufferObjectBlock::Attachment_Depth:		return "GL_DEPTH_ATTACHMENT";
	case FrameBufferObjectBlock::Attachment_Stencil:	return "GL_STENCIL_ATTACHMENT";
	default:											return ""; // Should not happen
	}
}

QString EvaluationUtils::mapFrameBufferTargetToString(int value)
{
	switch(value)
	{
	case FrameBufferObjectBlock::Access_Read:	return "GL_READ_FRAMEBUFFER";
	case FrameBufferObjectBlock::Access_Draw:	return "GL_DRAW_FRAMEBUFFER";
	case FrameBufferObjectBlock::Access_Both:	return "GL_FRAMEBUFFER";
	default:									return ""; // Should not happen
	}
}

QString EvaluationUtils::mapInternalFormatToString(int value)
{
	switch(value)
	{
	//base
	case TextureBaseBlock::Format_Red:					return "GL_RED";
	case TextureBaseBlock::Format_RG:					return "GL_RG";
	case TextureBaseBlock::Format_RGB:					return "GL_RGB";
	case TextureBaseBlock::Format_RGBA:					return "GL_RGBA";
	case TextureBaseBlock::Format_DepthStencil:			return "GL_DEPTH_STENCIL";
	case TextureBaseBlock::Format_DepthComponent:		return "GL_DEPTH_COMPONENT";
	//sized
	case TextureBaseBlock::Format_R8:					return "GL_R8";
	case TextureBaseBlock::Format_R8SNorm:				return "GL_R8_SNORM";
	case TextureBaseBlock::Format_R16:					return "GL_R16";
	case TextureBaseBlock::Format_R16SNorm:				return "GL_R16_SNORM";
	case TextureBaseBlock::Format_RG8:					return "GL_RG8";
	case TextureBaseBlock::Format_RG8SNorm:				return "GL_RG8_SNORM";
	case TextureBaseBlock::Format_RG16:					return "GL_RG16";
	case TextureBaseBlock::Format_RG16SNorm:			return "GL_RG16_SNORM";
	case TextureBaseBlock::Format_R3G3B2:				return "GL_R3_G3_B2";
	case TextureBaseBlock::Format_RGB4:					return "GL_RGB4";
	case TextureBaseBlock::Format_RGB5:					return "GL_RGB5";
	case TextureBaseBlock::Format_RGB8:					return "GL_RGB8";
	case TextureBaseBlock::Format_RGB8SNorm:			return "GL_RGB8_SNORM";
	case TextureBaseBlock::Format_RGB10:				return "GL_RGB10";
	case TextureBaseBlock::Format_RGB12:				return "GL_RGB12";
	case TextureBaseBlock::Format_RGB16SNorm:			return "GL_RGB16_SNORM";
	case TextureBaseBlock::Format_RGBA2:				return "GL_RGBA2";
	case TextureBaseBlock::Format_RGBA4:				return "GL_RGBA4";
	case TextureBaseBlock::Format_RGB5A1:				return "GL_RGB5_A1";
	case TextureBaseBlock::Format_RGBA8:				return "GL_RGBA8";
	case TextureBaseBlock::Format_RGBA8SNorm:			return "GL_RGBA8_SNORM";
	case TextureBaseBlock::Format_RGB10A2:				return "GL_RGB10_A2";
	case TextureBaseBlock::Format_RGB10A2UI:			return "GL_RGB10_A2UI";
	case TextureBaseBlock::Format_RGBA12:				return "GL_RGBA12";
	case TextureBaseBlock::Format_RGBA16:				return "GL_RGBA16";
	case TextureBaseBlock::Format_SRGB8:				return "GL_SRGB8";
	case TextureBaseBlock::Format_SRGB8Alpha8:			return "GL_SRGB8_ALPHA8";
	case TextureBaseBlock::Format_R16F:					return "GL_R16F";
	case TextureBaseBlock::Format_RG16F:				return "GL_RG16F";
	case TextureBaseBlock::Format_RGB16F:				return "GL_RGB16F";
	case TextureBaseBlock::Format_RGBA16F:				return "GL_RGBA16F";
	case TextureBaseBlock::Format_R32F:					return "GL_R32F";
	case TextureBaseBlock::Format_RG32F:				return "GL_RG32F";
	case TextureBaseBlock::Format_RGB32F:				return "GL_RGB32F";
	case TextureBaseBlock::Format_RGBA32F:				return "GL_RGBA32F";
	case TextureBaseBlock::Format_R11FG11FB10F:			return "GL_R11F_G11F_B10F";
	case TextureBaseBlock::Format_RGB9E5:				return "GL_RGB9_E5";
	case TextureBaseBlock::Format_R8I:					return "GL_R8I";
	case TextureBaseBlock::Format_R8UI:					return "GL_R8UI";
	case TextureBaseBlock::Format_R16I:					return "GL_R16I";
	case TextureBaseBlock::Format_R16UI:				return "GL_R16UI";
	case TextureBaseBlock::Format_R32I:					return "GL_R32I";
	case TextureBaseBlock::Format_R32UI:				return "GL_R32UI";
	case TextureBaseBlock::Format_RG8I:					return "GL_RG8I";
	case TextureBaseBlock::Format_RG8UI:				return "GL_RG8UI";
	case TextureBaseBlock::Format_RG16I:				return "GL_RG16I";
	case TextureBaseBlock::Format_RG16UI:				return "GL_RG16UI";
	case TextureBaseBlock::Format_RG32I:				return "GL_RG32I";
	case TextureBaseBlock::Format_RG32UI:				return "GL_RG32UI";
	case TextureBaseBlock::Format_RGB8I:				return "GL_RGB8I";
	case TextureBaseBlock::Format_RGB8UI:				return "GL_RGB8UI";
	case TextureBaseBlock::Format_RGB16I:				return "GL_RGB16I";
	case TextureBaseBlock::Format_RGB16UI:				return "GL_RGB16UI";
	case TextureBaseBlock::Format_RGB32I:				return "GL_RGB32I";
	case TextureBaseBlock::Format_RGB32UI:				return "GL_RGB32UI";
	case TextureBaseBlock::Format_RGBA8I:				return "GL_RGBA8I";
	case TextureBaseBlock::Format_RGBA8UI:				return "GL_RGBA8UI";
	case TextureBaseBlock::Format_RGBA16I:				return "GL_RGBA16I";
	case TextureBaseBlock::Format_RGBA16UI:				return "GL_RGBA16UI";
	case TextureBaseBlock::Format_RGBA32I:				return "GL_RGBA32I";
	case TextureBaseBlock::Format_RGBA32UI:				return "GL_RGBA32UI";

	//compressed
	case TextureBaseBlock::Format_CompressedRed:					return "GL_COMPRESSED_RED";
	case TextureBaseBlock::Format_CompressedRedRGTC1:				return "GL_COMPRESSED_RED_RGTC1";
	case TextureBaseBlock::Format_CompressedRG:						return "GL_COMPRESSED_RG";
	case TextureBaseBlock::Format_CompressedRGB:					return "GL_COMPRESSED_RGB";
	case TextureBaseBlock::Format_CompressedRGBA:					return "GL_COMPRESSED_RGBA";
	case TextureBaseBlock::Format_CompressedRGBA_BPTC_UNorm:		return "GL_COMPRESSED_RGBA_BPTC_UNORM";
	case TextureBaseBlock::Format_CompressedRGB_BPTC_Signed_Float:	return "GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT";
	case TextureBaseBlock::Format_CompressedRGB_BPTC_Unsigned_Float:return "GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT";
	case TextureBaseBlock::Format_CompressedRG_RGTC2:				return "GL_COMPRESSED_RG_RGTC2";
	case TextureBaseBlock::Format_CompressedSignedRedRGTC1:			return "GL_COMPRESSED_SIGNED_RED_RGTC1";
	case TextureBaseBlock::Format_CompressedSignedRG_RGTC2:			return "GL_COMPRESSED_SIGNED_RG_RGTC2";
	case TextureBaseBlock::Format_CompressedSRGB:					return "GL_COMPRESSED_SRGB";
	case TextureBaseBlock::Format_CompressedSRGBAlpha:				return "GL_COMPRESSED_SRGB_ALPHA";
	case TextureBaseBlock::Format_CompressedSRGB_Alpha_BPTC_UNorm:	return "GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM";

	//renderbuffer
	case TextureBaseBlock::Format_Depth24Stencil8:		return "GL_DEPTH24_STENCIL8";
	case TextureBaseBlock::Format_Depth32FStencil8:		return "GL_DEPTH32F_STENCIL8";
	case TextureBaseBlock::Format_DepthComponent16:		return "GL_DEPTH_COMPONENT16";
	case TextureBaseBlock::Format_DepthComponent24:		return "GL_DEPTH_COMPONENT24";
	case TextureBaseBlock::Format_DepthComponent32F:	return "GL_DEPTH_COMPONENT32F";
	case TextureBaseBlock::Format_StencilIndex1:		return "GL_STENCIL_INDEX1";
	case TextureBaseBlock::Format_StencilIndex4:		return "GL_STENCIL_INDEX4";
	case TextureBaseBlock::Format_StencilIndex8:		return "GL_STENCIL_INDEX8";
	case TextureBaseBlock::Format_StencilIndex16:		return "GL_STENCIL_INDEX16";
	default:											return ""; // Should not happen
	}
}

QString EvaluationUtils::mapBufferModeToString(int value)
{
	switch(value)
	{
	case TransformFeedbackBlock::BufferMode_Interleaved:	return "GL_INTERLEAVED_ATTRIBS";
	case TransformFeedbackBlock::BufferMode_Separate:		return "GL_SEPARATE_ATTRIBS";
	default:												return ""; // Should not happen
	}
}

QString EvaluationUtils::mapShaderTypeToString(BlockType value)
{
	switch(value)
	{
	case BlockType::Shader_Fragment:				return "GL_FRAGMENT_SHADER";
	case BlockType::Shader_Geometry:				return "GL_GEOMETRY_SHADER";
	case BlockType::Shader_TessellationControl:		return "GL_TESS_CONTROL_SHADER";
	case BlockType::Shader_TessellationEvaluation:	return "GL_TESS_EVALUATION_SHADER";
	case BlockType::Shader_Vertex:					return "GL_VERTEX_SHADER";
	default:										return ""; // Should not happen
	}
}

QString EvaluationUtils::mapFunctionTypeToString(int value)
{
	switch(value)
	{
	case TextureBaseBlock::CompareFunc_Always:		return "GL_ALWAYS";
	case TextureBaseBlock::CompareFunc_Never:		return "GL_NEVER";
	case TextureBaseBlock::CompareFunc_Less:		return "GL_LESS";
	case TextureBaseBlock::CompareFunc_Greater:		return "GL_GREATER";
	case TextureBaseBlock::CompareFunc_Equal:		return "GL_EQUAL";
	case TextureBaseBlock::CompareFunc_NotEqual:	return "GL_NOTEQUAL";
	case TextureBaseBlock::CompareFunc_GEqual:		return "GL_GEQUAL";
	case TextureBaseBlock::CompareFunc_LEqual:		return "GL_LEQUAL";
	default:										return ""; // Should not happen
	}
}

QString EvaluationUtils::mapTextureModeTypeToString(int value)
{
	switch(value)
	{
	case TextureBlock::TextureMode_DepthComponent:	return "GL_DEPTH_COMPONENT";
	case TextureBlock::TextureMode_StencilIndex:	return "GL_STENCIL_INDEX";
	default:										return ""; // Should not happen
	}
}

QString EvaluationUtils::mapSwizzleTypeToString(int value)
{
	switch(value)
	{
	case TextureBlock::Swizzle_Red:		return "GL_RED";
	case TextureBlock::Swizzle_Green:	return "GL_GREEN";
	case TextureBlock::Swizzle_Blue:	return "GL_BLUE";
	case TextureBlock::Swizzle_Alpha:	return "GL_ALPHA";
	case TextureBlock::Swizzle_One:		return "GL_ONE";
	case TextureBlock::Swizzle_Zero:	return "GL_ZERO";
	default:							return ""; // Should not happen
	}
}

QString EvaluationUtils::mapCubeMapSideToString(int value)
{
	switch (value) {
	case TextureBaseBlock::CubeMap_PositiveX:	return "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
	case TextureBaseBlock::CubeMap_PositiveY:	return "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
	case TextureBaseBlock::CubeMap_PositiveZ:	return "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
	case TextureBaseBlock::CubeMap_NegativeX:	return "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
	case TextureBaseBlock::CubeMap_NegativeY:	return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
	case TextureBaseBlock::CubeMap_NegativeZ:	return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";
	default:									return ""; // Should not happen
	}
}


QString EvaluationUtils::mapPixelDataFormatToString(int value)
{
	switch(value)
	{
	case ImageLoaderBlock::PixelFormat_BGR:					return "GL_BGR";
	case ImageLoaderBlock::PixelFormat_BGRA:				return "GL_BGRA";
	case ImageLoaderBlock::PixelFormat_BGRAInteger:			return "GL_BGRA_INTEGER";
	case ImageLoaderBlock::PixelFormat_BGRInteger:			return "GL_BGR_INTEGER";
	case ImageLoaderBlock::PixelFormat_DepthComponent:		return "GL_DEPTH_COMPONENT";
	case ImageLoaderBlock::PixelFormat_DepthStencil:		return "GL_DEPTH_STENCIL";
	case ImageLoaderBlock::PixelFormat_Red:					return "GL_RED";
	case ImageLoaderBlock::PixelFormat_RedInteger:			return "GL_RED_INTEGER";
	case ImageLoaderBlock::PixelFormat_RG:					return "GL_RG";
	case ImageLoaderBlock::PixelFormat_RGB:					return "GL_RGB";
	case ImageLoaderBlock::PixelFormat_RGBA:				return "GL_RGBA";
	case ImageLoaderBlock::PixelFormat_RGBAInteger:			return "GL_RGBA_INTEGER";
	case ImageLoaderBlock::PixelFormat_RGBInteger:			return "GL_RGB_INTEGER";
	case ImageLoaderBlock::PixelFormat_RGInteger:			return "GL_RG_INTEGER";
	case ImageLoaderBlock::PixelFormat_StencilIndex:		return "GL_STENCIL_INDEX";
	default:												return ""; // Should not happen
	}
}

QString EvaluationUtils::mapPixelDataTypeToString(int value)
{
	switch(value)
	{
	case ImageLoaderBlock::PixelType_Byte:						return "GL_BYTE";
	case ImageLoaderBlock::PixelType_Float:						return "GL_FLOAT";
	case ImageLoaderBlock::PixelType_Int:						return "GL_INT";
	case ImageLoaderBlock::PixelType_Short:						return "GL_SHORT";
	case ImageLoaderBlock::PixelType_UnsignedByte:				return "GL_UNSIGNED_BYTE";
	case ImageLoaderBlock::PixelType_UnsignedByte233Rev:		return "GL_UNSIGNED_BYTE_2_3_3_REV";
	case ImageLoaderBlock::PixelType_UnsignedByte332:			return "GL_UNSIGNED_BYTE_3_3_2";
	case ImageLoaderBlock::PixelType_UnsignedInt:				return "GL_UNSIGNED_INT";
	case ImageLoaderBlock::PixelType_UnsignedInt8888:			return "GL_UNSIGNED_INT_8_8_8_8";
	case ImageLoaderBlock::PixelType_UnsignedInt8888Rev:		return "GL_UNSIGNED_INT_8_8_8_8_REV";
	case ImageLoaderBlock::PixelType_UnsignedInt1010102:		return "GL_UNSIGNED_INT_10_10_10_2";
	case ImageLoaderBlock::PixelType_UnsignedInt2101010Rev:		return "GL_UNSIGNED_INT_2_10_10_10_REV";
	case ImageLoaderBlock::PixelType_UnsignedShort:				return "GL_UNSIGNED_SHORT";
	case ImageLoaderBlock::PixelType_UnsignedShort565:			return "GL_UNSIGNED_SHORT_5_6_5";
	case ImageLoaderBlock::PixelType_UnsignedShort565Rev:		return "GL_UNSIGNED_SHORT_5_6_5_REV";
	case ImageLoaderBlock::PixelType_UnsignedShort1555Rev:		return "GL_UNSIGNED_SHORT_1_5_5_5_REV";
	case ImageLoaderBlock::PixelType_UnsignedShort4444:			return "GL_UNSIGNED_SHORT_4_4_4_4";
	case ImageLoaderBlock::PixelType_UnsignedShort4444Rev:		return "GL_UNSIGNED_SHORT_4_4_4_4_REV";
	case ImageLoaderBlock::PixelType_UnsignedShort5551:			return "GL_UNSIGNED_SHORT_5_5_5_1";
	default:													return ""; // Should not happen
	}
}

GLenum EvaluationUtils::mapTextureTargetToOpenGL(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Target_1D:				return GL_TEXTURE_1D;
	case TextureBaseBlock::Target_1DArray:			return GL_TEXTURE_1D_ARRAY;
	case TextureBaseBlock::Target_2D:				return GL_TEXTURE_2D;
	case TextureBaseBlock::Target_2DArray:			return GL_TEXTURE_2D_ARRAY;
	case TextureBaseBlock::Target_2DRect:			return GL_TEXTURE_RECTANGLE;
	case TextureBaseBlock::Target_2DMS:				return GL_TEXTURE_2D_MULTISAMPLE;
	case TextureBaseBlock::Target_2DMSArray:		return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	case TextureBaseBlock::Target_3D:				return GL_TEXTURE_3D;
	case TextureBaseBlock::Target_CubeMap:			return GL_TEXTURE_CUBE_MAP;
	case TextureBaseBlock::Target_CubeArray:		return GL_TEXTURE_CUBE_MAP_ARRAY;
	case TextureBaseBlock::Target_Buffer:			return GL_TEXTURE_BUFFER;
	case TextureBaseBlock::Target_CubeMapNegX:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case TextureBaseBlock::Target_CubeMapPosX:		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case TextureBaseBlock::Target_CubeMapNegY:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case TextureBaseBlock::Target_CubeMapPosY:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case TextureBaseBlock::Target_CubeMapNegZ:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	case TextureBaseBlock::Target_CubeMapPosZ:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case TextureBaseBlock::Target_Proxy1D:			return GL_PROXY_TEXTURE_1D;
	case TextureBaseBlock::Target_Proxy1DArray:		return GL_PROXY_TEXTURE_1D_ARRAY;
	case TextureBaseBlock::Target_Proxy2D:			return GL_PROXY_TEXTURE_2D;
	case TextureBaseBlock::Target_Proxy2DRect:		return GL_PROXY_TEXTURE_RECTANGLE;
	case TextureBaseBlock::Target_ProxyCubeMap:		return GL_PROXY_TEXTURE_CUBE_MAP;
	default:										return 0; // Should not happen
	}
}

GLint EvaluationUtils::mapFilterTypeToOpenGL(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Filter_Nearest:				return GL_NEAREST;
	case TextureBaseBlock::Filter_NearestMipmapNearest:	return GL_NEAREST_MIPMAP_NEAREST;
	case TextureBaseBlock::Filter_NearestMipmapLinear:	return GL_NEAREST_MIPMAP_LINEAR;
	case TextureBaseBlock::Filter_Linear:				return GL_LINEAR;
	case TextureBaseBlock::Filter_LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
	case TextureBaseBlock::Filter_LinearMipmapLinear:	return GL_LINEAR_MIPMAP_LINEAR;
	default:											return 0; // Should not happen
	}
}

GLint EvaluationUtils::mapWrapModeToOpenGL(int value)
{
	switch(value)
	{
	case TextureBaseBlock::Wrap_ClampToBorder:	return GL_CLAMP_TO_BORDER;
	case TextureBaseBlock::Wrap_ClampToEdge:	return GL_CLAMP_TO_EDGE;
	case TextureBaseBlock::Wrap_Mirror:			return GL_MIRRORED_REPEAT;
	case TextureBaseBlock::Wrap_Repeat:			return GL_REPEAT;
	default:									return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapUsagePatternToOpenGL(int frequency, int access)
{
	switch(frequency)
	{
	case BufferBlock::Usage_Stream:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return GL_STREAM_DRAW;
		case BufferBlock::Usage_Read: return GL_STREAM_READ;
		case BufferBlock::Usage_Copy: return GL_STREAM_COPY;
		}
	case BufferBlock::Usage_Static:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return GL_STATIC_DRAW;
		case BufferBlock::Usage_Read: return GL_STATIC_READ;
		case BufferBlock::Usage_Copy: return GL_STATIC_COPY;
		}
	case BufferBlock::Usage_Dynamic:
		switch(access)
		{
		case BufferBlock::Usage_Draw: return GL_DYNAMIC_DRAW;
		case BufferBlock::Usage_Read: return GL_DYNAMIC_READ;
		case BufferBlock::Usage_Copy: return GL_DYNAMIC_COPY;
		}
	}

	// Should not happen
	return 0;
}

GLenum EvaluationUtils::mapDataTypeToOpenGL(GLDataType value)
{
	switch (value)
	{
	case GLDataType::Byte:		return GL_BYTE;
	case GLDataType::UByte:		return GL_UNSIGNED_BYTE;
	case GLDataType::Short:		return GL_SHORT;
	case GLDataType::UShort:	return GL_UNSIGNED_SHORT;
	case GLDataType::Int:		return GL_INT;
	case GLDataType::UInt:		return GL_UNSIGNED_INT;
	case GLDataType::HalfFloat:	return GL_HALF_FLOAT;
	case GLDataType::Float:		return GL_FLOAT;
	case GLDataType::Double:	return GL_DOUBLE;
	case GLDataType::Fixed:		return GL_FIXED;
	default:					return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapAttachmentToOpenGL(int value, unsigned int increment)
{
	switch (value)
	{
	case FrameBufferObjectBlock::Attachment_Color0:		return GL_COLOR_ATTACHMENT0 + increment;
	case FrameBufferObjectBlock::Attachment_Depth:		return GL_DEPTH_ATTACHMENT;
	case FrameBufferObjectBlock::Attachment_Stencil:	return GL_STENCIL_ATTACHMENT;
	default:											return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapFrameBufferTargetToOpenGL(int value)
{
	switch(value)
	{
	case FrameBufferObjectBlock::Access_Read:	return GL_READ_FRAMEBUFFER;
	case FrameBufferObjectBlock::Access_Draw:	return GL_DRAW_FRAMEBUFFER;
	case FrameBufferObjectBlock::Access_Both:	return GL_FRAMEBUFFER;
	default:									return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapInternalFormatToOpenGL(int value)
{
	switch(value)
	{
	//base
	case TextureBaseBlock::Format_Red:					return GL_RED;
	case TextureBaseBlock::Format_RG:					return GL_RG;
	case TextureBaseBlock::Format_RGB:					return GL_RGB;
	case TextureBaseBlock::Format_RGBA:					return GL_RGBA;
	case TextureBaseBlock::Format_DepthStencil:			return GL_DEPTH_STENCIL;
	case TextureBaseBlock::Format_DepthComponent:		return GL_DEPTH_COMPONENT;

	//sized
	case TextureBaseBlock::Format_R8:					return GL_R8;
	case TextureBaseBlock::Format_R8SNorm:				return GL_R8_SNORM;
	case TextureBaseBlock::Format_R16:					return GL_R16;
	case TextureBaseBlock::Format_R16SNorm:				return GL_R16_SNORM;
	case TextureBaseBlock::Format_RG8:					return GL_RG8;
	case TextureBaseBlock::Format_RG8SNorm:				return GL_RG8_SNORM;
	case TextureBaseBlock::Format_RG16:					return GL_RG16;
	case TextureBaseBlock::Format_RG16SNorm:			return GL_RG16_SNORM;
	case TextureBaseBlock::Format_R3G3B2:				return GL_R3_G3_B2;
	case TextureBaseBlock::Format_RGB4:					return GL_RGB4;
	case TextureBaseBlock::Format_RGB5:					return GL_RGB5;
	case TextureBaseBlock::Format_RGB8:					return GL_RGB8;
	case TextureBaseBlock::Format_RGB8SNorm:			return GL_RGB8_SNORM;
	case TextureBaseBlock::Format_RGB10:				return GL_RGB10;
	case TextureBaseBlock::Format_RGB12:				return GL_RGB12;
	case TextureBaseBlock::Format_RGB16SNorm:			return GL_RGB16_SNORM;
	case TextureBaseBlock::Format_RGBA2:				return GL_RGBA2;
	case TextureBaseBlock::Format_RGBA4:				return GL_RGBA4;
	case TextureBaseBlock::Format_RGB5A1:				return GL_RGB5_A1;
	case TextureBaseBlock::Format_RGBA8:				return GL_RGBA8;
	case TextureBaseBlock::Format_RGBA8SNorm:			return GL_RGBA8_SNORM;
	case TextureBaseBlock::Format_RGB10A2:				return GL_RGB10_A2;
	case TextureBaseBlock::Format_RGB10A2UI:			return GL_RGB10_A2UI;
	case TextureBaseBlock::Format_RGBA12:				return GL_RGBA12;
	case TextureBaseBlock::Format_RGBA16:				return GL_RGBA16;
	case TextureBaseBlock::Format_SRGB8:				return GL_SRGB8;
	case TextureBaseBlock::Format_SRGB8Alpha8:			return GL_SRGB8_ALPHA8;
	case TextureBaseBlock::Format_R16F:					return GL_R16F;
	case TextureBaseBlock::Format_RG16F:				return GL_RG16F;
	case TextureBaseBlock::Format_RGB16F:				return GL_RGB16F;
	case TextureBaseBlock::Format_RGBA16F:				return GL_RGBA16F;
	case TextureBaseBlock::Format_R32F:					return GL_R32F;
	case TextureBaseBlock::Format_RG32F:				return GL_RG32F;
	case TextureBaseBlock::Format_RGB32F:				return GL_RGB32F;
	case TextureBaseBlock::Format_RGBA32F:				return GL_RGBA32F;
	case TextureBaseBlock::Format_R11FG11FB10F:			return GL_R11F_G11F_B10F;
	case TextureBaseBlock::Format_RGB9E5:				return GL_RGB9_E5;
	case TextureBaseBlock::Format_R8I:					return GL_R8I;
	case TextureBaseBlock::Format_R8UI:					return GL_R8UI;
	case TextureBaseBlock::Format_R16I:					return GL_R16I;
	case TextureBaseBlock::Format_R16UI:				return GL_R16UI;
	case TextureBaseBlock::Format_R32I:					return GL_R32I;
	case TextureBaseBlock::Format_R32UI:				return GL_R32UI;
	case TextureBaseBlock::Format_RG8I:					return GL_RG8I;
	case TextureBaseBlock::Format_RG8UI:				return GL_RG8UI;
	case TextureBaseBlock::Format_RG16I:				return GL_RG16I;
	case TextureBaseBlock::Format_RG16UI:				return GL_RG16UI;
	case TextureBaseBlock::Format_RG32I:				return GL_RG32I;
	case TextureBaseBlock::Format_RG32UI:				return GL_RG32UI;
	case TextureBaseBlock::Format_RGB8I:				return GL_RGB8I;
	case TextureBaseBlock::Format_RGB8UI:				return GL_RGB8UI;
	case TextureBaseBlock::Format_RGB16I:				return GL_RGB16I;
	case TextureBaseBlock::Format_RGB16UI:				return GL_RGB16UI;
	case TextureBaseBlock::Format_RGB32I:				return GL_RGB32I;
	case TextureBaseBlock::Format_RGB32UI:				return GL_RGB32UI;
	case TextureBaseBlock::Format_RGBA8I:				return GL_RGBA8I;
	case TextureBaseBlock::Format_RGBA8UI:				return GL_RGBA8UI;
	case TextureBaseBlock::Format_RGBA16I:				return GL_RGBA16I;
	case TextureBaseBlock::Format_RGBA16UI:				return GL_RGBA16UI;
	case TextureBaseBlock::Format_RGBA32I:				return GL_RGBA32I;
	case TextureBaseBlock::Format_RGBA32UI:				return GL_RGBA32UI;

	//Compressed
	case TextureBaseBlock::Format_CompressedRed:					return GL_COMPRESSED_RED;
	case TextureBaseBlock::Format_CompressedRedRGTC1:				return GL_COMPRESSED_RED_RGTC1;
	case TextureBaseBlock::Format_CompressedRG:						return GL_COMPRESSED_RG;
	case TextureBaseBlock::Format_CompressedRGB:					return GL_COMPRESSED_RGB;
	case TextureBaseBlock::Format_CompressedRGBA:					return GL_COMPRESSED_RGBA;
	case TextureBaseBlock::Format_CompressedRGBA_BPTC_UNorm:		return GL_COMPRESSED_RGBA_BPTC_UNORM;
	case TextureBaseBlock::Format_CompressedRGB_BPTC_Signed_Float:	return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
	case TextureBaseBlock::Format_CompressedRGB_BPTC_Unsigned_Float:return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
	case TextureBaseBlock::Format_CompressedRG_RGTC2:				return GL_COMPRESSED_RG_RGTC2;
	case TextureBaseBlock::Format_CompressedSignedRedRGTC1:			return GL_COMPRESSED_SIGNED_RED_RGTC1;
	case TextureBaseBlock::Format_CompressedSignedRG_RGTC2:			return GL_COMPRESSED_SIGNED_RG_RGTC2;
	case TextureBaseBlock::Format_CompressedSRGB:					return GL_COMPRESSED_SRGB;
	case TextureBaseBlock::Format_CompressedSRGBAlpha:				return GL_COMPRESSED_SRGB_ALPHA;
	case TextureBaseBlock::Format_CompressedSRGB_Alpha_BPTC_UNorm:	return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;

	//renderbuffer
	case TextureBaseBlock::Format_Depth24Stencil8:		return GL_DEPTH24_STENCIL8;
	case TextureBaseBlock::Format_Depth32FStencil8:		return GL_DEPTH32F_STENCIL8;
	case TextureBaseBlock::Format_DepthComponent16:		return GL_DEPTH_COMPONENT16;
	case TextureBaseBlock::Format_DepthComponent24:		return GL_DEPTH_COMPONENT24;
	case TextureBaseBlock::Format_DepthComponent32F:	return GL_DEPTH_COMPONENT32F;
	case TextureBaseBlock::Format_StencilIndex1:		return GL_STENCIL_INDEX1;
	case TextureBaseBlock::Format_StencilIndex4:		return GL_STENCIL_INDEX4;
	case TextureBaseBlock::Format_StencilIndex8:		return GL_STENCIL_INDEX8;
	case TextureBaseBlock::Format_StencilIndex16:		return GL_STENCIL_INDEX16;
	default:											return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapBufferModeToOpenGL(int value)
{
	switch(value)
	{
	case TransformFeedbackBlock::BufferMode_Interleaved:	return GL_INTERLEAVED_ATTRIBS;
	case TransformFeedbackBlock::BufferMode_Separate:		return GL_SEPARATE_ATTRIBS;
	default:												return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapShaderTypeToOpenGL(BlockType value)
{
	switch(value)
	{
	case BlockType::Shader_Fragment:				return GL_FRAGMENT_SHADER;
	case BlockType::Shader_Geometry:				return GL_GEOMETRY_SHADER;
	case BlockType::Shader_TessellationControl:		return GL_TESS_CONTROL_SHADER;
	case BlockType::Shader_TessellationEvaluation:	return GL_TESS_EVALUATION_SHADER;
	case BlockType::Shader_Vertex:					return GL_VERTEX_SHADER;
	default:										return 0; // Should not happen
	}
}

GLint EvaluationUtils::mapFunctionTypeToOpenGL(int value)
{
	switch(value)
	{
	case TextureBaseBlock::CompareFunc_Always:		return GL_ALWAYS;
	case TextureBaseBlock::CompareFunc_Never:		return GL_NEVER;
	case TextureBaseBlock::CompareFunc_Less:		return GL_LESS;
	case TextureBaseBlock::CompareFunc_Greater:		return GL_GREATER;
	case TextureBaseBlock::CompareFunc_Equal:		return GL_EQUAL;
	case TextureBaseBlock::CompareFunc_NotEqual:	return GL_NOTEQUAL;
	case TextureBaseBlock::CompareFunc_GEqual:		return GL_GEQUAL;
	case TextureBaseBlock::CompareFunc_LEqual:		return GL_LEQUAL;
	default:										return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapTextureModeTypeToOpenGL(int value)
{
	switch(value)
	{
	case TextureBlock::TextureMode_DepthComponent:	return GL_DEPTH_COMPONENT;
	case TextureBlock::TextureMode_StencilIndex:	return GL_STENCIL_INDEX;
	default:										return 0; // Should not happen
	}
}

GLint EvaluationUtils::mapSwizzleTypeToOpenGL(int value)
{
	switch(value)
	{
	case TextureBlock::Swizzle_Red:		return GL_RED;
	case TextureBlock::Swizzle_Green:	return GL_GREEN;
	case TextureBlock::Swizzle_Blue:	return GL_BLUE;
	case TextureBlock::Swizzle_Alpha:	return GL_ALPHA;
	case TextureBlock::Swizzle_One:		return GL_ONE;
	case TextureBlock::Swizzle_Zero:	return GL_ZERO;
	default:							return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapCubeMapSideToOpenGL(int value)
{
	switch (value) {
	case TextureBaseBlock::CubeMap_PositiveX:	return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case TextureBaseBlock::CubeMap_PositiveY:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case TextureBaseBlock::CubeMap_PositiveZ:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case TextureBaseBlock::CubeMap_NegativeX:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case TextureBaseBlock::CubeMap_NegativeY:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case TextureBaseBlock::CubeMap_NegativeZ:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	default:									return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapPixelDataFormatToOpenGL(int value)
{
	switch(value)
	{
	case ImageLoaderBlock::PixelFormat_BGR:					return GL_BGR;
	case ImageLoaderBlock::PixelFormat_BGRA:				return GL_BGRA;
	case ImageLoaderBlock::PixelFormat_BGRAInteger:			return GL_BGRA_INTEGER;
	case ImageLoaderBlock::PixelFormat_BGRInteger:			return GL_BGR_INTEGER;
	case ImageLoaderBlock::PixelFormat_DepthComponent:		return GL_DEPTH_COMPONENT;
	case ImageLoaderBlock::PixelFormat_DepthStencil:		return GL_DEPTH_STENCIL;
	case ImageLoaderBlock::PixelFormat_Red:					return GL_RED;
	case ImageLoaderBlock::PixelFormat_RedInteger:			return GL_RED_INTEGER;
	case ImageLoaderBlock::PixelFormat_RG:					return GL_RG;
	case ImageLoaderBlock::PixelFormat_RGB:					return GL_RGB;
	case ImageLoaderBlock::PixelFormat_RGBA:				return GL_RGBA;
	case ImageLoaderBlock::PixelFormat_RGBAInteger:			return GL_RGBA_INTEGER;
	case ImageLoaderBlock::PixelFormat_RGBInteger:			return GL_RGB_INTEGER;
	case ImageLoaderBlock::PixelFormat_RGInteger:			return GL_RG_INTEGER;
	case ImageLoaderBlock::PixelFormat_StencilIndex:		return GL_STENCIL_INDEX;
	default:												return 0; // Should not happen
	}
}

GLenum EvaluationUtils::mapPixelDataTypeToOpenGL(int value)
{
	switch(value)
	{
	case ImageLoaderBlock::PixelType_Byte:						return GL_BYTE;
	case ImageLoaderBlock::PixelType_Float:						return GL_FLOAT;
	case ImageLoaderBlock::PixelType_Int:						return GL_INT;
	case ImageLoaderBlock::PixelType_Short:						return GL_SHORT;
	case ImageLoaderBlock::PixelType_UnsignedByte:				return GL_UNSIGNED_BYTE;
	case ImageLoaderBlock::PixelType_UnsignedByte233Rev:		return GL_UNSIGNED_BYTE_2_3_3_REV;
	case ImageLoaderBlock::PixelType_UnsignedByte332:			return GL_UNSIGNED_BYTE_3_3_2;
	case ImageLoaderBlock::PixelType_UnsignedInt:				return GL_UNSIGNED_INT;
	case ImageLoaderBlock::PixelType_UnsignedInt8888:			return GL_UNSIGNED_INT_8_8_8_8;
	case ImageLoaderBlock::PixelType_UnsignedInt8888Rev:		return GL_UNSIGNED_INT_8_8_8_8_REV;
	case ImageLoaderBlock::PixelType_UnsignedInt1010102:		return GL_UNSIGNED_INT_10_10_10_2;
	case ImageLoaderBlock::PixelType_UnsignedInt2101010Rev:		return GL_UNSIGNED_INT_2_10_10_10_REV;
	case ImageLoaderBlock::PixelType_UnsignedShort:				return GL_UNSIGNED_SHORT;
	case ImageLoaderBlock::PixelType_UnsignedShort565:			return GL_UNSIGNED_SHORT_5_6_5;
	case ImageLoaderBlock::PixelType_UnsignedShort565Rev:		return GL_UNSIGNED_SHORT_5_6_5_REV;
	case ImageLoaderBlock::PixelType_UnsignedShort1555Rev:		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case ImageLoaderBlock::PixelType_UnsignedShort4444:			return GL_UNSIGNED_SHORT_4_4_4_4;
	case ImageLoaderBlock::PixelType_UnsignedShort4444Rev:		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case ImageLoaderBlock::PixelType_UnsignedShort5551:			return GL_UNSIGNED_SHORT_5_5_5_1;
	default:													return 0; // Should not happen
	}
}

}
