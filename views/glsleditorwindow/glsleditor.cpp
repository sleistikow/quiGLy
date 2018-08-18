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

#include "glsleditor.h"
#include "glsleditortab.h"
#include "glsltextedit.h"

#include "commands/iuicommandqueue.h"

#include "data/ipipeline.h"
#include "data/ipipelinemanager.h"
#include "data/properties/property.h"
#include "data/properties/glsldocumentlistproperty.h"

#include "views/document.h"
#include "views/mainwindow/mainwindow.h"
#include "views/mainwindow/documentmanager.h"

#include <QLocale>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>

using namespace ysm;

GLSLEditor::GLSLEditor(QWidget* parentWidget, IView* parentView) :
	QWidget(parentWidget, Qt::Window),
	View(parentView),
	_currentTab(NULL)
{
	//Set the window title with unsaved marker.
	setWindowTitle("[*] GLSL Editor");

	//Create the available actions.
	_newTab = new QAction(QIcon(":/tango/24x24/actions/document-new"), tr("New Tab"), this);
	_open = new QAction(QIcon(":/tango/24x24/actions/document-open"), tr("Open..."), this);
	_save = new QAction(QIcon(":/tango/24x24/actions/document-save"), tr("Save"), this);
	_saveAs = new QAction(QIcon(":/tango/24x24/actions/document-save-as"), tr("Save As..."), this);
	_setBlock = new QAction(this);
	_close = new QAction(QIcon(":/tango/24x24/actions/window-close"), tr("Close..."), this);
	_closeAll = new QAction(tr("Close All..."), this);
	_settings = new QAction(QIcon(":/tango/24x24/categories/preferences-system"), tr("Settings"), this);

	//Set the shortcuts.
	_newTab->setShortcut(QKeySequence::AddTab);
	_open->setShortcut(QKeySequence::Open);
	_save->setShortcut(QKeySequence::Save);
	_saveAs->setShortcut(QKeySequence::SaveAs);
	_close->setShortcut(QKeySequence::Close);
	_closeAll->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_W));

	//Create the 'file' menu.
	_fileMenu = new QMenu(tr("File"), this);
	_fileMenu->addAction(_newTab);
	_fileMenu->addAction(_open);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_save);
	_fileMenu->addAction(_saveAs);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_close);
	_fileMenu->addAction(_closeAll);

	//Create the 'tools' menu.
	_toolsMenu = new QMenu(tr("Tools"), this);
	_toolsMenu->addAction(_setBlock);
	_toolsMenu->addSeparator();
	_toolsMenu->addAction(_settings);

	//Create the menu bar.
	_menuBar = new QMenuBar(this);

	//Create the tool bar.
	QToolBar* toolBar = new QToolBar();

	//Add the tool bar actions.
	toolBar->addAction(_newTab);
	toolBar->addSeparator();
	toolBar->addAction(_open);
	toolBar->addAction(_save);
	toolBar->addAction(_saveAs);
	toolBar->addSeparator();
	toolBar->addAction(_setBlock);

	//Create the main area layout.
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);

	//Create the tab widget.
	_tabWidget = new QTabWidget(this);
	_tabWidget->setDocumentMode(true);
	_tabWidget->setTabsClosable(true);
	mainLayout->addWidget(_menuBar);
	mainLayout->addWidget(toolBar);
	mainLayout->addWidget(_tabWidget);

	//Connect action signals to slots.
	connect(_newTab, &QAction::triggered, this, &GLSLEditor::newTab);
	connect(_open, &QAction::triggered, this, QOverload<>::of(&GLSLEditor::open));
	connect(_save, &QAction::triggered, this, &GLSLEditor::save);
	connect(_saveAs, &QAction::triggered, this, &GLSLEditor::saveAs);
	connect(_setBlock, &QAction::triggered, this, &GLSLEditor::setBlock);
	connect(_close, &QAction::triggered, this, &GLSLEditor::close);
	connect(_closeAll, &QAction::triggered, this, &GLSLEditor::closeAll);
/*	connect(_settings, &QAction::triggered, this, &GLSLEditor::settings); */

	//Watch for tab changes.
	connect(_tabWidget, &QTabWidget::currentChanged, this, &GLSLEditor::currentTabChanged);
	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &GLSLEditor::closeTab);

	//Connect to data change events.
	notifyAll(IChangeable::Remove, this, &GLSLEditor::releaseTabs);
	notifyAll(IChangeable::Change, this, &GLSLEditor::updateTabs);

	//Fire current tab changed for initialization.
	currentTabChanged(-1);

	//Set editor size.
	resize(800, 600);
}

QMenu* GLSLEditor::getFileMenu() const { return _fileMenu; }
QMenu* GLSLEditor::getToolsMenu() const { return _toolsMenu; }

void GLSLEditor::open()
{
	//Show open file dialog, ensure the user selected a file.
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"));
	if(!filename.isEmpty())
	{
		//Create file, and open ReadOnly.
		QFile file(filename);
		if(!file.open(QFile::ReadOnly))
		{
			//File could not be openend, notify the user.
			QMessageBox::warning(this, tr("File could not be opened"),
								 tr("An error occured: File could not be openend."),
								 QMessageBox::Ok);
		}
		else
		{
			//Create new editor tab.
			GLSLEditorTab* editorTab = new GLSLEditorTab(&file, this, this);
			_tabWidget->addTab(editorTab, filename);
			_tabWidget->setCurrentWidget(editorTab);

			//Close the file.
			file.close();
		}
	}
}

void GLSLEditor::save()
{
	//Get the current tab and execute it's save action.
	GLSLEditorTab* editorTab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(_tabWidget->currentIndex()));
	if(editorTab)
		editorTab->saveData();
}

void GLSLEditor::saveAs()
{
	//Get the current tab and execute it's save as action.
	GLSLEditorTab* editorTab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(_tabWidget->currentIndex()));
	if(editorTab)
		editorTab->saveDataAs();
}

void GLSLEditor::setBlock()
{
	//Get the active editor tab.
	GLSLEditorTab* editorTab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(_tabWidget->currentIndex()));
	if(editorTab)
	{
		//If a block is connected, disconnect it.
		IBlock* block = editorTab->getBlock();
		if(block)
		{
			//Ensure the user wants to disconnect.
			QMessageBox::StandardButton replyButton = QMessageBox::warning(this, block->getName(),
								 tr("The document will be disconnected from block %1 (#%2). Are you sure?")
																		   .arg(block->getName())
																		   .arg(block->getID()),
								 QMessageBox::Yes | QMessageBox::No);

			//Disconnect.
			if(replyButton == QMessageBox::Yes)
				editorTab->releaseFromBlock();
		}

		//Otherwise, connect a block.
		else
		{
			//Iterate over the document's blocks.
			QMap<QString, IBlock*> availableBlocks;
			foreach(IBlock* block, getActiveDocument()->getPipeline()->getBlocks())
			{
				//Check if block type is allowed shader type, then map block to descriptor string.
				switch(block->getType())
				{
				case BlockType::Shader_Fragment:
				case BlockType::Shader_Geometry:
				case BlockType::Shader_TessellationControl:
				case BlockType::Shader_TessellationEvaluation:
				case BlockType::Shader_Vertex:
					availableBlocks[QString("%1 (#%2)")
						.arg(block->getName())
						.arg(block->getID())] = block;

				//Suppress warnings.
				default: continue;
				}
			}

			//Show selection dialog.
			bool selectedOK = false;
			QString selectedItem = QInputDialog::getItem(this, tr("Select Block"),
														 tr("Select the block to which the document should be appended."),
														 availableBlocks.keys(), 0, false, &selectedOK);
			//Set the block.
			if(selectedOK)
				editorTab->appendToBlock(availableBlocks[selectedItem], getActiveDocument());
		}

		//Update tab manually.
		currentTabChanged(_tabWidget->currentIndex());
	}
}

void GLSLEditor::newTab() { _tabWidget->addTab(new GLSLEditorTab(this, this), tr("New Document")); }
void GLSLEditor::close() { closeTab(_tabWidget->currentIndex()); }
void GLSLEditor::closeAll()
{
	//Iterate over all tabs, close each tab once. Iterate backwards, because the tabs might change during iteration.
	//After executing close all, the tab count does not need to be 0. If the user decides not to close an open tab
	//(e.g. in the unsaved changes dialog), the tab will stay alive.
	for(int i = _tabWidget->count() - 1; i >= 0; i--)
		closeTab(i);
}

void GLSLEditor::closeTab(int tabIndex)
{
	//Get the tab at the specified index and check for modification.
	GLSLEditorTab* editorTab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(tabIndex));
	if(editorTab->isModified())
	{
		//Show message box, that hints for unsaved changes.
		QMessageBox::StandardButton replyButton = QMessageBox::warning(this, _tabWidget->tabText(tabIndex),
							 tr("The document has been modified, do you want to save your changes?"),
							 QMessageBox::Save | QMessageBox::Discard |  QMessageBox::Cancel);

		//Save or return.
		if(replyButton == QMessageBox::Save)
			editorTab->saveData();
		else if(replyButton == QMessageBox::Discard)
			editorTab->revertData();
		else if(replyButton == QMessageBox::Cancel)
			return;
	}

	_tabWidget->removeTab(tabIndex);
	delete editorTab;
}

void GLSLEditor::currentTabChanged(int)
{
	//Reactivate actions, if they were deactivated.
	if(!_currentTab)
	{
		_close->setEnabled(true);
		_closeAll->setEnabled(true);
		_save->setEnabled(true);
		_saveAs->setEnabled(true);
		_setBlock->setEnabled(true);
	}

	//Otherwise disconnect from modified signal.
	else
		disconnect(_currentTab, SIGNAL(modifiedChanged(bool)), this, SLOT(setWindowModified(bool)));

	//Get the current editor tab.
	_currentTab = qobject_cast<GLSLEditorTab*>(_tabWidget->currentWidget());

	//Recreate the default menu bar.
	_menuBar->clear();
	_menuBar->addMenu(getFileMenu());

	//Create the local scope menus if available.
	if(_currentTab)
	{
		_menuBar->addMenu(_currentTab->getEditMenu());

		//Connect to modified signal, and set initial state.
		connect(_currentTab, SIGNAL(modifiedChanged(bool)), this, SLOT(setWindowModified(bool)));
		setWindowModified(_currentTab->isModified());

		//Update append to block manually.
		if(_currentTab->getBlock())
		{
			_setBlock->setIcon(QIcon(":/tango/24x24/stock/object/stock_unlink"));
			_setBlock->setText(tr("Unlink"));
		}
		else
		{
			_setBlock->setIcon(QIcon(":/tango/24x24/stock/net/stock_link"));
			_setBlock->setText(tr("Link To..."));
		}
	}

	//Otherwise deactivate actions.
	else
	{
		_close->setEnabled(false);
		_closeAll->setEnabled(false);
		_save->setEnabled(false);
		_saveAs->setEnabled(false);
		_setBlock->setEnabled(false);
	}

	//Add additional menu items.
	_menuBar->addMenu(getToolsMenu());
}

void GLSLEditor::open(Document* document, IBlock* block)
{
	//Determines, whether a new selection was applied.
	int selection = -1;

	//Iterate over all code pages.
	GLSLDocumentList codePages = block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code)->getValue();
	for(int i = 0; i < codePages.getDocuments().count(); i++)
	{
		//Check if the code page is already openend.
		bool codePageFound = false;
		for(int j = 0; j < _tabWidget->count() && !codePageFound; j++)
		{
			GLSLEditorTab* tab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(j));
			codePageFound = (tab->getBlock() == block) && (tab->getCodeIndex() == i);

			//Update selection, if necessary.
			if(codePageFound && selection == -1)
				selection = j;
		}

		//No widget found, create a new one.
		if(!codePageFound)
		{
			int index = _tabWidget->addTab(new GLSLEditorTab(document, block, i, this, this), block->getName());

			//Force that only the firstly created tab will be selected.
			if(selection == -1)
				selection = index;
		}
	}

	//Update selection, if tab was not already selected
	GLSLEditorTab* selected = qobject_cast<GLSLEditorTab*>(_tabWidget->currentWidget());
	if(!selected || selected->getBlock() != block)
		_tabWidget->setCurrentIndex(selection);

	//Show the window.
	show();
}

void GLSLEditor::closeEvent(QCloseEvent *event)
{
	//Close all tabs.
	closeAll();

	//If open tabs remain, do not close.
	if(_tabWidget->count())
		event->ignore();
	else
		event->accept();
}

void GLSLEditor::releaseTabs(IBlock* block)
{
	//Iterate over all available tabs and disconnect them, if their block was removed.
	for(int i = _tabWidget->count() - 1; i >= 0; i--)
	{
		GLSLEditorTab* tab = qobject_cast<GLSLEditorTab*>(_tabWidget->widget(i));
		if(tab->getBlock() == block)
			tab->releaseFromBlock();
	}
}

void GLSLEditor::updateTabs(GLSLDocument* property)
{
	//Skip checks wether the document is part of the current tab, just update.
	currentTabChanged(_tabWidget->currentIndex());
}

void GLSLEditor::updateDocument() { }
void GLSLEditor::updateItem() { }
