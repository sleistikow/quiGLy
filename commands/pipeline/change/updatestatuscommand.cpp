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

#include "updatestatuscommand.h"
#include "data/properties/property.h"

using namespace ysm;

UpdateStatusCommand::UpdateStatusCommand(IPipelineItem* pipelineItem, PipelineItemStatus status, QString message) :
	_pipelineItem(pipelineItem),
	_status(status),
	_message(message)
{ }

bool UpdateStatusCommand::execute()
{
	//Set the new status.
	_pipelineItem->setStatus(_status, _message);
	return true;
}

bool UpdateStatusCommand::undo() { return false; }
bool UpdateStatusCommand::isUndoable() const { return false; }
bool UpdateStatusCommand::didChangeRendering() const { return false; }

QList<IChangeable*> UpdateStatusCommand::getChangedObjects(IChangeable::Operation operation)
{
	//Pipeline item itself is changed, as well as the status property.
	QList<IChangeable*> changedObjects = UICommand::getChangedObjects(operation);
	if(operation == IChangeable::Change)
		changedObjects << _pipelineItem << _pipelineItem->getProperty<StringProperty>(PropertyID::MessageLog);

	//Return list of changed objects.
	return changedObjects;
}
