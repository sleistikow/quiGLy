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

#include "rendercommand.h"
#include "data/pipeline/visitors/pipelinevisitor.h"
#include "data/common/dataexceptions.h"

#include <stdexcept>

namespace ysm
{
	RenderCommand::RenderCommand(Pipeline* parent, const RenderCommandType type, const QString& name) : PipelineItem(parent, name), _type{type}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};

		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};
	}

	RenderCommand::~RenderCommand()
	{

	}

	void RenderCommand::initRenderCommand()
	{
		PipelineItem::initPipelineItem();
	}

	bool RenderCommand::takeVisitor(PipelineVisitor* visitor)
	{
		if (!visitor)
			throw std::invalid_argument{"visitor may not be null"};

		return visitor->renderCommandVisited(this);
	}

	RenderCommandType RenderCommand::getCommand() const
	{
		return _type;
	}

	Block* RenderCommand::getAssignedBlock() const
	{
		return _assignedBlock;
	}

	void RenderCommand::assignBlock(IBlock* block)
	{
		Block* blockEx = dynamic_cast<Block*>(block);

		disconnect(this, SLOT(onBlockRemoved()));

		if (blockEx)
		{
			QString denialReason;

			if (!canAcceptBlockAssignment(blockEx, denialReason))
				throw RenderCommandException{this, denialReason};

			// Get notified about block removals
			connect(blockEx, SIGNAL(blockRemoved()), this, SLOT(onBlockRemoved()));
		}

		_assignedBlock = blockEx;
	}

	bool RenderCommand::canAcceptBlockAssignment(IBlock* block, QString& denialReason)
	{
		Q_UNUSED(block);
		Q_UNUSED(denialReason);

		return true;
	}

	void RenderCommand::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		PipelineItem::serialize(xmlElement, ctx);

		xmlElement->setAttribute("type", static_cast<int>(_type));

		if (_assignedBlock)
			xmlElement->setAttribute("block", ctx->getObjectID(_assignedBlock));
		else
			xmlElement->setAttribute("block", 0);
	}

	void RenderCommand::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		PipelineItem::deserialize(xmlElement, ctx);

		// Blocks are deserialized before render commands, so they already exist at this point
		unsigned int blockID = xmlElement->attribute("block").toUInt();
		Block* block = dynamic_cast<Block*>(ctx->findObject(blockID));

		if (!block && blockID > 0)
		{
			QString msg = QString("Block #%1 for the render command doesn't exist").arg(blockID);
			ctx->addMessage(msg);

			throw SerializationException{"The render command block doesn't exist"};
		}

		_assignedBlock = block;
	}

	void RenderCommand::onBlockRemoved()
	{
		// Our assigned block has been removed, so unassign it
		_assignedBlock = nullptr;
	}
}
