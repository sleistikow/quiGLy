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

#ifndef ITEMREGISTRY_H
#define ITEMREGISTRY_H

#include "propertyview/propertyviewfactory.h"
#include "pipelineview/visualitems/visualpipelineitemfactory.h"
#include "toolview/toolview.h"

#include "data/ipipeline.h"
#include "data/ipipelineitem.h"
#include "data/iblock.h"
#include "data/irendercommand.h"

#include <QObject>

namespace ysm
{
	class PropertyViewFactory;
	class VisualPipelineItemFactory;

	//! \brief Used to register all existing items to the interface.
	class Registry : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent object.
		 */
		Registry(QObject* parent);

		/*!
		 * \brief Gets a list of all registered blocks.
		 * \return All registered blocks.
		 */
		QList<BlockType> getBlocks() const;

		/*!
		 * \brief Gets the pipeline item factory.
		 * \return The pipeline item factory.
		 */
		VisualPipelineItemFactory* getVisualFactory() const;

		/*!
		 * \brief Gets the view factory.
		 * \return The view factory.
		 */
		PropertyViewFactory* getViewFactory() const;

		/*!
		 * \brief Retrieves the correct tool data for the given block type.
		 * \param blockType The block type.
		 * \return The tool's data.
		 */
		ToolView::Tool getTool(BlockType blockType) const;

	protected:

		/*!
		 * \brief Registers a block type, which can then be created through the user.
		 * \param blockType The block type.
		 * \param itemName The item name, displayed on the button.
		 * \param groupName Group name in the tool box.
		 */
		template<typename V, typename P> void registerBlockType(const BlockType blockType,
																const QString& itemName,
																const QString& groupName)
		{
			//Register to the factories, then to the toolbox.
			this->_visualFactory->setDefaultBlockConstructor<V>(blockType);
			this->_viewFactory->setDefaultBlockConstructor<P>(blockType);
			this->_toolMap[blockType] = { itemName, groupName };
		}

		/*!
		 * \brief Registers a render command type, which can then be created through the user.
		 * \param cmdType The render command type.
		 * \param itemName The item name, displayed on the button.
		 * \param groupName Group name in the tool box.
		 */
		template<typename V, typename P> void registerRenderCommandType(const RenderCommandType commandType)
		{
			//Register to the factories, then to the toolbox.
			this->_visualFactory->setDefaultRenderCommandConstructor<V>(commandType);
			this->_viewFactory->setDefaultRenderCommandConstructor<P>(commandType);
		}

	private:

		//! \brief Pipeline item factory.
		VisualPipelineItemFactory* _visualFactory;

		//! \brief View factory.
		PropertyViewFactory* _viewFactory;

		//! \brief Tool map, contains item and group name.
		QMap<BlockType, ToolView::Tool> _toolMap;
	};
}

#endif // ITEMREGISTRY_H
