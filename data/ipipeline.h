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

#ifndef IPIPELINE_H
#define IPIPELINE_H

#include <QVector>
#include "iserializable.h"
#include "commands/ichangeable.h"
#include "pipeline/pipelineitemstatus.h"
#include "blocks/blocktype.h"
#include "rendercommands/rendercommandtype.h"

namespace ysm
{
	class IBlock;
	class IPort;
	class IConnection;
	class IRenderCommand;

	/// @brief Interface for pipelines.
	class IPipeline : public ISerializable, public IChangeable
	{
	public:

		/// @brief Destruct instance.
		virtual ~IPipeline() { }

	public:

		/**
		 * @brief Gets the set OpenGL version for this pipeline.
		 * @return The OpenGL version as integer (e.g. 330 for 3.3).
		 */
		virtual unsigned int getOpenGLVersion() const = 0;

		/**
		 * @brief Sets the OpenGL version for this pipeline.
		 * @param version The OpenGL version as integer (e.g. 330 for 3.3).
		 */
		virtual void setOpenGLVersion(unsigned int version) = 0;

		/**
		 * @brief Validates the entire pipeline and updates the status of each pipeline item accordingly.
		 * @return The overall status of the pipeline.
		 */
		virtual PipelineItemStatus validatePipeline() = 0;

		/// @brief Resets the status of all pipeline items except those overriding their status.
		virtual void resetPipelineStatus() = 0;

		/**
		 * @brief Retrieves all blocks contained in the pipeline.
		 * @return List of all pipeline blocks.
		 */
		virtual QVector<IBlock*> getBlocks() const = 0;

		/**
		 * @brief Retrieves all blocks of the given type in the pipeline.
		 * @param type The block type.
		 * @return List of pipeline blocks.
		 */
		virtual QVector<IBlock*> getBlocks(const BlockType type) const = 0;

		/**
		 * @brief Adds a new block to the pipeline.
		 * @param The block type.
		 * @return The block.
		 */
		virtual IBlock* addBlock(const BlockType type) = 0;

		/**
		 * @brief Adds an existing block to the pipeline.
		 * @param The block.
		 * @return The block.
		 */
		virtual IBlock* addBlock(IBlock* block) = 0;

		/**
		 * @brief Deletes the given block from the pipeline.
		 * @param block The block.
		 * @param removeOnly If true, the item will not be deleted (only it's reference in the list).
		 */
		virtual void deleteBlock(const IBlock* block, bool removeOnly = false) = 0;

		/**
		 * @brief Connects two ports by a connection.
		 * @param src The source port.
		 * @param dest The destination port.
		 * @return The newly created connection
		 */
		virtual IConnection* addConnection(IPort* src, IPort* dest) = 0;

		/**
		 * @brief Adds an existing connection.
		 * @param connection The connection.
		 * @return The connection.
		 */
		virtual IConnection* addConnection(IConnection* connection) = 0;

		/**
		 * @brief Deletes an existing connection
		 * @param con The connection.
		 * @param removeOnly If true, the item will not be deleted (only it's reference in the list).
		 */
		virtual void deleteConnection(IConnection* con, bool removeOnly = false) = 0;

		/**
		 * @brief Retrieves all render commands in the pipeline.
		 * @return List of all render commands.
		 */
		virtual QVector<IRenderCommand*> getRenderCommands() const = 0;

		/**
		 * @brief Retrieves all render commands of the given type in the pipeline.
		 * @param type The command type.
		 * @return The render command.
		 */
		virtual QVector<IRenderCommand*> getRenderCommands(const RenderCommandType type) const = 0;

		/**
		 * @brief Adds a new render command to the pipeline.
		 * @param type The command type.
		 * @param idx The index where the new command should be added, -1 to append.
		 * @return The newly created render command.
		 */
		virtual IRenderCommand* addRenderCommand(const RenderCommandType type, int idx = -1) = 0;

		/**
		 * @brief Adds an existing render command to the pipeline.
		 * @param command The render command.
		 * @param idx The index where the new command should be added, -1 to append.
		 * @return The render command.
		 */
		virtual IRenderCommand* addRenderCommand(IRenderCommand* command, int idx = -1) = 0;

		/**
		 * @brief Deletes an existing render command.
		 * @param cmd The render command.
		 * @param removeOnly If true, the item will not be deleted (only it's reference in the list).
		 */
		virtual void deleteRenderCommand(const IRenderCommand* cmd, bool removeOnly = false) = 0;

		/// @brief Delete all render commands.
		virtual void clearRenderCommands() = 0;

	protected:

		/// @brief Initialize new instance.
		IPipeline() { }
	};
}

#endif
