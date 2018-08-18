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

#ifndef TOOLTAB_H
#define TOOLTAB_H

#include "toolview.h"
#include "views/viewitem.h"
#include "data/iblock.h"

#include <QWidget>
#include <QToolBox>
#include <QToolButton>
#include <QSignalMapper>

namespace ysm
{
	//! \brief Contains a tab in the tool view, that holds all tools for one document.
	//! The tab is also responsible for reacting to version changes.
	class ToolTab : public QToolBox, public ViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget,
		 * \param parentView The parent view.
		 */
		explicit ToolTab(QWidget* parentWidget, IView* parentView);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Registers a tool to the toolbox, and creates a group if neccessary.
		 * \param blockType The block type.
		 * \param tool The tool's data.
		 */
		void registerBlockType(BlockType blockType, const ToolView::Tool& tool);

	protected:

		/*!
		 * \brief Create the group, if not exists.
		 * \param groupName The group's name.
		 * \return The group's layout.
		 */
		QLayout* updateGroup(const QString& groupName);

		/*!
		 * \brief Update the buttons and disable unsupported blocks.
		 * \param pipeline The pipeline.
		 */
		void updateVersion(IPipeline* pipeline);

	protected slots:

		/*!
		 * \brief Creates a block using a command.
		 * \param blockType The block type.
		 */
		void createBlock(const int blockType);

	private:

		//! \brief Signal mapper used for tool box.
		QSignalMapper* _blockSignalMapper;

		//! \brief Contains all toolbox layouts indexed by group name.
		QMap<QString, QLayout*> _groupLayouts;

		//! \brief List of all available tool buttons and dummy block.
		QMap<QToolButton*, IBlock*> _dummyData;
	};

}

#endif // TOOLTAB_H
