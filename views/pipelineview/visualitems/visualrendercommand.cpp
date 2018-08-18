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

#include "visualrendercommand.h"
#include "visualblock.h"
#include "views/common/ysmpalette.h"
#include "../pipelinescene.h"
#include "../pipelinescenelayouter.h"
#include "data/properties/property.h"
#include "commands/iuicommandqueue.h"
#include "commands/pipeline/delete/deleterendercommandscommand.h"
#include "commands/pipeline/change/renamepipelineitemcommand.h"

#include <QtMath>

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsSimpleTextItem>

#define CONNECTION_CURVINESS 7.5

#define ITEM_WIDTH 80
#define ITEM_HEIGHT 40
#define ITEM_PADDING QMarginsF(10, 5, 10, 5)
#define ITEM_OFFSET 80

using namespace ysm;

VisualRenderCommand::RenderCommandConnection::RenderCommandConnection(VisualRenderCommand* parentItem) :
	QGraphicsObject(parentItem),
	_parentVisual(parentItem),
	_blockVisual(NULL)
{
	//Hide behind everything. Shape is inaccurate.
	setFlag(ItemStacksBehindParent, true);
	setZValue(-2);

	//Watch for movements to update the connection.
	connect(_parentVisual, &VisualRenderCommand::xChanged, this, &RenderCommandConnection::updatePath);
	connect(_parentVisual, &VisualRenderCommand::yChanged, this, &RenderCommandConnection::updatePath);
}

void VisualRenderCommand::RenderCommandConnection::setBlock(VisualBlock* visualBlock)
{
	//Store the target block.
	_blockVisual = visualBlock;

	//Watch for movements to update the connection.
	connect(_blockVisual, &VisualBlock::xChanged, this, &RenderCommandConnection::updatePath);
	connect(_blockVisual, &VisualBlock::yChanged, this, &RenderCommandConnection::updatePath);
	connect(_blockVisual, &VisualBlock::heightChanged, this, &RenderCommandConnection::updatePath);

	//Update manually once.
	updatePath();
}

QRectF VisualRenderCommand::RenderCommandConnection::boundingRect() const { return getPath().boundingRect(); }
void VisualRenderCommand::RenderCommandConnection::updatePath()
{
	//Do not update if block is missing.
	if(!_blockVisual) return;

	//Prepare geometry change to avoid artifacts.
	prepareGeometryChange();

	//Calculate source and target position.
	_sourcePos = QPointF(_parentVisual->boundingRect().center().x(), _parentVisual->boundingRect().bottom());
	_targetPos = QPointF(_blockVisual->boundingRect().center().x(), _blockVisual->boundingRect().top());
	_targetPos = mapFromItem(_blockVisual, _targetPos);

	//Then update.
	update();
}

QPainterPath VisualRenderCommand::RenderCommandConnection::getPath() const
{
	//Ensure the block exists.
	if(!_blockVisual) return QPainterPath();

	//Start from source draw simple bezier to target.
	double widthDifference = qSqrt(qAbs(_sourcePos.x() - _targetPos.x())) * CONNECTION_CURVINESS;
	QPointF center = QRectF(_sourcePos, _targetPos).center();

	QPainterPath path(QPointF(_sourcePos.x(), _sourcePos.y()));
	path.cubicTo(_sourcePos.x(), qMax(center.y(), _sourcePos.y() + widthDifference),
				 _targetPos.x(), qMin(center.y(), _targetPos.y() - widthDifference),
				 _targetPos.x(), _targetPos.y());

	return path;
}

void VisualRenderCommand::RenderCommandConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
														 QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Do not paint if block is missing.
	if(!_blockVisual) return;

	//Gradient from source to target.
	QLinearGradient gradient(_sourcePos, _targetPos);
	gradient.setColorAt(0, YSMPalette::getRenderCommandColor(_parentVisual->getRenderCommand()->getCommand()));
	gradient.setColorAt(1, YSMPalette::getBlockColor(_blockVisual->getBlock()->getType()));

	QPen connectionPen(QBrush(gradient), 2);
	connectionPen.setJoinStyle(Qt::RoundJoin);

	//Draw the connection stroke.
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(connectionPen);
	painter->drawPath(getPath());
}

VisualRenderCommand::VisualRenderCommand(IRenderCommand* renderCmd, IView* parentView) :
	VisualNamedItem(parentView),
	_renderCommand(renderCmd)
{
	//Create the connection to the block.
	_commandConnection = new RenderCommandConnection(this);
}

void VisualRenderCommand::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	//Get the item's inner alignment.
	float headerHeight = getHeaderHeight();
	float messageHeight = getMessageHeight();

	//Get the item rectangles. Inset bounding box, because outline of 5px will draw 2.5px outside.
	QRectF insetBoundingBox = boundingRect().marginsRemoved(QMarginsF(2.5, 2.5, 2.5, 2.5));
	QRectF headerRect = insetBoundingBox.adjusted(0, 0, 0, -ITEM_HEIGHT - messageHeight);
	QRectF messageRect = insetBoundingBox.adjusted(0, headerHeight + ITEM_HEIGHT, 0, 0);
	QRectF contentRect = insetBoundingBox.adjusted(0, headerHeight, -messageHeight, 0);
	QRectF blockRect = insetBoundingBox.adjusted(0, 0, -messageHeight, 0);

	//Use antialiasing.
	painter->setRenderHint(QPainter::Antialiasing);

	//Check if block holds message.
	QString statusMessage = _renderCommand->getProperty<StringProperty>(PropertyID::MessageLog)->getValue();
	if(!statusMessage.isEmpty())
	{
		//Get the message color.
		QColor messageColor = YSMPalette::getPipelineItemStatusColor(_renderCommand->getStatus());

		//Use smaller font.
		QFont font;
		font.setPixelSize(10);
		painter->setFont(font);

		//Adjust the message rect.
		QRectF messageBackgroundRect = messageRect.adjusted(0, -15, 0, 0);

		//Draw the message rect and message.
		painter->fillRect(messageBackgroundRect.marginsRemoved(QMarginsF(2, 2, 2, 2)), QBrush(messageColor));

		painter->setPen(QPen(messageColor.lightness() > 128 ? Qt::black : Qt::white));
		painter->drawText(messageRect.marginsRemoved(ITEM_PADDING), statusMessage,
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

	//Specify the item color.
	QColor itemColor = YSMPalette::getRenderCommandColor(_renderCommand->getCommand());
	if(isSelected())
		itemColor.setHsl(itemColor.hslHue(), itemColor.hslSaturation(), 255 - (255 - itemColor.lightness()) * 0.5);

	//Draw the content rect and name.
	painter->setClipping(true);
	painter->fillRect(blockRect, QBrush(itemColor));

	if(!isEditingName())
	{
		painter->setPen(QPen(itemColor.lightness() > 128 ? Qt::black : Qt::white));
		painter->drawText(headerRect.marginsRemoved(ITEM_PADDING), _renderCommand->getName(),
						  QTextOption(Qt::AlignCenter));
	}

	//Draw the block's identifier circle.
	QPainterPath identifierCircle;
	identifierCircle.addEllipse(contentRect.center(), 14, 14);
	painter->fillPath(identifierCircle, itemColor.lightness() > 128 ? Qt::black : Qt::white);

	//Draw the block's id.
	painter->setPen(QPen(itemColor));
	painter->drawText(contentRect, QString::number(_renderCommand->getID()), QTextOption(Qt::AlignCenter));

	//Draw the colored border without clip path.
	painter->setClipping(false);
	painter->setPen(QPen(itemColor, 5));
	painter->drawRoundedRect(blockRect, 10, 10);
}

int VisualRenderCommand::type() const { return PipelineScene::VisualCommandType; }

float VisualRenderCommand::getFullWidth() const { return ITEM_WIDTH; }
float VisualRenderCommand::getInnerWidth() const { return ITEM_WIDTH - ITEM_PADDING.left() - ITEM_PADDING.right(); }
float VisualRenderCommand::getContentHeight() const { return ITEM_HEIGHT; }

IPipelineItem* VisualRenderCommand::getPipelineItem() const { return _renderCommand; }
IRenderCommand* VisualRenderCommand::getRenderCommand() const { return _renderCommand; }

void VisualRenderCommand::visualBlocksCreated()
{
	//Try to access the visual block.
	VisualPipelineItem* visualItem = getPipelineScene()->getVisualItem(getRenderCommand()->getAssignedBlock());
	VisualBlock* visualBlock = qobject_cast<VisualBlock*>(visualItem);

	//Store block to connection, if available.
	if(visualBlock)
		_commandConnection->setBlock(visualBlock);
}

void VisualRenderCommand::deleteItem()
{
	//Just execute the delete command.
	executeCommand(new DeleteRenderCommandsCommand(_renderCommand->getPipeline(), _renderCommand));
}

QPointF VisualRenderCommand::getPreferredPosition()
{
	//Access the block that this command is connected to.
	IBlock* block = _renderCommand->getAssignedBlock();
	if(block)
	{
		//Access the visual block, then locate relative to it.
		VisualBlock* visualBlock = qgraphicsitem_cast<VisualBlock*>(getPipelineScene()->getVisualItem(block));
		if(visualBlock)
			return visualBlock->pos() + QPointF(.0f, visualBlock->boundingRect().top() - ITEM_OFFSET);
	}

	//If the block's position could not be accessed, use the base implementation.
	return VisualLayoutedItem::getPreferredPosition();
}

