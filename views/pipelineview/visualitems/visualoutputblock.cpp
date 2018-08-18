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

#include "visualoutputblock.h"

#include <QMessageBox>

using namespace ysm;

VisualOutputBlock::VisualOutputBlock(IBlock* block, IView* parentView) :
	VisualBlock(block, parentView)
{
	_createCommandAction = new QAction(QIcon(":/tango/16x16/actions/insert-object.png"), tr("Add Clear"), this);
    connect(_createCommandAction, SIGNAL(triggered(bool)), this, SLOT(createClearCommand()));

}

void VisualOutputBlock::createClearCommand()
{
	//Just execute the appropriate command.
	executeCommand(new AddRenderCommandCommand(getBlock(), RenderCommandType::Clear));
}

QList<QAction*> VisualOutputBlock::getContextActions() const
{
    //Get parent actions.
    QList<QAction*> baseList = VisualBlock::getContextActions();

    //Add seperator and editor action.
	baseList.prepend(NULL);
	baseList.prepend(_createCommandAction);

	//Iterate over all render commands, there is no other way to find assigned blocks.
	_createCommandAction->setDisabled(false);
	for(IRenderCommand* command : getBlock()->getPipeline()->getRenderCommands())
		if(command->getAssignedBlock() == getBlock() && command->getCommand() == RenderCommandType::Clear)
		{
            _createCommandAction->setDisabled(true);
            break;
		}

    return baseList;
}

