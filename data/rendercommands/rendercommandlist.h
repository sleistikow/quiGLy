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

#ifndef RENDERCOMMANDLIST_H
#define RENDERCOMMANDLIST_H

#include "data/pipeline/pipelineitemlist.h"

namespace ysm
{
	class Pipeline;
	class RenderCommand;

	class RenderCommandList : public PipelineItemList<RenderCommand>
	{
	public:
		// Construction
		explicit RenderCommandList(Pipeline* parent);

	public:
		/**
		 * @brief Adds an existing command to the list
		 */
		void addRenderCommand(const RenderCommand* cmd);

		/**
		 * @brief Inserts an existing command into the list at index @p idx
		 */
		void insertRenderCommand(const RenderCommand* cmd, const int idx);

		/**
		 * @brief Creates a new render command of class @p T
		 * If no render command could be created, an exception is thrown.
		 * @arg T The render command class
		 */
		template<typename T>
		T* newRenderCommand(const int idx = -1);

		/**
		 * @brief Creates a new render command of type @p type and adds it to the list
		 * If no command could be created, an exception is thrown.
		 * @return The newly created render command
		 */
		RenderCommand* newRenderCommand(const RenderCommandType type, const int idx = -1);

		/**
		 * @brief Finds all render commands of type @p type
		 */
		QVector<RenderCommand*> findRenderCommands(const RenderCommandType type) const;

	public: // ISerializable
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;
	};

	// Template member functions

	template<typename T>
	T* RenderCommandList::newRenderCommand(const int idx)
	{
		static_assert(std::is_base_of<RenderCommand, T>::value, "T must be derived from RenderCommand");

		T* cmd = new T(_pipeline);

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
}

#endif
