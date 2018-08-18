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

#include "defaultpropertyviewitem.h"

using namespace ysm;

DefaultPropertyViewItem::DefaultPropertyViewItem(IProperty* property, IView* parentView) :
	QObject(),
	ViewItem(parentView),
	IPropertyViewItem(),
	_property(property),
	_synchronizingData(false)
{
	//Create label and line edit.
	_label = new QLabel(property->getName());
	_lineEdit = new QLineEdit();

	//Connect to view and model.
	notifyStatic(IChangeable::Change, this, &DefaultPropertyViewItem::updateView);
	connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateModel()));
}

PipelineItemPropertyView* DefaultPropertyViewItem::getPropertyView() const { return _propertyView; }
void DefaultPropertyViewItem::setPropertyView(PipelineItemPropertyView* propertyView) { _propertyView = propertyView; }

QString DefaultPropertyViewItem::getModelValue() const { return _property->toString(); }
QString DefaultPropertyViewItem::getViewValue() const { return _lineEdit->text(); }

QWidget* DefaultPropertyViewItem::getLabel() const { return _label; }
QWidget* DefaultPropertyViewItem::getEdit() const { return _lineEdit; }
bool DefaultPropertyViewItem::isStretching() const { return false; }

void DefaultPropertyViewItem::updateView(IChangeable* property)
{
	//If the property matches update the view.
	if(_property == property)
		updateView();
}

void DefaultPropertyViewItem::updateView()
{
	//Avoid call cycles.
	if(!_synchronizingData)
	{
		_synchronizingData = true;
		_lineEdit->setText(getModelValue());
		_synchronizingData = false;
	}

	_lineEdit->setReadOnly(_property->isReadOnly());
}

void DefaultPropertyViewItem::updateModel()
{
	//Avoid call cycles.
	if(!_synchronizingData && getModelValue() != getViewValue())
	{
		//Set property from string.
		_synchronizingData = true;

		UpdatePropertiesCommand* updateCommand = new UpdatePropertiesCommand(_property->getOwner());
		updateCommand->addFromStringOperation(_property, getViewValue());
		executeCommand(updateCommand);
		_synchronizingData = false;
	}
}
