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

#ifndef TOOLVIEW_H
#define TOOLVIEW_H

#include "../view.h"

#include <QDockWidget>
#include <QStackedWidget>

namespace ysm
{
	class ToolTab;

	//! \brief Provides a tool box to add blocks.
	class ToolView : public QDockWidget, public View
	{
		Q_OBJECT

	public:

		//! \brief Simple struct holding tool data.
		struct Tool
		{
			//! \brief The tool's name.
			QString name;

			//! \brief The tool's group.
			QString group;
		};

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget,
		 * \param parentView The parent view.
		 */
		explicit ToolView(QWidget* parentWidget, IView* parentView);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief A document was closed, delete the corresponding tool view.
		 * \param document The document.
		 */
		void deleteToolView(Document* document);

	private:

		//! \brief All available document views.
		QStackedWidget* _viewStack;

		//! \brief Contains all tool views, indexed by document.
		QMap<Document*, ToolTab*> _views;
	};

}

#endif // TOOLVIEW_H
