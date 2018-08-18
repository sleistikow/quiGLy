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

#include "wizardcheckboxitem.h"
#include "../../wizardwindow.h"

using namespace ysm;

WizardCheckBoxItem::WizardCheckBoxItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout) :
	WizardDynamicItem(element, collection)
{
	//Create the item's widget.
	_widget = new QCheckBox(getCaption(), collection->getWindow());
	_widget->setToolTip(getDescription());

	//Register the new item.
	collection->getDelegate()->registerDynamicField(getName(), _widget);
	connect(_widget, &QCheckBox::toggled, collection, &WizardItemCollection::updateItems);
	layout->addRow(NULL, _widget);
}

bool WizardCheckBoxItem::isEnabled() const { return _widget->isEnabled(); }
void WizardCheckBoxItem::setEnabled(bool isEnabled) { _widget->setEnabled(isEnabled); }

void WizardCheckBoxItem::resetValue()
{
	//Ensure value is only reset if neccessary.
	bool defaultValue = getDefault() == "true";
	if(defaultValue != _widget->isChecked())
		_widget->setChecked(defaultValue);
}
