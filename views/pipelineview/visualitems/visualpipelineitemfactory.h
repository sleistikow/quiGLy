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

#ifndef VISUALPIPELINEITEMFACTORY_H
#define VISUALPIPELINEITEMFACTORY_H

#include "data/iblock.h"
#include "data/irendercommand.h"
#include "data/blocks/blocktype.h"
#include "data/rendercommands/rendercommandtype.h"

#include <QObject>
#include <QMap>

namespace ysm
{
	class VisualPipelineItem;
	class VisualRenderCommand;
	class VisualBlock;

	class IView;

	//! \brief Factory class that creates visual pipeline items from data pipeline items.
	class VisualPipelineItemFactory : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		explicit VisualPipelineItemFactory(QObject* parent = 0);

		/*!
		 * \brief Creates a new block item, that is able to represent the given block.
		 * \param block The data item.
		 * \param parentView The parent item.
		 * \return The visual item.
		 */
		VisualBlock* createBlockItem(IBlock* block, IView* parentView) const;

		/*!
		 * \brief Creates a new render command item, that is able to represent the given render command.
		 * \param renderCommand The data item.
		 * \param parentView The parent item.
		 * \return The visual item.
		 */
		VisualRenderCommand* createRenderCommandItem(IRenderCommand* renderCommand, IView* parentView) const;

		/*!
		 * \brief Register a block type. If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setBlockConstructor(const BlockType itemType,
								 VisualBlock* (* constructor)(IBlock*, IView*));

		/*!
		 * \brief Register a render command type. If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setRenderCommandConstructor(const RenderCommandType itemType,
										 VisualRenderCommand* (* constructor)(IRenderCommand*, IView*));

		/*!
		 * \brief Register a block type, and simply call the constructor passing the IBlock for creation.
		 * \param itemType The item type.
		 */
		template<typename T> void setDefaultBlockConstructor(const BlockType itemType)
		{ setBlockConstructor(itemType, defaultBlockConstructor<T>); }

		/*!
		 * \brief Register a block type, and simply call the constructor passing the IBlock for creation.
		 * \param itemType The item type.
		 */
		template<typename T> void setDefaultRenderCommandConstructor(const RenderCommandType itemType)
		{ setRenderCommandConstructor(itemType, defaultRenderCommandConstructor<T>); }

	protected:

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param block The item parameter.
		 * \return The visual item.
		 */
		template<typename T> static VisualBlock* defaultBlockConstructor(IBlock* block,
																		 IView* parentView)
		{ return new T(block, parentView); }

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param renderCommand The item parameter.
		 * \return The visual item.
		 */
		template<typename T> static VisualRenderCommand* defaultRenderCommandConstructor(IRenderCommand* renderCommand,
																						 IView* parentView)
		{ return new T(renderCommand, parentView); }

	private:

		//! \brief Contains all block constructors indexed by block type.
		QMap<BlockType, VisualBlock* (*)(IBlock*, IView*)> _blockConstructors;

		//! \brief Contains all render command constructors indexed by render command type.
		QMap<RenderCommandType, VisualRenderCommand* (*)(IRenderCommand*, IView*)> _renderCommandConstructors;
	};

}

#endif // VISUALPIPELINEITEMFACTORY_H
