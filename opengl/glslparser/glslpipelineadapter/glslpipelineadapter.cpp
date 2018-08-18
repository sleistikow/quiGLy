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

#include "glslpipelineadapter.h"
#include "../glsldocument.h"
#include "../glslcodeblock.h"
#include "../glslparser.h"

#include "iglslpipelinecheck.h"
#include "glsltexturedeclarationcheck.h"
#include "glsluniformdeclarationcheck.h"
#include "glslvaodeclarationcheck.h"
#include "glslversiondirectivecheck.h"
#include "glslextensiondirectivecheck.h"
#include "glslssbodeclarationcheck.h"
#include "glslubodeclarationcheck.h"
#include "glslredefinitioncheck.h"

#include <QRegularExpression>

#define BLOCK_CONNECTION "/** #quiGLy block #%1 **/"
#define SECTION_DIVIDER "/** #quiGLy section end **/"

#define SECTION_MARGIN 2

using namespace ysm;

GLSLPipelineAdapter::GLSLPipelineAdapter(GLSLDocument* parentDocument) :
	QObject(parentDocument),
	_pipelineBlock(NULL),
	_parentDocument(parentDocument)
{
	//Register checks, preprocessor first, then overall code checks, then specific code checks.
	addCheck(new GLSLVersionDirectiveCheck());
	addCheck(new GLSLExtensionDirectiveCheck());

	addCheck(new GLSLRedefinitionCheck());

	addCheck(new GLSLTextureDeclarationCheck());
	addCheck(new GLSLUniformDeclarationCheck());
	addCheck(new GLSLSSBODeclarationCheck());
	addCheck(new GLSLUBODeclarationCheck());

	//VAO check is registered dynamically for vertex shaders.
	_vaoCheck = new GLSLVaoDeclarationCheck();
}

GLSLPipelineAdapter::~GLSLPipelineAdapter()
{
	//Manually delete all pipeline checks.
	qDeleteAll(_pipelineChecks);
}

bool GLSLPipelineAdapter::checkCursorPosition(const QTextCursor& oldCursor, QTextCursor& newCursor)
{
	//Ensure fixed secton is available.
	if(!_pipelineBlock) return true;

	//Find the fixed section's end.
	int fixedSection = _parentDocument->find(SECTION_DIVIDER).selectionEnd();

	//If the cursor is outside the fixed section, position is always allowed.
	if(newCursor.selectionStart() > fixedSection)
		return true;

	//TODO: Cursor with selection.
	if(newCursor.hasSelection())
	{
		newCursor.clearSelection();
		return false;
	}

	//Tag offset is calculated by editable start, it's assumed that all tags have the same length.
	int tagLength = GLSLDocument::editableStart.length();

	//Create a cursor to select the text around the new cursor position.
	QTextCursor checkCursor(_parentDocument);
	checkCursor.setPosition(newCursor.position() - tagLength, QTextCursor::MoveAnchor);
	checkCursor.setPosition(newCursor.position() + tagLength, QTextCursor::KeepAnchor);

	//Find a possible tag that surrounds the cursor.
	QString surroundingText = checkCursor.selectedText();
	int cursorTagType = 0;

	//Identify the tag that surrounds the cursor (opening or closing).
	if(surroundingText.contains(GLSLDocument::editableBlockStart)) cursorTagType = -1;
	if(surroundingText.contains(GLSLDocument::editableBlockEnd)) cursorTagType = 1;
	if(surroundingText.contains(GLSLDocument::editableStart)) cursorTagType = -1;
	if(surroundingText.contains(GLSLDocument::editableEnd)) cursorTagType = 1;

	//Check wether the check cursor contains a tag, in this case the cursor's position is invalid.
	if(!cursorTagType)
		return true;

	//It's actually allowed that a cursor is directly behind an opening tag or directly in front of a closing tag.
	bool validPosition =
		surroundingText.startsWith(GLSLDocument::editableBlockStart) ||
		surroundingText.endsWith(GLSLDocument::editableBlockEnd) ||
		surroundingText.startsWith(GLSLDocument::editableStart) ||
		surroundingText.endsWith(GLSLDocument::editableEnd);

	//If the position is valid, no change is neccessary.
	if(validPosition)
		return true;

	//Calculate the cursors movement direction for adjustment. The cursor is always moved by only one character and
	//then continues moving because this method is called again. If the cursor was moved only inside the tag, continue
	//moving in the same direction, otherwise move in the direction of the editable part.
	bool moveBack = cursorTagType > 0;
	if(qAbs(oldCursor.position() - newCursor.position()) < tagLength)
		moveBack = oldCursor.position() > newCursor.position();

	//Adjust the cursor's position, by moving further in the calculated direction.
	newCursor.movePosition(moveBack ? QTextCursor::PreviousCharacter : QTextCursor::NextCharacter);

	//Cursor was changed.
	return false;
}

bool GLSLPipelineAdapter::checkLineEditingPermission(int pressedKey, QTextCursor textCursor)
{
	//Find the next tags from the cursor's start and end.
	int startOffset = textCursor.selectionStart() - GLSLDocument::editableStart.length() + 1;
	QTextCursor startTag = _parentDocument->find(GLSLDocument::editableStart, startOffset);
	QTextCursor endTag = _parentDocument->find(GLSLDocument::editableEnd, textCursor.selectionEnd());

	//If no end tag is found, editing is forbidden.
	if(endTag.isNull())
		return false;

	//If the next tag is a closing tag, the edit is permitted, as long as no newline is added.
	return
		(startTag.isNull() || startTag.selectionEnd() > endTag.selectionEnd()) &&
		(pressedKey != Qt::Key_Return && pressedKey != Qt::Key_Enter);
}

bool GLSLPipelineAdapter::checkBlockEditingPermission(QTextCursor textCursor)
{
	//Iterate over all blocks from the selection start to selection end.
	for(QTextBlock currentBlock = textCursor.block(); ;currentBlock = currentBlock.next())
	{
		//Check if the current block contains a tag, if so editing is permitted.
		if(!currentBlock.isValid() ||
			currentBlock.text().contains(GLSLDocument::editableBlockStart) ||
			currentBlock.text().contains(GLSLDocument::editableBlockEnd))
			return false;

		//If the block contains the cursor, no tag is contained in the cursor's selection.
		if(currentBlock.contains(textCursor.selectionEnd()))
			break;
	}

	//Find the next start and end tag.
	int tagOffset = textCursor.selectionEnd();
	QTextCursor startTag = _parentDocument->find(GLSLDocument::editableBlockStart, tagOffset);
	QTextCursor endTag = _parentDocument->find(GLSLDocument::editableBlockEnd, tagOffset);

	//If no end tag is found, editing is forbidden.
	if(endTag.isNull())
		return false;

	//If the next tag is a closing tag, the edit is permitted.
	return startTag.isNull() || startTag.selectionEnd() > endTag.selectionEnd();
}

bool GLSLPipelineAdapter::checkEditingPermission(int pressedKey, Qt::KeyboardModifiers keyModifiers,
												 QTextCursor textCursor)
{
	//Ensure fixed secton is available.
	if(!_pipelineBlock) return true;

	//Find the fixed section's end.
	int fixedSection = _parentDocument->find(SECTION_DIVIDER).selectionEnd();

	//Delete keys need to be handled differently, because they are not adding but removing text. Thus, the removed
	//character needs to be included in the selection so it's handled correctly.
	if(!textCursor.hasSelection())
		switch(pressedKey)
		{
			//Backward deletion.
			case Qt::Key_Backspace:
				textCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
				break;

			//Forward deletion.
			case Qt::Key_Delete:
				textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
				break;
		}

	//If the cursor is outside the fixed section, editing is always permitted.
	if(textCursor.selectionStart() > fixedSection)
		return true;

	//Perform line and block editing checks and return.
	return checkLineEditingPermission(pressedKey, textCursor) || checkBlockEditingPermission(textCursor);
}

void GLSLPipelineAdapter::insertStatements(const QList<IGLSLPipelineCheck::FixedStatement>& statements)
{
	//Find the fixed section's end.
	int fixedSection = _parentDocument->find(SECTION_DIVIDER).selectionEnd();

	//Create the header format.
	QTextBlockFormat headerFormat;
	headerFormat.setBackground(QColor(Qt::gray).light());

	//Create clear format for editable lines.
	QTextBlockFormat clearFormat;
	clearFormat.clearBackground();

	//Create cursor that will be used for editing.
	QTextCursor textCursor(_parentDocument);
	textCursor.beginEditBlock();
	textCursor.movePosition(QTextCursor::Start);

	//Clear the current header statements.
	if(fixedSection >= 0)
	{
		textCursor.setPosition(fixedSection, QTextCursor::KeepAnchor);
		textCursor.removeSelectedText();
	}

	//Set header format for first line. All newly added blocks will then inherit the setting.
	textCursor.setBlockFormat(headerFormat);

	//Insert all fixed sections, and group by header.
	QString commentHeader;
	foreach(IGLSLPipelineCheck::FixedStatement statement, statements)
	{
		//Add header grouping, if neccessary.
		if(commentHeader != statement.header)
		{
			textCursor.insertBlock();
			textCursor.insertText("/** #quiGLy section " + (commentHeader = statement.header) + " **/");
			textCursor.insertBlock();
		}

		//Add the code line by line.
		foreach(QString codeLine, statement.code)
		{
			//Add the code and check for closing tags.
			textCursor.insertText(codeLine);
			if(codeLine.contains(GLSLDocument::editableBlockEnd))
				textCursor.setBlockFormat(headerFormat);

			//Add next line and check for opening tags.
			textCursor.insertBlock();
			if(codeLine.contains(GLSLDocument::editableBlockStart))
				textCursor.setBlockFormat(clearFormat);
		}
	}

	//Insert concluding comment.
	textCursor.insertBlock();
	textCursor.insertText(QString(BLOCK_CONNECTION).arg(_pipelineBlock->getID()));

	textCursor.insertBlock();
	textCursor.insertText(SECTION_DIVIDER);

	//Add newline, if there is more text in this line.
	if(textCursor.block().text() != SECTION_DIVIDER)
	{
		//Add new line and apply format.
		textCursor.insertBlock();
		textCursor.setBlockFormat(clearFormat);
	}

	//Move to the next line, ensure it's also empty and available.
	//This ensures that it's possible to edit behind fixed section.
	textCursor.movePosition(QTextCursor::NextBlock);
	if(!textCursor.block().text().isEmpty())
	{
		textCursor.insertBlock();
		textCursor.setBlockFormat(clearFormat);
	}

	//Commit edit.
	textCursor.endEditBlock();
}

GLSLCodeBlock* GLSLPipelineAdapter::synchronizeDocument()
{
	//Match code to pipeline, if block is specified.
	GLSLCodeBlock* rootBlock = _parentDocument->getFullCodeBlock();
	if(_pipelineBlock)
	{
		//Execute all checks, to store parsed data to the document.
		QList<IGLSLPipelineCheck::FixedStatement> headerStatements;
		foreach(IGLSLPipelineCheck* pipelineCheck, _pipelineChecks)
			headerStatements.append(pipelineCheck->preCheck(_pipelineBlock, rootBlock));

		//Re-insert all header statements and parse them.
		insertStatements(headerStatements);

		//Re-parse the document to check for further errors.
		rootBlock = _parentDocument->getParser()->parseDocument();

		//Execute all checks, to mark errors.
		foreach(IGLSLPipelineCheck* pipelineCheck, _pipelineChecks)
			pipelineCheck->postCheck(_pipelineBlock, rootBlock);
	}

	//Otherwise, remove all markers by overwriting the content.
	else
	{
		//Use text cursor to keep undo operation's alive.
		QTextCursor textCursor(_parentDocument);
		textCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
		textCursor.insertText(_parentDocument->toPlainCode(false));
	}

	//Return the adjusted root block.
	return rootBlock;
}

QString& GLSLPipelineAdapter::validatePlainText(QString& plainText, bool keepLength)
{
	//Replace markers with whitespace of same length.
	if(keepLength)
		return plainText
			.replace(GLSLDocument::editableBlockStart, QString(GLSLDocument::editableBlockStart.length(), QChar::Space))
			.replace(GLSLDocument::editableBlockEnd, QString(GLSLDocument::editableBlockEnd.length(), QChar::Space))
			.replace(GLSLDocument::editableStart, QString(GLSLDocument::editableStart.length(), QChar::Space))
			.replace(GLSLDocument::editableEnd, QString(GLSLDocument::editableEnd.length(), QChar::Space));

	//Remove markers.
	return plainText
		.remove(GLSLDocument::editableBlockStart)
		.remove(GLSLDocument::editableBlockEnd)
		.remove(GLSLDocument::editableStart)
		.remove(GLSLDocument::editableEnd);
}

void GLSLPipelineAdapter::addCheck(IGLSLPipelineCheck* pipelineCheck) { _pipelineChecks.append(pipelineCheck); }
void GLSLPipelineAdapter::removeCheck(IGLSLPipelineCheck* pipelineCheck) { _pipelineChecks.removeOne(pipelineCheck); }

IBlock* GLSLPipelineAdapter::getBlock() const { return _pipelineBlock; }
void GLSLPipelineAdapter::setBlock(IBlock* block)
{
	//Ensure actual change.
	//TODO: Watch for changes using block's document.
	if(_pipelineBlock != block)
	{
		//Remove VAO check.
		if(_pipelineBlock && _pipelineBlock->getType() == BlockType::Shader_Vertex)
			removeCheck(_vaoCheck);

		//Update block and emit signal.
		_pipelineBlock = block;
		emit blockChanged(_pipelineBlock);

		//Add VAO check, if block is vertex shader.
		if(_pipelineBlock && _pipelineBlock->getType() == BlockType::Shader_Vertex)
			addCheck(_vaoCheck);
	}
}
