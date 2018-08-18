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

#include "deleteconnectionscommand.h"
#include "data/common/dataexceptions.h"

#include <QDebug>

using namespace ysm;


DeleteConnectionsCommand::DeleteConnectionsCommand(IPipeline* pipeline, IConnection* connection) :
	_pipeline(pipeline)
{
	//Add the connection to the connection list
	_connections.append(connection);

	//Mark the connections for delete operation.
	addChangedObject(connection, IChangeable::Remove);
}

DeleteConnectionsCommand::DeleteConnectionsCommand(IPipeline* pipeline, QList<IConnection*> connections) :
	_connections(connections),
	_pipeline(pipeline)
{
	//Mark the connections for delete operation.
	foreach(IConnection* connection, _connections)
		addChangedObject(connection, IChangeable::Remove);
}

DeleteConnectionsCommand::DeleteConnectionsCommand(IPipeline* pipeline, QVector<IConnection*> connections) :
	_pipeline(pipeline)
{
	//Store the connections.
	foreach(IConnection* connection, connections)
		_connections.append(connection);

	//Mark the connections for delete operation.
	foreach(IConnection* connection, _connections)
		addChangedObject(connection, IChangeable::Remove);
}

bool DeleteConnectionsCommand::execute()
{
	//Reset the deleted objects.
	resetChangedObjects(IChangeable::Remove);

	//Catch any pipeline failure.
	try
	{
		//Iterate over all connections to backup their properties and then delete them.
		//Backup the properties before deletion, because the blocks will not be backed up later.
		foreach(IConnection* connection, _connections)
		{
			backupProperties(connection);
			_pipeline->deleteConnection(connection, true);
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

bool DeleteConnectionsCommand::undo()
{
	//Try to re-add the connections.
	try
	{
		//Add to pipeline and mark as removed. Because this is an undo, the queue will assume this object as added
		//and forward the appropriate signal.
		foreach(IConnection* connection, _connections)
		{
			_pipeline->addConnection(connection);
			addChangedObject(connection, IChangeable::Remove);
		}
	}

	//Ensure that command fails on exception.
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//Restore the backed up properties.
	restoreProperties();

	//No exceptions, success.
	return true;
}
