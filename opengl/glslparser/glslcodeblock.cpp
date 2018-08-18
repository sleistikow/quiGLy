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

#include "glslcodeblock.h"
#include "glsldocument.h"

#include <QDebug>
#include <QTextBlock>

using namespace ysm;

GLSLCodeBlock::GLSLCodeBlock(GLSLDocument* codeDocument, GLSLCodeBlock* parent) :
	QObject(parent),
	_blockCursor(codeDocument),
	_parentBlock(parent),
	_codeDocument(codeDocument),
	_parentStatement(NULL),
	_isValid(false)
{ }

QTextCursor GLSLCodeBlock::getOpeningBracket() const
{
	//Copy the block cursor, and move it's position 1 character behind the block anchor.
	QTextCursor bracketCursor(_blockCursor);
	bracketCursor.setPosition(_blockCursor.selectionStart());
	bracketCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

	//Return the adjusted cursor.
	return bracketCursor;
}

QTextCursor GLSLCodeBlock::getClosingBracket() const
{
	//Copy the block cursor, and move it's anchor 1 character before the block position.
	QTextCursor bracketCursor(_blockCursor);
	bracketCursor.setPosition(_blockCursor.selectionEnd());
	bracketCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

	//Return the adjusted cursor.
	return bracketCursor;
}

int GLSLCodeBlock::getIndentationLevel() const { return _parentBlock ? _parentBlock->getIndentationLevel() + 1 : 0; }
int GLSLCodeBlock::getIndentationLevel(const QTextCursor& textCursor) const
{
	//Calculate the block's indentation level.
	int indentationLevel = getIndentationLevel();

	//Try to find a statement.
	GLSLStatement* statement = findStatement(textCursor);
	if(statement)
	{
		//Check if the cursor is on a different line than the statement's start (multiline statement).
		int cursorStart = _codeDocument->findBlock(textCursor.selectionStart()).blockNumber();
		int codeStart = _codeDocument->findBlock(statement->getCompleteStatement().selectionStart()).blockNumber();

		//Ensure, that if there is an opening brace terminating this statement, it's not on the cursor's line.
		bool braceBlock = statement->getTerminator() == '{' &&
			_codeDocument->findBlock(textCursor.selectionStart()).blockNumber() ==
			_codeDocument->findBlock(statement->getCompleteStatement().selectionEnd()).blockNumber();

		//Check, if this is actually a line of a multiline statement.
		if(cursorStart != codeStart && !braceBlock)
			indentationLevel++;
	}

	return indentationLevel;
}

GLSLDocument* GLSLCodeBlock::getDocument() const { return _codeDocument; }

QTextCursor GLSLCodeBlock::getCompleteBlock() const { return _blockCursor; }
void GLSLCodeBlock::setBlockAnchor(int cursorPosition) { _blockCursor.setPosition(cursorPosition); }
void GLSLCodeBlock::setBlockPosition(int cursorPosition) { _blockCursor.setPosition(cursorPosition, QTextCursor::KeepAnchor); }

GLSLCodeBlock* GLSLCodeBlock::getParentBlock() const { return _parentBlock; }

GLSLStatement* GLSLCodeBlock::getParentStatement() const { return _parentStatement; }
void GLSLCodeBlock::setParentStatement(GLSLStatement *parentStatement) { _parentStatement = parentStatement; }

bool GLSLCodeBlock::isValid() const { return _isValid; }
void GLSLCodeBlock::setValid(const bool isValid) { _isValid	= isValid; }

QSet<GLSLCodeBlock*> GLSLCodeBlock::getChildBlocks() const { return _childBlocks; }
void GLSLCodeBlock::addChildBlock(GLSLCodeBlock* childBlock) { _childBlocks.insert(childBlock); }
void GLSLCodeBlock::removeChildBlock(GLSLCodeBlock* childBlock) { _childBlocks.remove(childBlock); }

void GLSLCodeBlock::addStatement(GLSLStatement *statement) { _statements.append(statement); }
QList<GLSLStatement*> GLSLCodeBlock::getStatements(bool recursive) const
{
	//Get blocks statements.
	QList<GLSLStatement*> statements = _statements;

	//Do recursive search.
	if(recursive)
		foreach(GLSLCodeBlock* codeBlock, _childBlocks)
			statements.append(codeBlock->getStatements(true));

	return statements;
}

GLSLStatement* GLSLCodeBlock::findStatement(const QTextCursor& textCursor) const
{
	//Iterate and select the first statement that is ending behind the cursor.
	foreach(GLSLStatement* statement, _statements)
	{
		if(statement->getCompleteStatement().selectionStart() <= textCursor.selectionStart() &&
		   statement->getCompleteStatement().selectionEnd() >= textCursor.selectionEnd())
			return statement;
	}

	//No statement found.
	return NULL;
}

GLSLCodeBlock* GLSLCodeBlock::findChildBlock(const QTextCursor& textCursor)
{
	//Check, if this block's cursor contains the text cursor.
	if(_blockCursor.selectionStart() > textCursor.selectionStart() ||
	   _blockCursor.selectionEnd() < textCursor.selectionEnd())
		return NULL;

	//Iterate over the children, check if one contains the cursor.
	foreach(GLSLCodeBlock* child, _childBlocks)
	{
		//Check, if the child contains the cursor.
		GLSLCodeBlock* childResult = child->findChildBlock(textCursor);
		if(childResult)
			return childResult;
	}

	//If no child was found, but this block contains the cursor, return this block.
	return this;
}

