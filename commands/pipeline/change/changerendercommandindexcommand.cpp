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

#include "changerendercommandindexcommand.h"

#include "data/ipipeline.h"
#include "data/irendercommand.h"

using namespace ysm;

ChangeRenderCommandIndexCommand::ChangeRenderCommandIndexCommand(IPipeline* pipeline, int oldIndex, int newIndex) :
	_pipeline(pipeline),
	_oldIndex(oldIndex),
	_newIndex(newIndex)
{
	//Get the render command that will be changed.
	_renderCommand = _pipeline->getRenderCommands()[oldIndex];
}

bool ChangeRenderCommandIndexCommand::execute() { return move(_newIndex); }
bool ChangeRenderCommandIndexCommand::undo() { return move(_oldIndex); }

bool ChangeRenderCommandIndexCommand::move(int targetIndex)
{
	//Move the command.
	_pipeline->deleteRenderCommand(_renderCommand, true);
	_pipeline->addRenderCommand(_renderCommand, targetIndex);

	//Succeeded.
	return true;
}
