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

#include "glslindentor.h"
#include "glsldocument.h"
#include "glslcodeblock.h"

#define TAB_SIZE 4
#define TAB_TEXT "\t"

using namespace ysm;

GLSLIndentor::GLSLIndentor(GLSLDocument* parentDocument, QTextCursor& indentationCursor) :
	_parentDocument(parentDocument),
	_indentationCursor(indentationCursor)
{ }

int GLSLIndentor::getIndentationLevel()
{
	//Select the first character of the block.
	_indentationCursor.movePosition(QTextCursor::StartOfBlock);
	_indentationCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

	//Keep selecting the next character, until a non-indenting character is found.
	int indentationLevel = 0, whitespaceCount = 0;
	while(_indentationCursor.selectedText() == QString(QChar::Space) ||
		  _indentationCursor.selectedText() == QString(QChar::Nbsp) ||
		  _indentationCursor.selectedText() == QString(QChar::Tabulation))
	{
		//On whitespace, increase the whitespace count, which then increases the indentation level.
		if(_indentationCursor.selectedText() != QString(QChar::Tabulation))
			whitespaceCount++;

		//On tab or 4 whitespaces, increase the indentation level.
		if(whitespaceCount >= 4 || _indentationCursor.selectedText() == QString(QChar::Tabulation))
		{
			whitespaceCount = 0;
			indentationLevel++;
		}

		//Select the next character.
		_indentationCursor.setPosition(_indentationCursor.selectionEnd());
		_indentationCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	}

	//Return indentation level.
	return indentationLevel;
}

void GLSLIndentor::setIndentationLevel(int indentationLevel)
{
	//Select the first character of the block.
	_indentationCursor.movePosition(QTextCursor::StartOfBlock);
	_indentationCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

	//Keep selecting the next character, until a non-indenting character is found.
	while(_indentationCursor.selectedText() == QString(QChar::Space) ||
		  _indentationCursor.selectedText() == QString(QChar::Nbsp) ||
		  _indentationCursor.selectedText() == QString(QChar::Tabulation))
	{
		_indentationCursor.removeSelectedText();
		_indentationCursor.movePosition(QTextCursor::StartOfBlock);
		_indentationCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	}

	//Add the indentation as specified, starting at the anchor, which identifies the last position before non-indenting
	//character. This might actually be a newline, and if so, move to StartOfBlock would fail.
	_indentationCursor.setPosition(_indentationCursor.anchor());
	for(int i = 0; i < indentationLevel; i++)
		_indentationCursor.insertText(TAB_TEXT);
}

void GLSLIndentor::autoindentDocumentBlock()
{
	//Clear the cursor's selection, it's not used, but store it's old value.
	_indentationCursor.clearSelection();

	//Cursor specified the exact position for indention check. This is always right behind the last index of a closing
	//bracket.
	QString cursorBlockText = _indentationCursor.block().text();
	_indentationCursor.movePosition(QTextCursor::StartOfBlock);
	_indentationCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
								   cursorBlockText.lastIndexOf('}') + 1);

	//Get the indentation level by using the adjusted cursor.
	GLSLCodeBlock* rootBlock = _parentDocument->getWhitespaceCodeBlock();
	GLSLCodeBlock* codeBlock = rootBlock->findChildBlock(_indentationCursor);
	int indentationLevel = (codeBlock ? codeBlock : rootBlock)->getIndentationLevel(_indentationCursor);

	//Apply the indentation.
	setIndentationLevel(indentationLevel);
}

void GLSLIndentor::autoindentDocumentBlocks()
{
	//Retrieve the last block index to indent.
	int lastBlock = _parentDocument->findBlock(_indentationCursor.selectionEnd()).blockNumber();

	//Get the block underneath the anchor, then iterate until all blocks are handled.
	for(QTextBlock textBlock = _indentationCursor.block();
		textBlock.blockNumber() <= lastBlock && textBlock.isValid();
		textBlock = textBlock.next())
	{
		//Use the line indentation method for the current block.
		_indentationCursor.setPosition(textBlock.position());
		autoindentDocumentBlock();
	}
}

void GLSLIndentor::autoindentCodeBlock()
{
	//Retrieve a cursor marking the whole block.
	GLSLCodeBlock* rootBlock = _parentDocument->getWhitespaceCodeBlock();
	GLSLCodeBlock* codeBlock = rootBlock->findChildBlock(_indentationCursor);
	QTextCursor blockCursor = (codeBlock ? codeBlock : rootBlock)->getCompleteBlock();

	//Adjust the indentation cursor, to allow editing the correct block.
	_indentationCursor.setPosition(blockCursor.selectionStart());
	_indentationCursor.setPosition(blockCursor.selectionEnd(), QTextCursor::KeepAnchor);

	//Autoindent using the document blocks method.
	autoindentDocumentBlocks();
}

void GLSLIndentor::adjustIndentationLevel(int value)
{
	//Retrieve the last block index to indent.
	int lastBlock = _parentDocument->findBlock(_indentationCursor.selectionEnd()).blockNumber();

	//Get the block underneath the anchor, then iterate until all blocks are handled.
	for(QTextBlock textBlock = _indentationCursor.block();
		textBlock.blockNumber() <= lastBlock && textBlock.isValid();
		textBlock = textBlock.next())
	{
		//Adjust the indentation level for the given line.
		_indentationCursor.setPosition(textBlock.position());
		setIndentationLevel(getIndentationLevel() + value);
	}
}
