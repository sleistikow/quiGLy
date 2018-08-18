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

#include "wizarddynamicitem.h"
#include "../../wizardfile.h"
#include "../../wizardwindow.h"

using namespace ysm;

WizardDynamicItem::WizardDynamicItem(QDomElement element, QObject* parent) :
	WizardDependencyObject(element),
	QObject(parent)
{
	//Retrieve the data from DOM.
	_field = element.attribute("name");
	_default = element.attribute("default");
	_mandatory = element.attribute("mandatory", "false") == "true";
	_caption = WizardFile::getProperty(element, "caption");
	_description = WizardFile::getProperty(element, "description");
}

WizardDynamicItem::~WizardDynamicItem() { }

bool WizardDynamicItem::isComplete() const { return true; }
bool WizardDynamicItem::isStretching() const { return false; }

QString WizardDynamicItem::getName() const { return _field; }
QString WizardDynamicItem::getCaption() const { return _caption; }
QString WizardDynamicItem::getDefault() const { return _default; }
QString WizardDynamicItem::getDescription() const { return _description; }
bool WizardDynamicItem::isMandatory() const { return _mandatory; }

QLabel* WizardDynamicItem::getLabel(WizardWindow* parent) const { return WizardWindow::getLabel(getCaption()); }
