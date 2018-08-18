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

#include "blocklist.h"
#include "block.h"
#include "port.h"
#include "portlist.h"

namespace ysm
{
	BlockList::BlockList(Pipeline* parent) : PipelineItemList{parent}
	{
		_xmlElementName = "Block";
	}

	void BlockList::addBlock(Block* block)
	{
		if (!block)
			throw std::invalid_argument{"block may not be null"};

		append(block);
	}

	Block* BlockList::newBlock(const BlockType type)
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		// Create a new block by type by using the object factory
		Block* block = _pipeline->getManager()->getObjectFactory()->createBlock(type, _pipeline);

		if (!block)
			throw std::runtime_error{"Out of memory"};

		try
		{
			block->initBlock();			
		}
		catch (...)
		{
			delete block;
			throw;
		}

		append(block);
		return block;
	}

	QVector<Block*> BlockList::findBlocks(const BlockType type) const
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		return objects([type](const Block* block) { return (block->getType() == type); });
	}

	void BlockList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// Keep existing data if in import mode
		if (!ctx->isImportMode())
			clear();

		for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "type" << "id"))
				continue;

			BlockType type = static_cast<BlockType>(elem.attribute("type").toInt());
			PipelineItemID id = elem.attribute("id").toInt();
			Block* block = newBlock(type);

			id = ctx->setObjectID(block, id);
			block->setID(id);

			// Prevent memory leaks in case of deserialization failure
			try
			{
				block->deserialize(&elem, ctx);
			}
			catch (...)
			{
				QString msg = QString("Failed to deserialize block #%1 of type %2").arg(id).arg(static_cast<int>(type));
				ctx->addMessage(msg);

				remove(block);
			}
		}
	}

	void BlockList::remove(const int i, bool deleteObj)
	{
		// Emit blockRemoved signal
		if (i >= 0 && i < _objects.size())
		{
			Block* block = _objects[i];
			emit block->blockRemoved();
		}

		PipelineItemList::remove(i, deleteObj);
	}

	bool BlockList::remove(const Block* block, bool deleteObj)
	{
		// Emit blockRemoved signal
		if (block)
		{
			Block* blockNC = const_cast<Block*>(block);
			emit blockNC->blockRemoved();
		}

		return PipelineItemList::remove(block, deleteObj);
	}
}
