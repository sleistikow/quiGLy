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

#include "visualconnection.h"
#include "../pipelinescene.h"
#include "../pipelinescenelayouter.h"
#include "views/common/ysmpalette.h"
#include "visualblock.h"
#include "visualport.h"
#include "commands/iuicommandqueue.h"
#include "commands/pipeline/delete/deleteconnectionscommand.h"

#include <QPainter>
#include <QDebug>
#include <QtGlobal>

using namespace ysm;

VisualConnection::VisualConnection(IConnection* connection, IView* parentView) :
	PipelineConnection(NULL, parentView),
	_connection(connection),
	_sourceVisual(NULL),
	_targetVisual(NULL)
{
	//Render in background.
	setZValue(-1);
}

void VisualConnection::visualBlocksCreated()
{
	//Try to access the parent scene.
	PipelineScene* pipelineScene = qobject_cast<PipelineScene*>(scene());
	if(pipelineScene)
	{
		//Try to access the source block.
		VisualBlock* sourceBlock = qobject_cast<VisualBlock*>(pipelineScene->getVisualItem(_connection->getSource()));
		if(sourceBlock)
		{
			//Access the source port, and listen to changes.
			_sourceVisual = sourceBlock->getVisualPort(_connection->getSourcePort());
			connect(sourceBlock, &VisualBlock::xChanged, this, &VisualConnection::layoutChanged);
			connect(sourceBlock, &VisualBlock::yChanged, this, &VisualConnection::layoutChanged);
		}

		//Try to access the target block.
		VisualBlock* targetBlock = qobject_cast<VisualBlock*>(pipelineScene->getVisualItem(_connection->getDest()));
		if(targetBlock)
		{
			//Access the target port, and listen to changes.
			_targetVisual = targetBlock->getVisualPort(_connection->getDestPort());
			connect(targetBlock, &VisualBlock::xChanged, this, &VisualConnection::layoutChanged);
			connect(targetBlock, &VisualBlock::yChanged, this, &VisualConnection::layoutChanged);
		}

		//Initial layout update.
		layoutChanged();
	}
}

QPointF VisualConnection::getSourcePos() const
{
	//If the source visual exists, calculate it's position in local coordinates.
	if(_sourceVisual)
		return _sourceVisual->getVisualBlock()->mapToParent(_sourceVisual->pos());

	//No visual available.
	return QPointF();
}

QPointF VisualConnection::getTargetPos() const
{
	//If the target visual exists, calculate it's position in local coordinates.
	if(_targetVisual)
		return _targetVisual->getVisualBlock()->mapToParent(_targetVisual->pos());

	//No visual available.
	return QPointF();
}

QColor VisualConnection::getSourceColor() const
{
	//If the source visual exists, use it's color.
	if(_sourceVisual)
		return YSMPalette::getBlockColor(_sourceVisual->getVisualBlock()->getBlock()->getType());

	//Otherwise return the target position.
	return PipelineConnection::getSourceColor();
}

QColor VisualConnection::getTargetColor() const
{
	//Return the source visual's color, if the source is an out port.
	if(_targetVisual)
		return YSMPalette::getBlockColor(_targetVisual->getVisualBlock()->getBlock()->getType());

	//Otherwise return the default color.
	return PipelineConnection::getTargetColor();
}

void VisualConnection::restoreFromLayouter(PipelineSceneLayouter* layouter)
{
	//Restore the position.
	VisualPipelineItem::restoreFromLayouter(layouter);
}

IPipelineItem* VisualConnection::getPipelineItem() const { return _connection; }
IConnection* VisualConnection::getConnection() const { return _connection; }

int VisualConnection::type() const { return PipelineScene::VisualConnectionType; }

void VisualConnection::deleteItem()
{
	//Just execute the delete command.
	executeCommand(new DeleteConnectionsCommand(_connection->getPipeline(), _connection));
}

QRectF VisualConnection::calculateBoundingRect() const
{
	//Bounding rect behaviour is overriden, so this method should not be called.
	//Just in case return the actual bounding rect.
	return PipelineConnection<VisualPipelineItem>::boundingRect();
}
