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

#include "enumpropertyviewitem.h"
#include "views/common/enumnames/enumnamestemplate.h"

using namespace ysm;

EnumPropertyViewItem::EnumPropertyViewItem(IProperty* property, IView* parentView) :
	GenericPropertyViewItem(property, parentView)
{
	//Retrieve the list of all possible values.
	QMap<int, QString> enumValues = EnumPropertyHelper::getNames(dynamic_cast<EnumProperty*>(property));

	//Set label to property name.
	setLabel(new QLabel(property->getName()));

	//Create the combo box.
	_comboBox = new EnumComboBox<int>(enumValues);
	_comboBox->setDisabled(getProperty()->isReadOnly());
	_comboBox->setMinimumWidth(120);
	connect(_comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateModelValue()));
	setEdit(_comboBox);
}

int EnumPropertyViewItem::getViewValue() const { return _comboBox->currentData().toInt(); }
void EnumPropertyViewItem::updateViewValue()
{
	_comboBox->setCurrentIndex(_comboBox->findData(getModelValue()));
	_comboBox->setDisabled(getProperty()->isReadOnly());
}
