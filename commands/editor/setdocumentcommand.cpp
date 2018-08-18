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

#include "setdocumentcommand.h"
#include "data/properties/glsldocumentlistproperty.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"
#include "opengl/glslparser/glsldocument.h"

using namespace ysm;

SetDocumentCommand::SetDocumentCommand(GLSLDocument* document, IBlock* block, bool attach) :
	_document(document),
	_block(block),
	_attach(attach)
{
	//Retain the document to ensure it stays alive as long as the command does.
	_document->retain();

	//Document and property change.
	addChangedObject(_block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code), IChangeable::Change);
	addChangedObject(_document, IChangeable::Change);
}

SetDocumentCommand::~SetDocumentCommand() { _document->release(); }

void SetDocumentCommand::setDocument(bool attach)
{
	//Retrieve the block's currect documents.
	GLSLDocumentListProperty* listProperty = _block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code);
	GLSLDocumentList documentList = listProperty->getValue();

	//Add the tab's document.
	if(attach)
		documentList.addDocument(_document);
	else
		documentList.removeDocument(_document);

	//Set the property's new value.
	listProperty->setValue(documentList);

	//Update the document.
	_document->getPipelineAdapter()->setBlock(attach ? _block : NULL);
	_document->synchronizeDocument();
}

bool SetDocumentCommand::execute() { setDocument(_attach); return true; }
bool SetDocumentCommand::undo() { setDocument(!_attach); return true; }
