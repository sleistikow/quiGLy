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

#include "propertyview.h"

#include "../registry.h"
#include "../document.h"
#include "propertyviewfactory.h"
#include "pipelineitempropertyview.h"

#include "data/ipipeline.h"
#include "data/ipipelineitem.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"

#include <QDebug>

using namespace ysm;

PropertyView::PropertyView(QWidget* parentWidget, IView* parentView) :
	QDockWidget(parentWidget),
	View(parentView),
	_activeView(NULL)
{
	//Create stack that contains all property views.
	_viewStack = new QStackedWidget(this);
	setWidget(_viewStack);

	//Register for item deletion only, item's are created upon usage.
	notifyAll(IChangeable::Remove, this, &PropertyView::deletePropertyView);
	notifyAll(IChangeable::Remove, this, &PropertyView::deletePropertyViews);
}

void PropertyView::createPropertyView(IPipelineItem* pipelineItem)
{
	//Get the factory.
	PipelineItemPropertyView* view = NULL;
	PropertyViewFactory* viewFactory = getActiveDocument()->getRegistry()->getViewFactory();

	//Check if item is block.
	IBlock* block = dynamic_cast<IBlock*>(pipelineItem);
	if(block)
		view = viewFactory->createBlockPropertyView(block, this);

	//Check if item is connection.
	IConnection* connection = dynamic_cast<IConnection*>(pipelineItem);
	if(connection)
		view = viewFactory->createConnectionPropertyView(connection, this);

	//Check if item is render command.
	IRenderCommand* command = dynamic_cast<IRenderCommand*>(pipelineItem);
	if(command)
		view = viewFactory->createRenderCommandPropertyView(command, this);

	//Add to stack and store for easy access.
	if(view)
	{
		_viewStack->addWidget(view);
		_views[pipelineItem->getID()] = view;
	}
}

void PropertyView::deletePropertyView(IPipelineItem* pipelineItem)
{
	//Delete the property view, if it exists.
	if(_views.contains(pipelineItem->getID()))
	{
		delete _views[pipelineItem->getID()];
		_views.remove(pipelineItem->getID());
	}
}

void PropertyView::deletePropertyViews(Document* document)
{
	//Iterate over the pipeline's blocks.
	IPipeline* pipeline = document->getPipeline();
	foreach(IBlock* block, pipeline->getBlocks())
	{
		//Delete the block property view.
		deletePropertyView(block);

		//Delete the outgoing connection property views.
		foreach(IConnection* connection, block->getOutConnections())
			deletePropertyView(connection);

		//Delete the render command property views.
		foreach(IRenderCommand* renderCommand, pipeline->getRenderCommands())
			deletePropertyView(renderCommand);
	}
}

void PropertyView::updateDocument() { updateItem(); }
void PropertyView::updateItem()
{
	//Show the active property view.
	if(getActiveItem())
	{
		//Check if the property view exists, otherwise create it.
		if(!_views.contains(getActiveItem()->getID()))
			createPropertyView(getActiveItem());

		//Show the property view.
		if(_views.contains(getActiveItem()->getID()))
		{
			_viewStack->setCurrentWidget(_activeView = _views[getActiveItem()->getID()]);
			show();
		}
	}

	//Hide, if no view is selected.
	//TODO: Let the user decide wether to hide or not.
	else hide();
}
