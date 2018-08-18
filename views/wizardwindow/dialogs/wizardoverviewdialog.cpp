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

#include "wizardoverviewdialog.h"
#include "wizardselectdialog.h"

#include "../pages/wizardfilepage.h"
#include "../pages/collections/wizardsourceselect.h"
#include "../pages/collections/wizardoutputselect.h"

#include "views/mainwindow/documentmanager.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>

#include <QTemporaryDir>
#include <QDir>

using namespace ysm;

WizardOverviewDialog::WizardOverviewDialog(QWidget* parent) :
	QWizard(parent)
{
	//Create the lists layout.
	QWizardPage* overviewPage = new QWizardPage(this);
	QGridLayout* listsLayout = new QGridLayout(overviewPage);

	//Create the list views.
	QWidget* sourcesWidget = createListWidget(tr("Sources"), &_sourcesList);
	QWidget* passesWidget = createListWidget(tr("Render Passes"), &_passesList);
	QWidget* outputsWidget = createListWidget(tr("Outputs"), &_outputsList);

	//Add the lists to the main layout.
	listsLayout->addWidget(sourcesWidget, 0, 0);
	listsLayout->addWidget(passesWidget, 0, 1);
	listsLayout->addWidget(outputsWidget, 0, 2);

	//Connect to changes.
//	connect(_sourcesList, &QListWidget::itemSelectionChanged, this, &WizardOverviewDialog::currentSourceChanged);
//	connect(_passesList, &QListWidget::itemSelectionChanged, this, &WizardOverviewDialog::currentWizardChanged);
//	connect(_outputsList, &QListWidget::itemSelectionChanged, this, &WizardOverviewDialog::currentWizardChanged);

	//Create the actual wizard.
	_fileId = addPage(new WizardFilePage(true, 330, this));
	_overviewId = addPage(overviewPage);

	//Configure the window.
	setWizardStyle(ModernStyle);
}

WizardOverviewDialog::~WizardOverviewDialog() { qDeleteAll(_passes); }

QWidget* WizardOverviewDialog::createListWidget(const QString& title, QListWidget** listWidget)
{
	//Create the container widget.
	QWidget* containerWidget = new QWidget(this);
	QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
	containerLayout->setMargin(0);

	//Adjust the borders so the list style matches the select dialog.
	*listWidget = new QListWidget(this);
	(*listWidget)->setStyleSheet("* { border: 1px solid gray; }");

	//Build the layout.
	containerLayout->addWidget(new QLabel(title));
	containerLayout->addWidget(*listWidget);

	//Return the adjusted list.
	return containerWidget;
}

void WizardOverviewDialog::initializePage(int id)
{
	//Check if the page matches the overview page.
	if(id != _overviewId) return;

	//Create the buttons.
	QPushButton* addButton = new QPushButton(tr("Add Render Pass..."), this);
	connect(addButton, &QPushButton::clicked, this, &WizardOverviewDialog::addRenderPass);

	//Add the buttons.
	setButton(QWizard::CustomButton1, addButton);
	setOption(QWizard::HaveCustomButton1);

	//Adjust the button order.
	QList<QWizard::WizardButton> buttonLayout;
	buttonLayout << QWizard::CustomButton1 << QWizard::Stretch << QWizard::CancelButton << QWizard::FinishButton;
	setButtonLayout(buttonLayout);
}

void WizardOverviewDialog::addRenderPass()
{
	//Show the dialog to select the wizard to execute.
	WizardSelectDialog wizardSelectDialog(WizardFile::getBaseDirectory(), false, this);
	int dialogResult = wizardSelectDialog.exec();

	//Check wether the dialog was accepted.
	if(dialogResult == QDialog::Accepted)
	{
		//Create the actual wizard.
		WizardFile selectedWizard = wizardSelectDialog.getCurrentWizard();
		WizardWindow* wizardWindow = new WizardWindow(selectedWizard, false, this);

		//Iterate over the available references and register them.
		foreach(WizardWindow* window, _passes)
		{
			//Add all sources as reference.
			foreach(WizardWindow::Source source, window->getSources())
				if(source.isEnabled() && !source.isReference())
					wizardWindow->registerReference(source);

			//Add reusable outputs as reference.
			foreach(QString name, window->getOutputs())
				foreach(WizardWindow::Output output, window->getOutputs(name))
					if(output.isEnabled() && !output._source.isEmpty())
						wizardWindow->registerReference(output);
		}

		//Execute the wizard.
		dialogResult = wizardWindow->exec();

		//Check wether the wizard was completed.
		if(dialogResult == QDialog::Accepted)
			storeRenderPass(wizardWindow);
	}
}

void WizardOverviewDialog::storeRenderPass(WizardWindow* wizardWindow)
{
	//Retrieve the render pass name.
	QString renderPass = wizardWindow->field(WizardFilePage::NameField).toString();

	//Store the sources.
	foreach(WizardWindow::Source source, wizardWindow->getSources())
		if(source.isEnabled() && !source.isReference())
			_sourcesList->addItem(source.getUserName());

	//Store the outputs.
	foreach(QString name, wizardWindow->getOutputs())
		foreach(WizardWindow::Output output, wizardWindow->getOutputs(name))
			if(output.isEnabled())
			{
				//Also store to source, if reusable.
				_outputsList->addItem(output.getUserName());
				if(!output._source.isEmpty())
					_sourcesList->addItem(output.getUserName());
			}

	//Store the render pass.
	_passesList->addItem(renderPass);
	_passes.append(wizardWindow);
}

void WizardOverviewDialog::createProject(MainWindow* mainWindow)
{
	//Generate temporary project directory that is available on stack.
	QTemporaryDir temporaryDirectory(QDir::temp().absoluteFilePath("YSMProject"));
	QString temporaryFile = QDir(temporaryDirectory.path()).filePath("importProject.ysm");

	//If no target file set, create temporary directory.
	QString projectFile;
	if(projectFile.isEmpty())
		projectFile = temporaryFile;

	//Iterate over all passes and generate each.
	WizardParser wizardParser;
	foreach(WizardWindow* window, _passes)
		wizardParser.appendWizard(window);

	//Load the project file.
	if(wizardParser.finishProject(projectFile))
	{
		//Access the main window delegates.
		Document* activeDocument = mainWindow->getActiveDocument();
		DocumentManager* manager = mainWindow->getDocumentManager();

		//Load the project file according to specified options.
		if(true) manager->importDocument(activeDocument, projectFile);
		else manager->loadDocument(projectFile);
	}
}
