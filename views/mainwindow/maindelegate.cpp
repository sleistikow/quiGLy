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

#include "maindelegate.h"
#include "mainwindow.h"
#include "documentmanager.h"
#include "rendermanager.h"
#include "viewmanager.h"
#include "mainmenu.h"

#include "../document.h"

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/change/autolayoutitemscommand.h"
#include "commands/pipeline/change/validatepipelinecommand.h"

#include "opengl/glcontroller.h"

#include "views/pipelineview/pipelineview.h"
#include "views/pipelineview/pipelinetab.h"
#include "views/pipelineview/pipelinescene.h"

#include "views/wizardwindow/dialogs/wizardselectdialog.h"
#include "views/wizardwindow/dialogs/wizardoverviewdialog.h"
#include "views/wizardwindow/wizardwindow.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>

#define FILE_TYPE_NAME tr("quiGLy Project File")
#define ARCHIVE_TYPE_NAME tr("quiGLy Project Archive")
#define ANY_TYPE_NAME tr("quiGLy Files")

#define FILE_TYPE_IDENTIFIER ".ysm"
#define ARCHIVE_TYPE_IDENTIFIER ".ysm.zip"

#define ANY_TYPE ANY_TYPE_NAME + " (*" + FILE_TYPE_IDENTIFIER + " *" + ARCHIVE_TYPE_IDENTIFIER ")"
#define FILE_TYPE FILE_TYPE_NAME + " (*" + FILE_TYPE_IDENTIFIER + ")"
#define ARCHIVE_TYPE ARCHIVE_TYPE_NAME + " (*" + ARCHIVE_TYPE_IDENTIFIER + ")"

using namespace ysm;

MainDelegate::MainDelegate(MainWindow* mainWindow) :
	QObject(mainWindow),
	_mainWindow(mainWindow)
{ }

void MainDelegate::onNew()
{
	//Just create a new document.
	_mainWindow->getDocumentManager()->createDocument();
}

void MainDelegate::onSinglePass()
{
	//Show the dialog to select the wizard to execute.
	WizardSelectDialog wizardSelectDialog(WizardFile::getBaseDirectory(), true, _mainWindow);
	int dialogResult = wizardSelectDialog.exec();

	//Create single pass pipeline.
	if(dialogResult == WizardSelectDialog::Accepted)
	{
		//Create the simple wizard.
		WizardFile selectedWizard = wizardSelectDialog.getCurrentWizard();
		WizardWindow wizardWindow(selectedWizard, true, _mainWindow);

		//Execute the wizard.
		if(wizardWindow.exec() == QDialog::Accepted)
			wizardWindow.createProject(_mainWindow);
	}
}

void MainDelegate::onMultiPass()
{
	//Create the advanced wizard.
	WizardOverviewDialog advancedDialog(_mainWindow);

	//Execute the wizard and create the project.
	if(advancedDialog.exec() == QDialog::Accepted)
		advancedDialog.createProject(_mainWindow);
}

void MainDelegate::onOpen()
{
	//If the user selected a project, load it.
	QString filename = showOpenDialog();
	if(!filename.isEmpty())
		_mainWindow->getDocumentManager()->loadDocument(filename);
}

void MainDelegate::onSave()
{
	//Check, if the document needs to be backed by a file.
	Document* activeDocument = _mainWindow->getActiveDocument();
	if(activeDocument->getFile().isEmpty())
	{
		//Show save as dialog, ensure user continues.
		QString filename = showSaveAsDialog();
		if(filename.isEmpty())
			return;

		//Set the document's file, which will mark it as not newly created.
		activeDocument->setFile(filename);
	}

	//Save the document.
	_mainWindow->getDocumentManager()->saveDocument(activeDocument);
}

void MainDelegate::onSaveAs()
{
	//Show save as dialog, ensure user continues.
	QString filename = showSaveAsDialog();
	if(filename.isEmpty())
		return;

	//Set the document's new file.
	Document* activeDocument = _mainWindow->getActiveDocument();
	activeDocument->setFile(filename);

	//Save the document.
	_mainWindow->getDocumentManager()->saveDocument(activeDocument);
}

void MainDelegate::onImport()
{
	//If the user selected a project, import it.
	QString filename = showImportDialog();
	if(filename.isEmpty())
		return;

	//Import into the active document.
	Document* activeDocument = _mainWindow->getActiveDocument();
	_mainWindow->getDocumentManager()->importDocument(activeDocument, filename);
}

void MainDelegate::onExport()
{
	//Show save as dialog, ensure user continues.
	QString filename = showExportDialog();
	if(filename.isEmpty())
		return;

	//Get the active document.
	Document* activeDocument = _mainWindow->getActiveDocument();

	//Export the document, compress if archive type selected.
	bool compress = filename.endsWith(ARCHIVE_TYPE_IDENTIFIER);
	_mainWindow->getDocumentManager()->exportDocument(activeDocument, filename, true, compress);
}

void MainDelegate::onRecent()
{
	//Retrieve the sender, which must contain the file name as data.
	QAction* action = qobject_cast<QAction*>(sender());
	if(!action) return;

	//Load the file.
	QString filename = action->data().toString();
	_mainWindow->getDocumentManager()->loadDocument(filename);
}

void MainDelegate::onClose()
{
	//Notify the document manager to close the current document.
	_mainWindow->getDocumentManager()->closeDocument(_mainWindow->getActiveDocument());
}

void MainDelegate::onCloseAll()
{
	//Notify the document manager to close all documents.
	_mainWindow->getDocumentManager()->closeAll();
}

void MainDelegate::onUndo()
{
	//Undo the action on the active document.
	_mainWindow->getCommandQueue()->undo();
}

void MainDelegate::onRedo()
{
	//Redo the action on the active document.
	_mainWindow->getCommandQueue()->redo();
}

void MainDelegate::onAutoLayout()
{
	//Find the active layouter.
	Document* activeDocument = _mainWindow->getActiveDocument();
	PipelineTab* activeTab = _mainWindow->getViewManager()->getPipelineView()->findTab(activeDocument);
	PipelineSceneLayouter* layouter = activeTab->getPipelineScene()->getLayouter();

	//Execute the layout command.
	_mainWindow->executeCommand(new AutoLayoutItemsCommand(activeDocument->getPipeline(), layouter));
}

void MainDelegate::onRender()
{
	//Use the render manager to toggle rendering.
	_mainWindow->getRenderManager()->toggleRendering();
}

void MainDelegate::onValidate()
{
	//Ensure pipeline is available, then execute validate command.
	IPipeline* pipeline = _mainWindow->getActiveDocument()->getPipeline();
	if(pipeline)
		_mainWindow->executeCommand(new ValidatePipelineCommand(pipeline));
}

void MainDelegate::onOptions() { }

void MainDelegate::onVersion()
{
	//Retrieve the neccessary data.
	RenderManager* renderManager = _mainWindow->getRenderManager();
	GLController* controller = _mainWindow->getActiveDocument()->getGLController();
	IPipeline* pipeline = _mainWindow->getActiveDocument()->getPipeline();

	//Disable rendering.
	renderManager->setRendering(false);

	//Retrieve all versions supported by quiGLy.
	QList<RenderManager::OpenGLVersion> availableVersions = renderManager->getSupportedVersions();

	//Iterate over all versions and generate a string representation.
	int activeVersionIndex = 0;
	QMap<QString, QPair<RenderManager::OpenGLVersion, bool>> versionInfo;
	for(int i = 0; i < availableVersions.length(); i++)
	{
		//Get the current version and check if it's supported.
		RenderManager::OpenGLVersion version = availableVersions[i];
		bool isSupported = controller && controller->isOpenGLVersionSupported(version._major, version._minor);

		//Build the version string.
		QString versionString = QString("OpenGL %1.%2").arg(version._major).arg(version._minor);
		if(!isSupported)
			versionString += " (not supported)";

		//Store the current version.
		versionInfo[versionString] = qMakePair(version, isSupported);

		//Store the index of the active version.
		int numericVersion = version._major * 100 + version._minor * 10;
		if(numericVersion == pipeline->getOpenGLVersion())
			activeVersionIndex = i;

	}

	//Show the select dialog.
	bool userConfirmed = false;
	QString selectedVersion = QInputDialog::getItem(_mainWindow, "Select OpenGL Version", "Version:",
													versionInfo.keys(), activeVersionIndex, false, &userConfirmed);

	//Ensure the user confirmed to set the new version.
	if(userConfirmed)
	{
		//Set the new version.
		renderManager->setVersion(versionInfo[selectedVersion].first);

		//Notify the user, if the selected version is not supported.
		if(!versionInfo[selectedVersion].second)
			QMessageBox::critical(_mainWindow, "OpenGL Version",
								  "The selected OpenGL profile is not supported by your system.\n"
								  "Rendering is disabled.");
	}
}

void MainDelegate::onQuit() { }

QString MainDelegate::showOpenDialog()
{ return QFileDialog::getOpenFileName(_mainWindow, tr("Open..."), QString(), ANY_TYPE); }

QString MainDelegate::showSaveAsDialog()
{ return QFileDialog::getSaveFileName(_mainWindow, tr("Save as..."), QString(), FILE_TYPE); }

QString MainDelegate::showImportDialog()
{ return QFileDialog::getOpenFileName(_mainWindow, tr("Import..."), QString(), ANY_TYPE); }

QString MainDelegate::showExportDialog()
{ return QFileDialog::getSaveFileName(_mainWindow, tr("Export..."), QString(), ARCHIVE_TYPE + ";;" + FILE_TYPE); }
