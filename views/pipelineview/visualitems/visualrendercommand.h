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

#ifndef VISUALRENDERCOMMAND_H
#define VISUALRENDERCOMMAND_H

#include "data/irendercommand.h"
#include "visualnameditem.h"

namespace ysm
{
	class VisualBlock;

	//! \brief Base class for visual render commands.
	class VisualRenderCommand : public VisualNamedItem
	{
		Q_OBJECT

	private:

		//! \brief Class that draw a connection between render command and it's block.
		class RenderCommandConnection : public QGraphicsObject
		{
		public:

			/*!
			 * \brief Initialize new item.
			 * \param renderCmd Parent command, which must also implement QObject.
			 */
			RenderCommandConnection(VisualRenderCommand* parentItem);

		public:

			/*!
			 * \brief Set the target block.
			 * \param visualBlock The target block.
			 */
			void setBlock(VisualBlock* visualBlock);

			/*!
			 * \brief Gets the connection's (unclosed) path.
			 * \return The path, from source to target position.
			 */
			QPainterPath getPath() const;

			/*!
			 * \brief Returns the render command connection's bounding rect.
			 * \return The bounding rect.
			 */
			QRectF boundingRect() const Q_DECL_OVERRIDE;

			/*!
			 * \brief Paint the render command connection.
			 * \param painter The painter.
			 * \param option Paint options.
			 * \param widget The widget.
			 */
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

			//! \brief Update the path.
			void updatePath();

		private:

			//! \brief The parent render command.
			VisualRenderCommand* _parentVisual;

			//! \brief The connection block.
			VisualBlock* _blockVisual;

			//! \brief The source and target position.
			QPointF _sourcePos, _targetPos;
		};

	public:

		/*!
		 * \brief Initialize new item.
		 * \param renderCmd Represented command, which must also implement QObject.
		 * \param parentView The parent view item.
		 */
		VisualRenderCommand(IRenderCommand* renderCmd, IView* parentView);

	public:

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
		 * \brief Gets the displayed render command.
		 * \return The render command.
		 */
		IRenderCommand* getRenderCommand() const;

		/*!
		 * \brief Gets the visual item's type.
		 * \return The type.
		 */
		int type() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief Deletes this item from the pipeline.
		void deleteItem() Q_DECL_OVERRIDE;

		//! \brief Connects the connection to the dependant visual items.
		//! Must be called by the scene, after all blocks were created.
		void visualBlocksCreated();

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
		 * \brief Returns the object's preferred position, in case it's newly created.
		 * \return The preferred position.
		 */
		QPointF getPreferredPosition() Q_DECL_OVERRIDE;

	private:

		//! \brief The underlying render command.
		IRenderCommand* _renderCommand;

		//! \brief Connection to the block.
		RenderCommandConnection* _commandConnection;
	};
}

#endif
