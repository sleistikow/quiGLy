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

#ifndef MAINMENU_H
#define MAINMENU_H

#include "data/ipipeline.h"
#include "../view.h"

#include <QMenuBar>

namespace ysm
{
	class MainWindow;
	class MainDelegate;

	//! \brief The main window's menu bar.
	class MainMenu : public QMenuBar, public View
	{
		Q_OBJECT

	public:

		static const int RecentFileCount;

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param delegate The main delegate.
		 * \param parentWindow The parent window.
		 */
		MainMenu(MainDelegate* delegate, MainWindow* parentWindow);

		/*!
		 * \brief Store a recently opened file.
		 * \param recentFile The file.
		 */
		void prependRecent(QString recentFile);

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Update the tool bar icons from pipeline change.
		 * \param pipeline The pipeline.
		 */
		void documentChanged(IPipeline* pipeline);

		/*!
		 * \brief A document was closed, update the recent menu.
		 * \param document The document.
		 */
		void documentClosed(Document* document);

		//! \brief Update the recent file list.
		void updateRecentFiles();

	protected slots:

		//! \brief Update the menu icons.
		void updateMenu();

	private:

		//! \brief The delegate.
		MainDelegate* _delegate;

		//! \brief The current document.
		Document* _document;

		//! \brief The member actions.
		QAction* _undoAction;
		QAction* _redoAction;
		QAction* _validateAction;
		QAction* _renderAction;
		QAction* _saveAction;
		QAction* _saveAsAction;
		QAction* _closeAction;
		QAction* _importAction;
		QAction* _exportAction;
		QAction* _versionAction;

		//! \brief Recent files.
		QList<QAction*> _recentActions;
		QMenu* _recentMenu;
	};

}

#endif // MAINMENU_H
