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

#include "maintoolbar.h"
#include "maindelegate.h"
#include "mainwindow.h"
#include "../document.h"

using namespace ysm;

//Pragma that creates a new tool bar item (local variable).
#define ITEM_L(title, icon, slot) \
{ \
	QAction* action = new QAction(QIcon(icon), tr(title), this); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	addAction(action); \
}

//Pragma that creates a new tool bar item (member variable).
#define ITEM_M(action, title, icon, slot) \
	action = new QAction(QIcon(icon), tr(title), this); \
	connect(action, &QAction::triggered, _delegate, QOverload<>::of(&MainDelegate::slot)); \
	addAction(action);

//Pragma that creates a separator (for convenience).
#define ITEM_S addSeparator();

MainToolBar::MainToolBar(MainDelegate* delegate, MainWindow* parentWindow) :
	QToolBar(parentWindow),
	View(parentWindow),
	_delegate(delegate),
	_document(NULL)
{
	//Build the tool bar.
	ITEM_L("New", ":/tango/24x24/actions/document-new", onNew);
	ITEM_S;
	ITEM_L("Open", ":/tango/24x24/actions/document-open", onOpen);
	ITEM_M(_saveAction, "Save", ":/tango/24x24/actions/document-save", onSave);
	ITEM_M(_saveAsAction, "Save as", ":/tango/24x24/actions/document-save-as", onSaveAs);
	ITEM_S;
	ITEM_M(_autoLayoutAction, "Auto layout", ":/tango/24x24/stock/image/stock_modify-layout", onAutoLayout);
	ITEM_S;
	ITEM_M(_validateAction, "Validate", ":/tango/24x24/stock/generic/stock_mark", onValidate);
	ITEM_M(_renderAction, "Render", ":/tango/24x24/actions/", onRender);

	//Notify on document changes.
	notifyDynamic(IChangeable::Change, this, &MainToolBar::documentChanged);

	//Manually fire one update.
	updateToolBar();
}

void MainToolBar::updateToolBar()
{
	//Check wether document is available.
	_saveAction->setEnabled(_document);
	_saveAsAction->setEnabled(_document);
	_autoLayoutAction->setEnabled(_document);
	_validateAction->setEnabled(_document);

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
}

void MainToolBar::documentChanged(IPipeline* pipeline) { Q_UNUSED(pipeline); updateToolBar(); }

void MainToolBar::updateDocument()
{
	//Disconnect from old document.
	if(_document)
		disconnect(_document, &Document::renderingChanged, this, &MainToolBar::updateToolBar);

	//Update current document.
	_document = getActiveDocument();

	//Connect to new document.
	if(_document)
		connect(_document, &Document::renderingChanged, this, &MainToolBar::updateToolBar);

	//Manually fire one update.
	updateToolBar();
}

void MainToolBar::updateItem() { }

