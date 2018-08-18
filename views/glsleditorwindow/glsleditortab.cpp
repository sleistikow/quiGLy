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

#include "glsleditortab.h"
#include "views/common/ysmpalette.h"
#include "glsllinebar.h"
#include "glsltextedit.h"
#include "glslhighlighter.h"

#include "opengl/glslparser/glslparser.h"
#include "opengl/glslparser/glslstatements/glslstatement.h"
#include "opengl/glslparser/glslstatements/glsldeclaration.h"
#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"

#include "data/properties/property.h"
#include "data/properties/vaolayoutproperty.h"
#include "data/properties/glsldocumentlistproperty.h"
#include "data/types/glsldocumentlist.h"
#include "data/blocks/textureblock.h"
#include "data/types/vaolayout.h"

#include "commands/iuicommandqueue.h"
#include "commands/editor/savedocumentcommand.h"
#include "commands/editor/setdocumentcommand.h"

#include <QOpenGLFunctions>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

using namespace ysm;

GLSLEditorTab::GLSLEditorTab(Document* document, IBlock* block, int codeIndex,
							 QWidget* parentWidget, IView* parentView) :
	QWidget(parentWidget),
	ViewItem(parentView),
	_editMenu(NULL),
	_modified(false),
	_errorContextMenu(NULL)
{
	//Try to use the block's document if available.
	if(block && codeIndex >= 0)
	{
		GLSLDocumentListProperty* listProperty = block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code);
		_document = listProperty->getValue().getDocuments()[codeIndex];
	}

	//Create a document that is owned by the GLSL editor tab.
	//Ownership changes once it's connected to a block.
	else
		_document = new GLSLDocument();

	//Retain the document as long as it's connected to the tab.
	_document->retain();

	//Create the main area layout.
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);

	//Create the line bar layout widget.
	QWidget* lineBarLayoutWidget = new QWidget();
	mainLayout->addWidget(lineBarLayoutWidget);

	//Create the error widget.
	_errorList = new QListWidget();
	_errorList->setFrameStyle(QFrame::NoFrame);
	_errorList->setFixedHeight(0);
	_errorList->setContextMenuPolicy(Qt::CustomContextMenu);
	mainLayout->addWidget(_errorList);

	//Create the line bar layout.
	QHBoxLayout* lineBarLayout = new QHBoxLayout(lineBarLayoutWidget);
	lineBarLayout->setSpacing(0);
	lineBarLayout->setMargin(0);

	//Create the line bar.
	_lineBar = new GLSLLineBar();
	lineBarLayout->addWidget(_lineBar);

	//Create the text edit.
	_textEdit = new GLSLTextEdit(_document);
	_textEdit->setFrameStyle(QFrame::NoFrame);
	_lineBar->setDelegate(_textEdit);
	lineBarLayout->addWidget(_textEdit);

	//Adjust the list fonts.
	_errorList->setFont(_textEdit->font());

	//Watch for modifications.
	connect(_document, &GLSLDocument::modificationChanged, this, &GLSLEditorTab::setModified);
	connect(_document, &GLSLDocument::codeVerified, this, &GLSLEditorTab::updateCodeData);

	//Watch the lists.
	connect(_errorList, &QListWidget::doubleClicked, this, &GLSLEditorTab::highlightError);
	connect(_errorList, &QListWidget::customContextMenuRequested, this, &GLSLEditorTab::showErrorContext);

	//Initial update.
	setActiveDocument(document);
	updateCodeData(_document->getFullCodeBlock());
}

GLSLEditorTab::GLSLEditorTab(QFile* file, QWidget* parentWidget, IView* parentView) :
	GLSLEditorTab(NULL, NULL, -1, parentWidget, parentView)
{
	//Set the file name.
	_filename = file->fileName();

	//Load contents from file.
	QTextStream textStream(file);
	_document->setSavedContent(textStream.readAll());
	_document->revertDocument();
}

GLSLEditorTab::GLSLEditorTab(QWidget* parentWidget, IView* parentView) :
	GLSLEditorTab(NULL, NULL, -1, parentWidget, parentView)
{ }

GLSLEditorTab::~GLSLEditorTab()
{
	//Release the connection to the document.
	_document->release();
}

QMenu* GLSLEditorTab::getEditMenu()
{
	//Check if menu was already created.
	if(!_editMenu)
	{
		//Create the available actions.
		_undo = new QAction(QIcon(":/tango/24x24/actions/edit-undo"), tr("Undo"), this);
		_redo = new QAction(QIcon(":/tango/24x24/actions/edit-redo"), tr("Redo"), this);
		_cut = new QAction(QIcon(":/tango/24x24/actions/edit-cut"), tr("Cut"), this);
		_copy = new QAction(QIcon(":/tango/24x24/actions/edit-copy"), tr("Copy"), this);
		_paste = new QAction(QIcon(":/tango/24x24/actions/edit-paste"), tr("Paste"), this);

		//Set the shortcuts.
		_undo->setShortcut(QKeySequence::Undo);
		_redo->setShortcut(QKeySequence::Redo);
		_cut->setShortcut(QKeySequence::Cut);
		_copy->setShortcut(QKeySequence::Copy);
		_paste->setShortcut(QKeySequence::Paste);

		//Create the 'edit' menu.
		_editMenu = new QMenu(tr("Edit"), this);
		_editMenu->addAction(_undo);
		_editMenu->addAction(_redo);
		_editMenu->addSeparator();
		_editMenu->addAction(_cut);
		_editMenu->addAction(_copy);
		_editMenu->addAction(_paste);

		//Connect action signals to slots.
		connect(_undo, SIGNAL(triggered()), _document, SLOT(undo()));
		connect(_redo, SIGNAL(triggered()), _document, SLOT(redo()));
		connect(_cut, SIGNAL(triggered()), _textEdit, SLOT(cut()));
		connect(_copy, SIGNAL(triggered()), _textEdit, SLOT(copy()));
		connect(_paste, SIGNAL(triggered()), _textEdit, SLOT(paste()));

		//Disable commands by default.
		_undo->setEnabled(false);
		_redo->setEnabled(false);
		_cut->setEnabled(false);
		_copy->setEnabled(false);

		//Connect text edit availability signals to action slots.
		connect(_document, SIGNAL(undoAvailable(bool)), _undo, SLOT(setEnabled(bool)));
		connect(_document, SIGNAL(redoAvailable(bool)), _redo, SLOT(setEnabled(bool)));
		connect(_textEdit, SIGNAL(copyAvailable(bool)), _copy, SLOT(setEnabled(bool)));
		connect(_textEdit, SIGNAL(copyAvailable(bool)), _cut, SLOT(setEnabled(bool)));
	}

	return _editMenu;
}

void GLSLEditorTab::updateCodeData(GLSLCodeBlock* codeBlock)
{
	//Recreate the error list recursively.
	_errorList->clear();
	_statementErrors.clear();
	addCodeData(codeBlock);

	//Adjust the height.
	_errorList->setFixedHeight(_errorList->count() * _errorList->sizeHintForRow(0));
}

void GLSLEditorTab::addCodeData(GLSLCodeBlock* codeBlock)
{
	//Ensure code block exists.
	if(!codeBlock) return;

	//Iterate over all statements.
	foreach(GLSLStatement* statement, codeBlock->getStatements())
	{
		//Add all error messages to list box.
		foreach(GLSLStatementError error, statement->getErrors())
		{
			//Create the error string.
			QString errorString = QString("Error in line %1: %2")
					.arg(statement->getCompleteStatement().blockNumber() + 1)
					.arg(error.userMessage);

			//Create a list view item, that hold the error and advanced data.
			QListWidgetItem* errorItem = new QListWidgetItem(QIcon(":/16/tango/dialog-error"), errorString);
			errorItem->setBackgroundColor(YSMPalette::getPipelineItemStatusColor(PipelineItemStatus::Sick).lighter());
			errorItem->setSizeHint(QSize(0, 24));
			errorItem->setData(Qt::UserRole, QList<QVariant>()
							   << QVariant(statement->getCompleteStatement().selectionStart())
							   << QVariant(_statementErrors.count()));

			//Add to the error list.
			_statementErrors.append(error);
			_errorList->addItem(errorItem);
		}
	}

	//Traverse through children.
	foreach(GLSLCodeBlock* codeBlock, codeBlock->getChildBlocks())
		addCodeData(codeBlock);
}

void GLSLEditorTab::highlightError(QModelIndex modelIndex)
{
	//Get the user data.
	QList<QVariant> detailedInfo = _errorList->item(modelIndex.row())->data(Qt::UserRole).toList();
	int selectionStart = detailedInfo[0].toInt();

	//Create cursor starting before error.
	QTextCursor textCursor = _textEdit->textCursor();
	textCursor.setPosition(selectionStart);
	_textEdit->setTextCursor(textCursor);

	//Reown the focus to the text edit.
	_textEdit->setFocus();
}

void GLSLEditorTab::showErrorContext(QPoint contextPoint)
{
	//Get the selected widget and validate the document.
	QListWidgetItem* widgetItem = _errorList->itemAt(contextPoint);
	Document* activeDocument = getActiveDocument();
	if(widgetItem && activeDocument)
	{
		QList<QVariant> detailedInfo = widgetItem->data(Qt::UserRole).toList();
		GLSLStatementError statementError =	_statementErrors[detailedInfo[1].toInt()];

		//Recreate context menu.
		delete _errorContextMenu;
		_errorContextMenu = new QMenu(this);

		//Create context menu "header".
		QFont headerFont;
		headerFont.setBold(true);

		QAction* quickFixes = new QAction("QUICK FIXES", _errorContextMenu);
		quickFixes->setDisabled(true);
		quickFixes->setFont(headerFont);

		//Fill the context menu.
		_errorContextMenu->addAction(quickFixes);
		_errorContextMenu->addSeparator();

		//Append the actual actions.
		foreach(IGLSLQuickFix* quickFix, statementError.quickFixes)
		{
			//Create action and connect to quick fix.
			GLSLDocument* codeDocument = _document;
			GLSLStatement* statement = statementError.statement;
			QAction* action = new QAction(quickFix->getText(), statement);
			connect(action, &QAction::triggered, [=]() -> void
			{
				//Execute the quick fix on the active document.
				quickFix->execute(activeDocument, statement);

				//Synchronize the document to update to the pipeline.
				codeDocument->synchronizeDocument();
			});

			//Add to context menu.
			_errorContextMenu->addAction(action);
		}

		//Show the context menu.
		_errorContextMenu->popup(_errorList->mapToGlobal(contextPoint));
	}
}

void GLSLEditorTab::revertData()
{
	_document->revertDocument();
	_document->synchronizeDocument();
}

void GLSLEditorTab::saveData()
{
	//If no save destination is specifed, do a save as.
	IBlock* block = getBlock();
	if(_filename.isEmpty() && !block)
	{
		//saveDataAs will call saveData, so return here.
		saveDataAs();
		return;
	}

	//If an active document is available, use a command to save the document.
	if(block && getActiveDocument())
		executeCommand(new SaveDocumentCommand(_document));

	//Otherwise, manually execute the document's save command.
	//No synchronize required, because there is no block.
	else
		_document->saveDocument();

	//Check if file source is available, if so also write to file.
	if(!_filename.isEmpty())
	{
		//Create file, and open WriteOnly, which will truncate the file, if it already exists.
		QFile file(_filename);
		if(file.open(QFile::WriteOnly))
		{
			//Write contents to file and close.
			QTextStream textStream(&file);
			textStream << _document->getSavedCode(false);

			//Close the file.
			file.close();
		}
		else
			QMessageBox::warning(this, tr("File could not be opened"),
								 tr("An error occured, while saving to file: File could not be openend."),
								 QMessageBox::Ok);
	}
}

void GLSLEditorTab::saveDataAs()
{
	//Show save file dialog, ensure the user selected a file.
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"));
	if(!filename.isEmpty())
	{
		//Set the new file name and make sure the document is owned by this tab. This will prevent the document from
		//being deleted when the block is removed.
		_filename = filename;

		//Run the default save routine.
		saveData();
	}
}

void GLSLEditorTab::appendToBlock(IBlock* block, Document* document)
{
	//Check if the document is already connected to a block.
	IBlock* currentBlock = getBlock();
	if(currentBlock)
		QMessageBox::warning(this, tr("Document already in use."),
							 QString("The document is already connected to block '%1' (%2). It cannot be connected to two blocks.")
									  .arg(currentBlock->getName())
									  .arg(currentBlock->getID()),
							 QMessageBox::Ok);
	else
	{
		//Set the active document, then execute the command on it.
		setActiveDocument(document);
		executeCommand(new SetDocumentCommand(_document, block, true));
	}
}

void GLSLEditorTab::releaseFromBlock()
{
	//Ensure block is available.
	IBlock* block = getBlock();
	if(block && getActiveDocument())
	{
		//Fire command the command, then disconnect from the document.
		executeCommand(new SetDocumentCommand(_document, block, false));
		setActiveDocument(NULL);
	}
}

IBlock* GLSLEditorTab::getBlock() const { return _document->getPipelineAdapter()->getBlock(); }
GLSLTextEdit* GLSLEditorTab::getTextEdit() const { return _textEdit; }
int GLSLEditorTab::getCodeIndex() const
{
	//Check if block is specified.
	IBlock* block = getBlock();
	if(!block) return -1;

	//Find the tab's index in the document list.
	GLSLDocumentListProperty* listProperty = block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code);
	return listProperty->getValue().getDocuments().indexOf(_document);
}

bool GLSLEditorTab::isModified() { return _modified; }
void GLSLEditorTab::setModified(bool modified)
{
	//Make sure modified flag actually changes.
	if(_modified != modified)
		emit modifiedChanged(_modified = modified);
}
