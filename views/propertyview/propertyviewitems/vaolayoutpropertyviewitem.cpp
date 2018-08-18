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

#include "vaolayoutpropertyviewitem.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/iport.h"

#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

using namespace ysm;

VaoLayoutPropertyViewItem::VaoLayoutPropertyViewItem(IProperty* property, IView* parentView) :
	TabbedPropertyViewItem(property, parentView)
{
	//Create and connect the add button.
	QPushButton* addButton = new QPushButton("+");
	connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));
	addWidget(addButton, 1, 0);

	//Create and connect the delete button.
	QPushButton* deleteButton = new QPushButton("-");
	connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteEntry()));
	addWidget(deleteButton, 1, 1);

	//Create and connect the reset button.
	QPushButton* configButton = new QPushButton("Reset");
	connect(configButton, SIGNAL(clicked(bool)), this, SLOT(autoConfigure()));
	addWidget(configButton, 2);
}

VaoLayoutPropertyViewItemTab* VaoLayoutPropertyViewItem::createTab(QFormLayout* layout, int entry)
{
	//Create the tab and access the data port.
	VaoLayoutPropertyViewItemTab* tabItem = new VaoLayoutPropertyViewItemTab();
	IBlock* parentBlock = dynamic_cast<IBlock*>(getProperty()->getOwner());

	//Create the interface elements.
	tabItem->_connection = new ConnectionComboBox(parentBlock->getPort(PortType::Data_In), this);
	tabItem->_index = new QSpinBox();
	tabItem->_name = new QLineEdit();
	tabItem->_normalized = new QCheckBox("Normalized");
	tabItem->_offset = new QSpinBox();
	tabItem->_size = new QSpinBox();
	tabItem->_stride = new QSpinBox();
	tabItem->_type = new EnumComboBox<GLDataType>(GLTypes::getDataTypeNames());
	tabItem->_glslType = new EnumComboBox<GLSLDataType>(GLTypes::getGLSLDataTypeNames());

	//Configure interface elements.
	tabItem->_index->setRange(0, INT_MAX);
	tabItem->_offset->setRange(0, INT_MAX);
	tabItem->_size->setRange(0, INT_MAX);
	tabItem->_stride->setRange(0, INT_MAX);

	//Connect to model update.
	connect(tabItem->_connection, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));
	connect(tabItem->_index, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_name, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_normalized, SIGNAL(toggled(bool)), this, SLOT(updateModelValue()));
	connect(tabItem->_offset, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_size, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_stride, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_type, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));
	connect(tabItem->_glslType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));

	//Create the layout.
	layout->addRow(new QLabel("Connection"), tabItem->_connection);
	layout->addRow(new QLabel("Index / Attribute Location"), tabItem->_index);
	layout->addRow(new QLabel("Name"), tabItem->_name);
	layout->addRow(new QLabel("Data Type"), tabItem->_type);
	layout->addRow(new QLabel("Size"), tabItem->_size);
	layout->addRow(new QLabel("GLSL Type"), tabItem->_glslType);
	layout->addRow(NULL, tabItem->_normalized);
	layout->addRow(new QLabel("Offset"), tabItem->_offset);
	layout->addRow(new QLabel("Stride"), tabItem->_stride);

	//Update the values.
	updateTab(tabItem, entry);
	return tabItem;
}

void VaoLayoutPropertyViewItem::updateTab(VaoLayoutPropertyViewItemTab* item, int entry)
{
	//Get the layout entry.
	VaoLayout::VaoLayoutEntry layoutEntry = getCachedValue().getEntries()->value(entry);

	//Set the values.
	item->_connection->setCurrentConnection(layoutEntry.bufferConnection);
	item->_index->setValue(layoutEntry.index);
	item->_name->setText(layoutEntry.name);
	item->_normalized->setChecked(layoutEntry.normalized);
	item->_offset->setValue(layoutEntry.offset);
	item->_size->setValue(layoutEntry.size);
	item->_stride->setValue(layoutEntry.stride);
	item->_type->setCurrentEnum(layoutEntry.type);
	item->_glslType->setCurrentEnum(layoutEntry.glslType);
}

void VaoLayoutPropertyViewItem::updateModelValue()
{	
	//Create the new layout and add all tab entries.
	VaoLayout newLayout;
	for(int i = 0; i < getCachedValue().getEntries()->count(); i++)
	{
		//Create the next tab entry.
		VaoLayoutPropertyViewItemTab* item = getItem(i);
		newLayout.addEntry(item->_connection->getCurrentConnection(), item->_name->text(), item->_index->value(),
						   item->_size->value(), item->_type->getCurrentEnum(), item->_normalized->isChecked(),
						   item->_stride->value(), item->_offset->value(), item->_glslType->getCurrentEnum());
	}

	//Call the base to store the property.
	//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
	getCachedValue() = newLayout;
	TabbedPropertyViewItem::updateModelValue();
	updateViewValue();
}

int VaoLayoutPropertyViewItem::getEntryCount() { return getCachedValue().getEntries()->count(); }

void VaoLayoutPropertyViewItem::addEntry()
{
	//Ensure that connections are available.
	IBlock* block = dynamic_cast<IBlock*>(getProperty()->getOwner());
	if(block->getPort(PortType::Data_In)->getInConnections().count())
	{
		//Add a new entry, then store the changed model value.
		//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
		getCachedValue().addEntry(NULL, QString(), 0, 0, GLDataType::Byte, false, 0, 0, GLSLDataType::Float);
		TabbedPropertyViewItem::updateModelValue();
		updateViewValue();

		//Display the new tab.
		switchToLastTab();
	}
}

void VaoLayoutPropertyViewItem::deleteEntry()
{
	//Remove the entry, then store the changed model value.
	if(getCurrentEntry() >= 0)
	{
		//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
		getCachedValue().getEntries()->remove(getCurrentEntry());
		TabbedPropertyViewItem::updateModelValue();
		updateViewValue();
	}
}

void VaoLayoutPropertyViewItem::autoConfigure()
{
	//Retrieve the layout's block.
	IBlock* block = dynamic_cast<IBlock*>(getProperty()->getOwner());

	//Try to autoconfigure a new layout.
	VaoLayout autoLayout;
	if (autoLayout.autoConfigureLayout(block))
	{
		//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
		getCachedValue() = autoLayout;
		TabbedPropertyViewItem::updateModelValue();
		updateViewValue();
	}

	//Auto configuration not successful.
	else
		QMessageBox::warning(nullptr, "Layout configuration", "The layout cannot be automatically configured.", QMessageBox::Ok);
}
