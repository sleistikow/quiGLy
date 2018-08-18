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

#include "visualpipelineitem.h"

#include "../../document.h"
#include "../pipelinescenelayouter.h"
#include "../pipelinescene.h"

#include "commands/iuicommandqueue.h"
#include "data/iproperty.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

using namespace ysm;

VisualPipelineItem::VisualPipelineItem(QGraphicsItem* parentGraphics, IView* parentView) :
	QGraphicsObject(parentGraphics),
	ViewItem(parentView),
	_boundingRectDefined(false)
{
	//Set item flags.
	setFlag(ItemIsFocusable, true);
	setFlag(ItemIsSelectable, true);
	setCacheMode(DeviceCoordinateCache);
	setZValue(0);

	//Update on changes.
//	notifyStatic(this, &VisualPipelineItem::update);
	notifyStatic(IChangeable::Change, this, &VisualPipelineItem::updateProperty);
	notifyStatic(IChangeable::Change, this, &VisualPipelineItem::updateVisual);

	//Create context menu actions.
	_deleteAction = new QAction(QIcon(":/tango/16x16/actions/edit-delete.png"), tr("Delete"), this);
	connect(_deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteItem()));
}

PipelineScene* VisualPipelineItem::getPipelineScene() const
{
	//Parent object must be pipeline scene.
	return qobject_cast<PipelineScene*>(scene());
}

void VisualPipelineItem::restoreLayout()
{
	//Get the scene and check if it's valid.
	PipelineScene* pipelineScene = getPipelineScene();
	if(pipelineScene)
		restoreFromLayouter(pipelineScene->getLayouter());
}

void VisualPipelineItem::restoreFromLayouter(PipelineSceneLayouter* layouter)
{
	//Restore selection.
	setSelected(layouter->getItemSelected(getPipelineItem()));
}

QVariant VisualPipelineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	//Ensure the scene is available.
	PipelineScene* pipelineScene = getPipelineScene();
	if(pipelineScene)
	{
		//Retrieve the scene's layouter.
		PipelineSceneLayouter* sceneLayouter = pipelineScene->getLayouter();
		IPipelineItem* pipelineItem = getPipelineItem();

		//Forward selection changes.
		if(change == ItemSelectedChange && sceneLayouter->getItemSelected(pipelineItem) != value.toBool())
			sceneLayouter->setItemSelected(pipelineItem, value.toBool());
	}

	//Call the base.
	return QGraphicsObject::itemChange(change, value);
}

QList<QAction*> VisualPipelineItem::getContextActions() const { return QList<QAction*>({ _deleteAction }); }

void VisualPipelineItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	//Select the item.
	getPipelineScene()->clearSelection();
	setSelected(true);

	//Retrieve the context actions and verify them.
	QList<QAction*> contextActions = getContextActions();
	if(contextActions.isEmpty())
		return;

	//Create the context menu.
	QMenu contextMenu;
	foreach(QAction* action, contextActions)
	{
		//Add action or separator if action is NULL.
		if(action)
			contextMenu.addAction(action);
		else
			contextMenu.addSeparator();
	}

	//Show the context menu.
	contextMenu.exec(event->screenPos());
}

void VisualPipelineItem::updateProperty(IProperty *property)
{
	//Update, if the represented item's property changed.
	if(property->getOwner() == getPipelineItem() && isVisualProperty(property->getID()))
	{
		invalidateBoundingRect();
		update();
	}
}

void VisualPipelineItem::updateVisual(IPipelineItem *pipelineItem)
{
	//Update, if the represented item changed.
	if(pipelineItem == getPipelineItem())
	{
		invalidateBoundingRect();
		update();
	}
}

bool VisualPipelineItem::isVisualProperty(PropertyID propertyID)
{
	//Message log change requires redrawing.
	return propertyID  == PropertyID::MessageLog;
}

QRectF VisualPipelineItem::boundingRect() const
{
	//If bounding rect is not available, calculate it. This can happen, if the bounding rect is required before it has
	//been invalidated. The method must be const, so prepareGeometryChange() cannot be called here.
	if(!_boundingRectDefined)
	{
		_boundingRect = calculateBoundingRect();
		_boundingRectDefined = true;
	}

	//Return the cached bounding rect.
	return _boundingRect;
}
void VisualPipelineItem::invalidateBoundingRect()
{
	//Notify Qt about the bounding rect change.
	prepareGeometryChange();

	//Calculate and cache the new bounding rect.
	_boundingRect = calculateBoundingRect();
	_boundingRectDefined = true;
}
