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

#include "wizarditemspage.h"
#include "../dialogs/wizarditemdialog.h"
#include "../wizardwindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>

using namespace ysm;

WizardItemsPage::WizardItemsPage(QDomElement rootElement, WizardWindow* parent) :
	WizardPageDelegate(parent),
	WizardDependencyObject(rootElement),
	_parent(parent),
	_items(rootElement),
	_nextIndex(0)
{
	//Initialize the list view.
	_itemList = new QListWidget(this);
	_prefix = rootElement.attribute("name");

	//Initialize the buttons container.
	QWidget* buttonsWidget = new QWidget(this);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);

	//Initialize the buttons.
	_addButton = new QPushButton(tr("Add"), buttonsWidget);
	_editButton = new QPushButton(tr("Edit"), buttonsWidget);
	_removeButton = new QPushButton(tr("Delete"), buttonsWidget);

	//Connfigure the buttons.
	connect(_addButton, &QPushButton::clicked, this, &WizardItemsPage::add);
	connect(_editButton, &QPushButton::clicked, this, &WizardItemsPage::edit);
	connect(_removeButton, &QPushButton::clicked, this, &WizardItemsPage::remove);

	//Layout the buttons.
	buttonsLayout->addWidget(_addButton);
	buttonsLayout->addWidget(_editButton);
	buttonsLayout->addWidget(_removeButton);
	buttonsLayout->addStretch();
	buttonsLayout->setMargin(0);

	//Initialize the main layout.
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(_itemList);
	mainLayout->addWidget(buttonsWidget);

	//Configure the page.
	setTitle(WizardFile::getProperty(rootElement, "title"));
	setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
}

WizardItemsPage::~WizardItemsPage()
{
	//TODO
}

QString WizardItemsPage::dynamicName(const QString& name) const
{
	//Prefix the name by the source name.
	return QString("%1%2").arg(_prefix).arg(name);
}

bool WizardItemsPage::isComplete() const { return _itemList->count(); }
void WizardItemsPage::initializePage() { }

void WizardItemsPage::add()
{
	//Create a new dialog and according delegate.
	WizardItemDialog* itemDialog = new WizardItemDialog(_parent);
	WizardEnumerationDelegate* delegate = new WizardEnumerationDelegate(this, _nextIndex++);

	//Display the dialog.
	itemDialog->setDelegate(delegate);
	itemDialog->generateLayout(_items);
	if(itemDialog->exec() == QDialog::Rejected)
	{
		//Clear the data on reject.
		delete itemDialog;
		delete delegate;
		return;
	}

	//Store the dialog data.
	QString itemIndex = _parent->appendItem(_prefix);
	_dialogs[itemIndex] = qMakePair(itemDialog, delegate);

	//Add a new item to the list.
	QListWidgetItem* itemWidget = new QListWidgetItem(itemDialog->getName(), _itemList);
	itemWidget->setData(Qt::UserRole, itemIndex);

	//Notify about change.
	emit completeChanged();
}

void WizardItemsPage::edit()
{
	//Get the selected item.
	QListWidgetItem* itemWidget = _itemList->currentItem();
	if(!itemWidget)
		return;

	//Find the item's dialog.
	QString itemIndex = itemWidget->data(Qt::UserRole).toString();
	if(!_dialogs.contains(itemIndex))
		return;

	//Display the dialog again.
	_dialogs[itemIndex].first->exec();

	//Rename the item, if neccessary.
	itemWidget->setText(_dialogs[itemIndex].first->getName());
}

void WizardItemsPage::remove()
{
	//Get the selected item.
	QListWidgetItem* itemWidget = _itemList->currentItem();
	if(!itemWidget)
		return;

	//Find the item's dialog.
	QString itemIndex = itemWidget->data(Qt::UserRole).toString();
	if(!_dialogs.contains(itemIndex))
		return;

	//Remove the item from the list.
	ItemDialog dialog = _dialogs[itemIndex];
	_parent->removeItem(_prefix, itemIndex);
	_dialogs.remove(itemIndex);

	//Clear the data.
	delete dialog.first;
	delete dialog.second;
	delete itemWidget;

	//Notify about change.
	emit completeChanged();
}
