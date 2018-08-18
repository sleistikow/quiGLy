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

#ifndef VISUALPORT_H
#define VISUALPORT_H

#include "data/iport.h"
#include "../pipelineconnection.h"
#include "visualpipelineitem.h"

#include <QGraphicsObject>

namespace ysm
{
	class VisualBlock;
	class BlueprintConnection;

	//! \brief A visual representation of a block's port.
	class VisualPort : public VisualPipelineItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param port The represented port.
		 * \param parent The parent block.
		 */
		VisualPort(IPort* port, VisualBlock* parent);

		/*!
		 * \brief Paint the visual pipeline item.
		 * \param painter The painter.
		 * \param option Paint options.
		 * \param widget The widget.
		 */
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the underlying port.
		 * \return The port.
		 */
		IPort* getPort() const;

		/*!
		 * \brief Gets the displayed pipeline item.
		 * \return The pipeline item.
		 */
		IPipelineItem* getPipelineItem() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the visual block that this port belongs to.
		 * \return The block.
		 */
		VisualBlock* getVisualBlock() const;

		/*!
		 * \brief Gets a connected blueprint connection if available.
		 * \return The blueprint connection.
		 */
		BlueprintConnection* getBlueprintConnection() const;

		/*!
		 * \brief Sets a temporary blueprint connection.
		 * \param blueprintConnection The blueprint connection.
		 */
		void setBlueprintConnection(BlueprintConnection* blueprintConnection);

		/*!
		 * \brief Gets the visual item's type.
		 * \return The type.
		 */
		int type() const Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF calculateBoundingRect() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns all context menu actions.
		 * \return The actions. Add NULL for a seperator.
		 */
		QList<QAction*> getContextActions() const Q_DECL_OVERRIDE;

		/*!
		 * \brief The mouse was pressed, start the drag.
		 * \param event The event arguments.
		 */
		void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief The mouse was moved, drag the item.
		 * \param event The event arguments.
		 */
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief The mouse was released, finalize the drag.
		 * \param event The event arguments.
		 */
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

		//! \brief Deletes this item from the pipeline.
		void deleteItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Change to the pipeline, check for version change.
		 * \param pipeline The pipeline.
		 */
		void updateVersion(IPipeline* pipeline);

	protected slots:

		/*!
		 * \brief One of the blueprints' state changed.
		 * \param state The new state.
		 */
		void blueprintStateChanged(PipelineConnectionState);

	private:

		//! \brief The represented port.
		IPort* _port;

		//! \brief Blueprint connection that startet at this port, if exists.
		BlueprintConnection* _sourcingBlueprint;

		//! \brief Blueprint blueprint connection, that is temporarily targeting this port, if exists.
		BlueprintConnection* _targetingBlueprint;
	};

}

#endif // VISUALPORT_H
