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

#include "wizarditemdialog.h"
#include "../wizardwindow.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

using namespace ysm;

WizardItemDialog::WizardItemDialog(WizardWindow* wizardWindow) :
	QDialog(wizardWindow),
	_nameEdit(NULL),
	_okButton(NULL),
	_cancelButton(NULL),
	_parent(wizardWindow)
{ }

void WizardItemDialog::generateLayout(QDomElement rootElement)
{
	//Retrieve the data.
	_autoName = rootElement.attribute("autoName") == "true";

	//Add line edit, if user names the items.
	QFormLayout* mainLayout = new QFormLayout(this);
	if(!_autoName)
	{
		//Create the name line edit.
		_nameEdit = new QLineEdit(this);
		mainLayout->addRow(WizardWindow::getLabel(tr("Name")), _nameEdit);
	}

	//Create the dialog select.
	_itemSwitch = new WizardCollectionStack(_parent);
	_itemSwitch->setDelegate(this);
	_itemSwitch->generateLayout(mainLayout, rootElement);

	//Connect to size adjustments.
	connect(_itemSwitch, &WizardCollectionStack::currentTypeChanged, this, &WizardItemDialog::adjustSize);

	//Create the dialog buttons.
	mainLayout->addItem(new QSpacerItem(0, 5));
	mainLayout->addRow(generateButtons());

	//Initialize the buttons.
	notifyCompleteChanged();
}

QWidget* WizardItemDialog::generateButtons()
{
	//Create the buttons.
	_okButton = new QPushButton(tr("Ok"));
	_cancelButton = new QPushButton(tr("Cancel"));
	connect(_okButton, &QPushButton::clicked, this, &WizardItemDialog::accept);
	connect(_cancelButton, &QPushButton::clicked, this, &WizardItemDialog::reject);

	//Create the buttons widget.
	QWidget* buttonsWidget = new QWidget(this);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
	buttonsLayout->addStretch();
	buttonsLayout->setMargin(0);
	buttonsLayout->addWidget(_okButton);
	buttonsLayout->addWidget(_cancelButton);

	//Return the widget.
	return buttonsWidget;
}

IWizardDynamicDelegate* WizardItemDialog::getDelegate() const { return _delegate; }
void WizardItemDialog::setDelegate(IWizardDynamicDelegate* delegate) { _delegate = delegate; }

void WizardItemDialog::setName(const QString& name) { if(_nameEdit) _nameEdit->setText(name); }
QString WizardItemDialog::getName() const
{
	//Check if the item can be automatically named.
	if(_autoName)
	{
		//Find the source that names the item.
		WizardCollectionStack::ItemType itemType = _itemSwitch->getCurrentType();
		QString nameSource = _delegate->dynamicName(itemType._nameSource);
		WizardWindow::Source source = _parent->getSource(nameSource);

		//Retrieve the source's user name.
		return source.getUserName();
	}

	//Return the user name or empty string.
	return _nameEdit ? _nameEdit->text() : QString();
}

void WizardItemDialog::notifyCompleteChanged()
{
	//Update the button state, no need to forward.
	if(_okButton) _okButton->setEnabled(_itemSwitch->isComplete());
}
