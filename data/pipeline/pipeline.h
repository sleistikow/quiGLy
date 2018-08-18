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

#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>

#include "data/ipipeline.h"
#include "data/iversionable.h"

namespace ysm
{
	class Port;
	class Connection;
	class BlockList;
	class RenderCommandList;
	class PipelineVisitor;
	class PipelineManager;

	/**
	 * @brief Represents a pipeline
	 */
	class Pipeline : public QObject, public IPipeline
	{
		Q_OBJECT

	public:
		// Construction
		explicit Pipeline(PipelineManager* manager);
		virtual ~Pipeline();

	public:
		// Object access
		/**
		 * @brief Access to the pipeline's manager
		 */
		PipelineManager* getManager();

		/**
		 * @brief Access to the pipeline's block list
		 */
		BlockList* getBlockList();

		/**
		 * @brief Access to the pipeline's render command list
		 */
		RenderCommandList* getRenderCommandList();

		// Visitors
		/**
		 * @brief Takes a visitor and iterates through all pipelines, blocks, ports and connections
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public: // IPipeline
		// General attributes
		unsigned int getOpenGLVersion() const override;
		void setOpenGLVersion(unsigned int version) override;

		// Validation
		PipelineItemStatus validatePipeline() override;
		void resetPipelineStatus() override;

		// Blocks
		QVector<IBlock*> getBlocks() const override;
		QVector<IBlock*> getBlocks(const BlockType type) const override;
		IBlock* addBlock(const BlockType type) override;
		IBlock* addBlock(IBlock* block) override;
		void deleteBlock(const IBlock* block, bool removeOnly = false) override;

		// Connections
		IConnection* addConnection(IPort* src, IPort* dest) override;
		IConnection* addConnection(IConnection* connection) override;
		void deleteConnection(IConnection* con, bool removeOnly = false) override;

		// Render commands
		QVector<IRenderCommand*> getRenderCommands() const override;
		QVector<IRenderCommand*> getRenderCommands(const RenderCommandType type) const override;
		IRenderCommand* addRenderCommand(const RenderCommandType type, int idx = -1) override;
		IRenderCommand* addRenderCommand(IRenderCommand* command, int idx) override;
		void deleteRenderCommand(const IRenderCommand* cmd, bool removeOnly = false) override;
		void clearRenderCommands() override;

	public:
		// ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		unsigned int _openGLVersion{DEFAULT_MINIMUM_VERSION};

		BlockList* _blocks{nullptr};
		RenderCommandList* _renderCommands{nullptr};

		PipelineManager* _manager{nullptr};
	};
}

#endif
