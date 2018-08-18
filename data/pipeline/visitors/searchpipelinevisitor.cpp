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

#include "searchpipelinevisitor.h"
#include "data/blocks/block.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"

namespace ysm
{
	SearchPipelineVisitor::SearchPipelineVisitor(PipelineItemID searchID) : _itemID{searchID}
	{

	}

	IPipelineItem* SearchPipelineVisitor::getFoundItem() const
	{
		return _foundItem;
	}

	bool SearchPipelineVisitor::blockVisited(Block* block)
	{
		if (block->getID() == _itemID)
		{
			_foundItem = block;
			return false;
		}

		return true;
	}

	bool SearchPipelineVisitor::renderCommandVisited(RenderCommand* command)
	{
		if (command->getID() == _itemID)
		{
			_foundItem = command;
			return false;
		}

		return true;
	}

	bool SearchPipelineVisitor::portVisited(Port* port)
	{
		if (port->getID() == _itemID)
		{
			_foundItem = port;
			return false;
		}

		return true;
	}

	bool SearchPipelineVisitor::connectionVisited(Connection* connection)
	{
		if (connection->getID() == _itemID)
		{
			_foundItem = connection;
			return false;
		}

		return true;
	}
}
