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

#include "abstractview.h"
#include "document.h"
#include "commands/iuicommandqueue.h"

using namespace ysm;

AbstractView::AbstractView(IView* parent) :
	_parentView(parent),
	_activeDocument(NULL),
	_activeItem(NULL),
	_blockUpdates(false)
{
	//Register to the parent view.
	if(_parentView)
		_parentView->addChildView(this);
}

AbstractView::~AbstractView()
{
	//Block all updates.
	_blockUpdates = true;

	//Clear all child view's parent.
	foreach(IView* childView, _childViews)
		childView->setParentView(NULL);

	//Unregister from the parent view.
	if(_parentView)
		_parentView->removeChildView(this);
}

IView* AbstractView::getParentView() const { return _parentView; }
void AbstractView::setParentView(IView *parentView)
{
	//Disconnect from old parent.
	if(_parentView)
	{
		_parentView->removeChildView(this);
		if(inheritsActiveDocument())
			_activeDocument = NULL;
		if(inheritsActiveItem())
			_activeItem = NULL;
	}

	//Set new parent.
	_parentView = parentView;

	//Connect to new parent.
	if(_parentView)
	{
		_parentView->addChildView(this);
		if(inheritsActiveDocument())
			_activeDocument = _parentView->getActiveDocument();
		if(inheritsActiveItem())
			_activeItem = _parentView->getActiveItem();
	}
}

void AbstractView::addChildView(IView *childView) { _childViews.append(childView); }
void AbstractView::removeChildView(IView *childView) { _childViews.removeOne(childView); }

Document* AbstractView::getActiveDocument() const { return _activeDocument; }
void AbstractView::setActiveDocument(Document *document)
{
	//Ensure an actual change.
	if(_activeDocument == document) return;

	//If no document is specified, clear selection first.
	if(!document)
		setActiveItem(NULL);

	//Set the new active document.
	_activeDocument = document;

	//Internally update the state, if updates are allowed.
	if(tryBlockUpdates())
	{
		updateDocument();
		unblockUpdates();
	}

	//Forward the change to all inheriting children.
	foreach(IView* childView, _childViews)
		if(childView->inheritsActiveDocument())
			childView->setActiveDocument(document);

	//Forward the change to the parent.
	if(_parentView && inheritsActiveDocument())
		_parentView->setActiveDocument(document);
}

IPipelineItem* AbstractView::getActiveItem() const { return _activeItem; }
void AbstractView::setActiveItem(IPipelineItem *pipelineItem)
{
	//Ensure an actual change.
	if(_activeItem == pipelineItem) return;

	//Set the new active item.
	_activeItem = pipelineItem;

	//Internally update the state, if updates are allowed.
	if(tryBlockUpdates())
	{
		updateItem();
		unblockUpdates();
	}

	//Forward the change to all inheriting children.
	foreach(IView* childView, _childViews)
		if(childView->inheritsActiveItem())
			childView->setActiveItem(pipelineItem);

	//Forward the change to the parent.
	if(_parentView && inheritsActiveItem())
		_parentView->setActiveItem(pipelineItem);
}

void AbstractView::updateView(Document* document, const QList<IChangeable*>& changedObjects,
							  IChangeable::Operation operation)
{
	//Execute all notifications.
	foreach(IChangeable* changedObject, changedObjects)
		foreach(INotification* notification, _notifications)
			notification->notify(document, changedObject, operation);

	//Forward the change to all child views, that are operating on the active document.
	foreach(IView* childView, _childViews)
		childView->updateView(document, changedObjects, operation);
}

void AbstractView::resetView(Document* document)
{
	//Execute all resets.
	foreach(IReset* reset, _resets)
		reset->notify(document);

	//Forward the change to all child views, that are operating on the active document.
	foreach(IView* childView, _childViews)
		childView->resetView(document);
}

bool AbstractView::tryBlockUpdates()  { return (!_blockUpdates) && (_blockUpdates = true); }
void AbstractView::unblockUpdates() { _blockUpdates = false; }

IUICommandQueue* AbstractView::getCommandQueue() const { return getActiveDocument()->getCommandQueue(); }
void AbstractView::executeCommand(IUICommand *command) { getActiveDocument()->getCommandQueue()->execute(command); }

void AbstractView::serialize(QDomElement* xmlElement, SerializationContext* context) const { }
void AbstractView::deserialize(const QDomElement* xmlElement, SerializationContext* context) { }
