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

#ifndef IMAGEDATASOURCE_H
#define IMAGEDATASOURCE_H

#include "texturedatasource.h"

#include <QSize>

namespace ysm
{
	/**
	 * @brief Image data source (uses QImage for image loading)
	 */
	class ImageDataSource : public TextureDataSource
	{
	public:
		struct ImageGridCell
		{
			unsigned int cellOffset{0};
			QSize cellSize;

			bool operator ==(const ImageGridCell& cell) const
			{
				return (cellOffset == cell.cellOffset && cellSize == cell.cellSize);
			}
		};

		using ImageGridCells = QVector<ImageGridCell>;

	public:
		// Construction
		explicit ImageDataSource(Pipeline* pipeline, Block* block);

	public:
		// Image loading
		/**
		 * @brief Sets the image file to load; if @p loadImmediately is set to true, the image is immediately loaded
		 */
		void setImageFile(const QString& imageFile, QSize gridSize, bool loadImmediately = false);

	public:
		// General attributes
		/**
		 * @brief Retrieves the size of the loaded image (or 0x0 if none is loaded)
		 */
		QSize getImageSize();

		/**
		 * @brief Retrieves all cells of this image
		 */
		ImageGridCells getImageCells();

		// Data access
		const Vec4Data& getTexelColors() override;

	public:
		// ICacheable
		CacheObject::Key getCacheKey(bool retrieveForeignKey) override;
		CacheObject::CacheObjectData* createCacheData() override;

	private:
		struct ImageData : CacheObject::CacheObjectData
		{
			QSize imageSize;

			Vec4Data imageData;
			ImageGridCells imageCells;
		} _emptyData;

		/**
		 * @brief Gets the cached image data
		 * If no data could be cached, _emptyData is returned.
		 */
		const ImageData* getImageData();

	private:
		/**
		 * @brief Loads the set image file
		 * Throws an exception if something didn't work.
		 */
		void loadImage(ImageData* data);

		/**
		 * @brief Creates a new image cell
		 */
		void createImageCell(ImageData* data, QImage& img, int& index, QPoint startPos, QSize cellSize);

	private:
		QString _imageFile;
		QSize _imageGrid;
	};
}

#endif
