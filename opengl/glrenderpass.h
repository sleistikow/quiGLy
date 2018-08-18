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

#ifndef GLRENDERPASS_H
#define GLRENDERPASS_H

#include <QList>
#include <QSet>

#include "data/blocks/blocktype.h"
#include "data/blocks/porttype.h"

namespace ysm
{

	class IBlock;
	class IConnection;
	class IRenderCommand;

	/**
	 * @brief The GLRenderPass class represents a render pass inside a GLRenderView rendering process.
	 * A Pass is a representation of a specific Pipeline inside our Pipeline-Editor which is bound
	 * to one GL-context.
	 * That means, if a rendering process uses multiple shaders, therefore contains multiple pipeline-setups,
	 * the related GLRenderPassSet contains that much GLRenderPasses.
	 */
	class GLRenderPass
	{
	private:

		static bool isUniqueBlockType(BlockType type);
		static QList<BlockType> getRequiredBlockTypes();

	public:
		/**
		 * @brief getBlocksByType	Returns all Blocks which matches the given type.
		 * @param type				BlockType to search matches for
		 * @return					List of all found Blocks
		 */
		QList<IBlock*> getBlocksByType(BlockType type) const;

		/**
		 * @brief getUniqueBlock		Returns the only instance of the given type.
		 * @param type					BlockType to search a match for
		 * @return						The only instance of the given type
		 */
		IBlock* getUniqueBlock(BlockType type) const;

		/// @brief Returns the IndexBufferObject used within this pass, if any
		IBlock* getIndexBufferObjectBlock() const;

		/**
		 * @brief isValid Determines, whether this Renderpass can be rendered
		 * @return true, if valid, false otherwise
		 */
		bool isValid() const;

		/**
		 * @brief getInvolvedBlocks Returns a list of blocks which are part of this pass
		 * @return				  the involved blocks
		 */
		const QSet<IBlock*>& getInvolvedBlocks() const;

		/**
		 * @brief addInvolvedBlock	Adds a block which should be part of this pass
		 * @param block				Block to be added
		 */
		void addInvolvedBlock(IBlock* block);

		/**
		 * @brief getOutConnections	Returns a list of outgoing connections from the given block, being part of this pass.
		 * @return				  the involved connections
		 */
		QList<IConnection*> getOutConnections(IBlock* block, PortType type = PortType::Undefined) const;

		/**
		 * @brief getInvolvedRenderCommands Returns a list of rendercommands which are part of this pass
		 * @return							the involved rendercommands
		 */
		const QSet<IRenderCommand*>& getInvolvedRenderCommands() const;

		/**
		 * @brief addInvolvedRenderCommand	Adds a rendercommand which should be part of this pass
		 * @param rendercommand				RenderCommand to be added
		 */
		void addInvolvedRenderCommand(IRenderCommand* rendercommand);

	private:

		int _id;											/*!< The id of this pass, being unique within its parent. */

		QSet<IBlock*> _involvedBlocks;						/*!< Holds all blocks involved in this pass. */
		QSet<IRenderCommand*> _involvedRenderCommands;		/*!< Holds all rendercommands involved in this pass. */
	};
}


#endif // GLRENDERPASS_H
