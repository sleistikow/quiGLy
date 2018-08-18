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

#include "validatepipelinecommand.h"

#include "data/blocks/block.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"

using namespace ysm;

bool ValidatePipelineCommand::ListVisitor::blockVisited(Block* block)
{
	//Item and status property could change.
	_changedObjects.append(block);
	_changedObjects.append(block->getProperty<StringProperty>(PropertyID::MessageLog));

	//Keep iterating.
	return true;
}

bool ValidatePipelineCommand::ListVisitor::portVisited(Port* port)
{
	//Item and status property could change.
	_changedObjects.append(port);
	_changedObjects.append(port->getProperty<StringProperty>(PropertyID::MessageLog));

	//Keep iterating.
	return true;
}

bool ValidatePipelineCommand::ListVisitor::connectionVisited(Connection* connection)
{
	//Item and status property could change.
	_changedObjects.append(connection);
	_changedObjects.append(connection->getProperty<StringProperty>(PropertyID::MessageLog));

	//Keep iterating.
	return true;
}

bool ValidatePipelineCommand::ListVisitor::renderCommandVisited(RenderCommand* command)
{
	//Item and status property could change.
	_changedObjects.append(command);
	_changedObjects.append(command->getProperty<StringProperty>(PropertyID::MessageLog));

	//Keep iterating.
	return true;
}

ValidatePipelineCommand::ValidatePipelineCommand(IPipeline* pipeline, bool resetStatus) :
	_reset(resetStatus)
{
	//Pipeline needs to be casted to allow visitors.
	_pipeline = dynamic_cast<Pipeline*>(pipeline);
}

bool ValidatePipelineCommand::execute()
{
	//List all changed objects first.
	_pipeline->takeVisitor(&_listVisitor);

	//Execute the appropriate action.
	if(_reset)
		_pipeline->resetPipelineStatus();
	else
		_pipeline->validatePipeline();

	//Always succeeds.
	return true;
}

bool ValidatePipelineCommand::undo() { return false; }
bool ValidatePipelineCommand::isUndoable() const { return false; }
bool ValidatePipelineCommand::didChangeRendering() const { return false; }

QList<IChangeable*> ValidatePipelineCommand::getChangedObjects(IChangeable::Operation operation)
{
	//If change operation is queued, list all changed objects.
	QList<IChangeable*> changedObjects = UICommand::getChangedObjects(operation);
	if(operation == IChangeable::Change)
		changedObjects.append(_listVisitor._changedObjects);

	//Return list of changed objects.
	return changedObjects;
}
