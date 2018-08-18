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

#include "wizardcustompage.h"
#include "../wizardwindow.h"

#include <QVBoxLayout>

using namespace ysm;

WizardCustomPage::WizardCustomPage(QDomElement rootElement, WizardWindow* parent, bool delayCreate) :
	WizardPageDelegate(parent),
	WizardDependencyObject(rootElement),
	_itemCollection(NULL),
	_parent(parent)
{
	//Configure the page.
	if(!delayCreate)
	{
		//Initialize the main widget.
		QFormLayout* pageLayout = new QFormLayout(this);
		createItemCollection(pageLayout, rootElement);

		//Configure the page.
		setTitle(WizardFile::getProperty(rootElement, "title"));
		setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
	}
}

WizardCustomPage::WizardCustomPage(QWidget* parent) :
	WizardPageDelegate(parent),
	_itemCollection(NULL),
	_parent(NULL)
{ }

QString WizardCustomPage::dynamicName(const QString& name) const { return name; }
bool WizardCustomPage::isComplete() const { return !_itemCollection || _itemCollection->isComplete(); }
void WizardCustomPage::initializePage() { if(_itemCollection) _itemCollection->initializeItems(); }

void WizardCustomPage::createItemCollection(QFormLayout* formLayout, QDomElement rootElement)
{
	//Create the item collection from XML.
	if(_parent)
	{
		_itemCollection = new WizardItemCollection(_parent);
		_itemCollection->setDelegate(this);
		_itemCollection->generateLayout(formLayout, rootElement);
	}
}
