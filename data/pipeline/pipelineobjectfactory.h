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

#ifndef PIPELINEOBJECTFACTORY_H
#define PIPELINEOBJECTFACTORY_H

#include <QMap>
#include <functional>
#include "data/properties/propertyid.h"
#include "data/properties/propertytype.h"
#include "data/blocks/blocktype.h"
#include "data/rendercommands/rendercommandtype.h"

namespace ysm
{
	class IProperty;
	class Block;
	class RenderCommand;
	class Pipeline;

	/**
	 * @brief Factory class to create various objects by type
	 */
	class PipelineObjectFactory
	{
	public:
		using property_creator_type = std::function<IProperty*(PropertyID, const QString&, bool)>;
		using block_creator_type = std::function<Block*(Pipeline*)>;
		using rendercommand_creator_type = std::function<RenderCommand*(Pipeline*)>;

	public:
		explicit PipelineObjectFactory();

	public:
		/**
		 * @brief Registers a new property type
		 * @param creator The creator function
		 */
		void registerPropertyType(const PropertyType type, const property_creator_type creator);

		/**
		 * @brief Unregisters a property type
		 */
		void unregisterPropertyType(const PropertyType type);

		/**
		 * @brief Creates a new property
		 * If no property could be created, an exception is thrown.
		 * @return The newly created property
		 */
		IProperty* createProperty(const PropertyType type, const PropertyID id = PropertyID::None, const QString& name = "", const bool isReadOnly = false) const;

	public:
		/**
		 * @brief Registers a new block type
		 * @param creator The creator function
		 */
		void registerBlockType(const BlockType type, const block_creator_type creator);

		/**
		 * @brief Unregisters a block type
		 */
		void unregisterBlockType(const BlockType type);

		/**
		 * @brief Creates a new block
		 * If no block could be created, an exception is thrown.
		 * @return The newly created block
		 */
		Block* createBlock(const BlockType type, Pipeline* pipeline) const;

	public:
		/**
		 * @brief Registers a new render command type
		 * @param creator The creator function
		 */
		void registerRenderCommandType(const RenderCommandType type, const rendercommand_creator_type creator);

		/**
		 * @brief Unregisters a render command type
		 */
		void unregisterRenderCommandType(const RenderCommandType type);

		/**
		 * @brief Creates a new render command
		 * If no render command could be created, an exception is thrown.
		 * @return The newly created render command
		 */
		RenderCommand* createRenderCommand(const RenderCommandType type, Pipeline* pipeline) const;

	private:
		/**
		 * @brief Registers all standard properties
		 */
		void registerStandardProperties();

		/**
		 * @brief Registers all standard blocks
		 */
		void registerStandardBlocks();

		/**
		 * @brief Registers all standard render commands
		 */
		void registerStandardRenderCommands();

	private:
		QMap<PropertyType, property_creator_type> _propertyTypes;
		QMap<BlockType, block_creator_type> _blockTypes;
		QMap<RenderCommandType, rendercommand_creator_type> _renderCommandTypes;
	};
}

#endif
