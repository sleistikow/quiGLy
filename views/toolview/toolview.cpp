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

#include "toolview.h"
#include "tooltab.h"
#include "../document.h"

using namespace ysm;

ToolView::ToolView(QWidget* parentWidget, IView* parentView) :
	QDockWidget(parentWidget),
	View(parentView)
{
	//Create stack that contains all property views.
	_viewStack = new QStackedWidget(this);
	setWidget(_viewStack);

	//Register for item deletion only, item's are created upon usage.
	notifyAll(IChangeable::Remove, this, &ToolView::deleteToolView);
}

void ToolView::updateDocument()
{
	//Ensure document is available.
	if(!getActiveDocument()) return;

	//Check if the view already exists.
	if(!_views.contains(getActiveDocument()))
	{
		ToolTab* toolTab = new ToolTab(this, this);
		_viewStack->addWidget(toolTab);
		_views[getActiveDocument()] = toolTab;
	}

	//Set the active widget.
	_viewStack->setCurrentWidget(_views[getActiveDocument()]);
}

void ToolView::deleteToolView(Document *document)
{
	//Delete the tool view, if it exists.
	if(_views.contains(document))
	{
		delete _views[document];
		_views.remove(document);
	}
}

void ToolView::updateItem() { }
