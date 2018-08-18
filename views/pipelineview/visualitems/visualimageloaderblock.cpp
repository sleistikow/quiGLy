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

#include "visualimageloaderblock.h"
#include "data/iproperty.h"
#include "data/properties/filenameproperty.h"

#include <QPainter>

using namespace ysm;

VisualImageLoaderBlock::VisualImageLoaderBlock(IBlock* block, IView* parentView) :
	VisualBlock(block, parentView),
	_cachedImage(NULL)
{ }

void VisualImageLoaderBlock::paintContents(QPainter* painter, QRectF boundingRect)
{
	//Get the image data.
	QString imageFile = getBlock()->getProperty<FilenameProperty>(PropertyID::Img_FileName)->getValue();

    if (imageFile.isEmpty())
	{
		VisualBlock::paintContents(painter, boundingRect);
		return;
	}

	QSize imageSize = getBlock()->getProperty<SizeProperty>(PropertyID::Img_Size)->getValue();
	float imageWidth = imageSize.width();
	float imageHeight = imageSize.height();

	//Check if image has been loaded.
	if(imageFile != _cachedImageFile && !imageFile.isEmpty())
	{
		//Store the new image file.
		_cachedImageFile = imageFile;

		//Load the new image.
		delete _cachedImage;
		_cachedImage = new QImage(_cachedImageFile);
	}

	//Ensure image exists.
    if(_cachedImage && !_cachedImage->isNull())
	{
		//Calculate rectangle inside the image, that matches the bounding rect's ratio.
		QRectF sourceRect;
		if(boundingRect.width() / boundingRect.height() > imageWidth / imageHeight)
		{
			float ratioScaledHeight = boundingRect.height() / boundingRect.width() * imageWidth;
			sourceRect = QRectF(0, (imageHeight - ratioScaledHeight) / 2,
								imageWidth, ratioScaledHeight);
		}
		else
		{
			float ratioScaledWidth = boundingRect.width() / boundingRect.height() * imageHeight;
			sourceRect = QRectF((imageWidth - ratioScaledWidth) / 2, imageHeight,
								ratioScaledWidth, imageHeight);
		}

		//Draw the image.
		painter->drawImage(boundingRect, *_cachedImage, sourceRect);
	}

	//Otherwise draw default representation.
	else
		VisualBlock::paintContents(painter, boundingRect);
}

bool VisualImageLoaderBlock::isVisualProperty(PropertyID propertyID)
{
	//File name property change requires redrawing.
	return VisualBlock::isVisualProperty(propertyID) || propertyID == PropertyID::Img_FileName;
}
