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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QStack>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QEvent>

#include "../view.h"

namespace ysm
{
	class GLController;

	class Document;
	class DocumentManager;
	class ViewManager;
	class MainDelegate;
	class RenderManager;

	//! \brief The application's main window, acting as the glue between all views.
	class MainWindow : public QMainWindow, public View
	{
		Q_OBJECT

	private:

		//! \brief Specifies the timeout in ms, after which entries in the status bar will be hidden.
		static const int STATUS_BAR_TIMEOUT_MS = 5000;

		//! \brief Initialize new instance.
		MainWindow();

	public:

		/*!
		 * \brief Returns the main window's instance or creates it.
		 * \return The application's main window.
		 */
		static MainWindow* getInstance();

		/*!
		 * \brief Returns the delegate.
		 * \return The menu delegate.
		 */
		MainDelegate* getDelegate() const;

		/*!
		 * \brief Returns the document manager.
		 * \return The document manager.
		 */
		DocumentManager* getDocumentManager() const;

		/*!
		 * \brief Returns the render manager.
		 * \return The render manager.
		 */
		RenderManager* getRenderManager() const;

		/*!
		 * \brief Returns the view manager.
		 * \return The view manager.
		 */
		ViewManager* getViewManager() const;

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		//! \brief Saves the window state
		void saveWindowState();

		//! \brief Restores the window state
		void restoreWindowState();

		//! \brief Update the displayed document titles.
		void updateTitles();

		/*!
		 * \brief Update the window, because a document changed.
		 * \param document The document.
		 */
		void updateWindow(Document* document);

		/*!
		 * \brief Execute all close task, ask for confirmation.
		 * \param event The event args.
		 */
		void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

	private:

		//! \brief The shared main window.
		static MainWindow* _sharedInstance;

		//! \brief The main window's delegate.
		MainDelegate* _delegateInstance;

		//! \brief The document manager.
		DocumentManager* _documentManager;

		//! \brief The view manager.
		ViewManager* _viewManager;

		//! \brief The render manager.
		RenderManager* _renderManager;
	};
}

#endif // MAINWINDOW_H
