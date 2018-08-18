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

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "data/pipeline/pipelineitem.h"
#include "data/irendercommand.h"
#include "data/blocks/block.h"

namespace ysm
{
	/**
	 * @brief Represents a render command in a pipeline
	 */
	class RenderCommand : public PipelineItem<IRenderCommand>
	{
		Q_OBJECT

	public:
		static const RenderCommandType command_type{RenderCommandType::Noop};

	public:
		// Construction
		explicit RenderCommand(Pipeline* parent, const RenderCommandType type, const QString& name = "");
		virtual ~RenderCommand();

	public:
		// Initialization
		/**
		 * @brief Initializes this command
		 * If something goes wrong, an exception is thrown.
		 */
		virtual void initRenderCommand();

		// Visitors
		/**
		 * @brief Takes a visitor and iterates through all pipelines, blocks, ports and connections
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public: // IRenderCommand
		// General attributes
		RenderCommandType getCommand() const override;

		// Block assignment
		Block* getAssignedBlock() const override;
		void assignBlock(IBlock* block) override;
		bool canAcceptBlockAssignment(IBlock* block, QString& denialReason) override;

	public: // ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	protected:
		RenderCommandType _type{RenderCommandType::Noop};

		Block* _assignedBlock{nullptr};

	private slots:
		void onBlockRemoved();
	};
}

#endif
