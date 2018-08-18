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

#include "varyingspropertyviewitem.h"

namespace ysm
{
	VaryingsPropertyViewItem::VaryingsPropertyViewItem(IProperty* property, IView* parentView) :
		TabbedPropertyViewItem(property, parentView)
	{
		//Create and connect the add button.
		QPushButton* addButton = new QPushButton("+");
		connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addEntry()));
		addWidget(addButton, 1, 0);

		//Create and connect the delete button.
		QPushButton* deleteButton= new QPushButton("-");
		connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteEntry()));
		addWidget(deleteButton, 1 , 1);
	}

	VaryingsPropertyViewItemTab* VaryingsPropertyViewItem::createTab(QFormLayout *layout, int entry)
	{
		//Create the tab.
		VaryingsPropertyViewItemTab* tabItem = new VaryingsPropertyViewItemTab();

		//Create the interface elements.
		tabItem->_name = new QLineEdit();
		tabItem->_type = new EnumComboBox<GLSLDataType>(GLTypes::getGLSLDataTypeNames());

		//Connect to model update.
		connect(tabItem->_name, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
		connect(tabItem->_type, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));

		//Create the layout.
		layout->addRow(new QLabel("Name"), tabItem->_name);
		layout->addRow(new QLabel("Type"), tabItem->_type);

		//Update the values.
		updateTab(tabItem, entry);
		return tabItem;
	}

	void VaryingsPropertyViewItem::addEntry()
	{
		//Add a new entry to the cached value, the update the model.
		getCachedValue().addEntry(QString(), GLSLDataType::Float);
		TabbedPropertyViewItem::updateModelValue();

		//Refresh the view (to load the initial state) and display the new tab.
		updateViewValue();
		switchToLastTab();
	}

	void VaryingsPropertyViewItem::deleteEntry()
	{
		//Remove the entry, then update model value and the view.
		if(getCurrentEntry() >= 0)
		{
			getCachedValue().getEntries()->remove(getCurrentEntry());
			TabbedPropertyViewItem::updateModelValue();
			updateViewValue();
		}
	}

	int VaryingsPropertyViewItem::getEntryCount() { return getCachedValue().getEntries()->count(); }

	void VaryingsPropertyViewItem::updateTab(VaryingsPropertyViewItemTab *item, int entry)
	{
		//Get the layout entry.
		Varyings::VaryingEntry varyingEntry = getCachedValue().getEntries()->value(entry);

		//Set the values.
		item->_name->setText(varyingEntry.name);
		item->_type->setCurrentEnum(varyingEntry.type);
	}

	void VaryingsPropertyViewItem::updateModelValue()
	{
		//Create the new layout and add all tab entries.
		Varyings newVaryings;
		for(int i = 0; i < getCachedValue().getEntries()->count(); ++i)
		{
			//Create the next tab entry.
			VaryingsPropertyViewItemTab* item = getItem(i);
			newVaryings.addEntry(item->_name->text(), item->_type->getCurrentEnum());
		}

		//Call the base to store the property.
		getCachedValue() = newVaryings;
		TabbedPropertyViewItem::updateModelValue();
	}
}
