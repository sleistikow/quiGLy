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

#include "viewitem.h"

using namespace ysm;

ViewItem::ViewItem(IView* parentView) :
	AbstractView(parentView)
{
	//Default initialization, can be changed by calling initialize again in subclasses.
	initialize();
}

ViewItem::~ViewItem() { }

void ViewItem::initialize(Document *activeDocument)
{
	//Store the active document, either from argument or from parent.
	//Do not use the setters, because they would call subclass methods from the constructor.
	_activeDocument = activeDocument ? activeDocument : getParentView() ? getParentView()->getActiveDocument() : NULL;
	_activeItem = getParentView() ? getParentView()->getActiveItem() : NULL;
}

bool ViewItem::inheritsActiveDocument() const { return false; }
bool ViewItem::inheritsActiveItem() const { return true; }

void ViewItem::updateDocument() { }
void ViewItem::updateItem() { }
