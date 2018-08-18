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

#include "wizardstackeditem.h"

using namespace ysm;

WizardStackedItem::WizardStackedItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout) :
	WizardDynamicItem(element, collection),
	WizardPrefixDelegate(),
	_parent(collection),
	_isInitial(false)
{
	//Retrieve the relevant data.
	QString caption = WizardFile::getProperty(element, "caption");
	_prefix = element.attribute("name");
	_default = element.attribute("default");
	_delegate = collection->getDelegate();

	//Access the wizard.
	WizardWindow* parent = collection->getWindow();

	//Initialize the main widget.
	_collectionStack = new WizardCollectionStack(parent);
	_collectionStack->setDelegate(this);
	_collectionStack->setCaption(caption);
	_collectionStack->generateLayout(layout, element);
}

bool WizardStackedItem::isMandatory() const { return true; }
bool WizardStackedItem::isComplete() const { return _collectionStack->isComplete(); }
void WizardStackedItem::resetValue()
{
	//Ensure the values are changed.
	if(!_isInitial)
	{
		//Items are initialized.
		_isInitial = true;

		//Reset the collection stack.
		_collectionStack->setCurrentType(_default);
		_collectionStack->initializeItems();
	}
}

bool WizardStackedItem::isEnabled() const { return _collectionStack->isEnabled(); }
void WizardStackedItem::setEnabled(bool isEnabled) { _collectionStack->setEnabled(isEnabled);  }

void WizardStackedItem::notifyCompleteChanged()
{
	//Update the parent.
	_parent->updateItems();
	_isInitial = false;

	//Call the base.
	WizardPrefixDelegate::notifyCompleteChanged();
}
