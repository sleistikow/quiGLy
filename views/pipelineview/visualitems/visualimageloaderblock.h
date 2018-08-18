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

#ifndef VISUALIMAGELOADERBLOCK_H
#define VISUALIMAGELOADERBLOCK_H

#include "visualblock.h"

#include <QObject>

namespace ysm
{

	//! \brief Displays an image loader block to the user.
	class VisualImageLoaderBlock : public VisualBlock
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param block Represented block, which must implement QObject and ImageLoaderBlock.
		 * \param parentView The parent view item.
		 */
		explicit VisualImageLoaderBlock(IBlock* block, IView* parentView);

	protected:

		/**
		 * @brief Should return true, if the given property is visually represented by this view.
		 * @param propertyID The property ID.
		 * @return If true, the view will redraw if the property changes.
		 */
		bool isVisualProperty(PropertyID propertyID) Q_DECL_OVERRIDE;

		/*!
		 * \brief Draw the block's contents.
		 * \param painter The painter.
		 * \param boundingRect The bounding rect.
		 */
		void paintContents(QPainter* painter, QRectF boundingRect) Q_DECL_OVERRIDE;

	private:

		//! \brief The currently cached image.
		QImage* _cachedImage;

		//! \brief The cached image file name.
		QString _cachedImageFile;
	};

}

#endif // VISUALIMAGELOADERBLOCK_H
