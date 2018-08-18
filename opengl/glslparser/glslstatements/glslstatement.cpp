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

#include "glslstatement.h"
#include "../glslcodeblock.h"
#include "../glsldocument.h"

using namespace ysm;

void GLSLStatement::connectStatements(GLSLStatement *predecessor, GLSLStatement *successor)
{
	//Just set the values accordingly.
	predecessor->_nextStatement = successor;
	successor->_previousStatement = predecessor;
}

GLSLStatement::GLSLStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent) :
	QObject(parent),
	_document(codeDocument),
	_statementCursor(codeDocument),
	_statement(statement),
	_nextStatement(NULL),
	_previousStatement(NULL),
	_body(NULL)
{ }

QString GLSLStatement::getText() const { return _statement; }
GLSLDocument* GLSLStatement::getDocument() const { return _document; }

GLSLStatementType GLSLStatement::getType() const { return GLSLStatementType::Unknown; }

QChar GLSLStatement::getTerminator() const { return _terminator; }
void GLSLStatement::setTerminator(const QChar& terminator) { _terminator = terminator; }

QTextCursor GLSLStatement::getCompleteStatement() const { return _statementCursor; }
void GLSLStatement::setStatementAnchor(int cursorPosition) { _statementCursor.setPosition(cursorPosition); }
void GLSLStatement::setStatementPosition(int cursorPosition) { _statementCursor.setPosition(cursorPosition, QTextCursor::KeepAnchor); }

GLSLStatement* GLSLStatement::getNextStatement() const { return _nextStatement; }
GLSLStatement* GLSLStatement::getPreviousStatement() const { return _previousStatement; }

GLSLCodeBlock* GLSLStatement::getBody() const { return _body; }
void GLSLStatement::setBody(GLSLCodeBlock *body) { _body = body; }

QList<GLSLStatementError> GLSLStatement::getErrors() { return QList<GLSLStatementError>() << getGLSLErrors() << getPipelineErrors(); }
QList<GLSLStatementError> GLSLStatement::getGLSLErrors() { return QList<GLSLStatementError>(); }
QList<GLSLStatementError> GLSLStatement::getPipelineErrors() const { return _errors; }
void GLSLStatement::addPipelineError(GLSLStatementError error) { _errors.append(error); }
