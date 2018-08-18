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

#include "visualport.h"
#include "visualblock.h"
#include "../../document.h"
#include "../pipelinescene.h"
#include "../blueprintconnection.h"
#include "views/common/ysmpalette.h"
#include "commands/iuicommandqueue.h"
#include "commands/pipeline/add/addconnectioncommand.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

using namespace ysm;

VisualPort::VisualPort(IPort* port, VisualBlock* parent) :
	VisualPipelineItem(parent, parent),
	_port(port),
	_sourcingBlueprint(NULL),
	_targetingBlueprint(NULL)
{
	//Ports cannot be selected.
	setFlag(ItemIsFocusable, false);

	//Listen to pipeline version changes.
	notifyStatic(IChangeable::Change, this, &VisualPort::updateVersion);
}

QRectF VisualPort::calculateBoundingRect() const
{
	//TODO: Size not hardcoded.
	if(_port->isIn())
		return QRectF(QPointF(-5, -12), QSizeF(80, 24));
	if(_port->isOut())
		return QRectF(QPointF(-75, -12), QSizeF(80, 24));

	//Should not happen.
	return QRectF();
}

void VisualPort::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	//Get the port rectangle.
	QRectF insetBoundingBox = boundingRect();

	//Draw the port background, using the blueprint's state, if available.
	QBrush brush(YSMPalette::getBlockColor(getVisualBlock()->getBlock()->getType()));

	if(!_port->isSupported())
		brush.setColor(YSMPalette::getInactiveColor());
	if(_sourcingBlueprint || _targetingBlueprint)
		brush.setColor(Qt::white);
	if((_sourcingBlueprint && _sourcingBlueprint->getHighlightState() == PipelineConnectionState::VALID) ||
	   (_targetingBlueprint && _targetingBlueprint->getHighlightState() == PipelineConnectionState::VALID))
		brush.setColor(YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Healthy));
	if((_sourcingBlueprint && _sourcingBlueprint->getHighlightState() == PipelineConnectionState::INVALID) ||
	   (_targetingBlueprint && _targetingBlueprint->getHighlightState() == PipelineConnectionState::INVALID))
		brush.setColor(YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Sick));

	//Create the shape. TODO: Cache the shape.
	QPainterPath portArrow(insetBoundingBox.topLeft());

	//Right bounds.
	portArrow.lineTo(insetBoundingBox.right() - 5, insetBoundingBox.top());
	portArrow.lineTo(insetBoundingBox.right(), insetBoundingBox.center().y());
	portArrow.lineTo(insetBoundingBox.right() - 5, insetBoundingBox.bottom());

	//Left bounds.
	portArrow.lineTo(insetBoundingBox.left(), insetBoundingBox.bottom());
	portArrow.lineTo(insetBoundingBox.left() + 5, insetBoundingBox.center().y());

	//Fill the port.
	painter->setRenderHint(QPainter::Antialiasing);
	painter->fillPath(portArrow, brush);

	//Set small default font.
	QFont font = painter->font();
	font.setPixelSize(9);
	painter->setFont(font);

	//Draw the name.
	painter->setPen(QPen(brush.color().lightness() > 128 ? Qt::black : Qt::white));
	painter->drawText(insetBoundingBox.marginsRemoved(QMargins(10, 0, 10, 0)),
					  _port->getName(), QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
}

void VisualPort::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event);

	//Ensure the port is supported.
	if(!_port->isSupported()) return;

	//Check if a blueprint connection has been created.
	PipelineScene* pipelineScene = qobject_cast<PipelineScene*>(scene());
	if(!_sourcingBlueprint && pipelineScene)
	{
		//Create the new connection and add it to the pipeline.
		_sourcingBlueprint = new BlueprintConnection(this);
		pipelineScene->addItem(_sourcingBlueprint);

		//Connect to changes on that connection.
		connect(_sourcingBlueprint, &BlueprintConnection::highlighted, this, &VisualPort::blueprintStateChanged);
		update();
	}
}

void VisualPort::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//If a blueprint connection exists, adjust it's target position.
	//Because the blueprint is always added directly to the scene (see above), the position does not need to be mapped.
	if(_sourcingBlueprint)
		_sourcingBlueprint->setTargetPos(event->scenePos());
}

void VisualPort::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event);

	//Check if a blueprint connection exists.
	PipelineScene* pipelineScene = qobject_cast<PipelineScene*>(scene());
	if(_sourcingBlueprint && pipelineScene)
	{
		//Disconnect from the blueprint connection.
		disconnect(_sourcingBlueprint, &BlueprintConnection::highlighted,
				   this, &VisualPort::blueprintStateChanged);

		//Get the current target port, if it exists.
		VisualPort* targetPort = _sourcingBlueprint->getTargetVisual();
		if(targetPort)
			targetPort->setBlueprintConnection(NULL);

		//Check if port is valid, execute command.
		if(targetPort && _sourcingBlueprint->isValidTarget(targetPort))
			executeCommand(new AddConnectionCommand(_port,  targetPort->_port));

		//Delete the blueprint.
		_sourcingBlueprint->deleteLater();
		_sourcingBlueprint = NULL;

		//Update the port.
		update();
	}
}

QList<QAction*> VisualPort::getContextActions() const { return QList<QAction*>(); }
void VisualPort::blueprintStateChanged(PipelineConnectionState) { invalidateBoundingRect(); update(); }

BlueprintConnection* VisualPort::getBlueprintConnection() const { return _targetingBlueprint; }
void VisualPort::setBlueprintConnection(BlueprintConnection* blueprintConnection)
{
	//Disconnect from old blueprint.
	if(_targetingBlueprint)
		disconnect(_targetingBlueprint, &BlueprintConnection::highlighted,
				   this, &VisualPort::blueprintStateChanged);

	//Connect to new blueprint.
	_targetingBlueprint = blueprintConnection;
	if(_targetingBlueprint)
		connect(_targetingBlueprint, &BlueprintConnection::highlighted,
				this, &VisualPort::blueprintStateChanged);

	//Redraw, because the state might have changed. Also, overlap the connection.
	parentItem()->setZValue(_targetingBlueprint ? 2 : 0);
	update();
}

void VisualPort::deleteItem() { }
void VisualPort::updateVersion(IPipeline* pipeline) { Q_UNUSED(pipeline); update(); }

IPort* VisualPort::getPort() const { return _port; }
IPipelineItem* VisualPort::getPipelineItem() const { return _port; }
VisualBlock* VisualPort::getVisualBlock() const { return qgraphicsitem_cast<VisualBlock*>(parentItem()); }

int VisualPort::type() const { return PipelineScene::VisualPortType; }

