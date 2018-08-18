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

#ifndef VISUALBLOCK_H
#define VISUALBLOCK_H

#include "data/iblock.h"
#include "visualnameditem.h"

#include <QLineEdit>

namespace ysm
{
	class VisualPort;

	//! \brief Specializes the visual pipeline item to provide a base class for visual blocks.
	class VisualBlock : public VisualNamedItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new item.
		 * \param block Represented block, which must also implement QObject.
		 * \param parentView The parent view item.
		 */
		VisualBlock(IBlock* block, IView* parentView);

		/*!
		 * \brief Paint the visual pipeline item.
		 * \param painter The painter.
		 * \param option Paint options.
		 * \param widget The widget.
		 */
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the displayed pipeline item.
		 * \return The pipeline item.
		 */
		IPipelineItem* getPipelineItem() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the displayed block.
		 * \return The block.
		 */
		IBlock* getBlock() const;

		/*!
		 * \brief Returns the visual port for the given data port.
		 * \param port The data port.
		 * \return The visual port.
		 */
		VisualPort* getVisualPort(IPort* port) const;

		/*!
		 * \brief Gets the visual item's type.
		 * \return The type.
		 */
		int type() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief Deletes this item from the pipeline.
		void deleteItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Gets the content's height.
		 * \return The height.
		 */
		float getContentHeight() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Get the item's full width (with padding).
		 * \return The width.
		 */
		float getFullWidth() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Get the item's inner width (without padding).
		 * \return The width.
		 */
		float getInnerWidth() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Draw the block's contents.
		 * \param painter The painter.
		 * \param boundingRect The bounding rect.
		 */
		virtual void paintContents(QPainter* painter, QRectF boundingRect);

		/*!
		 * \brief Returns the object's preferred position, in case it's newly created.
		 * \return The preferred position.
		 */
		QPointF getPreferredPosition() Q_DECL_OVERRIDE;

		/*!
		 * \brief Change to the pipeline, check for version change.
		 * \param pipeline The pipeline.
		 */
		void updateVersion(IPipeline* pipeline);

	private:

		//! \brief The underlying block.
		IBlock* _block;

	};

}

#endif // VISUALBLOCK_H
