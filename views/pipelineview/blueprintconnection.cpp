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

#include "blueprintconnection.h"
#include "pipelinescene.h"
#include "views/common/ysmpalette.h"
#include "visualitems/visualblock.h"
#include "visualitems/visualport.h"

#include <QPainter>
#include <QDebug>

using namespace ysm;

BlueprintConnectionBase::BlueprintConnectionBase(QGraphicsObject* parentObject, IView* parentView) :
	QGraphicsObject(parentObject),
	ViewItem(parentView)
{ }

BlueprintConnection::BlueprintConnection(VisualPort* sourceVisual) :
	PipelineConnection(NULL, sourceVisual),
	_sourceVisual(sourceVisual),
	_targetVisual(NULL),
	_targetPos(sourceVisual->pos())
{
	//Render in foreground.
	setZValue(1);
}

QPointF BlueprintConnection::getSourcePos() const
{
	//Return the source visual's position, if the source is an out port.
	if(_sourceVisual->getPort()->isOut())
		return _sourceVisual->getVisualBlock()->mapToParent(_sourceVisual->pos());

	//Otherwise return the target position.
	return _targetPos;
}

QPointF BlueprintConnection::getTargetPos() const
{
	//Return the source visual's position, if the source is an in port.
	if(_sourceVisual->getPort()->isIn())
		return _sourceVisual->getVisualBlock()->mapToParent(_sourceVisual->pos());

	//Otherwise return the target position.
	return _targetPos;
}

void BlueprintConnection::setTargetPos(const QPointF &targetPos)
{
	//Get the underlying data port.
	IPort* sourcePort = _sourceVisual->getPort();

	//Set the new target position.
	//Adjust x value by 5 px, to avoid that the connection itself is retrieved by itemAt().
	_targetPos = QPointF(targetPos.x() + (sourcePort->isIn() ? 5 : -5), targetPos.y());

	//Clear the denial reason.
	_denialReason.clear();

	//Check if there is a target visual at the current target position.
	PipelineScene* pipelineScene = qobject_cast<PipelineScene*>(scene());
	if(pipelineScene)
	{
		//Leave the current target port.
		if(_targetVisual)
		{
			setHighlightState(PipelineConnectionState::NONE);
			_targetVisual->setBlueprintConnection(NULL);
			_targetVisual = NULL;
		}

		//Get the new target port.
		QGraphicsItem* targetVisual = pipelineScene->itemAt(targetPos, QTransform());
		if(targetVisual && targetVisual != _sourceVisual && targetVisual->type() == PipelineScene::VisualPortType)
		{
			//Get the visual port and verify it.
			_targetVisual = qgraphicsitem_cast<VisualPort*>(targetVisual);
			if(_targetVisual)
			{
				//Get the underlying data port.
				IPort* targetPort = _targetVisual->getPort();

				//Ensure the target visual is supported.
				bool forwardDirection = targetPort->isIn() && sourcePort->isOut();
				bool backwardDirection = sourcePort->isIn() && targetPort->isOut();
				bool sameBlock = sourcePort->getBlock() == targetPort->getBlock();
				if((forwardDirection || backwardDirection) && targetPort->isSupported() && !sameBlock)
				{
					//Adjust the position to click to the target and highlight the target.
					_targetPos = _targetVisual->parentItem()->mapToParent(_targetVisual->pos());
					setHighlightState(isValidTarget(_targetVisual) ? PipelineConnectionState::VALID :
																	 PipelineConnectionState::INVALID);

					//Notify the target about the connection.
					_targetVisual->setBlueprintConnection(this);
				}
			}
		}
	}

	//Redraw the connection.
	layoutChanged();
}

void BlueprintConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//Use base algorithm.
	PipelineConnection::paint(painter, option, widget);

	//Check for denial reason.
	if(!_denialReason.isEmpty())
	{
		//Calculate denial rectangle.
		QRectF denialRectangle = getDenialRectangle();

		//Adjust font to small and white.
		QFont labelFont = painter->font();
		labelFont.setPointSizeF(labelFont.pointSizeF() * 0.8);
		painter->setPen(QPen(Qt::white));
		painter->setFont(labelFont);

		//Fill rectangle, then paint text.
		painter->fillRect(denialRectangle, YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Sick));
		painter->drawText(denialRectangle, _denialReason, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
	}
}

QRectF BlueprintConnection::boundingRect() const
{
	//Get base rectangle, use minimum size of denial rectangle.
	return PipelineConnection::boundingRect().united(getDenialRectangle());
}

QRectF BlueprintConnection::getDenialRectangle() const
{
	//Calculate the rectangle.
	QRectF denialRectangle(-100, -20, 200, 40);
	denialRectangle.moveCenter(getPath().pointAtPercent(0.5));

	//If the connection	is only three points,
	return denialRectangle;
}

bool BlueprintConnection::isValidTarget(VisualPort *targetPort)
{
	//Determine which block is the actual source/target of the connection, then run the port checks.
	if (_sourceVisual->getPort()->isOut())
		return _sourceVisual->getVisualBlock()->getBlock()->
				canAcceptConnection(_sourceVisual->getPort(), targetPort->getPort(), _denialReason);
	else
		return targetPort->getVisualBlock()->getBlock()->
				canAcceptConnection(targetPort->getPort(), _sourceVisual->getPort(), _denialReason);
}

void BlueprintConnection::highlightChanged(PipelineConnectionState state) { emit highlighted(state); }

VisualPort* BlueprintConnection::getSourceVisual() const { return _sourceVisual; }
VisualPort* BlueprintConnection::getTargetVisual() const { return _targetVisual; }
