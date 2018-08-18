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

#include "mixerlayoutpropertyviewitem.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/iport.h"
#include "data/types/typeutils.h"
#include "views/common/enumnames/datatypehelper.h"

#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>

using namespace ysm;

MixerLayoutPropertyViewItem::MixerLayoutPropertyViewItem(IProperty* property, IView* parentView) :
	TabbedPropertyViewItem(property, parentView)
{
	//Create layout options.
	_layoutGrouped = new QRadioButton(tr("Interleaved"));
	_layoutSequential = new QRadioButton(tr("Separate"));

	connect(_layoutGrouped, SIGNAL(clicked(bool)), SLOT(updateModelValue()));
	connect(_layoutSequential, SIGNAL(clicked(bool)), SLOT(updateModelValue()));

	//Create layout group box.
	QGroupBox* layoutGroupBox = new QGroupBox(tr("Alignment"));
	addWidget(layoutGroupBox, 1);

	//Layout the group box.
	QVBoxLayout* groupBoxLayout = new QVBoxLayout(layoutGroupBox);
	groupBoxLayout->addWidget(_layoutGrouped);
	groupBoxLayout->addWidget(_layoutSequential);

	//Create and connect the add button.
	QPushButton* addButton = new QPushButton("+");
	connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));
	addWidget(addButton, 2, 0);

	//Create and connect the add button.
	QPushButton* deleteButton = new QPushButton("-");
	connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteEntry()));
	addWidget(deleteButton, 2, 1);

	//Create and connect the reset button.
	QPushButton* configButton = new QPushButton("Reset");
	connect(configButton, SIGNAL(clicked(bool)), this, SLOT(autoConfigure()));
	addWidget(configButton, 3);
}

MixerLayoutPropertyViewItemTab* MixerLayoutPropertyViewItem::createTab(QFormLayout* layout, int entry)
{
	//Create the tab and access the data port.
	MixerLayoutPropertyViewItemTab* tabItem = new MixerLayoutPropertyViewItemTab();
	IBlock* parentBlock = dynamic_cast<IBlock*>(getProperty()->getOwner());

	//Create the interface elements.
	tabItem->_connection = new ConnectionComboBox(parentBlock->getPort(PortType::Data_In), this);
	tabItem->_name = new QLineEdit();
	tabItem->_conversion = new EnumComboBox<DataType>(DataTypeHelper::getTypedConversions());
	tabItem->_swizzling = new SwizzleBox();

	//Configure interface elements.
	tabItem->_swizzling->addDimension("X");
	tabItem->_swizzling->addDimension("Y");
	tabItem->_swizzling->addDimension("Z");
	tabItem->_swizzling->addDimension("W");

	//Connect to model update.
	connect(tabItem->_connection, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));
	connect(tabItem->_name, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
	connect(tabItem->_conversion, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));
	connect(tabItem->_swizzling, SIGNAL(swizzled()), this, SLOT(updateModelValue()));

	//Create the layout.
	layout->addRow(new QLabel("Connection"), tabItem->_connection);
	layout->addRow(new QLabel("Name"), tabItem->_name);
	layout->addRow(new QLabel("Conversion"), tabItem->_conversion);
	layout->addRow(new QLabel("Swizzling"), tabItem->_swizzling);

	//Update the values.
	updateTab(tabItem, entry);
	return tabItem;
}

void MixerLayoutPropertyViewItem::updateTab(MixerLayoutPropertyViewItemTab* item, int entry)
{
	//Get the layout entry.
	MixerLayout::MixerLayoutEntry layoutEntry = getCachedValue().getEntries()->value(entry);

	//Set the values.
	item->_name->setText(layoutEntry.name);
	item->_conversion->setCurrentEnum(layoutEntry.typeConversion.targetType);
	item->_connection->setCurrentConnection(layoutEntry.dataConnection);
	item->_swizzling->setSourceType(getConnectionDataType(item));
	item->_swizzling->setTargetType(layoutEntry.typeConversion.targetType);
	item->_swizzling->setSwizzling(layoutEntry.typeConversion);

	//Update layout as struct flag.
	_layoutGrouped->setChecked(getCachedValue().getEntriesAsStruct());
	_layoutSequential->setChecked(!getCachedValue().getEntriesAsStruct());
}

void MixerLayoutPropertyViewItem::updateModelValue()
{
	//Create the new layout.
	MixerLayout newLayout;
	for(int i = 0; i < getCachedValue().getEntries()->count(); i++)
	{
		//Create the next tab entry.
		MixerLayoutPropertyViewItemTab* item = getItem(i);
		item->_swizzling->setSourceType(getConnectionDataType(item));
		item->_swizzling->setTargetType(item->_conversion->getCurrentEnum());
		newLayout.addEntry(item->_connection->getCurrentConnection(), item->_swizzling->getSwizzling(),
						   item->_name->text());
	}

	//Set layout as struct.
	newLayout.setEntriesAsStruct(_layoutGrouped->isChecked());

	//Call the base to store the property.
	//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
	getCachedValue() = newLayout;
	TabbedPropertyViewItem::updateModelValue();
	updateViewValue();
}

DataType MixerLayoutPropertyViewItem::getConnectionDataType(const MixerLayoutPropertyViewItemTab* tabItem) const
{
	//Ensure a connection is selected.
	IConnection* currentConnection = tabItem->_connection->getCurrentConnection();
	if(currentConnection == NULL)
		return DataType::NoType;

	//Connections between a Mixer and a DataSource always carry a property describing their output data type.
	EnumProperty* connectionDataType = currentConnection->getProperty<EnumProperty>(PropertyID::Data_OutputType);

	//Ensure the property was available.
	if(!connectionDataType)
		return DataType::NoType;

	//Evaluate the output type.
	return static_cast<DataType>(connectionDataType->getValue());
}

DataType MixerLayoutPropertyViewItem::getSelectedDataType(const MixerLayoutPropertyViewItemTab *tabItem) const
{
	//Retrieve the selected target data type, fall back to connection type.
	DataType targetType = tabItem->_conversion->getCurrentEnum();
	if(targetType == DataType::NoType)
		targetType = getConnectionDataType(tabItem);

	//Return the correct target type.
	return targetType;
}

int MixerLayoutPropertyViewItem::getEntryCount() { return getCachedValue().getEntries()->count(); }

void MixerLayoutPropertyViewItem::addEntry()
{
	//Ensure that connections are available.
	IBlock* block = dynamic_cast<IBlock*>(getProperty()->getOwner());
	if(block->getInConnections().count())
	{
		//Create default conversion options.
		TypeConversion::ConversionOptions conversionOptions;
		conversionOptions.targetType = DataType::NoType;

		//Add a new entry, then store the changed model value.
		//The updateModelValue() suppresses view refreshing events, so update manually afterwards.
		getCachedValue().addEntry(block->getInConnections().first(), conversionOptions, QString());
		TabbedPropertyViewItem::updateModelValue();
		updateViewValue();

		//Display the new tab.
		switchToLastTab();
	}
}

void MixerLayoutPropertyViewItem::deleteEntry()
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

void MixerLayoutPropertyViewItem::autoConfigure()
{
	//Retrieve the layout's block.
	IBlock* block = dynamic_cast<IBlock*>(getProperty()->getOwner());

	//Try to autoconfigure a new layout.
	MixerLayout autoLayout;
	if(autoLayout.autoConfigureLayout(block))
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
