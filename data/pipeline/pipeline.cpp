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

#include "pipeline.h"
#include "data/blocks/block.h"
#include "data/blocks/blocklist.h"
#include "data/blocks/port.h"
#include "data/blocks/portlist.h"
#include "data/blocks/connection.h"
#include "data/blocks/connectionlist.h"
#include "data/rendercommands/rendercommand.h"
#include "data/rendercommands/rendercommandlist.h"
#include "visitors/validatepipelinevisitor.h"
#include "visitors/resetpipelinevisitor.h"

namespace ysm
{
	Pipeline::Pipeline(PipelineManager* manager) : _blocks{new BlockList{this}}, _renderCommands{new RenderCommandList{this}}, _manager{manager}
	{

	}

	Pipeline::~Pipeline()
	{
		delete _blocks;
		delete _renderCommands;
	}

	PipelineManager* Pipeline::getManager()
	{
		return _manager;
	}

	BlockList* Pipeline::getBlockList()
	{
		return _blocks;
	}

	RenderCommandList* Pipeline::getRenderCommandList()
	{
		return _renderCommands;
	}

	bool Pipeline::takeVisitor(PipelineVisitor* visitor)
	{
		if (!visitor)
			throw std::invalid_argument{"visitor may not be null"};

		if (!visitor->pipelineVisited(this))
			return false;

		for (Block* block : *_blocks)
		{
			if (!block->takeVisitor(visitor))
				return false;
		}

		for (RenderCommand* command : *_renderCommands)
		{
			 if (!command->takeVisitor(visitor))
				 return false;
		}

		return true;
	}

	unsigned int Pipeline::getOpenGLVersion() const
	{
		return _openGLVersion;
	}

	void Pipeline::setOpenGLVersion(unsigned int version)
	{
		// We don't support versions lower than DEFAULT_MINIMUM_VERSION
		if (version < DEFAULT_MINIMUM_VERSION)
			version = DEFAULT_MINIMUM_VERSION;

		 _openGLVersion = version;
	}

	PipelineItemStatus Pipeline::validatePipeline()
	{
		ValidatePipelineVisitor visitor(this);

		takeVisitor(&visitor);
		return visitor.getStatus();
	}

	void Pipeline::resetPipelineStatus()
	{
		ResetPipelineVisitor visitor;
		takeVisitor(&visitor);
	}

	QVector<IBlock*> Pipeline::getBlocks() const
	{
		return _blocks->objects<IBlock>();
	}

	QVector<IBlock*> Pipeline::getBlocks(const BlockType type) const
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		return _blocks->objects<IBlock>([type](const Block* block) { return block->getType() == type; });
	}

	IBlock* Pipeline::addBlock(const BlockType type)
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		return _blocks->newBlock(type);
	}

	IBlock* Pipeline::addBlock(IBlock* block)
	{
		if (!block)
			throw std::invalid_argument{"block may not be null"};

		Block* blockEx = dynamic_cast<Block*>(block);

		if (!blockEx)
			throw std::runtime_error{"The block seems to be of an invalid object type"};

		_blocks->addBlock(blockEx);
		return block;
	}

	void Pipeline::deleteBlock(const IBlock* block, bool removeOnly)
	{
		if (!block)
			throw std::invalid_argument{"block may not be null"};

		const Block* blockX = dynamic_cast<const Block*>(block);

		if (!blockX)
			throw std::invalid_argument{"block is not an instance of Block"};

		_blocks->remove(blockX, !removeOnly);
	}

	IConnection* Pipeline::addConnection(IPort* src, IPort* dest)
	{
		if (!src)
			throw std::invalid_argument{"src may not be null"};

		if (!dest)
			throw std::invalid_argument{"dest may not be null"};

		Port* srcPort = dynamic_cast<Port*>(src);
		Port* destPort = dynamic_cast<Port*>(dest);

		if (!srcPort)
			throw std::invalid_argument{"src is not an instance of Port"};

		if (!destPort)
			throw std::invalid_argument{"dest is not an instance of Port"};

		return srcPort->connectTo(destPort);
	}

	IConnection* Pipeline::addConnection(IConnection* connection)
	{
		if (!connection)
			throw std::invalid_argument{"connection may not be null"};

		Connection* con = dynamic_cast<Connection*>(connection);

		if (!con)
			throw std::runtime_error{"The connection seems to be of an invalid object type"};

		// Check consistency first
		if (!con->getSource() || !con->getDest())
			throw std::runtime_error{"Trying to add an orphaned connection"};

		if (!_blocks->contains(con->getSource()))
			throw std::runtime_error{"The connection source doesn't exist"};

		if (!_blocks->contains(con->getDest()))
			throw std::runtime_error{"The connection destination doesn't exist"};

		// Add the connection to its source port's connection list
		con->getSourcePort()->getConnectionList()->addConnection(con);
		return con;
	}

	void Pipeline::deleteConnection(IConnection* con, bool removeOnly)
	{
		if (!con)
			throw std::invalid_argument{"con may not be null"};

		Connection* connection = dynamic_cast<Connection*>(con);

		if (!connection)
			throw std::invalid_argument{"con is not an instance of Connection"};

		connection->getSourcePort()->disconnectFrom(connection->getDestPort(), removeOnly);
	}

	QVector<IRenderCommand*> Pipeline::getRenderCommands() const
	{
		return _renderCommands->objects<IRenderCommand>();
	}

	QVector<IRenderCommand*> Pipeline::getRenderCommands(const RenderCommandType type) const
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		return _renderCommands->objects<IRenderCommand>([type](const RenderCommand* cmd) { return cmd->getCommand() == type; });
	}

	IRenderCommand* Pipeline::addRenderCommand(const RenderCommandType type, int idx)
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		return _renderCommands->newRenderCommand(type, idx);
	}

	IRenderCommand* Pipeline::addRenderCommand(IRenderCommand* command, int idx)
	{
		if (!command)
			throw std::invalid_argument{"command may not be null"};

		RenderCommand* cmd = dynamic_cast<RenderCommand*>(command);

		if (!cmd)
			throw std::runtime_error{"The render command seems to be of an invalid object type"};

		if (cmd->getAssignedBlock())
		{
			if (!_blocks->contains(cmd->getAssignedBlock()))
				throw std::runtime_error{"The render command's assigned block doesn't exist"};
		}

		if (idx != -1)
			_renderCommands->insertRenderCommand(cmd, idx);
		else
			_renderCommands->addRenderCommand(cmd);

		return cmd;
	}

	void Pipeline::deleteRenderCommand(const IRenderCommand* cmd, bool removeOnly)
	{
		if (!cmd)
			throw std::invalid_argument{"cmd may not be null"};

		const RenderCommand* command = dynamic_cast<const RenderCommand*>(cmd);

		if (!command)
			throw std::invalid_argument{"cmd is not an instance of RenderCommand"};

		_renderCommands->remove(command, !removeOnly);
	}

	void Pipeline::clearRenderCommands()
	{
		_renderCommands->clear();
	}

	void Pipeline::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		xmlElement->setAttribute("openGLVersion", _openGLVersion);

		QDomElement blocksElem = ctx->createElement("Blocks");

		xmlElement->appendChild(blocksElem);
		_blocks->serialize(&blocksElem, ctx);

		QDomElement cmdsElem = ctx->createElement("RenderCommands");

		xmlElement->appendChild(cmdsElem);
		_renderCommands->serialize(&cmdsElem, ctx);
	}

	void Pipeline::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		setOpenGLVersion(xmlElement->attribute("openGLVersion", QString::number(_openGLVersion)).toUInt());

		// Project wizard makes use of mixed elements container which contains both blocks and render commands
		QDomElement mixedElem = xmlElement->firstChildElement("Elements");
		if (!mixedElem.isNull())
		{
			_blocks->deserialize(&mixedElem, ctx);
			_renderCommands->deserialize(&mixedElem, ctx);
		}

		// If mixed elements container is not available, use default container
		else
		{
			QDomElement blocksElem = xmlElement->firstChildElement("Blocks");
			if (!blocksElem.isNull())
				_blocks->deserialize(&blocksElem, ctx);

			QDomElement cmdsElem = xmlElement->firstChildElement("RenderCommands");
			if (!cmdsElem.isNull())
				_renderCommands->deserialize(&cmdsElem, ctx);
		}
	}
}
