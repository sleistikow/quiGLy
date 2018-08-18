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

#ifndef IMAGELOADERBLOCK_H
#define IMAGELOADERBLOCK_H

#include "texturedatasourceblock.h"
#include "data/types/imagedatasource.h"
#include "data/properties/filenameproperty.h"
#include "data/properties/imagegridcellsproperty.h"

namespace ysm
{
	/**
	 * @brief Data source block for loading image files
	 */
	class ImageLoaderBlock : public TextureDataSourceBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::ImageLoader};

	public:
		// Construction
		explicit ImageLoaderBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief The texture wrapping mode
		 */
		enum PixelDataFormat
		{
			PixelFormat_Red,
			PixelFormat_RG,
			PixelFormat_RGB,
			PixelFormat_BGR,
			PixelFormat_RGBA,
			PixelFormat_BGRA,
			PixelFormat_RedInteger,
			PixelFormat_RGInteger,
			PixelFormat_RGBInteger,
			PixelFormat_BGRInteger,
			PixelFormat_RGBAInteger,
			PixelFormat_BGRAInteger,
			PixelFormat_StencilIndex,
			PixelFormat_DepthComponent,
			PixelFormat_DepthStencil,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getPixelDataFormatNames();

		/**
		 * @brief The texture wrapping mode
		 */
		enum PixelDataType
		{
			PixelType_UnsignedByte,
			PixelType_Byte,
			PixelType_UnsignedShort,
			PixelType_Short,
			PixelType_UnsignedInt,
			PixelType_Int,
			PixelType_Float,
			PixelType_UnsignedByte332,
			PixelType_UnsignedByte233Rev,
			PixelType_UnsignedShort565,
			PixelType_UnsignedShort565Rev,
			PixelType_UnsignedShort4444,
			PixelType_UnsignedShort4444Rev,
			PixelType_UnsignedShort5551,
			PixelType_UnsignedShort1555Rev,
			PixelType_UnsignedInt8888,
			PixelType_UnsignedInt8888Rev,
			PixelType_UnsignedInt1010102,
			PixelType_UnsignedInt2101010Rev,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getPixelDataTypeNames();
		/**
		 * @brief Gets the image file
		 */
		FilenameProperty* getImageFile();

		/**
		 * @brief Gets the image size
		 */
		SizeProperty* getImageSize() const;

		/**
		 * @brief Gets the image grid
		 */
		SizeProperty* getImageGrid() const;

	protected:
		void createProperties() override;

		void reloadDataSource() override;

	private:
		// The data source
		ImageDataSource _imageDataSource;

		// Properties
		FilenameProperty* _imageFile{nullptr};
		SizeProperty* _imageSize{nullptr};
		SizeProperty* _imageGrid{nullptr};
		ImageGridCellsProperty* _imageCells{nullptr};
		EnumProperty* _pixelDataFormat{nullptr};
		EnumProperty* _pixelDataType{nullptr};
	};
}

#endif
