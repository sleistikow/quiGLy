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

#include "mainmenu.h"
#include "maindelegate.h"
#include "mainwindow.h"
#include "../document.h"

#include <QSettings>
#include <QFileInfo>

using namespace ysm;

//Pragma that creates a new menu.
#define MENU(name, title) \
	QMenu* name = addMenu(tr(title));

//Pragma that creates a new sub menu (local variable).
#define SUBMENU_L(name, title, icon, parent) \
	QMenu* name = parent->addMenu(QIcon(icon), title);

//Pragma that creates a new sub menu (member variable).
#define SUBMENU_M(name, title, icon, parent) \
	name = parent->addMenu(QIcon(icon), title);

//Pragma that creates a new menu item (local variable).
#define ITEM_L(title, icon, slot, parent) \
{ \
	QAction* action = new QAction(QIcon(icon), tr(title), this); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	parent->addAction(action); \
}

//Pragma that creates a new menu item (local variable + shortcut).
#define ITEM_LK(title, icon, slot, key, parent) \
{ \
	QAction* action = new QAction(QIcon(icon), tr(title), this); \
	action->setShortcut(key); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	parent->addAction(action); \
}

//Pragma that creates a new menu item (member variable).
#define ITEM_M(action, title, icon, slot, parent) \
	action = new QAction(QIcon(icon), tr(title), this); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	parent->addAction(action);

//Pragma that creates a new menu item (member variable + shortcut).
#define ITEM_MK(action, title, icon, slot, key, parent) \
	action = new QAction(QIcon(icon), tr(title), this); \
	action->setShortcut(key); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	parent->addAction(action);

//Pragma that creates a new menu item (array member variable).
#define ITEM_A(list, slot, parent) \
{ \
	QAction* action = new QAction(QString(), this); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	parent->addAction(action); \
	list.append(action); \
}

//Pragma that creates a separator (for convenience).
#define ITEM_MS(action, parent) action = parent->addSeparator();

//Pragma that creates a separator (for convenience).
#define ITEM_S(parent) parent->addSeparator();

const int MainMenu::RecentFileCount = 5;

MainMenu::MainMenu(MainDelegate* delegate, MainWindow* parentWindow) :
	QMenuBar(parentWindow),
	View(parentWindow),
	_delegate(delegate),
	_document(NULL)
{
	{
		MENU(fileMenu, "File");
		{
			SUBMENU_L(newMenu, "New", ":/tango/16x16/actions/document-new", fileMenu);
			ITEM_LK("Empty  project", ":/tango/16x16/stock/document/stock_new", onNew, QKeySequence::New, newMenu);
			ITEM_L("Single pass wizard", ":/tango/16x16/stock/document/stock_script", onSinglePass, newMenu);
			ITEM_L("Multi pass wizard", ":/tango/16x16/stock/document/stock_scripts", onMultiPass, newMenu);
		}
		ITEM_LK("Open", ":/tango/16x16/actions/document-open", onOpen, QKeySequence::Open, fileMenu);
		ITEM_MK(_saveAction, "Save", ":/tango/16x16/actions/document-save", onSave, QKeySequence::Save, fileMenu);
		ITEM_MK(_saveAsAction, "Save as...", ":/tango/16x16/actions/document-save-as", onSaveAs, QKeySequence::SaveAs, fileMenu);
		ITEM_S(fileMenu);
		ITEM_MK(_closeAction, "Close",  ":/tango/16x16/actions/window-close", onClose, QKeySequence::Close, fileMenu);
		ITEM_L("Close all", ":/tango/", onCloseAll, fileMenu);
		ITEM_S(fileMenu);
		ITEM_MK(_importAction, "Import", ":/tango/", onImport, QKeySequence("Ctrl+Shift+I"), fileMenu);
		ITEM_MK(_exportAction, "Export...", ":/tango/", onExport, QKeySequence("Ctrl+Shift+E"), fileMenu);
		ITEM_S(fileMenu);
		{
			SUBMENU_M(_recentMenu, "Recently used files", ":/tango/16x16/actions/document-open-recent", fileMenu);
			ITEM_A(_recentActions, onRecent, _recentMenu);
			ITEM_A(_recentActions, onRecent, _recentMenu);
			ITEM_A(_recentActions, onRecent, _recentMenu);
			ITEM_A(_recentActions, onRecent, _recentMenu);
			ITEM_A(_recentActions, onRecent, _recentMenu);
		}
		ITEM_S(fileMenu);
		ITEM_LK("Exit", ":/tango/", onQuit, QKeySequence::Quit, fileMenu);
	}
	{
		MENU(editMenu, "Edit");
		ITEM_MK(_undoAction, "Undo", ":/tango/16x16/actions/edit-undo", onUndo, QKeySequence::Undo, editMenu);
		ITEM_MK(_redoAction, "Redo", ":/tango/16x16/actions/edit-redo", onRedo, QKeySequence::Redo, editMenu);
	}
	{
		MENU(runMenu, "Run");
		ITEM_M(_validateAction, "Validate", ":/tango/24x24/stock/generic/stock_mark", onValidate, runMenu);
		ITEM_M(_renderAction, "Render", ":/tango/24x24/actions/", onRender, runMenu);
	}
	{
		MENU(settingsMenu, "Settings");
		ITEM_M(_versionAction, "Select OpenGL version...", ":/tango/16x16/actions/document-properties", onVersion, settingsMenu);
	}

	//Notify on document changes.
	notifyDynamic(IChangeable::Change, this, &MainMenu::documentChanged);
	notifyAll(IChangeable::Remove, this, &MainMenu::documentClosed);

	//Manually fire one update.
	updateMenu();
}

void MainMenu::updateMenu()
{
	//Check wether document is available.
	_saveAction->setEnabled(_document);
	_saveAsAction->setEnabled(_document);
	_closeAction->setEnabled(_document);
	_importAction->setEnabled(_document);
	_exportAction->setEnabled(_document);
	_undoAction->setEnabled(_document);
	_redoAction->setEnabled(_document);
	_validateAction->setEnabled(_document);
	_versionAction->setEnabled(_document);

	//Check wether rendering is possible
	bool isRenderingSupported = _document && _document->isRenderingSupported();
	_renderAction->setEnabled(isRenderingSupported);

	//Check the rendering state.
	bool isRendering = _document && _document->isRendering();
	QString renderIcon = isRendering ? "media-playback-stop" : "media-playback-start";
	QString renderText = isRendering ? "Stop" : "Render";

	//Set the new icons.
	_renderAction->setIcon(QIcon(":/tango/24x24/actions/" + renderIcon));
	_renderAction->setText(renderText);

	//Update the recent files.
	updateRecentFiles();
}

void MainMenu::documentChanged(IPipeline* pipeline) { Q_UNUSED(pipeline); updateMenu(); }

void MainMenu::updateDocument()
{
	//Disconnect from old document.
	if(_document)
		disconnect(_document, &Document::renderingChanged, this, &MainMenu::updateMenu);

	//Update current document.
	_document = getActiveDocument();

	//Connect to new document.
	if(_document)
		connect(_document, &Document::renderingChanged, this, &MainMenu::updateMenu);

	//Manually fire one update.
	updateMenu();
}

void MainMenu::updateItem() { }

void MainMenu::documentClosed(Document* document)
{
	//Ensure document was saved.
	if(document->getFile().isEmpty()) return;

	//Access the recent file list.
	QSettings settings;
	QStringList recentFiles = settings.value("recentFiles").toStringList();

	//Adjust the recent documents.
	QStringList adjustedFiles = QStringList() << document->getFile();
	for(int i = 0; i < recentFiles.count() && adjustedFiles.count() < RecentFileCount; i++)
		if(recentFiles[i] != document->getFile())
			adjustedFiles.append(recentFiles[i]);

	//Store the new recent documents.
	settings.setValue("recentFiles", adjustedFiles);

	//Update the recent file list.
	updateRecentFiles();
}

void MainMenu::updateRecentFiles()
{
	//Access the recent file list.
	QSettings settings;
	QStringList recentFiles = settings.value("recentFiles").toStringList();

	//Iterate over the recent files and update the actions.
	int recentFileCount = qMin(_recentActions.size(), recentFiles.count());
	for(int i = 0; i < recentFileCount; i++)
	{
		//Show the action and set the text and data.
		_recentActions[i]->setText(QFileInfo(recentFiles[i]).fileName());
		_recentActions[i]->setData(recentFiles[i]);
		_recentActions[i]->setVisible(true);
	}

	//Hide all obsolete actions.
	for(int i = recentFileCount; i < RecentFileCount; i++)
		_recentActions[i]->setVisible(false);

	//Only show the seperator if files are available.
	_recentMenu->setEnabled(recentFileCount > 0);
}
