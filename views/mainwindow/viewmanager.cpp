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

#include "viewmanager.h"
#include "mainwindow.h"
#include "maindelegate.h"
#include "maintoolbar.h"
#include "mainmenu.h"
#include "documentmanager.h"

#include "../document.h"

#include "../logview/logview.h"
#include "../toolview/toolview.h"
#include "../commandview/commandview.h"
#include "../propertyview/propertyview.h"
#include "../pipelineview/pipelineview.h"

#include "../glsleditorwindow/glsleditor.h"

using namespace ysm;

//Pragma used to declare a menu widget for the main window.
#define WIDGET_M(field, type, name, command) \
	field = new type(_parentWindow->getDelegate(), _parentWindow); \
	field->setWindowTitle(tr(name)); \
	field->setObjectName(name); \
	_parentWindow->command(field);

//Pragma used to declare a dock widget for the main window.
#define WIDGET_D(field, type, dock, name, width) \
	field = new type(_parentWindow, _parentWindow); \
	field->setWindowTitle(tr(name)); \
	field->setObjectName(name); \
	field->setMinimumWidth(width); \
	_parentWindow->addDockWidget(dock, field);

//Pragma used to declare the center dock widget for the main window.
#define WIDGET_C(field, type, name) \
	field = new type(_parentWindow, _parentWindow); \
	field->setWindowTitle(tr(name)); \
	field->setObjectName(name); \
	_parentWindow->setCentralWidget(field);

ViewManager::ViewManager(MainWindow* parentWindow) :
	QObject(parentWindow),
	_parentWindow(parentWindow)
{
	//Create menus.
	WIDGET_M(_menuBar, MainMenu, "Menu", setMenuBar);
	WIDGET_M(_toolBar, MainToolBar, "Tools", addToolBar);

	//Create dock widgets.
	WIDGET_D(_logView, LogView, Qt::BottomDockWidgetArea, "Log", 350);
	WIDGET_D(_toolView, ToolView, Qt::LeftDockWidgetArea, "Blocks", 50);
	WIDGET_D(_commandView, CommandView, Qt::LeftDockWidgetArea, "Commands", 50);
	WIDGET_D(_propertyView, PropertyView, Qt::RightDockWidgetArea, "Properties", 350);

	//Initialize GLSL Editor.
	_glslEditor = new GLSLEditor(_parentWindow, _parentWindow);

	//Initialize the center widget.
	WIDGET_C(_pipelineView, PipelineView, "Pipelines");

	//Connect to widget signals.
	connect(_pipelineView, &PipelineView::editorRequested,
			_glslEditor, QOverload<Document*, IBlock*>::of(&GLSLEditor::open));
	connect(_pipelineView, &PipelineView::closeDocument,
			_parentWindow->getDocumentManager(), &DocumentManager::closeDocument);
}

MainMenu* ViewManager::getMainMenu() const { return _menuBar; }
PipelineView* ViewManager::getPipelineView() const { return _pipelineView; }
LogView* ViewManager::getLogView() const { return _logView; }
