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

#include "stringlistpropertyviewitem.h"

#include <QPushButton>

using namespace ysm;

StringListPropertyViewItem::StringListPropertyViewItem(IProperty* property, IView* parentView) :
	GenericPropertyViewItem(property, parentView)
{
	//No label.
	setLabel(new QLabel(property->getName()));

	//Create edit widget.
	QWidget* editWidget = new QWidget();
	QGridLayout* layout = new QGridLayout(editWidget);
	layout->setMargin(0);
	setEdit(editWidget);

	//Create edit widget items.
	_listView = new QListView(editWidget);
	_listView->setModel(_tableModel = new QStringListModel(this));
	_listView->setSelectionMode(QAbstractItemView::SingleSelection);
	_listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	_listView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	_listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	//Create add and delete buttons.
    addButton = new QPushButton("+");
    deleteButton = new QPushButton("-");

	//Add to layout.
	layout->addWidget(_listView, 0, 0, 1, 2);
	layout->addWidget(addButton, 1, 0);
	layout->addWidget(deleteButton, 1, 1);

	//Connect signals.
	connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));
	connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteEntry()));
	connect(_tableModel, SIGNAL(dataChanged(QModelIndex ,QModelIndex, QVector<int>)), this, SLOT(updateModelValue()));

	//Check if property is readonly.
    addButton->setHidden(getProperty()->isReadOnly());
    deleteButton->setHidden(getProperty()->isReadOnly());
}

QStringList StringListPropertyViewItem::getViewValue() const { return _tableModel->stringList(); }
void StringListPropertyViewItem::updateViewValue()
{
    _tableModel->setStringList(getModelValue());
    addButton->setHidden(getProperty()->isReadOnly());
    deleteButton->setHidden(getProperty()->isReadOnly());
}

void StringListPropertyViewItem::addEntry()
{
	//Adjust the list, then update.
	_tableModel->insertRow(_tableModel->rowCount());
	updateModelValue();
}

void StringListPropertyViewItem::deleteEntry()
{
	//Get the current selection.
	QItemSelectionModel* selection = _listView->selectionModel();
	if(!selection->currentIndex().isValid())
		return;

	//Remove the entry, then update.
	_tableModel->removeRow(selection->currentIndex().row());
	updateModelValue();
}
