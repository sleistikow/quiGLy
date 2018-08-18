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

#include "pipelinescenelayouter.h"
#include "pipelinetab.h"

#include "data/iconnection.h"
#include "data/ipipeline.h"
#include "data/ipipelinemanager.h"

#include "data/blocks/vertexpullerblock.h"
#include "data/common/serializationcontext.h"

#include <QtGlobal>
#include <QDebug>

using namespace ysm;

#define AUTO_X_SPACE 270
#define AUTO_Y_SPACE 340
#define COMMAND_Y_SPACE 100
#define X_SPACING_FACTOR 1
#define Y_SPACING_FACTOR 2

PipelineSceneLayouter::PipelineSceneLayouter(QObject *parent) :
	QObject(parent)
{ }

bool PipelineSceneLayouter::getLayoutInfo(IPipelineItem *pipelineItem, PipelineSceneLayouter::LayoutInfo **layoutInfo)
{
	if(!_itemLayoutInfos.contains(pipelineItem))
	{
		//Create new layout info.
		*layoutInfo = new LayoutInfo();

		//Set the default layout data.
		(*layoutInfo)->_rootBlock = NULL;
		(*layoutInfo)->_selected = false;

		//Store the layout info.
		_itemLayoutInfos[pipelineItem] = *layoutInfo;
		return true;
	}
	else
		*layoutInfo = _itemLayoutInfos[pipelineItem];

	//Layout info was not newly created.
	return false;
}

QPointF PipelineSceneLayouter::getItemPosition(IPipelineItem* pipelineItem, QPointF fallbackPosition)
{
	//Get the layout info, use fallback if not available.
	LayoutInfo* layoutInfo = NULL;
	if(getLayoutInfo(pipelineItem, &layoutInfo))
		setItemPosition(layoutInfo, fallbackPosition, false);

	//Return the block position.
	return layoutInfo->_currentPosition;
}

QPointF PipelineSceneLayouter::getAutoLayoutItemPosition(IPipelineItem* pipelineItem)
{
	//Get the layout info.
	LayoutInfo* layoutInfo = NULL;
	getLayoutInfo(pipelineItem, &layoutInfo);

	//Return the block's auto position.
	return layoutInfo->_rootBlock ? layoutInfo->_autoPosition : QPointF();
}

QPointF PipelineSceneLayouter::setItemPosition(IPipelineItem *pipelineItem, QPointF position)
{
	//Get the layout info.
	LayoutInfo* layoutInfo = NULL;
	getLayoutInfo(pipelineItem, &layoutInfo);

	//Set the new position.
	setItemPosition(layoutInfo, position, false);
	emit layoutChanged(pipelineItem);

	//Return the adjusted value.
	return layoutInfo->_currentPosition;
}

QPointF PipelineSceneLayouter::moveItem(IPipelineItem *pipelineItem, QPointF offset, bool previewOnly)
{
	//Get the layout info.
	LayoutInfo* layoutInfo = NULL;
	getLayoutInfo(pipelineItem, &layoutInfo);

	//Use the correct source position to calculate the new position.
	QPointF sourcePosition = previewOnly ? layoutInfo->_previewPosition : layoutInfo->_basePosition;
	setItemPosition(layoutInfo, sourcePosition + offset, previewOnly);
	emit layoutChanged(pipelineItem);

	//Return the adjusted value.
	return layoutInfo->_currentPosition;
}

bool PipelineSceneLayouter::getItemSelected(IPipelineItem* pipelineItem)
{
	//Get the layout info.
	LayoutInfo* layoutInfo = NULL;
	getLayoutInfo(pipelineItem, &layoutInfo);

	//Return the block position.
	return layoutInfo->_selected;
}

bool PipelineSceneLayouter::setItemSelected(IPipelineItem* pipelineItem, bool selected)
{
	//Get the layout info.
	LayoutInfo* layoutInfo = NULL;
	getLayoutInfo(pipelineItem, &layoutInfo);

	//Store the new value without adjustments.
	layoutInfo->_selected = selected;
	emit layoutChanged(pipelineItem);

	//Return the new value.
	return layoutInfo->_selected;
}

void PipelineSceneLayouter::unregisterItem(IPipelineItem* pipelineItem)
{
	//Remove the block's entry.
	if(_itemLayoutInfos.contains(pipelineItem))
	{
		//Remove the position.
		LayoutInfo* layoutInfo = _itemLayoutInfos[pipelineItem];
		_itemLayoutInfos.remove(pipelineItem);
		delete layoutInfo;
	}
}

void PipelineSceneLayouter::serialize(QDomElement* base, SerializationContext* context) const
{
	//Create the container element.
	QDomElement layoutContainer = context->createElement("LayoutInfo");

	//Store the layout info.
	foreach(IPipelineItem* pipelineItem, _itemLayoutInfos.keys())
	{
		//Ensure element is not null.
		if(!pipelineItem) return;

		//Create a new item element.
		QDomElement layoutItem = context->createElement("Layout");

		//Store the information.
		layoutItem.setAttribute("BlockID", context->getObjectID(pipelineItem));
		layoutItem.setAttribute("XPos", _itemLayoutInfos[pipelineItem]->_basePosition.x());
		layoutItem.setAttribute("YPos", _itemLayoutInfos[pipelineItem]->_basePosition.y());

		//Append the element to the container.
		layoutContainer.appendChild(layoutItem);
	}

	//Append the container to the base.
	base->appendChild(layoutContainer);
}

void PipelineSceneLayouter::deserialize(const QDomElement* base, SerializationContext* context)
{
	//Try to retreive the container node.
	QDomNodeList containerNodes = base->elementsByTagName("LayoutInfo");

	//DUE TO A BUG IN A PREVIOUS VERSION OF THE SERIALIZATION, THE LAYOUT ELEMENTS WERE _NOT_ INSIDE THE CONTAINER
	//ELEMENT. IN THIS CASE, THERE IS NO LAYOUTINFO TAG AVAILABLE.
	QDomElement fixedBase;
	if(!containerNodes.isEmpty() && containerNodes.at(0).isElement())
	{
		//Use the container as base.
		fixedBase = containerNodes.at(0).toElement();
		base = &fixedBase;
	}

	//Iterate over all layout elements.
	QDomNodeList itemNodes = base->elementsByTagName("Layout");
	for (int i = 0; i < itemNodes.count(); i++)
	{
		//Retrieve the node's details.
		QDomNode itemNode = itemNodes.at(i);
		if(itemNode.isElement())
		{
			QDomElement layoutItem = itemNode.toElement();
			ISerializable* pipelineItem = context->findObject(layoutItem.attribute("BlockID").toInt());

			//Get the layout info for the current item.
			LayoutInfo* layoutInfo = NULL;
			getLayoutInfo(dynamic_cast<IPipelineItem*>(pipelineItem), &layoutInfo);

			//Store the position.
			setItemPosition(layoutInfo, QPointF(layoutItem.attribute("XPos").toDouble(),
												layoutItem.attribute("YPos").toDouble()), false);
		}
	}
}

void PipelineSceneLayouter::setItemPosition(LayoutInfo* layoutInfo, QPointF position, bool previewOnly)
{
	//Adjust and store the current position.
	//TODO: Rounding to 10px simulates a grid, make grid adjustable or use auto layout features.
	layoutInfo->_currentPosition = QPointF(qRound(position.x() / 10) * 10,
										   qRound(position.y() / 10) * 10);

	//Non-preview move clears all data and stores the current position.
	if(!previewOnly)
	{
		layoutInfo->_previewPosition = layoutInfo->_currentPosition;
		layoutInfo->_basePosition = layoutInfo->_currentPosition;
	}

	//Preview move stores the exact position and keeps the base.
	else
		layoutInfo->_previewPosition = position;
}

void PipelineSceneLayouter::updateAutoLayout(IPipeline *pipeline)
{
	//Clear all auto layout info.
	foreach(LayoutInfo* layoutInfo, _itemLayoutInfos.values())
		layoutInfo->_rootBlock = NULL;

	//Find all available vertex pullers, which act as source.
	QSet<IBlock*> leftLayoutedBlocks, rightLayoutedBlocks, rootBlocks;
	foreach(IBlock* block, pipeline->getBlocks())
		if(dynamic_cast<VertexPullerBlock*>(block))
		{
			//Get the layout info.
			LayoutInfo* layoutInfo = NULL;
			getLayoutInfo(block, &layoutInfo);

			//Store the block's info.
			layoutInfo->_rootBlock = block;
			layoutInfo->_logicalPosition.setX(0);

			//Use as current edge.
			leftLayoutedBlocks.insert(block);
			rightLayoutedBlocks.insert(block);
			rootBlocks.insert(block);
		}

	//Iterate over the pipeline step by step, until all items are layouted.
	while(!leftLayoutedBlocks.empty() || !rightLayoutedBlocks.empty())
	{
		//Continue layouting in the directions.
		leftLayoutedBlocks = autoLayoutHorizontally(leftLayoutedBlocks, -1);
		rightLayoutedBlocks = autoLayoutHorizontally(rightLayoutedBlocks, 1);

		//If no more blocks are available, find blocks that are connected only in reverse direction.
		if(leftLayoutedBlocks.empty() && rightLayoutedBlocks.empty())
			foreach(IBlock* block, pipeline->getBlocks())
			{
				//Get the layout info.
				LayoutInfo* blockInfo = NULL;
				getLayoutInfo(block, &blockInfo);

				//Check if the block has a layouted neighbour.
				if(!blockInfo->_rootBlock)
				{
					//Check left neighbours.
					foreach(IConnection* connection, block->getInConnections())
					{
						//Get the layout info.
						LayoutInfo* layoutInfo = NULL;
						getLayoutInfo(connection->getSource(), &layoutInfo);

						//If the neighbour is layouted, continue with this block.
						if(layoutInfo->_rootBlock)
							rightLayoutedBlocks.insert(connection->getSource());
					}

					//Check right neighbours.
					foreach(IConnection* connection, block->getOutConnections())
					{
						//Get the layout info.
						LayoutInfo* layoutInfo = NULL;
						getLayoutInfo(connection->getDest(), &layoutInfo);

						//If the neighbour is layouted, continue with this block.
						if(layoutInfo->_rootBlock)
							leftLayoutedBlocks.insert(connection->getDest());
					}
				}
			}
	}

	//Calculate the vertical positions.
	autoLayoutVertically(rootBlocks);

	//Calculate the actual positions.
	foreach(LayoutInfo* layoutInfo, _itemLayoutInfos.values())
		if(layoutInfo->_rootBlock)
			layoutInfo->_autoPosition = QPointF(layoutInfo->_logicalPosition.x() * AUTO_X_SPACE,
												layoutInfo->_logicalPosition.y() * AUTO_Y_SPACE);

	//Layout the render commands after the blocks.
	foreach(IRenderCommand* renderCommand, pipeline->getRenderCommands())
	{
		//Get the command's layout info.
		LayoutInfo* commandInfo = NULL;
		getLayoutInfo(renderCommand, &commandInfo);

		//Ensure command has assigned block.
		commandInfo->_rootBlock = renderCommand->getAssignedBlock();
		if(commandInfo->_rootBlock)
		{
			//Get the block's layout info.
			LayoutInfo* blockInfo = NULL;
			getLayoutInfo(commandInfo->_rootBlock, &blockInfo);

			//Set position right above the block.
			commandInfo->_autoPosition = QPointF(blockInfo->_autoPosition.x(),
												 blockInfo->_autoPosition.y() - COMMAND_Y_SPACE);
		}
	}
}

QSet<IBlock*> PipelineSceneLayouter::autoLayoutHorizontally(QSet<IBlock*> sourceBlocks, int direction)
{
	//Iterate over all source blocks.
	QSet<IBlock*> layoutedBlocks;
	foreach(IBlock* sourceBlock, sourceBlocks)
	{
		//Get the block's layout info.
		LayoutInfo* sourceInfo = NULL;
		getLayoutInfo(sourceBlock, &sourceInfo);

		//Retrieve the correct connections.
		QVector<IConnection*> connections;
		if(direction < 0) connections = sourceBlock->getInConnections();
		if(direction > 0) connections = sourceBlock->getOutConnections();

		//Iterate over the connections.
		foreach(IConnection* connection, connections)
		{
			//Get the connection's corresponding block.
			IBlock* layoutedBlock = NULL;
			if(direction < 0) layoutedBlock = connection->getSource();
			if(direction > 0) layoutedBlock = connection->getDest();

			//Get the block's layout info.
			LayoutInfo* layoutInfo = NULL;
			getLayoutInfo(layoutedBlock, &layoutInfo);

			//Ensure the block is not already layouted in another pipeline.
			if(!layoutInfo->_rootBlock || layoutInfo->_rootBlock == sourceInfo->_rootBlock)
			{
				layoutInfo->_rootBlock = sourceInfo->_rootBlock;
				layoutInfo->_logicalPosition.setX(sourceInfo->_logicalPosition.x() + direction);
				layoutedBlocks.insert(layoutedBlock);
			}
		}
	}

	//Return all layouted blocks.
	return layoutedBlocks;
}

void PipelineSceneLayouter::autoLayoutVertically(QSet<IBlock*> rootBlocks)
{
	//Iterate over all pipelines.
	int pipelineOffset = 0;
	foreach(IBlock* rootBlock, rootBlocks)
	{
		//Iterate over all layouted items of the current pipeline and align them vertically.
		int maxOffset = 0;
		QMap<int, int> verticalCounts;
		foreach(LayoutInfo* layoutInfo, _itemLayoutInfos.values())
		{
			//Ensure block is part of current pipeline.
			if(layoutInfo->_rootBlock == rootBlock)
			{
				//Check if a block on the same horizontal position was already layouted.
				int verticalPosition = verticalCounts.value(layoutInfo->_logicalPosition.x(), 0);
				verticalCounts[layoutInfo->_logicalPosition.x()] = verticalPosition + 1;

				//Set the vertical position.
				layoutInfo->_logicalPosition.setY(verticalPosition + pipelineOffset);
				maxOffset = qMax(maxOffset, verticalPosition);
			}
		}

		//Adjust total offset for each pipeline.
		pipelineOffset += maxOffset + 1;
	}
}
