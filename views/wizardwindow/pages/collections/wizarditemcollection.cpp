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

#include "wizarditemcollection.h"
#include "../pageitems/wizarddynamicitem.h"
#include "../../wizardwindow.h"

#include <QGroupBox>

using namespace ysm;

WizardItemCollection::WizardItemCollection(WizardWindow* parent) :
	QObject(parent),
	_isEnabled(true),
	_window(parent)
{ }

WizardItemCollection::~WizardItemCollection() { qDeleteAll(_dynamicItems); }

bool WizardItemCollection::generateLayout(QFormLayout* layout, QDomElement rootElement, bool isChild)
{
	//Iterate over the child elements.
	bool isStretching = false;
	for(QDomElement element = rootElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		//Check if the current element is a spacer.
		if(element.tagName() == "spacer")
		{
			//Add a default spacer item.
			layout->addItem(new QSpacerItem(0, 5));
		}

		//Check if the current element is a fieldset.
		if(element.tagName() == "fieldset")
		{
			//Retrieve the fieldset data.
			QString caption = WizardFile::getProperty(element, "caption");

			//Initialize the fieldset.
			QGroupBox* fieldset = new QGroupBox(_window);
			fieldset->setTitle(caption);

			//Recursively create the fieldset's children.
			QFormLayout* fieldsetLayout = new QFormLayout(fieldset);
			if(generateLayout(fieldsetLayout, element, true))
			{
				//Adjust the layou, if a child wants full height.
				QSizePolicy sizePolicy = fieldset->sizePolicy();
				sizePolicy.setVerticalStretch(1);
				fieldset->setSizePolicy(sizePolicy);

				//Ensure that the parent stretches.
				isStretching = true;
			}

			//Append the fieldset to the current layout.
			layout->addRow(fieldset);
		}

		//Check if current element is input.
		if(element.tagName() == "input")
		{
			//Use the factory to build the dynamic item.
			WizardDynamicItem* dynamicItem = _itemFactory.createItem(this, element, layout);
			if(dynamicItem)
			{
				//Check, wether the item wants to stretch.
				if(dynamicItem->isStretching())
					isStretching = true;

				//Store the created item.
				_dynamicItems[dynamicItem->getName()] = dynamicItem;
			}
		}
	}

	//Return wether the layout wants full height.
	return isStretching;
}

void WizardItemCollection::initializeItems()
{
	//Iterate over all items and reset them.
	foreach(WizardDynamicItem* dynamicItem, _dynamicItems)
		dynamicItem->resetValue();

	//Initialize the item's states.
	updateItems();
}

void WizardItemCollection::updateItems()
{
	//Iterate over all items and update their state.
	foreach(WizardDynamicItem* dynamicItem, _dynamicItems)
	{
		//Iterate over the item's dependencies and validate them.
		bool dependenciesFulfilled = _isEnabled;
		QList<QString> dependencies = dynamicItem->getDependencies();
		foreach(QString dependency, dependencies)
		{
			//Retrieve the dependency's value, compare as string.
			QString fieldValue = _delegate->dynamicField(dependency).toString();
			if(!dynamicItem->checkDependency(dependency, fieldValue))
				dependenciesFulfilled = false;
		}

		//Enable, if all dependencies are fulfilled.
		if(dependenciesFulfilled)
			dynamicItem->setEnabled(true);

		//Disable and reset, if dependencies are missing.
		else
		{
			dynamicItem->setEnabled(false);
			dynamicItem->resetValue();
		}
	}

	//Notify about state change.
	_delegate->notifyCompleteChanged();
}

bool WizardItemCollection::isComplete() const
{
	//Iterate over all item's and check their state.
	foreach(WizardDynamicItem* dynamicItem, _dynamicItems)
		if(dynamicItem->isMandatory() && dynamicItem->isEnabled() && !dynamicItem->isComplete())
			return false;

	//All items validated.
	return true;
}

QList<WizardDynamicItem*> WizardItemCollection::getItems() const { return _dynamicItems.values(); }
WizardDynamicItem* WizardItemCollection::getItem(const QString &name) const { return _dynamicItems.value(name); }

bool WizardItemCollection::isEnabled() const { return _isEnabled; }
void WizardItemCollection::setEnabled(bool isEnabled)
{
	//Ensure state changes.
	if(_isEnabled == isEnabled) return;

	//Set new value and update.
	_isEnabled = isEnabled;
	updateItems();
}

WizardWindow* WizardItemCollection::getWindow() const { return _window; }

IWizardDynamicDelegate* WizardItemCollection::getDelegate() const { return _delegate; }
void WizardItemCollection::setDelegate(IWizardDynamicDelegate* delegate) { _delegate = delegate; }
