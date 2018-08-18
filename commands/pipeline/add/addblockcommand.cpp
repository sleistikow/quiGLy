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

#include "addblockcommand.h"
#include "data/common/dataexceptions.h"

#include <QDebug>

using namespace ysm;

AddBlockCommand::AddBlockCommand(IPipeline* pipeline, const BlockType blockType) :
	_blockType(blockType),
	_createdBlock(NULL),
	_pipeline(pipeline)
{ }

bool AddBlockCommand::execute()
{
	//Try to create and store the block.
	try
	{
		if(_createdBlock)
			_pipeline->addBlock(_createdBlock);
		else
			_createdBlock = _pipeline->addBlock(_blockType);
		addChangedObject(_createdBlock, IChangeable::Add);
	}

	//Ensure that command fails on exception to avoid failing undo.
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//Backup properties on first execution.
	backupProperties(_pipeline);
	clearProperties(_pipeline);
	propertyBackupsDone();

	//No exceptions, success.
	return true;
}

bool AddBlockCommand::undo()
{
	//Remove the block.
	_pipeline->deleteBlock(_createdBlock, true);

	//Reset the added objects and restore properties.
	resetChangedObjects(IChangeable::Add);
	restoreProperties();

	//Always succeed.
	return true;
}

IBlock* AddBlockCommand::getBlock() const { return _createdBlock; }
