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

#include "addrendercommandcommand.h"
#include "data/common/dataexceptions.h"

#include <QDebug>

using namespace ysm;

AddRenderCommandCommand::AddRenderCommandCommand(IBlock* targetBlock, const RenderCommandType commandType) :
	_commandType(commandType),
	_createdCommand(NULL),
	_targetBlock(targetBlock)
{ addChangedObject(_targetBlock, IChangeable::Change); }

bool AddRenderCommandCommand::execute()
{
	//Try to create and store the render command.
	try
	{
		if(_createdCommand)
			_targetBlock->getPipeline()->addRenderCommand(_createdCommand);
		else
			_createdCommand = _targetBlock->getPipeline()->addRenderCommand(_commandType);
		_createdCommand->assignBlock(_targetBlock);
		addChangedObject(_createdCommand, IChangeable::Add);
	}

	//Ensure that command fails on exception to avoid failing undo.
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//Backup properties on first execution.
	backupProperties(_targetBlock->getPipeline());
	clearProperties(_targetBlock->getPipeline());
	propertyBackupsDone();

	//No exceptions, success.
	return true;
}

bool AddRenderCommandCommand::undo()
{
	//Remove the command.
	_targetBlock->getPipeline()->deleteRenderCommand(_createdCommand, true);

	//Reset the added objects and restore properties.
	resetChangedObjects(IChangeable::Add);
	restoreProperties();

	//Always succeed.
	return true;
}

IRenderCommand* AddRenderCommandCommand::getRenderCommand() const { return _createdCommand; }
