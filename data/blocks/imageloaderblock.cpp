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

#include "imageloaderblock.h"
#include "textureblock.h"
#include "connection.h"

namespace ysm
{
	ImageLoaderBlock::ImageLoaderBlock(Pipeline* parent) :
		TextureDataSourceBlock(&_imageDataSource, parent, BlockType::ImageLoader, "Image Loader"),
		_imageDataSource{parent, this}
	{

	}

	QMap<int, QString> ImageLoaderBlock::getPixelDataFormatNames()
	{
		QMap<int, QString> enumNames;

		enumNames[PixelFormat_Red] = "Red";
		enumNames[PixelFormat_RG] = "RG";
		enumNames[PixelFormat_RGB] = "RGB";
		enumNames[PixelFormat_RGBA] = "RGBA";
		enumNames[PixelFormat_BGR] = "BGR";
		enumNames[PixelFormat_BGRA] = "BGRA";
		enumNames[PixelFormat_RedInteger] = "Red_Integer";
		enumNames[PixelFormat_RGInteger] = "RG_Integer";
		enumNames[PixelFormat_RGBInteger] = "RGB_Integer";
		enumNames[PixelFormat_RGBAInteger] = "RGBA_Integer";
		enumNames[PixelFormat_BGRInteger] = "BGR_Integer";
		enumNames[PixelFormat_BGRAInteger] = "BGRA_Integer";
		enumNames[PixelFormat_DepthComponent] = "Depth Component";
		enumNames[PixelFormat_DepthStencil] = "Depth Stencil";
		enumNames[PixelFormat_StencilIndex] = "Stencil Integer";

		return enumNames;
	}

	QMap<int, QString> ImageLoaderBlock::getPixelDataTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[PixelType_Byte] = "Byte";
		enumNames[PixelType_Float] = "Float";
		enumNames[PixelType_Int] = "Int";
		enumNames[PixelType_Short] = "Short";
		enumNames[PixelType_UnsignedByte] = "Unsigned Byte";
		enumNames[PixelType_UnsignedByte332] = "Unsigned Byte 3_3_2";
		enumNames[PixelType_UnsignedByte233Rev] = "Unsigned Byte 2_3_3 Rev";
		enumNames[PixelType_UnsignedInt] = "Unsigned Int";
		enumNames[PixelType_UnsignedInt8888] = "Unsigned Int 8_8_8_8";
		enumNames[PixelType_UnsignedInt8888Rev] = "Unsigned Int 8_8_8_8 Rev";
		enumNames[PixelType_UnsignedInt1010102] = "Unsigned Int 10_10_10_2";
		enumNames[PixelType_UnsignedInt2101010Rev] = "Unsigned Int 2_10_10_10 Rev";
		enumNames[PixelType_UnsignedShort] = "Unsigned Short";
		enumNames[PixelType_UnsignedShort565] = "Unsigned Short 5_6_5";
		enumNames[PixelType_UnsignedShort565Rev] = "Unsigned Short 5_6_5 Rev";
		enumNames[PixelType_UnsignedShort5551] = "Unsigned Short 5_5_5_1";
		enumNames[PixelType_UnsignedShort1555Rev] = "Unsigned Short 1_5_5_5 Rev";
		enumNames[PixelType_UnsignedShort4444] = "Unsigned Short 4_4_4_4";
		enumNames[PixelType_UnsignedShort4444Rev] = "Unsigned Short 4_4_4_4 Rev";

		return enumNames;
	}

	FilenameProperty* ImageLoaderBlock::getImageFile()
	{
		return _imageFile;
	}

	SizeProperty* ImageLoaderBlock::getImageSize() const
	{
		return _imageSize;
	}

	SizeProperty* ImageLoaderBlock::getImageGrid() const
	{
		return _imageGrid;
	}

	void ImageLoaderBlock::createProperties()
	{
		TextureDataSourceBlock::createProperties();

		_imageFile = _properties->newProperty<FilenameProperty>(PropertyID::Img_FileName, "Image File");

		_imageSize = _properties->newProperty<SizeProperty>(PropertyID::Img_Size, "Size", true);
		_imageSize->setSerializable(false);
		_imageSize->delegateValue(
					[this]()->const QSize& { static QSize __ret; __ret = _imageDataSource.getImageSize(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_imageGrid = _properties->newProperty<SizeProperty>(PropertyID::Img_Grid, "Grid");
		*_imageGrid = QSize{1, 1};

		_imageCells = _properties->newProperty<ImageGridCellsProperty>(PropertyID::Img_Cells, "Cells", true);
		_imageCells->setSerializable(false);
		_imageCells->delegateValue(
					[this]()->const ImageDataSource::ImageGridCells& { static ImageDataSource::ImageGridCells __ret; __ret = _imageDataSource.getImageCells(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_pixelDataFormat = _properties->newProperty<EnumProperty>(PropertyID::Img_PixelDataFormat, "Pixeldata Format");
		*_pixelDataFormat = PixelFormat_RGBA;

		_pixelDataType = _properties->newProperty<EnumProperty>(PropertyID::Img_PixelDataType, "Pixeldata Type");
		*_pixelDataType = PixelType_Float;
	}

	void ImageLoaderBlock::reloadDataSource()
	{
		QString imgFile = *_imageFile;

		setStatus(PipelineItemStatus::Healthy);

		// Set the file name for our image data source and let it load the image immediately
		_imageDataSource.setImageFile(imgFile, *_imageGrid, true);
	}
}
