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

#include "wizardstackedpage.h"
#include "../wizardwindow.h"

using namespace ysm;

WizardStackedPage::WizardStackedPage(QDomElement rootElement, WizardWindow* parent) :
	WizardPageDelegate(parent),
	WizardDependencyObject(rootElement),
	_parent(parent)
{
	//Retrieve the relevant data.
	_prefix = rootElement.attribute("name");

	//Initialize the main widget.
	_collectionStack = new WizardCollectionStack(parent);
	_collectionStack->setDelegate(this);
	_collectionStack->generateLayout(new QFormLayout(this), rootElement);

	//Configure the page.
	setTitle(WizardFile::getProperty(rootElement, "title"));
	setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
}

QString WizardStackedPage::dynamicName(const QString& name) const
{
	//Prefix the field name by the source name.
	return QString("%1.%2").arg(_prefix).arg(name);
}

bool WizardStackedPage::isComplete() const { return true; }

void WizardStackedPage::initializePage()
{
	//Reset the collection stack.
	_collectionStack->initializeItems();
}
