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

#include "wizardcomboboxitem.h"
#include "../../wizardwindow.h"

using namespace ysm;

WizardComboBoxItem::WizardComboBoxItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout) :
	WizardDynamicItem(element, collection)
{
	//Create the item's widget.
	_widget = new QComboBox(collection->getWindow());
	_widget->setToolTip(getDescription());

	//Configure the widget.
	for(QDomElement option = element.firstChildElement("option"); !option.isNull();
		option = option.nextSiblingElement("option"))
	{
		//Just insert the option into the combo box.
		_options.append(qMakePair(option.text(), option.attribute("value")));
		_widget->addItem(option.text(), option.attribute("value"));
	}

	//Register the new item.
	collection->getDelegate()->registerDynamicField(getName(), _widget, "currentData");
	connect(_widget, SIGNAL(currentIndexChanged(int)), collection, SLOT(updateItems()));
	layout->addRow(getLabel(collection->getWindow()), _widget);
}

void WizardComboBoxItem::addOption(const QString &option, const QString &value) { _widget->addItem(option, value); }
void WizardComboBoxItem::removeOption(const QString &value) { _widget->removeItem(_widget->findData(value)); }
QString WizardComboBoxItem::getOption() const { return _widget->currentData().toString(); }
void WizardComboBoxItem::setOption(const QString& option)
{
	//Activate the given option.
	return _widget->setCurrentIndex(_widget->findData(option));
}

void WizardComboBoxItem::resetOptions()
{
	//Reset all options.
	_widget->clear();

	//Add the default options.
	for(QPair<QString, QString> option : _options)
		_widget->addItem(option.first, option.second);
}

bool WizardComboBoxItem::isEnabled() const { return _widget->isEnabled(); }
void WizardComboBoxItem::setEnabled(bool isEnabled) { _widget->setEnabled(isEnabled); }

void WizardComboBoxItem::resetValue()
{
	//Ensure value is only reset if neccessary.
	int defaultIndex = _widget->findData(getDefault());
	if(_widget->currentIndex() != defaultIndex)
		_widget->setCurrentIndex(defaultIndex);
}
