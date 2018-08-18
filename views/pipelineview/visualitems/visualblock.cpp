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

#include "visualblock.h"
#include "visualport.h"
#include "../pipelinescene.h"
#include "../pipelinescenelayouter.h"
#include "views/common/ysmpalette.h"
#include "data/properties/property.h"
#include "data/iconnection.h"
#include "commands/iuicommandqueue.h"
#include "commands/pipeline/delete/deleteblockscommand.h"
#include "commands/pipeline/change/renamepipelineitemcommand.h"

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

//TODO: Size not hardcoded.
#define BLOCK_WIDTH 168
#define PORT_HEIGHT 24
#define PORT_SPACER 8
#define BLOCK_PADDING QMarginsF(5, 5, 5, 5)
#define PORT_OFFSET 10
#define X_OFFSET 280
#define Y_OFFSET 200

using namespace ysm;

VisualBlock::VisualBlock(IBlock* block, IView* parentView) :
	VisualNamedItem(parentView),
	_block(block)
{
	//Add in ports corresponding to the block.
	QVector<IPort*> inPorts = _block->getInPorts();
	for(int i = 0; i < inPorts.count(); i++)
	{
		//Create the port.
		VisualPort* inPort = new VisualPort(inPorts[i], this);

		//Calculate it's position.
		float yPos = -(inPorts.count() / 2.0f - 0.5f - i) * (PORT_HEIGHT + PORT_SPACER);
		inPort->setPos(-BLOCK_WIDTH / 2 - PORT_OFFSET, yPos);
	}

	//Add out ports corresponding to the block.
	QVector<IPort*> outPorts = _block->getOutPorts();
	for(int i = 0; i < _block->getOutPorts().count(); i++)
	{
		//Create the port.
		VisualPort* inPort = new VisualPort(outPorts[i], this);

		//Calculate it's position.
		float yPos = -(outPorts.count() / 2.0f - 0.5f - i) * (PORT_HEIGHT + PORT_SPACER);
		inPort->setPos(BLOCK_WIDTH / 2 + PORT_OFFSET, yPos);
	}

	//Listen to pipeline version changes.
	notifyStatic(IChangeable::Change, this, &VisualBlock::updateVersion);
}

float VisualBlock::getFullWidth() const { return BLOCK_WIDTH; }
float VisualBlock::getInnerWidth() const { return BLOCK_WIDTH - BLOCK_PADDING.left() - BLOCK_PADDING.right(); }

float VisualBlock::getContentHeight() const
{
	//Calculate the height depending on the number of ports.
	int maxPortCount = qMax(_block->getInPorts().count(), _block->getOutPorts().count());
	return PORT_SPACER + maxPortCount * (PORT_HEIGHT + PORT_SPACER);
}

void VisualBlock::paintContents(QPainter* painter, QRectF boundingRect)
{
	//Draw default content visualization, just a simple background.
	QLinearGradient linearGradient(0, boundingRect.top(), 0, boundingRect.bottom());
	linearGradient.setColorAt(0, QColor(230, 230, 230));
	linearGradient.setColorAt(1, QColor(255, 255, 255));
	painter->fillRect(boundingRect, QBrush(linearGradient));
}

void VisualBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	//Get the block's inner alignment.
	float headerHeight = getHeaderHeight();
	float contentHeight = getContentHeight();
	float messageHeight = getMessageHeight();

	//Get the block rectangles. Inset bounding box, because outline of 5px will draw 2.5px outside.
	QRectF insetBoundingBox = boundingRect().marginsRemoved(QMarginsF(2.5, 2.5, 2.5, 2.5));
	QRectF headerRect = insetBoundingBox.adjusted(0, 0, 0, -contentHeight - messageHeight);
	QRectF messageRect = insetBoundingBox.adjusted(0, headerHeight + contentHeight, 0, 0);
	QRectF contentRect = insetBoundingBox.adjusted(0, headerHeight, 0, -messageHeight);
	QRectF blockRect = insetBoundingBox.adjusted(0, 0, 0, -messageHeight);

	//Use antialiasing.
	painter->setRenderHint(QPainter::Antialiasing);

	//Check if block holds message.
	QString statusMessage = _block->getProperty<StringProperty>(PropertyID::MessageLog)->getValue();
	if(!statusMessage.isEmpty())
	{
		//Get the message color.
		QColor messageColor = YSMPalette::getPipelineItemStatusColor(_block->getStatus());

		//Use smaller font.
		QFont font;
		font.setPixelSize(10);
		painter->setFont(font);

		//Adjust the message rect.
		QRectF messageBackgroundRect = messageRect.adjusted(0, -15, 0, 0);

		//Draw the message rect and message.
		painter->fillRect(messageBackgroundRect.marginsRemoved(QMarginsF(2, 2, 2, 2)), QBrush(messageColor));

		painter->setPen(QPen(messageColor.lightness() > 128 ? Qt::black : Qt::white));
		painter->drawText(messageRect.marginsRemoved(BLOCK_PADDING), statusMessage,
						  QTextOption(Qt::AlignCenter));

		//Draw outline without clip for antialiasing.
		painter->setPen(QPen(messageColor, 5));
		painter->drawRoundedRect(messageBackgroundRect, 10, 10);
	}

	//Specify the clipping path to be a rounded rect and set up painter.
	QPainterPath clipPath;
	clipPath.addRoundedRect(blockRect, 10, 10);
	painter->setClipPath(clipPath);
	painter->setFont(QFont());

	//Specify the block color.
	QColor blockColor = YSMPalette::getBlockColor(_block->getType());
	if(!_block->isSupported())
		blockColor =  YSMPalette::getInactiveColor();
	if(isSelected())
		blockColor.setHsl(blockColor.hslHue(), blockColor.hslSaturation(), 255 - (255 - blockColor.lightness()) * 0.5);

	//Draw the contents.
	paintContents(painter, contentRect);

	//Draw the header rect and name.
	painter->setClipping(true);
	painter->fillRect(headerRect, QBrush(blockColor));

	if(!isEditingName())
	{
		painter->setPen(QPen(blockColor.lightness() > 128 ? Qt::black : Qt::white));
		painter->drawText(headerRect.marginsRemoved(BLOCK_PADDING), _block->getName(), QTextOption(Qt::AlignCenter));
	}

	//Draw the block's identifier circle.
	QPainterPath identifierCircle;
	identifierCircle.addEllipse(contentRect.center(), 14, 14);
	painter->fillPath(identifierCircle, blockColor);

	//Draw the block's id.
	painter->setPen(QPen(blockColor.lightness() > 128 ? Qt::black : Qt::white));
	painter->drawText(contentRect, QString::number(_block->getID()), QTextOption(Qt::AlignCenter));

	//Draw the colored border without clip path.
	painter->setClipping(false);
	painter->setPen(QPen(blockColor, 5));
	painter->drawRoundedRect(blockRect, 10, 10);
}

VisualPort* VisualBlock::getVisualPort(IPort *port) const
{
	//Iterate over the children.
	foreach(QGraphicsItem* childItem, childItems())
		if(childItem->type() == PipelineScene::VisualPortType)
		{
			VisualPort* visualPort = qgraphicsitem_cast<VisualPort*>(childItem);
			if(visualPort->getPort() == port)
				return visualPort;
		}

	//No port found.
	return NULL;
}

IPipelineItem* VisualBlock::getPipelineItem() const { return _block; }
IBlock* VisualBlock::getBlock() const { return _block; }

int VisualBlock::type() const { return PipelineScene::VisualBlockType; }

void VisualBlock::deleteItem()
{
	//Just execute the delete command.
	executeCommand(new DeleteBlocksCommand(_block->getPipeline(), _block));
}

QPointF VisualBlock::getPreferredPosition()
{
	//Get base preferred position, it's used as fallback.
	QPointF fallbackPosition = VisualLayoutedItem::getPreferredPosition();

	//Check if block has incoming connections and calculate the center position of all incoming blocks.
	QPointF incomingCenter;
	int incomingCenterBlocks = 0;

	//Iterate over the connections.
	foreach(IConnection* connection, _block->getInConnections())
	{
		//Try to access the source block.
		IBlock* block = connection->getSource();
		if(block)
		{
			//Try to access the visual source block and use it's position.
			VisualBlock* visualBlock = qgraphicsitem_cast<VisualBlock*>(getPipelineScene()->getVisualItem(block));
			if(visualBlock)
			{
				incomingCenter += visualBlock->pos() + QPointF(X_OFFSET, .0f);
				incomingCenterBlocks++;
			}

			/*
			//Adjust the y position, using this block's index on its incoming connection.
			int targetCount = block->getOutConnections().count();
			for(int i = 0; i < targetCount; i++)
				if(block->getOutConnections()[i] == connection)
				{
					incomingCenter += QPointF(.0f, 2 * Y_OFFSET * (i - (targetCount - 1) / 2.f));
					break;
				}
			*/
		}
	}

	//Calculate the center position.
	if(incomingCenterBlocks) incomingCenter /= incomingCenterBlocks;

	//Check if block has outgoing connections and calculate the center position of all outgoing blocks.
	QPointF outgoingCenter;
	int outgoingCenterBlocks = 0;

	//Iterate over the connections.
	foreach(IConnection* connection, _block->getOutConnections())
	{
		//Try to access the source block.
		IBlock* block = connection->getDest();
		if(block)
		{
			//Try to access the visual source block and use it's position.
			VisualBlock* visualBlock = qgraphicsitem_cast<VisualBlock*>(getPipelineScene()->getVisualItem(block));
			if(visualBlock)
			{
				outgoingCenter += visualBlock->pos() - QPointF(X_OFFSET, .0f);
				outgoingCenterBlocks++;
			}

			/*
			//Adjust the y position, using this block's index on its incoming connection.
			int targetCount = block->getInConnections().count();
			for(int i = 0; i < targetCount; i++)
				if(block->getInConnections()[i] == connection)
				{
					outgoingCenter += QPointF(.0f, 2 * Y_OFFSET * (i - (targetCount - 1) / 2.f));
					break;
				}
			*/
		}
	}

	//Calculate the center position.
	if(outgoingCenterBlocks) outgoingCenter /= outgoingCenterBlocks;

	//Depending on the availability of the results, calculate the final position.
	if(outgoingCenterBlocks && incomingCenterBlocks) return (incomingCenter + outgoingCenter) / 2;
	if(outgoingCenterBlocks) return outgoingCenter;
	if(incomingCenterBlocks) return incomingCenter;
	return fallbackPosition;
}

void VisualBlock::updateVersion(IPipeline* pipeline) { Q_UNUSED(pipeline); update(); }
