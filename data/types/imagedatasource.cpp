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

#include "imagedatasource.h"
#include "data/blocks/block.h"

#include <QImage>
#include <QtMath>

namespace ysm
{
	ImageDataSource::ImageDataSource(Pipeline* pipeline, Block* block) : TextureDataSource(pipeline, block)
	{
		_outputs = TexelColors;
	}

	void ImageDataSource::setImageFile(const QString& imageFile, QSize gridSize, bool loadImmediately)
	{
		// No file loaded, remove anything from this source from the cache
		if (imageFile.isEmpty())
			removeFromCache();

		_imageFile = imageFile;
		_imageGrid = gridSize;

		if (loadImmediately && !_imageFile.isEmpty())
		{
			// Force loading of the image file by accessing the image data
			getImageData();
		}
	}

	const Vec4Data& ImageDataSource::getTexelColors()
	{
		return getImageData()->imageData;
	}

	QSize ImageDataSource::getImageSize()
	{
		return getImageData()->imageSize;
	}

	QVector<ImageDataSource::ImageGridCell> ImageDataSource::getImageCells()
	{
		return getImageData()->imageCells;
	}

	CacheObject::Key ImageDataSource::getCacheKey(bool retrieveForeignKey)
	{
		Q_UNUSED(retrieveForeignKey);

		CacheObject::Key key;

		// Key consists of class qualifier + image filename + grid size
		key = QString("ImageDataSource/%1@%2x%3").arg(_imageFile).arg(_imageGrid.width()).arg(_imageGrid.height());
		return key;
	}

	CacheObject::CacheObjectData* ImageDataSource::createCacheData()
	{
		_block->setStatus(PipelineItemStatus::Healthy);

		if (!_imageFile.isEmpty())
		{
			// Data must be created on the heap, will be managed by the cache pool
			ImageData* data = new ImageData;

			try
			{
				loadImage(data);
			}
			catch (std::exception& excp)
			{
				delete data;
				data = nullptr;

				QString msg = QString("The image failed to load: %1").arg(excp.what());
				_block->setStatus(PipelineItemStatus::Chilled, msg);
			}

			return data;
		}

		return nullptr;
	}

	const ImageDataSource::ImageData* ImageDataSource::getImageData()
	{
		const ImageData* data = getCachedData<ImageData>();

		if (!data)
		{
			// No data cached (maybe the file could not be loaded); return an empty dummy
			data = &_emptyData;
		}

		return data;
	}

	void ImageDataSource::loadImage(ImageData* data)
	{
		QImage img;

		if (img.load(_imageFile))
		{
			QSize imgSize = data->imageSize = img.size();

			data->imageData.resize(imgSize.width() * imgSize.height());

			QSize imgGrid = _imageGrid;

			// Ensure that our grid size is at least 1x1 and not bigger than our image
			if (imgGrid.width() <= 0)
				imgGrid.setWidth(1);
			else if (imgGrid.width() > imgSize.width())
				imgGrid.setWidth(imgSize.width());

			if (imgGrid.height() <= 0)
				imgGrid.setHeight(1);
			else if (imgGrid.height() > imgSize.height())
				imgGrid.setHeight(imgSize.height());

			QSize cellSize = QSize{qCeil((double)imgSize.width() / (double)imgGrid.width()), qCeil((double)imgSize.height() / (double)imgGrid.height())};
			int dataIndex = 0;

			// Iterate over each cell
			for (int r = 0; r < imgGrid.height(); ++r)
			{
				for (int c = 0; c < imgGrid.width(); ++c)
				{
					// Get the starting position of our cell in the image
					QPoint startPos{c * cellSize.width(), r * cellSize.height()};

					// Create a new cell entry
					createImageCell(data, img, dataIndex, startPos, cellSize);
				}
			}
		}
		else
			throw std::runtime_error{qPrintable(QString("The image '%1' could not be loaded").arg(_imageFile))};
	}

	void ImageDataSource::createImageCell(ImageData* data, QImage& img, int& index, QPoint startPos, QSize cellSize)
	{
		QSize imgSize = img.size();

		// Make sure that we don't fall out of bounds
		cellSize.setWidth(qMin(cellSize.width(), imgSize.width() - startPos.x()));
		cellSize.setHeight(qMin(cellSize.height(), imgSize.height() - startPos.y()));

		ImageGridCell gridCell;

		gridCell.cellOffset = index;
		gridCell.cellSize = cellSize;

		// Load the pixel data from the image
		for (int r = startPos.y(); r < (startPos.y() + cellSize.height()); ++r)
		{
			for (int c = startPos.x(); c < (startPos.x() + cellSize.width()); ++c)
			{
				// We store normalized (0.0-1.0) colors
				QRgb rgb = img.pixel(c, r);
				QVector4D clr{qRed(rgb) / 255.0f, qGreen(rgb) / 255.0f, qBlue(rgb) / 255.0f, 1.0f};

				if (img.hasAlphaChannel())
					clr.setW(qAlpha(rgb) / 255.0f);

				data->imageData[index++] = clr;
			}
		}

		data->imageCells.append(std::move(gridCell));
	}
}
