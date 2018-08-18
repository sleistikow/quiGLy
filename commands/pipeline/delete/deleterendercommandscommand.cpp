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

#include "deleterendercommandscommand.h"
#include "data/common/dataexceptions.h"
#include "data/irendercommand.h"

#include <QDebug>

using namespace ysm;

DeleteRenderCommandsCommand::DeleteRenderCommandsCommand(IPipeline* pipeline, IRenderCommand* command) :
	_pipeline(pipeline)
{
	//Add the command to the command list.
	_commands.append(command);

	//Mark the render commands for delete operation.
	addChangedObject(command, IChangeable::Remove);
}

DeleteRenderCommandsCommand::DeleteRenderCommandsCommand(IPipeline* pipeline, QList<IRenderCommand*> commands) :
	_commands(commands),
	_pipeline(pipeline)
{
	//Mark the render commands for delete operation.
	foreach(IRenderCommand* command, _commands)
		addChangedObject(command, IChangeable::Remove);
}

DeleteRenderCommandsCommand::DeleteRenderCommandsCommand(IPipeline* pipeline, QVector<IRenderCommand*> commands) :
	_pipeline(pipeline)
{
	//Store the connections.
	foreach(IRenderCommand* command, commands)
		_commands.append(command);

	//Mark the render commands for delete operation.
	foreach(IRenderCommand* command, _commands)
		addChangedObject(command, IChangeable::Remove);
}

bool DeleteRenderCommandsCommand::execute()
{
	//Reset the deleted objects.
	resetChangedObjects(IChangeable::Remove);

	//Catch any pipeline failure.
	try
	{
		//Iterate over all render commands to backup their properties and then delete them.
		//Backup the properties before deletion, because the blocks will not be backed up later.
		foreach(IRenderCommand* command, _commands)
		{
			backupProperties(command);
			_pipeline->deleteRenderCommand(command, true);
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

bool DeleteRenderCommandsCommand::undo()
{
	//Try to re-add the render commands.
	try
	{
		//Add to pipeline and mark as removed. Because this is an undo, the queue will assume this object as added
		//and forward the appropriate signal.
		foreach(IRenderCommand* command, _commands)
		{
			_pipeline->addRenderCommand(command);
			addChangedObject(command, IChangeable::Remove);
		}
	}

	//Ensure that command fails on exception.
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//Restore the backed up properties.
	restoreProperties();

	//No exceptions, success.
	return true;
}
