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

#ifndef TEXTUREBASEBLOCK_H
#define TEXTUREBASEBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
    /**
     * @brief Texture base block
     */
    class TextureBaseBlock : public Block
    {
        Q_OBJECT

    public:
        // Types
        /**
         * @brief The texture wrapping mode
         */
        enum WrapMode
        {
            Wrap_Repeat,
            Wrap_Mirror,
            Wrap_ClampToEdge,
            Wrap_ClampToBorder,
        };

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getWrapModeNames();

        /**
         * @brief The filtering type
         */
		enum FilteringType
        {
            Filter_Nearest,
            Filter_Linear,
			Filter_NearestMipmapNearest,
			Filter_NearestMipmapLinear,
			Filter_LinearMipmapNearest,
			Filter_LinearMipmapLinear,
        };

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getMinificationFilteringTypeNames();

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getMagnificationFilteringTypeNames();

		/**
		 * @brief The texture Target type
		 */
		enum TargetType
		{
			Target_1D,
			Target_1DArray,
			Target_2D,
			Target_2DArray,
			Target_2DRect,
			Target_2DMS,
			Target_2DMSArray,
			Target_3D,
			Target_CubeMap,
			Target_CubeMapPosX,
			Target_CubeMapNegX,
			Target_CubeMapPosY,
			Target_CubeMapNegY,
			Target_CubeMapPosZ,
			Target_CubeMapNegZ,
			Target_CubeArray,
			Target_Buffer,
			Target_Proxy1D,
			Target_Proxy2D,
			Target_Proxy1DArray,
			Target_Proxy2DRect,
			Target_ProxyCubeMap,
			Target_Proxy2DMS,
			Target_Proxy3D,
			Target_Proxy2DArray,
			Target_Proxy2DMSArray,
		};
		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getTargetTypeNames();

		/**
		 * @brief The sampler type
		 */
		enum SamplerType
		{
			Sampler_Sampler1D,
			Sampler_Sampler2D,
			Sampler_Sampler3D,
			Sampler_SamplerCube,
			Sampler_Sampler2DRect,
			Sampler_Sampler1DArray,
			Sampler_Sampler2DArray,
			Sampler_SamplerCubeArray,
			Sampler_SamplerBuffer,
			Sampler_Sampler2DMS,
			Sampler_Sampler2DMSArray,
			Sampler_Sampler1DShadow,
			Sampler_Sampler2DShadow,
			Sampler_SamplerCubeShadow,
			Sampler_Sampler2DRectShadow,
			Sampler_Sampler1DArrayShadow,
			Sampler_Sampler2DArrayShadow,
			Sampler_SamplerCubeArrayShadow,
		};
		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getSamplerTypeNames();
		/**
		 * @brief The texture Cube map sides
		 */
		enum CubeMapSide
		{
			CubeMap_PositiveX,
			CubeMap_NegativeX,
			CubeMap_PositiveY,
			CubeMap_NegativeY,
			CubeMap_PositiveZ,
			CubeMap_NegativeZ,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getCubeMapSideNames();

		/**
		 * @brief The texture comparefunctions
		 */
		enum CompareFuncType
		{
			CompareFunc_Always,
			CompareFunc_Never,
			CompareFunc_Less,
			CompareFunc_LEqual,
			CompareFunc_GEqual,
			CompareFunc_Greater,
			CompareFunc_Equal,
			CompareFunc_NotEqual,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getCompareFuncTypeNames();

		/**
		 * @brief The texture Target type
		 */
		enum InternalFormat
		{
			//base formats
			Format_DepthComponent,
			Format_DepthStencil,
			Format_Red,
			Format_RG,
			Format_RGB,
			Format_RGBA,

			Format_DepthComponent32F,
			Format_DepthComponent24,
			Format_DepthComponent16,
			Format_Depth32FStencil8,
			Format_Depth24Stencil8,

			//sized formats
			Format_R8,
			Format_R8SNorm,
			Format_R16,
			Format_R16SNorm,
			Format_RG8,
			Format_RG8SNorm,
			Format_RG16,
			Format_RG16SNorm,
			Format_R3G3B2,
			Format_RGB4,
			Format_RGB5,
			Format_RGB8,
			Format_RGB8SNorm,
			Format_RGB10,
			Format_RGB12,
			Format_RGB16SNorm,
			Format_RGBA2,
			Format_RGBA4,
			Format_RGB5A1,
			Format_RGBA8,
			Format_RGBA8SNorm,
			Format_RGB10A2,
			Format_RGB10A2UI,
			Format_RGBA12,
			Format_RGBA16,
			Format_SRGB8,
			Format_SRGB8Alpha8,
			Format_R16F,
			Format_RG16F,
			Format_RGB16F,
			Format_RGBA16F,
			Format_R32F,
			Format_RG32F,
			Format_RGB32F,
			Format_RGBA32F,
			Format_R11FG11FB10F,
			Format_RGB9E5,
			Format_R8I,
			Format_R8UI,
			Format_R16I,
			Format_R16UI,
			Format_R32I,
			Format_R32UI,
			Format_RG8I,
			Format_RG8UI,
			Format_RG16I,
			Format_RG16UI,
			Format_RG32I,
			Format_RG32UI,
			Format_RGB8I,
			Format_RGB8UI,
			Format_RGB16I,
			Format_RGB16UI,
			Format_RGB32I,
			Format_RGB32UI,
			Format_RGBA8I,
			Format_RGBA8UI,
			Format_RGBA16I,
			Format_RGBA16UI,
			Format_RGBA32I,
			Format_RGBA32UI,

			//Compressed
			Format_CompressedRed,
			Format_CompressedRG,
			Format_CompressedRGB,
			Format_CompressedRGBA,
			Format_CompressedSRGB,
			Format_CompressedSRGBAlpha,
			Format_CompressedRedRGTC1,
			Format_CompressedSignedRedRGTC1,
			Format_CompressedRG_RGTC2,
			Format_CompressedSignedRG_RGTC2,
			Format_CompressedRGBA_BPTC_UNorm,
			Format_CompressedSRGB_Alpha_BPTC_UNorm,
			Format_CompressedRGB_BPTC_Signed_Float,
			Format_CompressedRGB_BPTC_Unsigned_Float,

			//RenderBuffer
			Format_StencilIndex1,
			Format_StencilIndex4,
			Format_StencilIndex8,
			Format_StencilIndex16,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getInternalFormatNames(const EnumProperty* prop);


    public:
        // Construction
        explicit TextureBaseBlock(Pipeline* parent, BlockType type, const QString& name);
        virtual ~TextureBaseBlock();

    public:
        // Property access
        /**
         * @brief Gets the minification filtering type
         */
		EnumProperty* getMinFilterType();

        /**
         * @brief Gets the magnification filtering type
         */
		EnumProperty* getMagFilterType();

        /**
         * @brief Gets the wrap mode in S direction
         */
		EnumProperty* getWrapModeS();

        /**
         * @brief Gets the wrap mode in T direction
         */
		EnumProperty* getWrapModeT();

        /**
         * @brief Gets the wrap mode in R direction
         */
		EnumProperty* getWrapModeR();

		/**
		 * @brief Gets the anisotropy value
		 */
		FloatProperty* getAnisotropy();

		/**
		 * @brief Gets the minLOD level
		 */
		FloatProperty* getMinLOD();
		/**
		 * @brief Gets the maxLOD level
		 */
		FloatProperty* getMaxLOD();

		/**
		 * @brief Gets the bordercolor
		 */
		ColorProperty* getBorderColor();

		/**
		 * @brief Gets the compareMode
		 */
		BoolProperty* getCompareMode();

        // Port access
        /**
         * @brief Gets the single out-port
         */
        Port* getGenericOutPort();

    protected:
        void createProperties() override;
		void createPorts() override;

    protected:
        // Properties
		FloatProperty* _anisotropy{nullptr};
		EnumProperty* _minFilterType{nullptr};
		EnumProperty* _magFilterType{nullptr};
		EnumProperty* _wrapModeS{nullptr};
		EnumProperty* _wrapModeT{nullptr};
		EnumProperty* _wrapModeR{nullptr};
		FloatProperty* _minLOD{nullptr};
		FloatProperty* _maxLOD{nullptr};
		ColorProperty* _borderColor{nullptr};
		BoolProperty* _compareMode{nullptr};
		EnumProperty* _internalFormat{nullptr};
		EnumProperty* _compareFunc{nullptr};

		// Ports
		Port* _outPort{nullptr};
    };
}

#endif
