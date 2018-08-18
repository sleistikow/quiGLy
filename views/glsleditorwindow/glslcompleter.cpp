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

#include "glslcompleter.h"
#include "iglslcompleterdelegate.h"
#include "opengl/glslparser/glsldocumentinfo.h"
#include "opengl/glslparser/keywordreader.h"
#include "opengl/glslparser/glslcodeblock.h"
#include "opengl/glslparser/glslstatements/glslstatement.h"
#include "opengl/glslparser/glslstatements/glsldeclaration.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"

#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QToolTip>

using namespace ysm;

GLSLCompleterModel::GLSLCompleterModel(GLSLDocument* glslDocument, QObject* parent) :
	QAbstractListModel(parent),
	_document(glslDocument)
{
	//Connect to the document.
	connect(_document, &GLSLDocument::codeVerified, this, &GLSLCompleterModel::updateData);

	//Run initial data update.
	updateData();
}

void GLSLCompleterModel::findLocalScopeDeclarations(GLSLCodeBlock* codeBlock)
{
	//Ensure the block exists.
	if(!codeBlock) return;

	//Get the local scope declarations.
	foreach(GLSLDeclaration* declaration, codeBlock->getStatements<GLSLDeclaration>(GLSLStatementType::Declaration))
	{
		//If global scope, skip.
		if(declaration->isGlobalScope())
			continue;

		//Do not add invalid declarations.
		if(!declaration->getGLSLErrors().isEmpty())
			continue;

		//Add all remaining declarations.
		if(!_userKeywordNames.contains(declaration->getName()))
		{
			_userKeywordNames.append(declaration->getName());
			_userKeywords.append(declaration);
		}
	}

	//Add all parent declarations.
	findLocalScopeDeclarations(codeBlock->getParentBlock());
}

void GLSLCompleterModel::findGlobalScopeDeclarations(GLSLCodeBlock* codeBlock)
{
	//Ensure the block exists.
	if(!codeBlock) return;

	//Get the global scope declarations.
	foreach(GLSLDeclaration* declaration, codeBlock->getStatements<GLSLDeclaration>(GLSLStatementType::Declaration, true))
	{
		//Do not add local scope declarations.
		if(!declaration->isGlobalScope())
			continue;

		//Do not add instance declarations, they are contained because of their structural declaration.
		if(declaration->getStructureDeclaration())
			continue;

		//Do not add structural declarations without instance declarations. Their members are accessed directly.
		if(declaration->getDataType().isEmpty() && !declaration->getInstanceDeclaration())
			continue;

		//Do not add invalid declarations.
		if(!declaration->getGLSLErrors().isEmpty())
			continue;

		//Add all remaining declarations.
		if(!_userKeywordNames.contains(declaration->getName()))
		{
			_userKeywordNames.append(declaration->getName());
			_userKeywords.append(declaration);
		}
	}
}

void GLSLCompleterModel::setCurrentBlock(GLSLCodeBlock* codeBlock)
{
	//Notify about changes.
	beginResetModel();

	//Update the user declarations.
	_userKeywords.clear();
	_userKeywordNames.clear();

	findLocalScopeDeclarations(codeBlock);
	findGlobalScopeDeclarations(_document->getFullCodeBlock());

	//Fire changes.
	endResetModel();
}

void GLSLCompleterModel::updateData()
{
	//Notify about changes.
	beginResetModel();

	//Get the document data.
	int version = _document->getDocumentInfo()->getVersion().first;
	IBlock* block = _document->getPipelineAdapter()->getBlock();

	//Update the GLSL keywords.
	_glslKeywords.clear();
	_glslKeywords.append(KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_BUILTIN));
	_glslKeywords.append(KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_FUNCTIONS));
	_glslKeywords.append(KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_CONSTANT));

	//Filter the keywords.
	if(version)
		_glslKeywords = _glslKeywords.ofVersion(version);
	if(block)
		_glslKeywords = _glslKeywords.ofShader(block->getType());

	//Remove all expression keywords.
	for(int i = _glslKeywords.count() - 1; i >= 0; i--)
		if(_glslKeywords[i].isRegex)
			_glslKeywords.removeAt(i);

	//Fire changes.
	endResetModel();
}

int GLSLCompleterModel::rowCount(const QModelIndex& parent) const
{
	//No children.
	if(parent.isValid())
		return 0;

	//Return the appended count.
	return _glslKeywords.count() + _userKeywords.count();
}

QVariant GLSLCompleterModel::data(const QModelIndex& index, int role) const
{
	//Ensure index is valid.
	int currentIndex = index.row();
	if(currentIndex < 0)
		return QVariant::Invalid;

	//Check if in global scope range.
	if(currentIndex < _userKeywords.count())
	{
		GLSLDeclaration* declaration = _userKeywords[currentIndex];
		GLSLDeclaration* instance = declaration->getInstanceDeclaration();
		switch(role)
		{
		case Qt::DisplayRole:
		case Qt::UserRole + 1:
			if(instance) return instance->getName();
			return declaration->getName();
		case Qt::DecorationRole:
			if(declaration->isMethod()) return QIcon(":/glsl/glsl/func");
			if(declaration->hasQualifier("buffer")) return QIcon(":/glsl/glsl/buffer");
			if(declaration->hasQualifier("uniform")) return QIcon(":/glsl/glsl/uniform");
			if(declaration->hasQualifier("in")) return QIcon(":/glsl/glsl/in");
			if(declaration->hasQualifier("out")) return QIcon(":/glsl/glsl/out");
			if(declaration->isGlobalScope()) return QIcon(":/glsl/glsl/global");
			return QIcon(":/glsl/glsl/local");
		case Qt::ToolTipRole:
			if(instance) return	declaration->getText() + instance->getText();
			return declaration->getText();
		default:
			return QVariant::Invalid;
		}
	}

	//Check if GLSL keyword.
	currentIndex -= _userKeywords.count();
	if(currentIndex < _glslKeywords.count())
	{
		KeywordReader::Keyword keyword = _glslKeywords[currentIndex];
		switch(role)
		{
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		case Qt::UserRole + 1:
			return keyword.text;
		case Qt::DecorationRole:
			if(keyword.type == "function") return QIcon(":/glsl/glsl/glfunc");
			if(keyword.type == "builtin") return QIcon(":/glsl/glsl/glvar");
			if(keyword.type == "constant") return QIcon(":/glsl/glsl/glconst");
			return QVariant::Invalid;
		default:
			return QVariant::Invalid;
		}
	}

	return QVariant::Invalid;
}

GLSLCompleterView::GLSLCompleterView(QWidget *parent) :
	QListView(parent)
{
	//Set properties.
	setAutoScroll(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GLSLCompleterView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	//Call the base first.
	QListView::currentChanged(current, previous);

	//Get the tool tip text.
	QString toolTipText = model()->itemData(current)[Qt::ToolTipRole].toString();

	//Calculate position and show it.
	QRect itemRect = visualRect(current);
	QToolTip::showText(mapToGlobal(itemRect.topLeft() + QPoint(width(), -itemRect.height())), toolTipText, this);
}

GLSLCompleter::GLSLCompleter(GLSLDocument* glslDocument, QObject* parent) :
	QCompleter(parent),
	_delegate(NULL),
	_glslDocument(glslDocument)
{
	//Set the model and completion mode.
	setModel(_model = new GLSLCompleterModel(_glslDocument, this));
	setPopup(new GLSLCompleterView());
	setCompletionMode(QCompleter::PopupCompletion);
	setCompletionRole(Qt::UserRole + 1);

	//Connect signals and slots.
	connect(_glslDocument, &GLSLDocument::contentsChanged, this, &GLSLCompleter::documentChanged);
	connect(this, QOverload<const QString&>::of(&QCompleter::activated), this, &GLSLCompleter::forwardCompletion);
}

IGLSLCompleterDelegate* GLSLCompleter::getDelegate() const { return _delegate; }
void GLSLCompleter::setDelegate(IGLSLCompleterDelegate* delegate) { _delegate = delegate; }

void GLSLCompleter::documentChanged()
{
	//Delegate must be available.
	if(!_delegate)
		return;

	//Get the completer's new prefix.
	QString completerPrefix = _delegate->getCompleterPrefix(this);

	//Set the completer's new prefix.
	setCompletionPrefix(completerPrefix);
}

void GLSLCompleter::forwardCompletion(const QString& completion)
{
	//Delegate must be available.
	if(_delegate)
		_delegate->completerCompleted(this, completion);

	//Hide in any case.
	popup()->hide();
}

bool GLSLCompleter::handleKeyPress(QKeyEvent* args)
{
	//Some keys need special treatment.
	bool isCtrl = args->modifiers() == Qt::ControlModifier || args->modifiers() == Qt::ControlModifier;
	if(popup()->isVisible())
		switch (args->key())
		{
		//Keys used to complete the current selection must not be handled by the delegate. Otherwise they are not
		//forwarded correctly to the completer.
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			return true;

		//Keys that hide the completer, but do not block the keypress.
		case Qt::Key_Space:
		case Qt::Key_Left:
		case Qt::Key_Right:
			popup()->hide();
			return false;
	   }
	else
		switch (args->key())
		{
		//Space + ctrl key shows the completer and blocks the keypress (meta on OS X).
		case Qt::Key_Space:
			if(isCtrl)
			{
				complete();
				return true;
			}
		}

	//Key press not known, do not handle it.
	return false;
}

void GLSLCompleter::complete()
{
	//Calculate the completer's position.
	QRect completerRect = _delegate->getCompleterRect(this);
	if(completerRect.width() == 0)
		completerRect.setWidth(200);

	//Get current block and show completer.
	_model->setCurrentBlock(_delegate->getCompleterBlock(this));
	QCompleter::complete(completerRect);
}
