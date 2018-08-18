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

#include "visualpipelineitemfactory.h"
#include "visualpipelineitem.h"

using namespace ysm;

VisualPipelineItemFactory::VisualPipelineItemFactory(QObject *parent) :
	QObject(parent)
{ }

VisualBlock* VisualPipelineItemFactory::createBlockItem(IBlock* block,
														IView* parentView) const
{
	//Make sure the block is specified.
	if(block)
	{
		//Check if there is a constructor available, and create the visual item.
		BlockType blockType = block->getType();
		if(_blockConstructors.contains(blockType))
			return _blockConstructors[blockType](block, parentView);
	}

	return NULL;
}

VisualRenderCommand* VisualPipelineItemFactory::createRenderCommandItem(IRenderCommand* renderCommand,
																		IView* parentView) const
{
	//Make sure the command is specified.
	if(renderCommand)
	{
		//Check if there is a constructor available, and create the visual item.
		RenderCommandType renderCommandType = renderCommand->getCommand();
		if(_renderCommandConstructors.contains(renderCommandType))
			return _renderCommandConstructors[renderCommandType](renderCommand, parentView);
	}

	return NULL;
}

void VisualPipelineItemFactory::setBlockConstructor(const BlockType itemType,
													VisualBlock* (* constructor)(IBlock*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_blockConstructors[itemType] = constructor;
	else if(_blockConstructors.contains(itemType))
		_blockConstructors.remove(itemType);
}

void VisualPipelineItemFactory::setRenderCommandConstructor(const RenderCommandType itemType,
															VisualRenderCommand* (*constructor)(IRenderCommand*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_renderCommandConstructors[itemType] = constructor;
	else if(_renderCommandConstructors.contains(itemType))
		_renderCommandConstructors.remove(itemType);
}
