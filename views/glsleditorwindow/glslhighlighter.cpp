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

#include "glslhighlighter.h"
#include "views/logview/logview.h"

#include "opengl/glslparser/keywordreader.h"
#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/glsldocumentinfo.h"
#include "opengl/glslparser/glslpipelineadapter/iglslpipelinecheck.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"

#include <QXmlReader>
#include <QXmlStreamReader>
#include <QDebug>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#define MULTILINE_COMMENT 0x01
#define MULTILINE_EDITABLE 0x02

using namespace ysm;

QMap<QString, QTextCharFormat> GLSLHighlighter::_highlightFormats;
QList<QString> GLSLHighlighter::_highlightingOrder;
QPair<QRegularExpression, QRegularExpression> GLSLHighlighter::_multilineComment;
QPair<QRegularExpression, QRegularExpression> GLSLHighlighter::_editableBlock;

GLSLHighlighter::GLSLHighlighter(GLSLDocument* parent) :
	QSyntaxHighlighter(parent),
	_document(parent)
{
	//Load highlighting rules.
	loadHighlightingRules();

	//Watch the document for changes.
	GLSLPipelineAdapter* pipelineAdapter = parent->getPipelineAdapter();
	connect(pipelineAdapter, &GLSLPipelineAdapter::blockChanged, this, &GLSLHighlighter::updateKeywordList);
	connect(parent, &GLSLDocument::codeVerified, this, &GLSLHighlighter::updateKeywordList);

	//Initially update the keyword list.
	updateKeywordList();
}

void GLSLHighlighter::loadHighlightingRules()
{
	//Check if keyword list has been loaded.
	if(_highlightFormats.isEmpty())
	{
		//Try to load the XML file.
		QFile xmlFile(":/glsl/highlight.xml");
		if(xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			//Parse the XML.
			QXmlStreamReader xmlReader(&xmlFile);
			while(!xmlReader.error() && !xmlReader.isEndDocument())
			{
				//Read next tag, ensure starting tag.
				if(xmlReader.readNext() != QXmlStreamReader::StartElement)
					continue;

				//"syntaxHighlighter" is simply ignored.
				if(xmlReader.name() == "syntaxHighlighter")
					continue;

				//"highlighter" adds a new highlighting rule.
				if(xmlReader.name() == "highlighter")
				{
					//Read the highlighting data and store it.
					QString type;
					QTextCharFormat rule;
					while(xmlReader.readNextStartElement())
					{
						//Extract the data.
						if(xmlReader.name() == "type")
							type = xmlReader.readElementText().trimmed();
						if(xmlReader.name() == "fontColor")
							rule.setForeground(QColor(xmlReader.readElementText().trimmed()));
						if(xmlReader.name() == "fontWeight")
							rule.setFontWeight(xmlReader.readElementText().trimmed().toInt());
						if(xmlReader.name() == "fontStyle")
							rule.setFontItalic(xmlReader.readElementText().trimmed().toInt());
					}

					//Store the rule.
					if(!type.isEmpty())
					{
						_highlightFormats[type] = rule;
						_highlightingOrder.append(type);
					}
				}
			}

			//On success, manually add post internal data.
			if(!xmlReader.hasError())
			{
				//Hidden format.
				QTextCharFormat hiddenFormat;
				hiddenFormat.setFontLetterSpacing(5);
				hiddenFormat.setForeground(QColor(Qt::transparent));
				_highlightFormats["__hiddenControl"] = hiddenFormat;
				_highlightingOrder.append("__hiddenControl");

				//Editable line format.
				QTextCharFormat editableLineFormat;
				editableLineFormat.setBackground(QColor(Qt::white));
				editableLineFormat.setFontUnderline(true);
				_highlightFormats["__editableLine"] = editableLineFormat;
				_highlightingOrder.append("__editableLine");

				//Editable block format.
				QTextCharFormat editableBlockFormat;
				editableBlockFormat.setBackground(QColor(Qt::white));
				_highlightFormats["__editableBlock"] = editableBlockFormat;
				_highlightingOrder.append("__editableBlock");

				//Fixed section
			}

			//XML could not be parsed.
			else
				LogView::log(QString("highlight.xml could not be parsed. No syntax highlighting available.\nLine %1: %2")
							 .arg(xmlReader.lineNumber())
							 .arg(xmlReader.errorString()));

			//Cleanup.
			xmlReader.clear();
			xmlFile.close();
		}

		//XML file not available.
		else
			LogView::log("highlight.xml could not be opened. No syntax highlighting available.");

		//Create the multiline comment regex.
		_multilineComment.first = QRegularExpression("/\\*");
		_multilineComment.second = QRegularExpression("\\*/");

		//Create the editable block regex.
		_editableBlock.first = QRegularExpression(GLSLDocument::editableBlockStart);
		_editableBlock.second = QRegularExpression(GLSLDocument::editableBlockEnd);
	}
}

void GLSLHighlighter::highlightBlock(const QString &text)
{
	//Retrieve relevant data.
	int blockState = previousBlockState();

	//Iterate over all keyword types.
	foreach(QString type, _highlightingOrder)
	{
		//Manually look for multiline comments.
		if(type == "comment")
		{
			//Check if previous block was multiline comment. Otherwise, check if this block starts a multiline comment.
			int multilineStart = 0, multilineEnd = 0;
			if(!(blockState & MULTILINE_COMMENT))
				multilineStart = _multilineComment.first.match(text).capturedStart();

			//Iterate over all existing multiline comments to highlight and check wether this block ends as comment.
			//Flag is cleared and reset if no ending tag could be found.
			blockState &= !MULTILINE_COMMENT;
			while(multilineStart >= 0)
			{
				//Find the ending of the current multiline comment, starting behind the opening /*. Otherwise "/*/"
				//could be matched as opening and closing comment.
				multilineEnd = _multilineComment.second.match(text, multilineStart + 2).capturedEnd();

				//If no ending tag was found, mark the current block as comment.
				if(multilineEnd == -1)
				{
					blockState |= MULTILINE_COMMENT;
					multilineEnd = text.length();
				}

				//Format the comment.
				setFormat(multilineStart, multilineEnd - multilineStart, _highlightFormats["comment"]);

				//Try to find the next multiline comment.
				multilineStart = _multilineComment.first.match(text, multilineEnd).capturedStart();
			}
		}

		//Manually look for editable blocks.
		if(type == "__editableBlock")
		{
			//Check if the block closes an editable block.
			if(blockState & MULTILINE_EDITABLE)
				if(_editableBlock.second.match(text).hasMatch())
					blockState &= !MULTILINE_EDITABLE;

			//Check if the block is an editable block.
			if(blockState & MULTILINE_EDITABLE)
				setFormat(0, text.length(), _highlightFormats["__editableBlock"]);

			//Check if the block opens an editable block.
			if(!(blockState & MULTILINE_EDITABLE))
				if(_editableBlock.first.match(text).hasMatch())
					blockState |= MULTILINE_EDITABLE;
		}

		//Highlight default keywords from XML.
		else
		{
			//Check which match is used.
			int matchId = type == "__editableLine" ? 1 : 0;

			//Highlight all keywords of the given type.
			foreach(QRegularExpression keyword, _keywordLists[type])
			{
				//Find all matches and highlight them, using the type's style.
				QRegularExpressionMatchIterator matches = keyword.globalMatch(text);
				while(matches.hasNext())
				{
					QRegularExpressionMatch match = matches.next();
					setFormat(match.capturedStart(matchId), match.capturedLength(matchId), _highlightFormats[type]);
				}
			}
		}
	}

	//Set the block state for the next block.
	setCurrentBlockState(blockState);
}

void GLSLHighlighter::updateKeywordList()
{
	//Retrieve a list of all keywords and retrieve the current configuration.
	KeywordReader::KeywordList keywords = KeywordReader().getKeywords();
	IBlock* block = _document->getPipelineAdapter()->getBlock();
	int version = _document->getDocumentInfo()->getVersion().first;

	//Filter the keyword list if neccessary.
	if(block)
		keywords = keywords.ofShader(block->getType());
	if(version)
		keywords = keywords.ofVersion(version);

	//Iterate over all keywords and create the patterns.
	_keywordLists.clear();
	foreach(KeywordReader::Keyword keyword, keywords)
	{
		if(!keyword.isRegex)
			_keywordLists[keyword.type].append(QRegularExpression("\\b" + keyword.text + "\\b"));
		else
			_keywordLists[keyword.type].append(QRegularExpression(keyword.text));
	}

	//Append single line comments.
	_keywordLists["comment"].append(QRegularExpression("//.*"));

	//Append hidden control keywords.
	_keywordLists["__hiddenControl"].append(QRegularExpression(GLSLDocument::editableBlockStart));
	_keywordLists["__hiddenControl"].append(QRegularExpression(GLSLDocument::editableBlockEnd));
	_keywordLists["__hiddenControl"].append(QRegularExpression(GLSLDocument::editableStart));
	_keywordLists["__hiddenControl"].append(QRegularExpression(GLSLDocument::editableEnd));

	//Append editable line keywords.
	QString editableStart = GLSLDocument::editableStart;
	QString editableEnd = GLSLDocument::editableEnd;
	_keywordLists["__editableLine"].append(QRegularExpression(editableStart + "(.*)" + editableEnd));

	//Update the whole document.
	rehighlight();
}
