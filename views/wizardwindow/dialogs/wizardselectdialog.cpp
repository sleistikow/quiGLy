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

#include "wizardselectdialog.h"
#include "wizardoverviewdialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>

using namespace ysm;
WizardSelectDialog::WizardSelectDialog(const QString& resourcePath, bool multiPass, QWidget* parent) :
	QDialog(parent),
	_resourcePath(resourcePath),
	_multiPass(multiPass)
{
	//Create the list view.
	_wizardList = new QListWidget(this);

	//Create the description view.
	_wizardDescription = new QTextEdit(this);
	_wizardDescription->setReadOnly(true);

	//Do not show the text cursor in description.
	_wizardDescription->viewport()->setCursor(Qt::ArrowCursor);

	//Adjust the borders so both widgets look equal.
	_wizardList->setStyleSheet("* { border: 1px solid gray; }");
	_wizardDescription->setStyleSheet("* { border: 1px solid gray; padding: 10px; background: white; }");

	//Create the buttons.
	QPushButton* selectButton = new QPushButton(tr("Select..."), this);
	QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);

	//Create wizard list container widget.
	QWidget* wizardsWidget = new QWidget(this);
	QHBoxLayout* wizardsLayout = new QHBoxLayout(wizardsWidget);

	//Layout both wizard widgets horizontally.
	wizardsLayout->setMargin(0);
	wizardsLayout->addWidget(_wizardList);
	wizardsLayout->addWidget(_wizardDescription);

	//Create buttons container widget.
	QWidget* buttonsWidget = new QWidget(this);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);

	//Advanced button only for multi pass wizards.
	buttonsLayout->setMargin(0);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(selectButton);
	buttonsLayout->addWidget(cancelButton);

	//Create dialog layout.
	QVBoxLayout* dialogLayout = new QVBoxLayout(this);
	dialogLayout->addWidget(wizardsWidget);
	dialogLayout->addWidget(buttonsWidget);

	//Connect to changes.
	connect(_wizardList, &QListWidget::itemSelectionChanged, this, &WizardSelectDialog::currentWizardChanged);
	connect(selectButton, &QPushButton::clicked, this, &WizardSelectDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &WizardSelectDialog::reject);

	//Initialize the window.
	setWindowTitle("Select Project Wizard...");
	setMinimumSize(400, 300);
	resize(400, 300);

	//Load the available wizards.
	loadResources();
}

bool WizardSelectDialog::loadResources()
{
	//Load the resource directory.
	QDir resourceDirectory(WizardFile::getWizardsDirectory(_resourcePath));
	if(!resourceDirectory.exists())
		return false;

	//Access all wizard sub-directories.
	resourceDirectory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	//Add all available wizards.
	QStringList wizardDirectories = resourceDirectory.entryList();
	foreach(QString wizardDirectory, wizardDirectories)
	{
		//Try to parse the file and store the wizard.
		WizardFile wizardFile(_resourcePath, wizardDirectory);
		if(wizardFile.isValid())
			addWizard(wizardFile);
	}

	//All resources successfully loaded.
	return true;
}

void WizardSelectDialog::addWizard(const WizardFile& wizard)
{
	//Append the wizard.
	_wizardList->addItem(wizard.getProperty(WizardFile::Name));
	_wizards.append(wizard);
}

void WizardSelectDialog::currentWizardChanged()
{
	//Empty description if no selection was made.
	QString currentDescription;

	//Retrieve the selected item's description.
	int currentRow = _wizardList->currentRow();
	if(_wizardList->item(currentRow)->isSelected())
		currentDescription = _wizards[currentRow].getProperty(WizardFile::Description);

	//Show the wizard's description.
	_wizardDescription->setPlainText(currentDescription);
}

WizardFile WizardSelectDialog::getCurrentWizard() const
{
	//Retrieve the selected item description.
	int currentRow = _wizardList->currentRow();
	if(_wizardList->item(currentRow)->isSelected())
		return _wizards[currentRow];

	//No row selected.
	return WizardFile::Invalid;
}
