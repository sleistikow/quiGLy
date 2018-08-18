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

#ifndef PIPELINESCENE_H
#define PIPELINESCENE_H

#include "../viewitem.h"
#include "data/ipipeline.h"
#include "data/ipipelineitem.h"
#include "pipelinetab.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMenu>
#include <QMap>
#include <QList>

namespace ysm
{
	class VisualPipelineItem;
	class VisualConnection;
	class VisualPipelineItemFactory;
	class PipelineSceneLayouter;
	class VisualPort;
	class BlueprintConnection;
	class PipelineTab;

	//! \brief A graphics scene, that renders an IPipeline.
	class PipelineScene : public QGraphicsScene, public ViewItem
	{
		Q_OBJECT

	public:

		//! \brief Defines all user visual child types in this scene.
		enum VisualTypes
		{
			VisualBlockType = QGraphicsItem::UserType + 1,
			VisualCommandType = QGraphicsItem::UserType + 2,
			VisualPortType = QGraphicsItem::UserType + 3,
			VisualConnectionType = QGraphicsItem::UserType + 4,
		};

		/*!
		 * \brief Initialize new instance.
		 * \param parentView The parent view.
		 */
		PipelineScene(PipelineTab* parentView);

		/*!
		 * \brief Gets the layouter.
		 * \return The layouter.
		 */
		PipelineSceneLayouter* getLayouter() const;

		/*!
		 * \brief Gets the visual item corresponding to the given data item.
		 * \param dataItem The data item.
		 * \return The visual item.
		 */
		VisualPipelineItem* getVisualItem(IPipelineItem* dataItem) const;

		/*!
		 * \brief Gets the visual item corresponding to the given data item's ID.
		 * \param dataItem The data item's ID.
		 * \return The visual item.
		 */
		VisualPipelineItem* getVisualItem(PipelineItemID dataItemID) const;

		/*!
		 * \brief Gets the parent pipeline tab.
		 * \return The pipeline tab.
		 */
		PipelineTab* getPipelineTab() const;

	public slots:

		/*!
		 * \brief Update the item's layout.
		 * \param pipelineItem The pipeline item.
		 */
		void updateItemLayout(IPipelineItem* pipelineItem);

		//! \brief Deletes all selected items.
		void deleteSelectedItems();

	protected:

		/*!
		 * \brief Draw the background raster.
		 * \param painter The painter.
		 * \param rect The rectangle to draw.
		 */
		void drawBackground(QPainter* painter, const QRectF& rect) Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Serialize the view and it's children recursively.
		 * \param xmlElement The XML element.
		 * \param context The context.
		 */
		void serialize(QDomElement* xmlElement, SerializationContext* context) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Deserialize the view and it's children recursively.
		 * \param xmlElement The XML element.
		 * \param context The context.
		 */
		void deserialize(const QDomElement* xmlElement, SerializationContext* context) Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Create a new visual block from pipeline block.
		 * \param pipelineBlock The pipeline block that was added.
		 */
		void createVisualBlock(IBlock* pipelineBlock);

		/*!
		 * \brief Create a new visual connection from pipeline connection.
		 * \param pipelineBlock The pipeline connection that was added.
		 */
		void createVisualConnection(IConnection* connection);

		/*!
		 * \brief Create a new visual render command from pipeline render command.
		 * \param renderCommand The pipeline render command that was added.
		 */
		void createVisualRenderCommand(IRenderCommand* renderCommand);

		//! \brief Completely recreate and update the view.
		void recreatePipelineItems();

		/*!
		 * \brief Remove an existing visual item.
		 * \param pipelineItem The pipeline item that was removed.
		 */
		void deleteVisualItem(IPipelineItem* pipelineItem);

	protected slots:

		//! \brief The selection changed, request the correct active item.
		void updateActiveItem();

	private:

		//! \brief The parent view.
		PipelineTab* _parentView;

		//! \brief Map containing all visual blocks and connections.
		QMap<PipelineItemID, VisualPipelineItem*> _visualItems;

		//! \brief The layouter that stores the layout information.
		PipelineSceneLayouter* _sceneLayouter;
	};

}
#endif // PIPELINESCENE_H
