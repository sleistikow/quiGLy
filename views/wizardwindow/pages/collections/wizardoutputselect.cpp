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

#include "wizardoutputselect.h"
#include "../pageitems/wizarddynamicitem.h"
#include "../../wizardwindow.h"

using namespace ysm;

const QString WizardOutputSelect::EnabledField("enabled");

WizardOutputSelect::WizardOutputSelect(WizardWindow* parent) :
	WizardItemCollection(parent),
	_enableOption(NULL)
{ }

bool WizardOutputSelect::generateLayout(QFormLayout *layout, QDomElement rootElement, bool isChild)
{
	//Add the check box to the root element.
	QString enableCaption = WizardFile::getProperty(rootElement, "caption");
	if(!isChild && !enableCaption.isEmpty())
	{
		//Create the check box.
		_enableOption = new QCheckBox(enableCaption, getWindow());
		_enableOption->setChecked(false);

		//Register the import option.
		layout->addRow(NULL, _enableOption);
		connect(_enableOption, &QCheckBox::toggled, this, &WizardOutputSelect::updateItems);
		getDelegate()->registerDynamicField(EnabledField, _enableOption);
	}

	//Build the actual collection items.
	return WizardItemCollection::generateLayout(layout, rootElement, isChild);
}

void WizardOutputSelect::initializeItems()
{
	//Set the enabled flag to default.
	_enableOption->setChecked(_default);

	//Run base initialization.
	WizardItemCollection::initializeItems();
}

void WizardOutputSelect::updateItems()
{
	//If output is disabled, disable all items.
	if(_enableOption && !_enableOption->isChecked())
	{
		//Iterate over all items and disable them.
		foreach(WizardDynamicItem* dynamicItem, getItems())
		{
			dynamicItem->setEnabled(false);
			dynamicItem->resetValue();
		}

		//Manually emit change signal.
		getDelegate()->notifyCompleteChanged();
	}

	//Otherwise run the default update.
	else WizardItemCollection::updateItems();
}

bool WizardOutputSelect::isComplete() const
{
	//If output is disabled, it's ignored.
	if(!_enableOption || !_enableOption->isChecked()) return true;

	//Otherwise run the default checks.
	return WizardItemCollection::isComplete();
}

bool WizardOutputSelect::isDefault() const { return _default; }
void WizardOutputSelect::setDefault(bool isEnabled) { _default = isEnabled; }
