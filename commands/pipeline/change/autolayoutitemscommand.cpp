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

#include "autolayoutitemscommand.h"
#include "views/pipelineview/pipelinescenelayouter.h"

using namespace ysm;

AutoLayoutItemsCommand::AutoLayoutItemsCommand(IPipeline* pipeline, PipelineSceneLayouter* layouter) :
	_pipeline(pipeline),
	_layouter(layouter)
{
	//Calculate a new auto layout.
	_layouter->updateAutoLayout(_pipeline);

	//Layout all pipeline blocks.
	foreach(IBlock* block, _pipeline->getBlocks())
	{
		//Store the old and new position.
		_oldPositions[block] = _layouter->getItemPosition(block, QPointF());
		_newPositions[block] = _layouter->getAutoLayoutItemPosition(block);
	}

	//Layout all pipeline commands.
	foreach(IRenderCommand* renderCommand, _pipeline->getRenderCommands())
	{
		//Store the old and new position.
		_oldPositions[renderCommand] = _layouter->getItemPosition(renderCommand, QPointF());
		_newPositions[renderCommand] = _layouter->getAutoLayoutItemPosition(renderCommand);
	}
}

bool AutoLayoutItemsCommand::execute()
{
	//Iterate over all items and set their new position.
	foreach(IPipelineItem* pipelineItem, _newPositions.keys())
		_layouter->setItemPosition(pipelineItem, _newPositions[pipelineItem]);

	//Always succeeds.
	return true;
}

bool AutoLayoutItemsCommand::undo()
{
	//Iterate over all items and set their old position.
	foreach(IPipelineItem* pipelineItem, _oldPositions.keys())
		_layouter->setItemPosition(pipelineItem, _oldPositions[pipelineItem]);

	//Always succeeds.
	return true;
}

bool AutoLayoutItemsCommand::didChangeRendering() const { return false; }
