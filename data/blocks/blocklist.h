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

#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include "data/pipeline/pipelineitemlist.h"
#include "data/iblock.h"
#include "blocktype.h"
#include "data/common/dataexceptions.h"

namespace ysm
{
	class Block;

	class BlockList : public PipelineItemList<Block>
	{
	public:
		explicit BlockList(Pipeline* parent);

	public:
		/**
		 * @brief Adds an existing block to the list
		 */
		void addBlock(Block* block);

		/**
		 * @brief Creates a new block of class @p T
		 * If no block could be created, an exception is thrown.
		 * @arg T The block class
		 */
		template<typename T>
		T* newBlock();

		/**
		 * @brief Creates a new block of type @p type and adds it to the list
		 * If no block could be created, an exception is thrown.
		 */
		Block* newBlock(const BlockType type);

		/**
		 * @brief Finds all blocks of type @p type
		 */
		QVector<Block*> findBlocks(const BlockType type) const;

	public:
		void remove(const int i, bool deleteObj = true) override;
		bool remove(const Block* block, bool deleteObj = true) override;

	public: // ISerializable
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;
	};

	// Template member functions

	template<typename T>
	T* BlockList::newBlock()
	{
		static_assert(std::is_base_of<Block, T>::value, "T must be derived from Block");

		T* block = new T(_pipeline);

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
}

#endif
