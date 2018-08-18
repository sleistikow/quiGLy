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

#include "wizardsourceselect.h"
#include "../pageitems/wizardcomboboxitem.h"
#include "../../wizardwindow.h"

using namespace ysm;

WizardSourceSelect::WizardSourceSelect(QString type, bool disable, WizardWindow* parent) :
	WizardItemCollection(parent),
	_type(type),
	_hasDisableOption(disable),
	_comboBox(NULL)
{ }

bool WizardSourceSelect::generateLayout(QFormLayout *layout, QDomElement rootElement, bool isChild)
{
	//Build the actual collection items.
	bool isStretching = WizardItemCollection::generateLayout(layout, rootElement, isChild);

	//After it's created, retrieve the primary combo box.
	QString primary = rootElement.attribute("primary");
	if(!isChild && !primary.isEmpty())
	{
		//Store the primary combo box data.
		_comboBox = dynamic_cast<WizardComboBoxItem*>(getItem(primary));
		_disabled = rootElement.attribute("disable");
	}

	//Return the original result.
	return isStretching;
}

void WizardSourceSelect::initializeItems()
{
	//Ensure primary combo box is available.
	if(_comboBox)
	{
		//Reset the available options.
		_comboBox->resetOptions();
		_references.clear();

		//Remove the disabled option, if not allowed.
		if(!_hasDisableOption)
			_comboBox->removeOption(_disabled);

		//Append all matching sources.
		foreach(WizardWindow::Reference reference, getWindow()->getReferences(_type))
		{
			_comboBox->addOption(reference._name, reference._identifier);
			_references.insert(reference._identifier);
		}
	}

	//Run base initialization.
	WizardItemCollection::initializeItems();

	//Set combo box to none, if available.
	if(_hasDisableOption && _comboBox)
		_comboBox->setOption(_disabled);
}

void WizardSourceSelect::updateItems()
{
	//If source is disabled or referencing, it's disabled.
	if(isDisabled() || isReferencing())
	{
		//Iterate over all items and disable them.
		foreach(WizardDynamicItem* dynamicItem, getItems())
			if(dynamicItem != _comboBox)
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

bool WizardSourceSelect::isComplete() const
{
	//If source is disabled or referencing, it's ignored.
	if(isDisabled() || isReferencing()) return true;

	//Otherwise run the default checks.
	return WizardItemCollection::isComplete();
}

bool WizardSourceSelect::isDisabled() const { return !_comboBox || _comboBox->getOption() == _disabled; }
bool WizardSourceSelect::isReferencing() const { return _comboBox && _references.contains(_comboBox->getOption()); }
