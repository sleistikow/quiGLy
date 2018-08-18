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

#include "savedocumentcommand.h"

using namespace ysm;

SaveDocumentCommand::SaveDocumentCommand(GLSLDocument* document) :
	_document(document),
	_isRedo(false)
{ addChangedObject(document, IChangeable::Change); }

bool SaveDocumentCommand::execute()
{
	//Check if command is executed first time, otherwise restore the content.
	QString content = _document->getSavedContent();
	if(_isRedo)
		_document->setPlainText(_content);

	//Store the old content.
	_content = content;
	_isRedo = true;

	//Synchronize the document first, then save the synchronized content.
	_document->synchronizeDocument();
	_document->saveDocument();

	//Always succeeds.
	return true;
}

bool SaveDocumentCommand::undo()
{
	//Restore the old content, save current content for redo.
	QString content = _document->toPlainText();
	_document->setPlainText(_content);
	_content = content;

	//Synchronize the document first, then save the synchronized content.
	_document->synchronizeDocument();
	_document->saveDocument();

	//Always succeeds.
	return true;
}
