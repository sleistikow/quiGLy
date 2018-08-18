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

#include "pipelinetab.h"
#include "pipelinescene.h"
#include "pipelineview.h"
#include "pipelinescenelayouter.h"

#include "visualitems/visualpipelineitem.h"
#include "visualitems/visuallayouteditem.h"
#include "visualitems/visualrendercommand.h"
#include "visualitems/visualconnection.h"
#include "visualitems/visualblock.h"

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/change/movelayouteditemscommand.h"
#include "commands/pipeline/delete/deleterendercommandscommand.h"
#include "commands/pipeline/delete/deleteconnectionscommand.h"
#include "commands/pipeline/delete/deleteblockscommand.h"

#include "../document.h"

#include <QScrollBar>
#include <QFormLayout>
#include <QToolButton>
#include <QLabel>
#include <QDebug>
#include <QtMath>


using namespace ysm;

PipelineTab::PipelineTab(PipelineView* parentView, Document* document) :
	QGraphicsView(parentView),
	ViewItem(parentView),
	_parentView(parentView),
	_pipelineScene(NULL),
	_zoomLevel(1.0f),
	_mode(Default)
{
    //Set scene modes.
	setTransformationAnchor(NoAnchor);
	setDragMode(RubberBandDrag);
	setAcceptDrops(true);
	setInteractive(true);

	//Adjust scroll bar policy.
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//Override default initialization, set a fixed document.
	initialize(document);

	//Create the scene.
	_pipelineScene = new PipelineScene(this);
	setScene(_pipelineScene);

	//Attach the scene to document serialization.
	document->attach(_pipelineScene);

	//Adjust the initial size.
	setSceneRect(-5000, -5000, 10000, 10000);
}

void PipelineTab::updateSceneRect()
{
    //Start out with the item's minimal rect.
    QRectF sceneRect = _pipelineScene->itemsBoundingRect();

    //Add a border of the current screen size, so it's possible to scroll exactly to the outer item's border
    //(so that they are not visible).
    QRectF visibleRect = mapToScene(rect()).boundingRect();
    sceneRect.adjust(-visibleRect.width(), -visibleRect.height(), visibleRect.width(), visibleRect.height());

	//Set the new scene rect, then restore the old center.
	setSceneRect(sceneRect);
}

void PipelineTab::updateMessages()
{
	//Simple redraw will help to update all messages.
	_pipelineScene->update();
}

PipelineScene* PipelineTab::getPipelineScene() const { return _pipelineScene; }
void PipelineTab::requestEditor(IBlock* shaderBlock) { _parentView->requestEditor(shaderBlock); }

void PipelineTab::wheelEvent(QWheelEvent* event)
{
    //Get the mouse location prior to zoom.
    QPointF mouseLocation = mapToScene(event->pos());

    //Adjust the zoom level.
	_zoomLevel = qMax(qMin(_zoomLevel + (event->delta() * 0.002f), 1.0f), 0.33f);

    //Calculate the new zoom matrix.
    QTransform transformation;
    transformation.scale(_zoomLevel, _zoomLevel);

    //Set the new zoom level.
    setTransform(transformation);

	//Update the scene rect after zooming.
	updateSceneRect();

    //Move the scene, so that the new mouse location matches the old location.
    QPointF centerTranslation = mapToScene(event->pos()) - mouseLocation;
    translate(centerTranslation.x(), centerTranslation.y());

    //Accept the event.
	event->accept();
}

void PipelineTab::keyPressEvent(QKeyEvent* event)
{
	//Forward the event.
	QGraphicsView::keyPressEvent(event);
}

void PipelineTab::keyReleaseEvent(QKeyEvent* event)
{
	//Delete the selected items on DEL.
	if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
	{
		//List all selected render commands.
		QList<IRenderCommand*> selectedCommands;
		foreach(QGraphicsItem* selectedItem, _pipelineScene->selectedItems())
		{
			VisualRenderCommand* visualItem = dynamic_cast<VisualRenderCommand*>(selectedItem);
			if(visualItem)
				selectedCommands.append(visualItem->getRenderCommand());
		}

		//List all selected connections.
		QList<IConnection*> selectedConnections;
		foreach(QGraphicsItem* selectedItem, _pipelineScene->selectedItems())
		{
			VisualConnection* visualItem = dynamic_cast<VisualConnection*>(selectedItem);
			if(visualItem)
				selectedConnections.append(visualItem->getConnection());
		}

		//List all selected blocks.
		QList<IBlock*> selectedBlocks;
		foreach(QGraphicsItem* selectedItem, _pipelineScene->selectedItems())
		{
			VisualBlock* visualItem = dynamic_cast<VisualBlock*>(selectedItem);
			if(visualItem)
				selectedBlocks.append(visualItem->getBlock());
		}

		//Clear the selection before deleting.
		_pipelineScene->clearSelection();

		//Delete render commands, then connections and then blocks.
		//Otherwise, connections and render commands could be implicitly removed.
		getCommandQueue()->beginCommandBlock();
		executeCommand(new DeleteRenderCommandsCommand(getActiveDocument()->getPipeline(), selectedCommands));
		executeCommand(new DeleteConnectionsCommand(getActiveDocument()->getPipeline(), selectedConnections));
		executeCommand(new DeleteBlocksCommand(getActiveDocument()->getPipeline(), selectedBlocks));
		getCommandQueue()->endCommandBlock();

		//Do not forward the keypress as it was handled.
		return;
	}

	//Forward the event.
	QGraphicsView::keyReleaseEvent(event);
}

void PipelineTab::mousePressEvent(QMouseEvent* event)
{
	//Store the initial mouse position.
	_mouseDownPosition = _mouseMovePosition = mapToScene(event->pos());

	//Find the first visual pipeline item beneath the cursor.
	_mouseDownItem = NULL;
	foreach(QGraphicsItem* graphicsItem, items(event->pos()))
		if((_mouseDownItem = dynamic_cast<VisualPipelineItem*>(graphicsItem)))
			break;

	//Check which button was pressed.
	bool isCtrl = event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::ControlModifier;
	switch(event->button())
	{

	//Left mouse button for dragging.
	case Qt::LeftButton:

		//If the current item is not selected, clear the selection. Otherwise, a multi selection might be possible.
		//If CTRL (meta on OS X) is pressed, also keep the current selection.
		if(!_mouseDownItem || (!_mouseDownItem->isSelected() && !isCtrl))
			_pipelineScene->clearSelection();

		//If the clicked item is layouted, dragging is possible.
		_mode = dynamic_cast<VisualLayoutedItem*>(_mouseDownItem) ? DraggingPossible : Default;

		//Allow handling by child items.
		event->ignore();
		break;

	//Right mouse button for context menu and panning.
	case Qt::RightButton:

		//Panning is always possible. If the no panning is performed, the selection is adjusted on release.
		_mode = PanningPossible;

		//No further handling allowed.
		event->accept();
		break;

	//Unhandled action.
	default:

		//Allow handling by child items.
		event->ignore();
		break;
	}

	//Forward the event to base, if not handled.
	if(!event->isAccepted())
		QGraphicsView::mousePressEvent(event);
}

void PipelineTab::mouseMoveEvent(QMouseEvent* event)
{
	//Retrieve cursor and transformation.
	QCursor globalCursor = cursor();
	QTransform transformation = transform();

	//If the left mouse button is down, ensure it stays inside the visible area.
	if(event->buttons().testFlag(Qt::LeftButton))
	{
		//Calculate an offset that would move the cursor back in.
		QRect insetRect = rect().adjusted(50, 50, -50, -50);
		QPoint cursorOffset(
			qMin(insetRect.right() - event->pos().x(), 0) + qMax(insetRect.left() - event->pos().x(), 0),
			qMin(insetRect.bottom() - event->pos().y(), 0) + qMax(insetRect.top() - event->pos().y(), 0));

		//Check if the cursor needs to be adjusted.
		if(!cursorOffset.isNull())
		{
			//Pan accordingly to the cursor adjustment.
			transformation = transformation.translate(cursorOffset.x() / _zoomLevel, cursorOffset.y() / _zoomLevel);
			setTransform(transformation);
		}
	}

	//Calculate the offset of the current move.
	QPointF offset = mapToScene(event->pos()) - _mouseMovePosition;

	//Check the current mode.
	switch(_mode)
	{

	//Drag the selected items in dragging mode.
	case DraggingPossible:
	case Dragging:

		//Change mode during cursor manipulation, to avoid call cycles.
		_mode = Dragging;

		//Find all selected layouted items and offset them.
		foreach(QGraphicsItem* graphicsItem, _pipelineScene->selectedItems())
		{
			//Try to cast to layouted item, if it's possible offset the item.
			VisualLayoutedItem* layoutedItem = dynamic_cast<VisualLayoutedItem*>(graphicsItem);
			if(layoutedItem)
				_pipelineScene->getLayouter()->moveItem(layoutedItem->getPipelineItem(), offset, true);
		}

		//Update the scene rect.
		updateSceneRect();

		//No further handling allowed.
		event->accept();
		break;

	//Offset the view in panning mode.
	case PanningPossible:
	case Panning:

		//Change mode to panning.
		_mode = Panning;

		//Adjust the cursor's style.
		globalCursor.setShape(Qt::ClosedHandCursor);
		setCursor(globalCursor);

		//Retrieve the current transformation, then translate.
		transformation = transformation.translate(offset.x(), offset.y());
		setTransform(transformation);

		//No further handling allowed.
		event->accept();
		break;

	//Ignore the manipulation.
	case Ignore:

		//No further handling allowed.
		event->accept();
		break;

	//Unhandled action.
	default:

		//Allow handling by child items.
		event->ignore();
		break;
	}

	//Update the latest move position after translations are done.
	_mouseMovePosition = mapToScene(event->pos());

	//Forward the event to base, if not handled.
	if(!event->isAccepted())
		QGraphicsView::mouseMoveEvent(event);
}

void PipelineTab::mouseReleaseEvent(QMouseEvent *event)
{
	//Change back to default mode.
	_latestMode = _mode;
	_mode = Default;

	//Calculate the offset of the complete move.
	QPointF totalOffset = mapToScene(event->pos()) - _mouseDownPosition;

	//Go back to default cursor.
	setCursor(QCursor());

	//Check the previous mode.
	switch(_latestMode)
	{

	// Keep the selection after dragging and store the move.
	case Dragging:

		//Store the move as command, to make it undoable.
		executeCommand(new MoveLayoutedItemCommand(_pipelineScene->selectedItems(), _pipelineScene->getLayouter(),
												   totalOffset));

		//No further handling allowed.
		event->accept();
		break;

	// Keep the selection after panning.
	case Panning:

		//No further handling allowed.
		event->accept();
		break;

	//Unhandled action.
	default:

		//Allow handling by child items.
		event->ignore();
		break;
	}

	//Forward the event to base, if not handled.
	if(!event->isAccepted())
		QGraphicsView::mouseReleaseEvent(event);
}

void PipelineTab::contextMenuEvent(QContextMenuEvent* event)
{
	//Check the current mode.
	switch(_latestMode)
	{

	//If action was performed, do not allow cntext menu.
	case Dragging:
	case Panning:

		//No further handling allowed.
		event->accept();
		break;

	//Unhandled action.
	default:

		//Allow handling by child items.
		event->ignore();
		break;
	}

	//Forward the event to base, if not handled.
	if(!event->isAccepted())
		QGraphicsView::contextMenuEvent(event);
}
