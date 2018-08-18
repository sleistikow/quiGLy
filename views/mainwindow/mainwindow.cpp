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

#include "mainwindow.h"

#include "../document.h"
#include "commands/iuicommandqueue.h"

#include "documentmanager.h"
#include "viewmanager.h"
#include "rendermanager.h"
#include "maindelegate.h"

#include <QSettings>

using namespace ysm;

MainWindow* MainWindow::_sharedInstance = NULL;

MainWindow::MainWindow() :
	QMainWindow(),
	View(NULL)
{
	//Initialize window.
	setWindowTitle(QString("[*] Unnamed"));
	setWindowIcon(QIcon(":/Icon.ico"));

	//Initialize sub-classes:
	//Delegate needs to be available before the document manager.
	//Document manager needs to be available before the view manager and render manager.
	_delegateInstance = new MainDelegate(this);
	_documentManager = new DocumentManager(this);
	_viewManager = new ViewManager(this);
	_renderManager = new RenderManager(this);

	//Connect to document changes.
	notifyDynamic(IChangeable::Change, this, &MainWindow::updateWindow);

	//Restore the window state.
	restoreWindowState();
}

MainWindow* MainWindow::getInstance()
{
	//Check if instance is already initialized.
	if(!_sharedInstance)
		_sharedInstance = new MainWindow();

	//Return the shared instance.
	return _sharedInstance;
}

void MainWindow::saveWindowState()
{
	//Access application settings.
	QSettings settings;

	//Store state and geometry.
	settings.setValue("state", saveState());
	settings.setValue("geometry", saveGeometry());
}

void MainWindow::restoreWindowState()
{
	//Access application settings.
	QSettings settings;

	//Restore state and geometry.
	restoreState(settings.value("state", saveState()).toByteArray());
	restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
}

void MainWindow::updateTitles()
{
	//Update the title.
	setWindowTitle(getActiveDocument() ? "[*] " + getActiveDocument()->getName() : "quiGLy");
	setWindowModified(getActiveDocument() && getActiveDocument()->hasUnsavedChanges());
}

void MainWindow::updateDocument()
{
	//Update the titles and the render views.
	_renderManager->updateRenderViews();
	updateTitles();
}

void MainWindow::updateItem() { }

void MainWindow::updateWindow(Document* document) { Q_UNUSED(document); updateTitles(); }

void MainWindow::closeEvent(QCloseEvent *event)
{
	//Check for unsaved changes.
	if(!_documentManager->closeAll())
	{
		//Do not close the window.
		event->ignore();
		return;
	}

	//Save the window state.
	saveWindowState();

	//Accept event to close.
	event->accept();
}

RenderManager* MainWindow::getRenderManager() const { return _renderManager; }
DocumentManager* MainWindow::getDocumentManager() const { return _documentManager; }
MainDelegate* MainWindow::getDelegate() const { return _delegateInstance; }
ViewManager* MainWindow::getViewManager() const { return _viewManager; }
