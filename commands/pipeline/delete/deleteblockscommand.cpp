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

#include "deleteblockscommand.h"
#include "deleterendercommandscommand.h"
#include "deleteconnectionscommand.h"

#include "data/iproperty.h"
#include "data/irendercommand.h"
#include "data/common/dataexceptions.h"

#include "commands/iuicommandqueue.h"

#include <QDebug>

using namespace ysm;

DeleteBlocksCommand::DeleteBlocksCommand(IPipeline* pipeline, IBlock* block) :
	_pipeline(pipeline)
{
	//Add the block to the block list.
	_blocks.append(block);

	//Mark the blocks for delete operation.
	addChangedObject(block, IChangeable::Remove);
}

DeleteBlocksCommand::DeleteBlocksCommand(IPipeline* pipeline, QList<IBlock*> blocks) :
	  _blocks(blocks),
	  _pipeline(pipeline)
{
	//Mark the blocks for delete operation.
	foreach(IBlock* block, _blocks)
		addChangedObject(block, IChangeable::Remove);
}

DeleteBlocksCommand::DeleteBlocksCommand(IPipeline* pipeline, QVector<IBlock*> blocks) :
	  _pipeline(pipeline)
{
	//Store the blocks.
	foreach(IBlock* block, blocks)
		_blocks.append(block);

	//Mark the blocks for delete operation.
	foreach(IBlock* block, _blocks)
		addChangedObject(block, IChangeable::Remove);
}

bool DeleteBlocksCommand::executeImplicit(IUICommandQueue* commandQueue)
{
	//Append additional commands to delete all dependent objects.
	foreach(IBlock* block, _blocks)
	{
		//Delete the block's render commands.
		foreach(IRenderCommand* renderCommand, _pipeline->getRenderCommands())
			if(renderCommand->getAssignedBlock() == block)
				commandQueue->execute(new DeleteRenderCommandsCommand(_pipeline, renderCommand));

		//Delete the block's incoming connections.
		commandQueue->execute(new DeleteConnectionsCommand(_pipeline, block->getInConnections()));
		commandQueue->execute(new DeleteConnectionsCommand(_pipeline, block->getOutConnections()));
	}

	//Execution successful.
	return true;
}

bool DeleteBlocksCommand::execute()
{
	//Reset the deleted objects.
	resetChangedObjects(IChangeable::Remove);

	//Catch any pipeline failure.
	try
	{
		//Iterate over all blocks and delete them.
		//Backup the properties before deletion, because the blocks will not be backed up later.
		foreach(IBlock* block, _blocks)
		{
			backupProperties(block);
			_pipeline->deleteBlock(block, true);
		}
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

bool DeleteBlocksCommand::undo()
{
	//Try to re-add the blocks.
	try
	{
		//Add to pipeline and mark as removed. Because this is an undo, the queue will assume this object as added
		//and forward the appropriate signal.
		foreach(IBlock* block, _blocks)
		{
			_pipeline->addBlock(block);
			addChangedObject(block, IChangeable::Remove);
		}
	}

	//Ensure that command fails on exception.
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//Restore the backed up properties.
	restoreProperties();

	//No exceptions, success.
	return true;
}
