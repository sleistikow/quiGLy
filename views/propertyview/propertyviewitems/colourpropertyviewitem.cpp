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

#include "colourpropertyviewitem.h"

#include <QColorDialog>
#include <QDebug>


using namespace ysm;

ColourPropertyViewItem::ColourPropertyViewItem(IProperty* property, IView* parentView) :
	GenericPropertyViewItem(property, parentView)
{
    //Set lebel to property name.
	setLabel(new QLabel(property->getName()));

	//Create edit and connect signal.
	_colorSelect = new ColorSelect(NULL);
	setEdit(_colorSelect);

	//Connect to state changes.
	connect(_colorSelect, SIGNAL(colorChanged()), this, SLOT(updateModelValue()));
}

QVector4D ColourPropertyViewItem::getViewValue() const { return _colorSelect->getColorVector(); }
void ColourPropertyViewItem::updateViewValue()
{
	_colorSelect->setColorVector(getModelValue());
	_colorSelect->setDisabled(getProperty()->isReadOnly());
}
