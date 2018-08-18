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

#include "updatepropertiescommand.h"

using namespace ysm;

UpdatePropertiesCommand::UpdatePropertiesCommand(IPipelineItem* pipelineItem) :
	_pipelineItem(pipelineItem),
	_wasExecuted(false)
{ }

UpdatePropertiesCommand::~UpdatePropertiesCommand() { qDeleteAll(_commands); }

bool UpdatePropertiesCommand::execute()
{
	//Executed.
	_wasExecuted = true;

	//Execute all sub-commands.
	_pipelineItem->beginPropertyUpdate();
	foreach(IUpdatePropertyCommand* command, _commands)
		command->redo();
	_pipelineItem->endPropertyUpdate();

	//Backup properties on first execution.
	backupProperties(_pipelineItem->getPipeline());
	clearProperties(_pipelineItem->getPipeline());
	propertyBackupsDone();

	//TODO: Error checks.
	return true;
}

bool UpdatePropertiesCommand::undo()
{
	//Executed.
	_wasExecuted = true;

	//Restore the backed up properties.
	restoreProperties();

	//TODO: Error checks.
	return true;
}

