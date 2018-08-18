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

#include "glsltextedit.h"
#include "glsllinebar.h"
#include "glslhighlighter.h"
#include "glslcompleter.h"

#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/glslindentor.h"
#include "opengl/glslparser/glslparser.h"

#include "opengl/glslparser/glslstatements/glslstatement.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"

#include <QTextBlock>
#include <QFontDatabase>
#include <QAbstractItemView>
#include <QStringListModel>
#include <QStringList>

#include <QDebug>

using namespace ysm;

GLSLTextEdit::GLSLTextEdit(GLSLDocument* document, QWidget* parent) :
	QPlainTextEdit(parent),
	_glslDocument(document)
{
	//Create the document.
	setDocument(_glslDocument);

	//Set the font, disable frame.
	QFont monoFont("Courier");
	monoFont.setStyleHint(QFont::Monospace);
	setFont(monoFont);

	//Adjust the editor.
	setTabStopWidth(4 * fontMetrics().maxWidth());

	//Activate autocompletion.
	_completer = new GLSLCompleter(_glslDocument, this);
	_completer->setDelegate(this);
	_completer->setWidget(this);

	//Connect to signals, used internally.
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCurrentLine()));
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(emitLineBarBlocksChanged(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(emitLineBarBlocksChanged(QRect,int)));

	//Watch for modifications.
	connect(_glslDocument, &GLSLDocument::codeVerified, this, &GLSLTextEdit::updateCodeMarkers);

	//Initial update.
	updateCodeMarkers(_glslDocument->getFullCodeBlock());
}

int GLSLTextEdit::getLineBarBlockCount(const GLSLLineBar*) const { return blockCount();  }

QVector<GLSLLineBarBlock> GLSLTextEdit::getLineBarBlocks(const GLSLLineBar*) const
{
	//Iterate over the blocks, and list all visible blocks with their number and position.
	QVector<GLSLLineBarBlock> barBlocks;
	for(QTextBlock block = firstVisibleBlock(); block.isValid(); block = block.next())
		if(block.isVisible())
			barBlocks.append(GLSLLineBarBlock(block.blockNumber(),
											  blockBoundingGeometry(block).translated(contentOffset())));

	return barBlocks;
}

QString GLSLTextEdit::getCompleterPrefix(const GLSLCompleter*) const
{
	//Select the text underneath the cursor and return it.
	QTextCursor wordCursor = textCursor();
	wordCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
	return wordCursor.selectedText();
}

QRect GLSLTextEdit::getCompleterRect(const GLSLCompleter*) const
{
	//Use the cursor rect, adjust the width to 0.
	QRect completerRect = cursorRect();
	completerRect.setWidth(0);

	return completerRect;
}

GLSLCodeBlock* GLSLTextEdit::getCompleterBlock(const GLSLCompleter* completer) const
{
	Q_UNUSED(completer);

	//Find the saved block at the given text cursor location.
	return _glslDocument->getFullCodeBlock()->findChildBlock(textCursor());
}

void GLSLTextEdit::completerCompleted(const GLSLCompleter*, const QString& completion)
{
	//Remove the current word, then add the complete completion.
	QTextCursor insertionCursor = textCursor();
	insertionCursor.setKeepPositionOnInsert(false);

	insertionCursor.select(QTextCursor::WordUnderCursor);
	insertionCursor.removeSelectedText();
	insertionCursor.insertText(completion);

	//Adjust the actual cursor position.
	setTextCursor(insertionCursor);
}

void GLSLTextEdit::emitLineBarBlocksChanged(const QRect&, int) { emit lineBarBlocksChanged(); }
void GLSLTextEdit::emitLineBarBlocksChanged(int) { emit lineBarBlocksChanged(); }

void GLSLTextEdit::updateCurrentLine()
{
	//Retrieve one cursor for editing and one backup.
	QTextCursor adjustedCursor = textCursor();
	QTextCursor latestCursor = textCursor();

	//Check wether the cursor's position needs to be adjusted.
	bool cursorValid = _glslDocument->getPipelineAdapter()->checkCursorPosition(_latestCursor, adjustedCursor);

	//Store the new cursor, so it's up to date in case the cursor is updated again.
	_latestCursor = latestCursor;

	//If the cursor did not change, update the selections.
	if(cursorValid)
	{
		//Create list that holds extra selections, start with code markers.
		QList<QTextEdit::ExtraSelection> extraSelections = _codeMarkers;
		QTextEdit::ExtraSelection selection;

		//Add current line highlighting.
		selection.format.setBackground(QColor(255, 255, 0, 80));
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();

		//Use the selection.
		extraSelections.append(selection);
		setExtraSelections(extraSelections);
	}

	//Set the new cursor position. This method is invoked again by the change of the cursor.
	else
		setTextCursor(adjustedCursor);
}

void GLSLTextEdit::updateCodeMarkers(GLSLCodeBlock* codeBlock)
{
	//Create the code markers.
	_codeMarkers.clear();
	addCodeMarkers(codeBlock);

	//Update current line.
	updateCurrentLine();
}

void GLSLTextEdit::addCodeMarkers(GLSLCodeBlock* codeBlock)
{
	//Ensure code block exists.
	if(!codeBlock) return;

	//Iterate over all statements.
	foreach(GLSLStatement* statement, codeBlock->getStatements())
	{
		//Highlight error messages.
		if(!statement->getErrors().isEmpty())
		{
			//Create red underline.
			QTextEdit::ExtraSelection selection;
			selection.cursor = statement->getCompleteStatement();
			selection.format.setFontUnderline(true);
			selection.format.setUnderlineColor(Qt::red);
			_codeMarkers.append(selection);
		}
	}

	//Traverse through children.
	foreach(GLSLCodeBlock* codeBlock, codeBlock->getChildBlocks())
		addCodeMarkers(codeBlock);
}

void GLSLTextEdit::focusOutEvent(QFocusEvent* focusEvent)
{
	//Emit signal.
	if(focusEvent->lostFocus())
		emit editingFinished();
}

void GLSLTextEdit::keyPressEvent(QKeyEvent* args)
{
	//Check if the completer wants to handle the signal.
	if(_completer->handleKeyPress(args))
		args->ignore();

	//Check for shortcuts that are always allowed.
	else if(args->matches(QKeySequence::Undo) || args->matches(QKeySequence::Redo))
		args->accept();

	//Check if key press can adjust text.
	else if(!args->text().isEmpty())
	{
		//Use the GLSL pipeline adapter to verify the current key press.
		GLSLPipelineAdapter* adapter = _glslDocument->getPipelineAdapter();
		if(!adapter->checkEditingPermission(args->key(), args->modifiers(), textCursor()))
			args->ignore();
	}

	//If the keypress is accepeted, check if futher action is required.
	if(args->isAccepted())
	{
		//Get the current text cursor and an indentor.
		QTextCursor blockCursor = textCursor();
		GLSLIndentor blockIndentor(_glslDocument, blockCursor);

		//Switch over the key before execution to prevent actual behavior.
		switch(args->key())
		{
		//Tab forces indentation of current selection.
		case Qt::Key_Tab:
		case Qt::Key_Backtab:

			//Check if cursor has selection.
			if(blockCursor.hasSelection())
			{
				//Check the direction of the tab.
				bool backTab = args->modifiers() == Qt::ShiftModifier || args->key() == Qt::Key_Backtab;
				bool isCtrl = args->modifiers() == Qt::ControlModifier || args->modifiers() == Qt::ControlModifier;

				//Control modifier forces autoindentation, otherwise adjust indentation (meta on OS X).
				blockCursor.beginEditBlock();
				if(isCtrl)
					blockIndentor.autoindentDocumentBlocks();
				else
					blockIndentor.adjustIndentationLevel(backTab ? -1 : 1);
				blockCursor.endEditBlock();
				return;
			}
		}

		//Call the base to execute the key press.
		QPlainTextEdit::keyPressEvent(args);

		//Switch over the key after execution to include normal behavior.
		switch(args->key())
		{
		//New line keys force auto indentation of new line.
		case Qt::Key_Enter:
		case Qt::Key_Return:

			//Autoindent newly added line.
			blockCursor.beginEditBlock();
			blockIndentor.autoindentDocumentBlock();
			blockCursor.endEditBlock();
			return;
		}

		//Opening brackets force auto indentation of current line.
		if (args->text().contains('{'))
		{
			//Create cursor that is right before the already inserted bracket.
			blockCursor.movePosition(QTextCursor::PreviousCharacter);

			//Indent single line.
			blockCursor.beginEditBlock();
			blockIndentor.autoindentDocumentBlock();
			blockCursor.endEditBlock();
			return;
		}

		//Closing brackets force auto indentation of block before bracket.
		if (args->text().contains('}'))
		{
			//Create cursor that is right before the already inserted bracket.
			blockCursor.movePosition(QTextCursor::PreviousCharacter);

			//Indent the whole code block.
			blockCursor.beginEditBlock();
			blockIndentor.autoindentCodeBlock();
			blockCursor.endEditBlock();
			return;
		}
	}
}

GLSLDocument* GLSLTextEdit::getGLSLDocument() const { return _glslDocument; }
