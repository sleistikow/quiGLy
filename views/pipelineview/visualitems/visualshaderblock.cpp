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

#include "visualshaderblock.h"
#include "../pipelinescene.h"
#include "../pipelinetab.h"

#include "opengl/glslparser/glsldocument.h"

#include "data/types/glsldocumentlist.h"
#include "data/properties/glsldocumentlistproperty.h"

#include "commands/iuicommandqueue.h"
#include "commands/editor/setdocumentcommand.h"

#include <QGraphicsSceneMouseEvent>

using namespace ysm;

VisualShaderBlock::VisualShaderBlock(IBlock* block, IView* parentView) :
	VisualBlock(block, parentView)
{
	//Create context menu actions.
	_editorAction = new  QAction(QIcon(":/tango/16x16/actions/gtk-edit.png"), tr("Edit Code"), this);
	_addCodeAction = new  QAction(QIcon(":/tango/16x16/actions/document-new.png"), tr("Add Code"), this);

	connect(_editorAction, SIGNAL(triggered(bool)), this, SLOT(showCodeEditor()));
	connect(_addCodeAction, SIGNAL(triggered(bool)), this, SLOT(addCode()));
}

QList<QAction*> VisualShaderBlock::getContextActions() const
{
	//Get parent actions.
	QList<QAction*> baseList = VisualBlock::getContextActions();

	//Add seperator and editor action.
	baseList.prepend(NULL);
	baseList.prepend(_addCodeAction);

	//Editor only available, if code pages are available. Otherwise, use add code action.
	if(getBlock()->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code)->getValue().getDocuments().count())
		baseList.prepend(_editorAction);

	return baseList;
}

void VisualShaderBlock::addCode()
{
	//Fire command to add a new document, then request editor for the document.
	executeCommand(new SetDocumentCommand(new GLSLDocument(), getBlock(), true));
	getPipelineScene()->getPipelineTab()->requestEditor(getBlock());
}

void VisualShaderBlock::showCodeEditor() { getPipelineScene()->getPipelineTab()->requestEditor(getBlock()); }

