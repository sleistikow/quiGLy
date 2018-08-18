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

#include "glsldocument.h"
#include "glsldocumentinfo.h"
#include "glslparser.h"

#include "glslpipelineadapter/glslpipelineadapter.h"
#include "views/glsleditorwindow/glslhighlighter.h"
#include "commands/iuicommandqueue.h"

#include <QTextCursor>
#include <QTextBlock>
#include <QPlainTextDocumentLayout>
#include <QRegularExpression>
#include <QDebug>

using namespace ysm;

const QString GLSLDocument::editableStart = "<§<";
const QString GLSLDocument::editableEnd = ">§>";

const QString GLSLDocument::editableBlockStart = "<#<";
const QString GLSLDocument::editableBlockEnd = ">#>";

GLSLDocument::GLSLDocument(QObject* parent) :
	QTextDocument(parent),
	_retainCount(0),
	_fullCodeBlock(NULL),
	_whitespaceCodeBlock(NULL),
	_fullCodeDirty(true),
	_whitespaceCodeDirty(true),
	_verificationFlag(false)
{
	//Set plain document layout.
	setDocumentLayout(new QPlainTextDocumentLayout(this));

	//Create the document's relations.
	_parser = new GLSLParser(this);
	_info = new GLSLDocumentInfo(this);
	_adapter = new GLSLPipelineAdapter(this);
	_highlighter = new GLSLHighlighter(this);

	//Watch content changes.
	connect(this, &GLSLDocument::contentsChanged, this, &GLSLDocument::invalidateCode);
}

GLSLDocument::~GLSLDocument()
{
	//Delete document info.
	delete _info;
}

void GLSLDocument::retain() { _retainCount++; }
void GLSLDocument::release() { if(!--_retainCount) delete this; }

void GLSLDocument::invalidateCode()
{
	//Check if verification is being executed.
	if(_verificationFlag) return;

	//Mark code as dirty.
	_whitespaceCodeDirty = true;
	_fullCodeDirty = true;
}

void GLSLDocument::synchronizeDocument()
{
	//Parse and synchronize the document.
	_fullCodeBlock = _parser->parseDocument();
	_fullCodeBlock = _adapter->synchronizeDocument();
	_fullCodeDirty = false;

	//Notify about parse.
	_verificationFlag = true;
	emit codeVerified(_fullCodeBlock);
	_verificationFlag = false;
}


void GLSLDocument::saveDocument() { _savedContent = toPlainText(); setModified(false); }
void GLSLDocument::revertDocument() { setPlainText(_savedContent); setModified(false); }

GLSLParser* GLSLDocument::getParser() const { return _parser; }
GLSLDocumentInfo* GLSLDocument::getDocumentInfo() const { return _info; }
GLSLPipelineAdapter* GLSLDocument::getPipelineAdapter() const { return _adapter; }

QString GLSLDocument::getSavedContent() const { return _savedContent; }
void GLSLDocument::setSavedContent(QString savedContent) { _savedContent = savedContent; }

QString GLSLDocument::toPlainCode(bool keepLength) const
{
	//Convert to code by removing markers.
	QString plainText = toPlainText();
	return _adapter->validatePlainText(plainText, keepLength);
}

QString GLSLDocument::getSavedCode(bool keepLength) const
{
	//Convert to code by removing markers.
	QString plainText = getSavedContent();
	return _adapter->validatePlainText(plainText, keepLength);
}

GLSLCodeBlock* GLSLDocument::getFullCodeBlock() const
{
	// Ensure code block is clean.
	if(_fullCodeDirty)
	{
		_fullCodeBlock = _parser->parseDocument(false);
		_fullCodeDirty = false;
	}

	// Return the code block.
	return _fullCodeBlock;
}

GLSLCodeBlock* GLSLDocument::getWhitespaceCodeBlock() const
{
	// Ensure code block is clean.
	if(_whitespaceCodeDirty)
	{
		_whitespaceCodeBlock = _parser->parseDocument(true);
		_whitespaceCodeDirty = false;
	}

	// Return the code block.
	return _whitespaceCodeBlock;
}
