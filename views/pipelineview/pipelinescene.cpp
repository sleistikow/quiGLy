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

#include "pipelinescene.h"
#include "pipelinescenelayouter.h"
#include "blueprintconnection.h"
#include "pipelinetab.h"

#include "commands/uicommandqueue.h"
#include "commands/pipeline/delete/deleteblockscommand.h"
#include "commands/pipeline/delete/deleterendercommandscommand.h"
#include "commands/pipeline/delete/deleteconnectionscommand.h"

#include "visualitems/visualpipelineitem.h"
#include "visualitems/visualpipelineitemfactory.h"
#include "visualitems/visualblock.h"
#include "visualitems/visualconnection.h"
#include "visualitems/visualrendercommand.h"

#include "../document.h"
#include "../registry.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

using namespace ysm;

PipelineScene::PipelineScene(PipelineTab* parentView) :
	QGraphicsScene(parentView),
	ViewItem(parentView),
	_parentView(parentView)
{
	//Create the scene layouter.
	_sceneLayouter = new PipelineSceneLayouter(this);

	//Watch for data changes.
	notifyStatic(this, &PipelineScene::recreatePipelineItems);
	notifyStatic(IChangeable::Add, this, &PipelineScene::createVisualBlock);
	notifyStatic(IChangeable::Add, this, &PipelineScene::createVisualConnection);
	notifyStatic(IChangeable::Add, this, &PipelineScene::createVisualRenderCommand);
	notifyStatic(IChangeable::Remove, this, &PipelineScene::deleteVisualItem);

	//Watch for layout changes.
	connect(_sceneLayouter, &PipelineSceneLayouter::layoutChanged, this, &PipelineScene::updateItemLayout);
	connect(this, &PipelineScene::selectionChanged, this, &PipelineScene::updateActiveItem);
}

void PipelineScene::createVisualBlock(IBlock* pipelineBlock)
{
	//Get the factory used to created the new object.
	VisualPipelineItemFactory* factory = getActiveDocument()->getRegistry()->getVisualFactory();

	//Create the correct visual item.
	VisualBlock* visualItem = factory->createBlockItem(pipelineBlock, this);
	if(visualItem)
	{
		//Register item in data map for fast access.
		_visualItems[pipelineBlock->getID()] = visualItem;

		//Add visual item to scene.
		addItem(visualItem);
		visualItem->restoreLayout();
	}
}

void PipelineScene::createVisualConnection(IConnection* connection)
{
	//Create the visual connection.
	VisualConnection* visualConnection = new VisualConnection(connection, this);

	//Register item in data map for fast access.
	_visualItems[connection->getID()] = visualConnection;

	//Add visual item to scene.
	//TODO: Wait for all blocks, in case a command adds block AND connection!
	addItem(visualConnection);
	visualConnection->visualBlocksCreated();
	visualConnection->restoreLayout();
}

void PipelineScene::createVisualRenderCommand(IRenderCommand* renderCommand)
{
	//Get the factory used to created the new object.
	VisualPipelineItemFactory* factory = getActiveDocument()->getRegistry()->getVisualFactory();

	//Create the correct visual item.
	VisualRenderCommand* visualItem = factory->createRenderCommandItem(renderCommand, this);
	if(visualItem)
	{
		//Register item in data map for fast access.
		_visualItems[renderCommand->getID()] = visualItem;

		//Add visual item to scene.
		//TODO: Wait for all blocks, in case a command adds block AND command!
		addItem(visualItem);
		visualItem->visualBlocksCreated();
		visualItem->restoreLayout();
	}
}

void PipelineScene::recreatePipelineItems()
{
	//Remove all existing objects.
	_visualItems.clear();
	clear();

	//Iterate over existing pipeline blocks to create their visual representations.
	foreach(IBlock* pipelineBlock, getActiveDocument()->getPipeline()->getBlocks())
		createVisualBlock(pipelineBlock);

	//Iterate over existing pipeline blocks to create the visual connections.
	foreach(IBlock* pipelineBlock, getActiveDocument()->getPipeline()->getBlocks())
		foreach(IConnection* connection, pipelineBlock->getOutConnections())
			createVisualConnection(connection);

	//Iterate over existing render commands to create their visual representations.
	foreach(IRenderCommand* renderCommand, getActiveDocument()->getPipeline()->getRenderCommands())
		createVisualRenderCommand(renderCommand);
}

void PipelineScene::deleteVisualItem(IPipelineItem* pipelineItem)
{
	//Simply delete the visual item, this will disconnect it from parent.
	delete getVisualItem(pipelineItem);
}

void PipelineScene::updateItemLayout(IPipelineItem *pipelineItem)
{
	//Find the corresponding visual item and restore it's layout.
	VisualPipelineItem* visualItem = getVisualItem(pipelineItem);
	if(visualItem)
		visualItem->restoreLayout();
}

void PipelineScene::updateItem()
{
	//Find the active visual item.
	VisualPipelineItem* visualItem = getVisualItem(getActiveItem());

	//Select the active item only.
	clearSelection();
	if(visualItem)
		visualItem->setSelected(true);
}

void PipelineScene::deleteSelectedItems()
{
	//Get the selected blocks, by checking their type.
	QList<IBlock*> selectedBlocks;
	foreach(QGraphicsItem* selectedItem, selectedItems())
	{
		//Append selected item to item list.
		if(selectedItem->type() == VisualBlockType)
		{
			//Clear selection from item, to prevent errors.
			selectedBlocks.append(qgraphicsitem_cast<VisualBlock*>(selectedItem)->getBlock());
			selectedItem->setSelected(false);
		}
	}

	//If blocks are selected, delete them.
	if (selectedBlocks.size() > 0)
		executeCommand(new DeleteBlocksCommand(getActiveDocument()->getPipeline(), selectedBlocks));

	//Do the same for commands.
	QList<IRenderCommand*> selectedCommands;
	foreach(QGraphicsItem* selectedItem, selectedItems())
		if(selectedItem->type() == VisualCommandType)
		{
			selectedCommands.append(qgraphicsitem_cast<VisualRenderCommand*>(selectedItem)->getRenderCommand());
			selectedItem->setSelected(false);
		}

	if (selectedCommands.size() > 0)
		executeCommand(new DeleteRenderCommandsCommand(getActiveDocument()->getPipeline(), selectedCommands));

	//Do the same for connections.
	QList<IConnection*> selectedConnections;
	foreach(QGraphicsItem* selectedItem, selectedItems())
		if(selectedItem->type() == VisualConnectionType)
		{
			selectedConnections.append(qgraphicsitem_cast<VisualConnection*>(selectedItem)->getConnection());
			selectedItem->setSelected(false);
		}

	if (selectedConnections.size() > 0)
		executeCommand(new DeleteConnectionsCommand(getActiveDocument()->getPipeline(), selectedConnections));
}

VisualPipelineItem* PipelineScene::getVisualItem(IPipelineItem *dataItem) const
{
	//Return the visual item, if available.
	if(dataItem && _visualItems.contains(dataItem->getID()))
		return _visualItems[dataItem->getID()];
	return NULL;
}

VisualPipelineItem* PipelineScene::getVisualItem(PipelineItemID dataItemID) const
{
	//Return the visual item, if available.
	if(_visualItems.contains(dataItemID))
		return _visualItems[dataItemID];
	return NULL;
}

PipelineTab* PipelineScene::getPipelineTab() const { return _parentView; }
PipelineSceneLayouter* PipelineScene::getLayouter() const { return _sceneLayouter; }

void PipelineScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	//Fill with background color.
	painter->fillRect(rect, QBrush(QColor(207, 207, 196)));

	//TODO: Raster not hardcoded.
	QPen linePen = QPen(QColor(195, 195, 195));
	QPen mainLinePen = QPen(QColor(190, 190, 190), 2);

	//Draw the vertical background raster.
	for(int x = (int) rect.left() / 10 * 10; x < rect.right(); x += 10)
	{
		painter->setPen(x % 90 ? linePen : mainLinePen);
		painter->drawLine(x, rect.top(), x, rect.bottom());
	}

	//Draw the horizontal background raster.
	painter->setPen(linePen);
	for(int y = (int) rect.top() / 10 * 10; y < rect.bottom(); y += 10)
	{
		painter->setPen(y % 90 ? linePen : mainLinePen);
		painter->drawLine(rect.left(), y, rect.right(), y);
	}
}

void PipelineScene::updateActiveItem()
{
	//Ensure changes are handled.
	if(!tryBlockUpdates()) return;

	//Get the selected items.
	QList<QGraphicsItem*> allItems = selectedItems();
	if(allItems.count() == 1)
	{
		//Try to cast to pipeline item.
		VisualPipelineItem* visualItem = qgraphicsitem_cast<VisualPipelineItem*>(allItems.first());
		if(visualItem)
		{
			setActiveItem(visualItem->getPipelineItem());
			unblockUpdates();
			return;
		}
	}

	//If not available, clear selection.
	setActiveItem(NULL);
	unblockUpdates();
}

void PipelineScene::serialize(QDomElement* xmlElement, SerializationContext* context) const
{
	//Serialize the layouter, then the children.
	_sceneLayouter->serialize(xmlElement, context);
	ViewItem::serialize(xmlElement, context);
}

void PipelineScene::deserialize(const QDomElement* xmlElement, SerializationContext* context)
{
	//Deserialize the children, then the layouter.
	ViewItem::deserialize(xmlElement, context);
	_sceneLayouter->deserialize(xmlElement, context);
}

