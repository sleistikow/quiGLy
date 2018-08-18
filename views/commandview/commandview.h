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

#ifndef COMMANDVIEW_H
#define COMMANDVIEW_H

#include "../view.h"

#include <QListWidget>
#include <QDockWidget>

namespace ysm
{
	class PipelineView;
	class IRenderCommand;

	//! \brief View that displays the pipeline's commands in a list, so they can be ordered.
	class CommandView : public QDockWidget, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view.
		 */
		explicit CommandView(QWidget* parentWidget, IView* parentView);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief A render command was created, add it to the list.
		 * \param renderCommand The render command.
		 */
		void createRenderCommand(IRenderCommand* renderCommand);

		/*!
		 * \brief A render command was deleted, remove it from the list.
		 * \param renderCommand The render command.
		 */
		void deleteRenderCommand(IRenderCommand* renderCommand);

	private slots:

		/*!
		 * \brief A row was moved, reoder the render commands.
		 * \param oldIndex The row's old index.
		 * \param start The start index.
		 * \param end The end index.
		 * \param newIndex the row's new index.
		 * \param row The row.
		 */
		void rowMoved(QModelIndex oldIndex, int start, int end, QModelIndex newIndex, int row);

		//! \brief The list's selection changed, require a new item selection.
		void selectionChanged();

	private:

		//! \brief The widget that displays the list.
		QListWidget* _list;
	};
}

#endif // COMMANDVIEW_H
