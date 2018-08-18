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

#include "texturebaseblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

#include <QOpenGLFunctions_4_2_Core>


namespace ysm
{
	TextureBaseBlock::TextureBaseBlock(Pipeline* parent, BlockType type, const QString& name) : Block(parent, type, name)
    {

    }
	QMap<int, QString> TextureBaseBlock::getTargetTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Target_1D]				= "1D";
		enumNames[Target_1DArray]			= "1D Array";
		enumNames[Target_2D]				= "2D";
		enumNames[Target_2DArray]			= "2D Array";
		enumNames[Target_2DRect]			= "2D Rectangle";
		enumNames[Target_2DMS]				= "2D Multi-Sampling";
		enumNames[Target_2DMSArray]			= "2D Multi-Sampling Array";
		enumNames[Target_3D]				= "3D";
		enumNames[Target_CubeMap]			= "CubeMap";
		enumNames[Target_CubeArray]			= "CubeArray";
		enumNames[Target_Buffer]			= "Buffer";
		enumNames[Target_CubeMapNegX]		= "Cubemap Negative X";
		enumNames[Target_CubeMapPosX]		= "Cubemap Positive X";
		enumNames[Target_CubeMapNegY]		= "Cubemap Negative Y";
		enumNames[Target_CubeMapPosY]		= "Cubemap Positive Y";
		enumNames[Target_CubeMapNegZ]		= "Cubemap Negative Z";
		enumNames[Target_CubeMapPosZ]		= "Cubemap Positive Z";
		enumNames[Target_Proxy1D]			= "Proxy Texture 1D";
		enumNames[Target_Proxy1DArray]		= "Proxy Texture 1D Array";
		enumNames[Target_Proxy2D]			= "Proxy Texture 2D";
		enumNames[Target_Proxy2DArray]		= "Proxy Texture 2D Array";
		enumNames[Target_Proxy2DMS]			= "Proxy Texture 2D Multisample";
		enumNames[Target_Proxy2DMSArray]	= "Proxy Texture 2D Multisample Array";
		enumNames[Target_Proxy2DRect]		= "Proxy Texture Rectangle";
		enumNames[Target_Proxy3D]			= "Proxy Texture 3D";
		enumNames[Target_ProxyCubeMap]		= "Proxy Texture Cubemap";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getSamplerTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Sampler_Sampler1D]				= "sampler1D";
		enumNames[Sampler_Sampler2D]				= "sampler2D";
		enumNames[Sampler_Sampler3D]				= "sampler3D";
		enumNames[Sampler_SamplerCube]				= "samplerCube";
		enumNames[Sampler_Sampler2DRect]			= "sampler2DRect";
		enumNames[Sampler_Sampler1DArray]			= "sampler1DArray";
		enumNames[Sampler_Sampler2DArray]			= "sampler2DArray";
		enumNames[Sampler_SamplerCubeArray]			= "samplerCubeArray";
		enumNames[Sampler_SamplerBuffer]			= "samplerBuffer";
		enumNames[Sampler_Sampler2DMS]				= "sampler2DMS";
		enumNames[Sampler_Sampler2DMSArray]			= "sampler2DMSArray";
		enumNames[Sampler_Sampler1DShadow]			= "sampler1DShadow";
		enumNames[Sampler_Sampler2DShadow]			= "sampler2DShadow";
		enumNames[Sampler_SamplerCubeShadow]		= "samplerCubeShadow";
		enumNames[Sampler_Sampler2DRectShadow]		= "sampler2DRectShadow";
		enumNames[Sampler_Sampler1DArrayShadow]		= "sampler1DArrayShadow";
		enumNames[Sampler_Sampler2DArrayShadow]		= "sampler2DArrayShadow";
		enumNames[Sampler_SamplerCubeArrayShadow]	= "samplerCubeArrayShadow";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getCubeMapSideNames()
	{
		QMap<int, QString> enumNames;

		enumNames[CubeMap_PositiveX] = "Positive X";
		enumNames[CubeMap_PositiveY] = "Positive Y";
		enumNames[CubeMap_PositiveZ] = "Positive Z";
		enumNames[CubeMap_NegativeX] = "Negative X";
		enumNames[CubeMap_NegativeY] = "Negative Y";
		enumNames[CubeMap_NegativeZ] = "Negative Z";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getWrapModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Wrap_Repeat]			= "Repeat";
		enumNames[Wrap_Mirror]			= "Mirror";
		enumNames[Wrap_ClampToEdge]		= "Clamp to Edge";
		enumNames[Wrap_ClampToBorder]	= "Clamp to Border";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getMinificationFilteringTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Filter_Nearest]				= "Nearest";
		enumNames[Filter_Linear]				= "Linear";
		enumNames[Filter_NearestMipmapNearest]	= "Nearest Mipmap Nearest";
		enumNames[Filter_NearestMipmapLinear]	= "Nearest Mipmap Linear";
		enumNames[Filter_LinearMipmapNearest]	= "Linear Mipmap Nearest";
		enumNames[Filter_LinearMipmapLinear]	= "Linear Mipmap Linear";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getMagnificationFilteringTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Filter_Nearest]	= "Nearest";
		enumNames[Filter_Linear]	= "Linear";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getCompareFuncTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[CompareFunc_Always]	= "Always";
		enumNames[CompareFunc_Never]	= "Never";
		enumNames[CompareFunc_Less]		= "Less";
		enumNames[CompareFunc_Greater]	= "Greater";
		enumNames[CompareFunc_Equal]	= "Equal";
		enumNames[CompareFunc_NotEqual] = "NotEqual";
		enumNames[CompareFunc_GEqual]	= "GEqual";
		enumNames[CompareFunc_LEqual]	= "LEqual";

		return enumNames;
	}

	QMap<int, QString> TextureBaseBlock::getInternalFormatNames(const EnumProperty* prop)
	{
		QMap<int, QString> enumNames;

		//mutable storage
		if(prop->getVersion() < 420)
		{
			enumNames[Format_DepthComponent]	= "Depth Component";
			enumNames[Format_DepthStencil]		= "Depth Stencil";
			enumNames[Format_Red]				= "Red";
			enumNames[Format_RG]				= "RG";
			enumNames[Format_RGB]				= "RGB";
			enumNames[Format_RGBA]				= "RGBA";

			//compressed
			enumNames[Format_CompressedRed]						= "CompressedRed";
			enumNames[Format_CompressedRedRGTC1]				= "CompressedRedRGTC1";
			enumNames[Format_CompressedRG]						= "CompressedRG";
			enumNames[Format_CompressedRGB]						= "CompressedRGB";
			enumNames[Format_CompressedRGBA]					= "CompressedRGBA";
			enumNames[Format_CompressedRGBA_BPTC_UNorm]			= "CompressedRGBA_BPTC_UNorm";
			enumNames[Format_CompressedRGB_BPTC_Signed_Float]	= "CompressedRGB_BPTC_Signed_Float";
			enumNames[Format_CompressedRGB_BPTC_Unsigned_Float]	= "CompressedRGB_BPTC_Unsigned_Float";
			enumNames[Format_CompressedRG_RGTC2]				= "CompressedRG_RGTC2";
			enumNames[Format_CompressedSignedRedRGTC1]			= "CompressedSignedRed_RGTC1";
			enumNames[Format_CompressedSignedRG_RGTC2]			= "CompressedSignedRG_RGTC2";
			enumNames[Format_CompressedSRGB]					= "CompressedSRGB";
			enumNames[Format_CompressedSRGBAlpha]				= "CompressedSRGB_Alpha";
			enumNames[Format_CompressedSRGB_Alpha_BPTC_UNorm]	= "CompressedSRGB_Alpha_BPTC_UNorm";

		}

		enumNames[Format_Depth24Stencil8]	= "Depth24Stencil8";
		enumNames[Format_Depth32FStencil8]	= "Depth32FStencil8";
		enumNames[Format_DepthComponent16]	= "DepthComponent16";
		enumNames[Format_DepthComponent24]	= "DepthComponent24";
		enumNames[Format_DepthComponent32F]	= "DepthComponent32F";
		enumNames[Format_StencilIndex8]		= "StencilIndex8";

		enumNames[Format_R8]				= "R8";
		enumNames[Format_R8SNorm]			= "R8SNorm";
		enumNames[Format_R16]				= "R16";
		enumNames[Format_R16SNorm]			= "R16SNorm";
		enumNames[Format_RG8]				= "RG8";
		enumNames[Format_RG8SNorm]			= "RG8SNorm";
		enumNames[Format_RG16]				= "RG16";
		enumNames[Format_RG16SNorm]			= "RG16SNorm";
		enumNames[Format_R3G3B2]			= "R3G3B2";
		enumNames[Format_RGB4]				= "RGB4";
		enumNames[Format_RGB5]				= "RGB5";
		enumNames[Format_RGB8]				= "RGB8";
		enumNames[Format_RGB8SNorm]			= "RGB8SNorm";
		enumNames[Format_RGB10]				= "RGB10";
		enumNames[Format_RGB12]				= "RGB12";
		enumNames[Format_RGB16SNorm]		= "RGB16SNorm";
		enumNames[Format_RGBA2]				= "RGBA2";
		enumNames[Format_RGBA4]				= "RGBA4";
		enumNames[Format_RGB5A1]			= "RGB5A1";
		enumNames[Format_RGBA8]				= "RGBA8";
		enumNames[Format_RGBA8SNorm]		= "RGBA8SNorm";
		enumNames[Format_RGB10A2]			= "RGB10A2";
		enumNames[Format_RGB10A2UI]			= "RGB10A2UI";
		enumNames[Format_RGBA12]			= "RGBA12";
		enumNames[Format_RGBA16]			= "RGBA16";
		enumNames[Format_SRGB8]				= "SRGB8";
		enumNames[Format_SRGB8Alpha8]		= "SRGB8Alpha8";
		enumNames[Format_R16F]				= "R16F";
		enumNames[Format_RG16F]				= "RG16F";
		enumNames[Format_RGB16F]			= "RGB16F";
		enumNames[Format_RGBA16F]			= "RGBA16F";
		enumNames[Format_R32F]				= "R32F";
		enumNames[Format_RG32F]				= "RG32F";
		enumNames[Format_RGB32F]			= "RGB32F";
		enumNames[Format_RGBA32F]			= "RGBA32F";
		enumNames[Format_R11FG11FB10F]		= "R11FG11FB10F";
		enumNames[Format_RGB9E5]			= "RGB9E5";
		enumNames[Format_R8I]				= "R8I";
		enumNames[Format_R8UI]				= "R8UI";
		enumNames[Format_R16I]				= "R16I";
		enumNames[Format_R16UI]				= "R16UI";
		enumNames[Format_R32I]				= "R32I";
		enumNames[Format_R32UI]				= "R32UI";
		enumNames[Format_RG8I]				= "RG8I";
		enumNames[Format_RG8UI]				= "RG8UI";
		enumNames[Format_RG16I]				= "RG16I";
		enumNames[Format_RG16UI]			= "RG16UI";
		enumNames[Format_RG32I]				= "RG32I";
		enumNames[Format_RG32UI]			= "RG32UI";
		enumNames[Format_RGB8I]				= "RGB8I";
		enumNames[Format_RGB8UI]			= "RGB8UI";
		enumNames[Format_RGB16I]			= "RGB16I";
		enumNames[Format_RGB16UI]			= "RGB16UI";
		enumNames[Format_RGB32I]			= "RGB32I";
		enumNames[Format_RGB32UI]			= "RGB32UI";
		enumNames[Format_RGBA8I]			= "RGBA8I";
		enumNames[Format_RGBA8UI]			= "RGBA8UI";
		enumNames[Format_RGBA16I]			= "RGBA16I";
		enumNames[Format_RGBA16UI]			= "RGBA16UI";
		enumNames[Format_RGBA32I]			= "RGBA32I";
		enumNames[Format_RGBA32UI]			= "RGBA32UI";

		return enumNames;
	}

    TextureBaseBlock::~TextureBaseBlock()
    {

    }

    EnumProperty* TextureBaseBlock::getMinFilterType()
    {
        return _minFilterType;
    }

    EnumProperty* TextureBaseBlock::getMagFilterType()
    {
        return _magFilterType;
    }

    EnumProperty* TextureBaseBlock::getWrapModeR()
    {
		return _wrapModeR;
	}

	Port* TextureBaseBlock::getGenericOutPort()
	{
		return _outPort;
	}

    EnumProperty* TextureBaseBlock::getWrapModeT()
    {
        return _wrapModeT;
    }

    EnumProperty* TextureBaseBlock::getWrapModeS()
    {
        return _wrapModeS;
    }

	FloatProperty* TextureBaseBlock::getAnisotropy()
	{
		return _anisotropy;
	}

	FloatProperty* TextureBaseBlock::getMinLOD()
	{
		return _minLOD;
	}

	FloatProperty* TextureBaseBlock::getMaxLOD()
	{
		return _maxLOD;
	}

	BoolProperty* TextureBaseBlock::getCompareMode()
	{
		return _compareMode;
	}

	ColorProperty* TextureBaseBlock::getBorderColor()
	{
		return _borderColor;
	}

    void TextureBaseBlock::createProperties()
    {
        Block::createProperties();

		_anisotropy = _properties->newProperty<FloatProperty>(PropertyID::TextureBase_Anisotropy, "Anisotropy");
		*_anisotropy = 1.0f;

		_magFilterType = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_MagFilter, "Magnification Filter");
		*_magFilterType = Filter_Linear;

		_minFilterType = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_MinFilter, "Minification Filter");
		*_minFilterType = Filter_Linear;

		_minLOD = _properties->newProperty<FloatProperty>(PropertyID::TextureBase_MinLOD, "Minimum level-of-detail");
		*_minLOD = -1000.0f;

		_maxLOD = _properties->newProperty<FloatProperty>(PropertyID::TextureBase_MaxLOD, "Maximum level-of-detail");
		*_maxLOD = 1000.0f;

		_wrapModeS = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_WrapModeS, "Wrap Mode S");
        *_wrapModeS = Wrap_Repeat;

		_wrapModeT = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_WrapModeT, "Wrap Mode T");
		*_wrapModeT = Wrap_Repeat;

		_wrapModeR = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_WrapModeR, "Wrap Mode R");
        *_wrapModeR = Wrap_Repeat;

		_borderColor = _properties->newProperty<ColorProperty>(PropertyID::TextureBase_BorderColor, "Bordercolor");
		*_borderColor = QVector4D(0, 0, 0, 0);

		_compareMode = _properties->newProperty<BoolProperty>(PropertyID::TextureBase_CompareMode, "Compare Ref to Texture");
		*_compareMode = false;

		_compareFunc = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_CompareFunc, "Compare Function");
		*_compareFunc = CompareFunc_Always;

		_internalFormat = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat, "Internal Format");
		*_internalFormat = Format_RGBA32F;

    }

	void TextureBaseBlock::createPorts()
	{
		Block::createPorts();

		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}
}
