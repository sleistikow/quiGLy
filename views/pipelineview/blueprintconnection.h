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

#ifndef BLUEPRINTCONNECTION_H
#define BLUEPRINTCONNECTION_H

#include "pipelineconnection.h"

#include <QGraphicsObject>

namespace ysm
{
	class VisualPort;

	//! \brief Base class for blueprint pipeline connection, joins graphics object and view item.
	class BlueprintConnectionBase : public QGraphicsObject, public ViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentObject The parent object.
		 * \param parentView The parent item.
		 */
		BlueprintConnectionBase(QGraphicsObject* parentObject, IView* parentView);
	};

	//! \brief A blueprint connection, that show the connection that is being created by the user.
	class BlueprintConnection : public PipelineConnection<BlueprintConnectionBase>
	{
		Q_OBJECT

	public:

		//! \brief Initialize new instance.
		BlueprintConnection(VisualPort* sourceVisual);

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF boundingRect() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Set the connection's target position, relative to the parent's coordinate system.
		 * This will update the connection.
		 * \param targetPos The target position.
		 */
		void setTargetPos(const QPointF& targetPos);

		/*!
		 * \brief Gets the source visual.
		 * \return The source visual.
		 */
		VisualPort* getSourceVisual() const;

		/*!
		 * \brief Gets the target visual, if one exists.
		 * \return The target visual.
		 */
		VisualPort* getTargetVisual() const;

		/*!
		 * \brief Checks, if the given target port is a valid target port.
		 * \param targetPort The target port.
		 * \return True, if port is valid.
		 */
		bool isValidTarget(VisualPort* targetPort);

		/*!
		 * \brief Paint the visual pipeline item.
		 * \param painter The painter.
		 * \param option Paint options.
		 * \param widget The widget.
		 */
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

	signals:

		//! \brief The highlight state changed.
		void highlighted(PipelineConnectionState);

	protected:

		/*!
		 * \brief Gets the connection's source position, relative to the connection's center.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		QPointF getSourcePos() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the connection's target position, relative to the connection's center.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		QPointF getTargetPos() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Calculates the rectangle, that displays the denial message.
		 * \return The rectangle.
		 */
		QRectF getDenialRectangle() const;

		//! \brief The highlight state changed.
		void highlightChanged(PipelineConnectionState state) Q_DECL_OVERRIDE;

	private:

		//! \brief The visual source port.
		VisualPort* _sourceVisual;

		//! \brief The visual target port, if one exists.
		VisualPort* _targetVisual;

		//! \brief The target position.
		QPointF _targetPos;

		//! \brief The denial reason, if available.
		QString _denialReason;
	};

}

#endif // BLUEPRINTCONNECTION_H
