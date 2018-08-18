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
#include "rendercommandlist.h"
#include "data/common/dataexceptions.h"
#include "data/pipeline/pipelinemanager.h"

namespace ysm
{
	RenderCommandList::RenderCommandList(Pipeline* parent) : PipelineItemList{parent}
	{
		_xmlElementName = "RenderCommand";
	}

	void RenderCommandList::addRenderCommand(const RenderCommand* cmd)
	{
		if (!cmd)
			throw std::invalid_argument{"cmd may not be null"};

		append(cmd);
	}

	void RenderCommandList::insertRenderCommand(const RenderCommand* cmd, const int idx)
	{
		if (!cmd)
			throw std::invalid_argument{"cmd may not be null"};

		insert(idx, cmd);
	}

	RenderCommand* RenderCommandList::newRenderCommand(const RenderCommandType type, const int idx)
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		// Create a new block by type by using the object factory
		RenderCommand* cmd = _pipeline->getManager()->getObjectFactory()->createRenderCommand(type, _pipeline);

		if (!cmd)
			throw std::runtime_error{"Out of memory"};

		try
		{
			cmd->initRenderCommand();
		}
		catch (...)
		{
			delete cmd;
			throw;
		}

		if (idx == -1)
			append(cmd);
		else
			insert(idx, cmd);

		return cmd;
	}

	QVector<RenderCommand*> RenderCommandList::findRenderCommands(const RenderCommandType type) const
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		return objects([type](const RenderCommand* cmd) { return (cmd->getCommand() == type); });
	}

	void RenderCommandList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// Keep existing data if in import mode
		if (!ctx->isImportMode())
			clear();

		for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "type"))
				continue;

			RenderCommandType type = static_cast<RenderCommandType>(elem.attribute("type").toInt());
			PipelineItemID id = elem.attribute("id").toInt();
			RenderCommand* cmd = newRenderCommand(type);

			id = ctx->setObjectID(cmd, id);
			cmd->setID(id);

			// Prevent memory leaks in case of deserialization failure
			try
			{
				cmd->deserialize(&elem, ctx);
			}
			catch (...)
			{
				QString msg = QString("Failed to deserialize render command #%1 of type %2").arg(id).arg(static_cast<int>(type));
				ctx->addMessage(msg);

				remove(cmd);				
			}
		}
	}
}
