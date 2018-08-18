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

#include "glrenderpass.h"
#include "data/iblock.h"
#include "data/iport.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"

namespace ysm
{

bool GLRenderPass::isUniqueBlockType(BlockType type)
{
	// TODO: add missing

	switch(type)
	{
	case BlockType::FragmentTests:
	case BlockType::Rasterization:
	case BlockType::TessellationPrimitiveGenerator:
	case BlockType::VertexArrayObject:
	case BlockType::VertexPuller:
	case BlockType::FrameBufferObject:
	case BlockType::CameraControl:
	case BlockType::TransformFeedback:
		return true;
	default:
		return false;
	}
}

QList<BlockType> GLRenderPass::getRequiredBlockTypes()
{
	static QList<BlockType> blockTypes;
	if(blockTypes.isEmpty())
	{
		blockTypes << BlockType::VertexArrayObject
				   << BlockType::VertexPuller
				   << BlockType::Shader_Vertex;
	}

	return blockTypes;
}

QList<IBlock*> GLRenderPass::getBlocksByType(BlockType type) const
{
	QList<IBlock*> result;

	for(IBlock* block : _involvedBlocks)
		if(block->getType() == type)
			result.append(block);

	return result;
}

IBlock* GLRenderPass::getUniqueBlock(BlockType type) const
{
	if(!isUniqueBlockType(type))
		return nullptr;

	QList<IBlock*> result = getBlocksByType(type);
	if(result.isEmpty())
		return nullptr;

	return result.first();
}

IBlock* GLRenderPass::getIndexBufferObjectBlock() const
{
	IBlock* vertexPuller = getUniqueBlock(BlockType::VertexPuller);
	QVector<IConnection*> iboCon = vertexPuller->getPort(PortType::VertexPuller_IndexList)->getInConnections();
	if(!iboCon.isEmpty())
	{
		IBlock* ibo = iboCon[0]->getSource();
		if(_involvedBlocks.contains(ibo))
			return ibo;
	}

	return nullptr;
}

bool GLRenderPass::isValid() const
{
	for(BlockType type : getRequiredBlockTypes())
		if(getBlocksByType(type).isEmpty())
			return false;

	return true;
}

const QSet<IBlock*>& GLRenderPass::getInvolvedBlocks() const
{
	return _involvedBlocks;
}

void GLRenderPass::addInvolvedBlock(IBlock* block)
{
	if(!block)
		throw std::runtime_error("block may not be null");

	IBlock* existing = getUniqueBlock(block->getType());
	if(existing && existing != block)
		throw std::runtime_error("tried to add a unique block again");

	// Finally, append the block
	_involvedBlocks.insert(block);
}

QList<IConnection*> GLRenderPass::getOutConnections(IBlock* block, PortType type) const
{
	QList<IConnection*> connections;
	for(IConnection* connection : (type == PortType::Undefined) ?
		block->getOutConnections() : block->getPort(type)->getOutConnections())
	{
		if(_involvedBlocks.contains(connection->getDest()))
			connections.append(connection);
	}

	return connections;
}

const QSet<IRenderCommand*>& GLRenderPass::getInvolvedRenderCommands() const
{
	return _involvedRenderCommands;
}

void GLRenderPass::addInvolvedRenderCommand(IRenderCommand* rendercommand)
{
	if(!rendercommand)
		throw std::runtime_error("rendercommand may not be null");

	// Finally, append the rendercommand
	_involvedRenderCommands.insert(rendercommand);
}

} // namespace ysm
