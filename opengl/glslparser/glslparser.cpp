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

#include "glslparser.h"
#include "glslcodeblock.h"
#include "glslstatementfactory.h"
#include "glsldocument.h"

#include "glslstatements/glsldeclaration.h"
#include "glslstatements/glslversiondirective.h"
#include "glslstatements/glslextensiondirective.h"

#include <QDebug>

using namespace ysm;

GLSLParser::GLSLParser(GLSLDocument* codeDocument) :
	QObject(codeDocument),
	_codeDocument(codeDocument),
	_currentBlock(NULL),
	_currentStream(NULL)
{
	//Register all known statement types.
	_statementFactory = new GLSLStatementFactory(this);
	_statementFactory->registerStatement<GLSLDeclaration>();
	_statementFactory->registerStatement<GLSLVersionDirective>();
	_statementFactory->registerStatement<GLSLExtensionDirective>();
}

void GLSLParser::parseStatementCharacter()
{
	switch(_currentCharacter.unicode())
	{

	//Check for single line comment. Clear the non-whitespace stack, to prevent parsing fails between code and comment.
	case '/':
		if(_currentNonWhitespaceWord.endsWith('/'))
			_currentState |= States::ParsingSingleLineComment | States::ClearNonWhitespaceStack;
		break;

	//Check for multi line comment. Clear the non-whitespace stack,	to prevent parsing fails between code and comment.
	//For example: /*/ NO COMMENT BUT THIS WOULD FIT IF THE STACK WAS NOT CLEARED.
	case '*':
		if(_currentNonWhitespaceWord.endsWith('/'))
			_currentState |= States::ParsingMultiLineComment | States::ClearNonWhitespaceStack;
		break;

	//Parse child block.
	case '{':
		_currentState |= States::ClearStatementStack | NewBlock;
		break;

	//Finish this block.
	case '}':
		//Make sure this is not the root block. The root block always stretches over the whole document.
		_currentState |= States::ClearStatementStack;
		if(_currentBlock->getParentBlock())
			_currentBlock->setValid(true);
		break;

	//Parse preprocessor.
	case '#':
		_currentState |= States::ParsingPreprocessor;
		break;

	//Finish this statement.
	case ';':
		_currentState |= States::ClearStatementStack;
		break;

	//Parse new line.
	case '\n':
	case '\r':
		if(!_currentNonWhitespaceWord.endsWith('\\'))
			_currentState |= States::NewLine;

		break;
	}
}

void GLSLParser::parseSingleLineCommentCharacter()
{
	//Check for comment end, nothing else to parse for single line comments. Non-whitespace stack is also automatically
	//cleared.
	switch(_currentCharacter.unicode())
	{
	case '\n':
	case '\r':
		if(!_currentNonWhitespaceWord.endsWith('\\'))
			_currentState |= States::NewLine;

		break;
	}
}

void GLSLParser::parseMultiLineCommentCharacter()
{
	switch(_currentCharacter.unicode())
	{

	//Check for comment end. Clear the non-whitespace stack, to prevent parsing fails between code and comment.
	//For example: /* COMMENT *// NO COMMENT BUT TWO BACKSLASHES.
	case '/':
		if(_currentNonWhitespaceWord.endsWith('*'))
		{
			_currentState &= ~States::ParsingMultiLineComment;
			_currentState |= States::ClearNonWhitespaceStack;
		}

		break;

	//Parse new line.
	case '\n':
	case '\r':
		if(!_currentNonWhitespaceWord.endsWith('\\'))
			_currentState |= States::NewLine;

		break;
	}
}

void GLSLParser::clearStatementStack(bool indentationOnly, bool endNonWhitespace)
{
	//Check if the statement is valid. Statements that are terminated by semicolon are allowed to be empty.
	_currentStatement = _currentStatement.trimmed();
	if(!_currentStatement.isEmpty() || _currentCharacter == ';')
	{
		//Create the statement. If indentation only is activated, do not try to recognize the statement.
		GLSLStatement* statement = indentationOnly ?
					new GLSLStatement(_currentStatement, _codeDocument, _currentBlock) :
					_statementFactory->createStatement(_currentStatement, _codeDocument, _currentBlock);

		//The statement ends after a semicolon or before the first whitespace before a bracket.
		//If this statement ends, because the document ends, do not cut the last character.
		statement->setStatementAnchor(_currentStatementAnchor);
		statement->setStatementPosition(endNonWhitespace ? _currentNonWhitespaceAnchor : _currentStream->pos());
		statement->setTerminator(_currentCharacter);

		//Connect to the last statement, if available.
		if(_lastStatement)
			GLSLStatement::connectStatements(_lastStatement, statement);

		//Store the statement inside the current block.
		_lastStatement = statement;
		_currentBlock->addStatement(_lastStatement);
	}

	//Clear to read next statement.
	_currentStatement.clear();
}

GLSLCodeBlock* GLSLParser::parseCodeBlock(bool indentationOnly)
{
	//Create the new block as child of the current block, and use it as current block. Clear the new block flag.
	_lastStatement = NULL;
	_currentState &= ~States::NewBlock;
	_currentBlock = new GLSLCodeBlock(_codeDocument, _currentBlock);
	_currentBlock->setBlockAnchor(_currentStream->pos());

	//Iterate over the code character by character, until the block is completed.
	while(!_currentStream->atEnd() && !_currentBlock->isValid())
	{
		//Read the next character.
		*_currentStream >> _currentCharacter;

		//Check the current parser state and execute subroutines.
		if(_currentState.testFlag(States::ParsingSingleLineComment))
		{
			//Move the statement anchor, because statements must not be in single line comments.
			parseSingleLineCommentCharacter();
			_currentStatementAnchor = _currentStream->pos();
		}
		else if(_currentState.testFlag(States::ParsingMultiLineComment))
			parseMultiLineCommentCharacter();
		else
		{
			parseStatementCharacter();

			//Update statement stack.
			if(!_currentState.testFlag(States::ClearStatementStack))
			{
				//Ignore whitespace if statement is still empty.
				if(_currentStatement.isEmpty() && _currentCharacter.isSpace())
					_currentStatementAnchor = _currentStream->pos();
				else
					_currentStatement.append(_currentCharacter);
			}

			//Clear comment start from statement stack.
			if(_currentState.testFlag(States::ParsingMultiLineComment) ||
				_currentState.testFlag(States::ParsingSingleLineComment))
				_currentStatement.resize(_currentStatement.count() - 2);
		}

		//Check for preprocessor statement endings.
		if(_currentState.testFlag(States::ParsingPreprocessor) && _currentState.testFlag(States::NewLine))
			_currentState |= States::ClearStatementStack;

		//If statement stack is about to be cleared, create the statement.
		if(_currentState.testFlag(States::ClearStatementStack))
		{
			clearStatementStack(indentationOnly, _currentCharacter != ';');
			_currentStatementAnchor = _currentStream->pos();
		}

		//Update the non-whitespace character stack.
		if(_currentCharacter.isSpace() || _currentState.testFlag(States::ClearNonWhitespaceStack))
			_currentNonWhitespaceWord.clear();
		else
		{
			_currentNonWhitespaceWord.append(_currentCharacter);
			_currentNonWhitespaceAnchor = _currentStream->pos();
		}

		//Create child block.
		if(_currentState.testFlag(States::NewBlock))
		{
			//Cache the last statement, in case the new block is the statement's body.
			GLSLStatement* lastStatement = _lastStatement;

			//Parse the block and store it.
			GLSLCodeBlock* childBlock = parseCodeBlock(indentationOnly);
			_currentBlock->addChildBlock(childBlock);
			if(lastStatement && lastStatement->getTerminator() != ';')
			{
				lastStatement->setBody(childBlock);
				childBlock->setParentStatement(lastStatement);
			}

			//Restore the last statement.
			_lastStatement = lastStatement;
		}

		//Clear new line states.
		if(_currentState.testFlag(States::NewLine))
			_currentState &= ~(States::ParsingPreprocessor |
							   States::ParsingSingleLineComment);

		//Clear loop states.
		_currentState &= ~(States::ClearNonWhitespaceStack |
						   States::ClearStatementStack |
						   States::NewLine);
	}

	//Clear last statement stack. If the stream suddenly ends, the last statement might not be parsed, yet.
	if(_currentStream->atEnd())
		clearStatementStack(indentationOnly, true);

	//Set the block's cursor. Decrease the position by one, if the block was closed by a bracket, because the bracket
	//has been parsed, but is not part of the block's content.
	_currentBlock->setBlockPosition(_currentStream->pos() - (_currentBlock->isValid() ? 1 : 0));

	//Return the created block, and reset the current block.
	GLSLCodeBlock* createdBlock = _currentBlock;
	_currentBlock = _currentBlock->getParentBlock();

	return createdBlock;
}

GLSLCodeBlock* GLSLParser::parseDocument(bool indentationOnly)
{
	//Get the plain code string and a stream.
	QString plainCode = _codeDocument->toPlainCode();
	_currentStream = new QTextStream(&plainCode);

	//Initialize the parser.
	_currentBlock = NULL;
	_currentState = States::Default;
	_currentNonWhitespaceWord.clear();
	_currentStatement.clear();
	_currentStatementAnchor = 0;
	_currentNonWhitespaceAnchor = 0;

	//Parse the root block.
	GLSLCodeBlock* rootBlock = parseCodeBlock(indentationOnly);

	//Clean up.
	delete _currentStream;
	_currentStream = NULL;

	//Return parsed data.
	return rootBlock;
}
