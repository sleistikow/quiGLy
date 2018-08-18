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

#ifndef VISUALCONNECTION_H
#define VISUALCONNECTION_H

#include "data/iconnection.h"
#include "visualpipelineitem.h"
#include "../pipelineconnection.h"

namespace ysm
{
	class VisualPort;

	//! \brief Represents a connection to the user.
	class VisualConnection : public PipelineConnection<VisualPipelineItem>
	{
		Q_OBJECT

	public:
		/*!
		 * \brief Initialize new instance.
		 * \param connection The represented connection.
		 * \param parentView The parent view item.
		 */
		VisualConnection(IConnection* connection, IView* parentView);

		/*!
		 * \brief Gets the underlying connection.
		 * \return The connection.
		 */
		IConnection* getConnection() const;

		/*!
		 * \brief Gets the displayed pipeline item.
		 * \return The pipeline item.
		 */
		IPipelineItem* getPipelineItem() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the visual item's type.
		 * \return The type.
		 */
		int type() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief Connects the connection to the dependant visual items.
		//! Must be called by the scene, after all blocks were created.
		void visualBlocksCreated();

		//! \brief Deletes this item from the pipeline.
		void deleteItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF calculateBoundingRect() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the connection's source position, relative to the parent's coordinate system.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		QPointF getSourcePos() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the connection's target position, relative to the parent's coordinate system.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		QPointF getTargetPos() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Get's the source's connection color.
		 * \return The color.
		 */
		QColor getSourceColor() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Get's the target's connection color.
		 * \return The color.
		 */
		QColor getTargetColor() const Q_DECL_OVERRIDE;

		//! \brief Restores the layout using the parent scene's layouter.
		void restoreFromLayouter(PipelineSceneLayouter* layouter) Q_DECL_OVERRIDE;

	private:

		//! \brief The represented connection.
		IConnection* _connection;

		//! \brief The visual source port.
		VisualPort* _sourceVisual;

		//! \brief The visual target port.
		VisualPort* _targetVisual;
	};
}

#endif // VISUALCONNECTION_H
