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

#ifndef PIPELINECONNECTION_H
#define PIPELINECONNECTION_H

#include "../viewitem.h"

#include <QGraphicsObject>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPainterPath>
#include <QtMath>

namespace ysm
{

	//! \brief Possible connection highlight states.
	enum PipelineConnectionState
	{
		VALID,
		INVALID,
		NONE,
	};

	/*!
	 * \brief Base class for connections in the pipeline. Provides the routing algorithm.
	 * Class is generic, to allow deriving it from a custom base class.
	 * NOTE THAT T MUST IMPLEMENT QGRAPHICSOBJECT AND VIEWITEM!
	 */
	template<typename T> class PipelineConnection : public T
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentObject The parent object.
		 * \param parentView The parent item.
		 */
		PipelineConnection(QGraphicsObject* parentObject, IView* parentView);

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF boundingRect() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Paint the visual pipeline item.
		 * \param painter The painter.
		 * \param option Paint options.
		 * \param widget The widget.
		 */
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the highlight state.
		 * \return The highlight state.
		 */
		PipelineConnectionState getHighlightState() const;

		/*!
		 * \brief Sets the highlight state.
		 * \param state The highlight state.
		 */
		void setHighlightState(const PipelineConnectionState& state);

		/*!
		 * \brief Gets the connection's exact shape.
		 * \return The shape, surrounding the connection.
		 */
		QPainterPath shape() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the connection's (unclosed) path.
		 * \return The path, from source to target position.
		 */
		QPainterPath getPath() const;

	protected:

		/*!
		 * \brief Gets the connection's source position,relative to the parent's coordinate system.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		virtual QPointF getSourcePos() const = 0;

		/*!
		 * \brief Gets the connection's target position, relative to the parent's coordinate system.
		 * Called after layoutChanged(), to recalculate the connection's position and path.
		 * \return The source position.
		 */
		virtual QPointF getTargetPos() const = 0;

		/*!
		 * \brief Get's the source's connection color.
		 * \return The color.
		 */
		virtual QColor getSourceColor() const;

		/*!
		 * \brief Get's the target's connection color.
		 * \return The color.
		 */
		virtual QColor getTargetColor() const;

		//! \brief Updates the connection.
		virtual void layoutChanged();

		//! \brief The highlight state changed.
		virtual void highlightChanged(PipelineConnectionState);

	private:

		//! \brief The last calculated source and target positions.
		QPointF _sourcePos, _targetPos;

		//! \brief The current highlight state.
		PipelineConnectionState _highlightState;
	};
}

#include "views/common/ysmpalette.h"

#include <QPen>
#include <QPainter>

#define SELECTION_OFFSET 5
#define CONNECTION_CURVINESS 7.5

//Implement the template methods in header file.
namespace ysm
{

	template<typename T> PipelineConnection<T>::PipelineConnection(QGraphicsObject* parentObject,
																   IView* parentView) :
		T(parentObject, parentView),
		_sourcePos(0, 0),
		_targetPos(0, 0),
		_highlightState(NONE)
	{ }

	template<typename T> QColor PipelineConnection<T>::getSourceColor() const { return Qt::white; }
	template<typename T> QColor PipelineConnection<T>::getTargetColor() const { return Qt::white; }

	template<typename T> void PipelineConnection<T>::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
														   QWidget* widget)
	{
		Q_UNUSED(widget);
		Q_UNUSED(option);

		//Adjust the appearance.
		QPen connectionPen = QPen(Qt::white);
		if(_highlightState == PipelineConnectionState::VALID)
			connectionPen.setColor(YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Healthy));
		else if(_highlightState == PipelineConnectionState::INVALID)
			connectionPen.setColor(YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Sick));
		else if(!this->isSelected())
		{
			//Gradient from source to target.
			QLinearGradient gradient(_sourcePos, _targetPos);
			gradient.setColorAt(0, getSourceColor());
			gradient.setColorAt(1, getTargetColor());
			connectionPen.setBrush(QBrush(gradient));
		}

		connectionPen.setWidth(2);
		connectionPen.setJoinStyle(Qt::RoundJoin);

		//Draw the connection stroke.
		painter->setRenderHint(QPainter::Antialiasing);
		painter->setPen(connectionPen);
		painter->drawPath(getPath());
	}

	template<typename T> void PipelineConnection<T>::layoutChanged()
	{
		//Get new source and target positions.
		this->prepareGeometryChange();

		//Get the new source and target pos, relative to the current position.
		QPointF sourcePos = getSourcePos();
		QPointF targetPos = getTargetPos();

		//Adjust the position to fire redraw. A connection's position is right in the center between source and target.
		//Therefore, calculate the source and target position in parent coordinates.
		QRectF parentBoundingRect(sourcePos, targetPos);
		this->setPos(parentBoundingRect.center());

		//Get the new source and target pos, relative to the new position.
		_sourcePos = this->mapFromParent(getSourcePos());
		_targetPos = this->mapFromParent(getTargetPos());
	}

	template<typename T> void PipelineConnection<T>::highlightChanged(PipelineConnectionState) { }

	template<typename T> QRectF PipelineConnection<T>::boundingRect() const
	{
		//Use path to get rect.
		return getPath().boundingRect();
	}

	template<typename T> PipelineConnectionState PipelineConnection<T>::getHighlightState() const
	{
		return _highlightState;
	}

	template<typename T> void PipelineConnection<T>::setHighlightState(const PipelineConnectionState& state)
	{
		//Set new highlight state and redraw.
		if(_highlightState != state)
		{
			highlightChanged(_highlightState = state);
			this->update();
		}
	}

	template<typename T> QPainterPath PipelineConnection<T>::getPath() const
	{
		//Start from source draw simple bezier to target.
		double heightDifference = qSqrt(qAbs(_sourcePos.y() - _targetPos.y())) * CONNECTION_CURVINESS;
		QPainterPath path(QPointF(_sourcePos.x(), _sourcePos.y()));
		path.cubicTo(qMax(0.0, _sourcePos.x() + heightDifference), _sourcePos.y(),
					 qMin(0.0, _targetPos.x() - heightDifference), _targetPos.y(),
					 _targetPos.x(), _targetPos.y());

		return path;
	}

	template<typename T> QPainterPath PipelineConnection<T>::shape() const
	{
		//Create shape, get path.
		QPainterPath path = getPath();

		//Start beneath source port.
		QPainterPath shape(QPointF(_sourcePos.x(), _sourcePos.y() + SELECTION_OFFSET));
		QPointF lastPoint = _sourcePos;

		//Iterate forwards, offset to one side.
		for(int i = 0; i <= 100; i += 5)
		{
			QPointF nextPoint = path.pointAtPercent(i / 100.0);
			double currentAngle = qAtan2(nextPoint.x() - lastPoint.x(), nextPoint.y() - lastPoint.y());
			shape.lineTo(nextPoint.x() - qCos(currentAngle) * SELECTION_OFFSET,
						nextPoint.y() + qSin(currentAngle) * SELECTION_OFFSET);
			lastPoint =	nextPoint;
		}

		//Continue above target port.
		shape.lineTo(_targetPos.x(), _targetPos.y() - SELECTION_OFFSET);
		lastPoint = _targetPos;

		//Iterate backwards, offset to the other side.
		for(int i = 100; i >= 0; i -= 5)
		{
			QPointF nextPoint = path.pointAtPercent(i / 100.0);
			double currentAngle = qAtan2(nextPoint.x() - lastPoint.x(), nextPoint.y() - lastPoint.y());
			shape.lineTo(nextPoint.x() - qCos(currentAngle) * SELECTION_OFFSET,
						nextPoint.y() + qSin(currentAngle) * SELECTION_OFFSET);
			lastPoint =	nextPoint;
		}

		return shape;
	}
}

#endif // PIPELINECONNECTION_H
