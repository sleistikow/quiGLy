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

#include "addconnectioncommand.h"
#include "data/common/dataexceptions.h"
#include "data/iconnection.h"

#include <QDebug>

using namespace ysm;

AddConnectionCommand::AddConnectionCommand(IPort* sourcePort, IPort* targetPort) :
	UIDataChangingCommand(),
	_createdConnection(NULL),
	_canExecute(false)
{
	//Ports are correctly specified.
	if(sourcePort->isOut() && targetPort->isIn())
	{
		_sourcePort = sourcePort;
		_targetPort = targetPort;
		_canExecute = true;
	}

	//Ports are reversed.
	if(sourcePort->isIn() && targetPort->isOut())
	{
		_targetPort = sourcePort;
		_sourcePort = targetPort;
		_canExecute = true;
	}

	//Ensure pipeline on both ports matches.
	if(sourcePort->getPipeline() != targetPort->getPipeline())
		_canExecute = false;

	//Store the pipeline.
	_pipeline = sourcePort->getPipeline();
}

bool AddConnectionCommand::execute()
{
	//Ensure command can be executed.
	if(!_canExecute) return false;

	//Try to create and store the connection.
	try
	{
		if(_createdConnection)
			_sourcePort->getPipeline()->addConnection(_createdConnection);
		else
			_createdConnection = _sourcePort->getPipeline()->addConnection(_sourcePort, _targetPort);
		addChangedObject(_createdConnection, IChangeable::Add);
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

bool AddConnectionCommand::undo()
{
	//Remove the connection.
	_sourcePort->getPipeline()->deleteConnection(_createdConnection, true);

	//Reset the added objects and restore properties.
	resetChangedObjects(IChangeable::Add);
	restoreProperties();

	//Always succeed.
	return true;
}

IConnection* AddConnectionCommand::getConnection() const { return _createdConnection; }
