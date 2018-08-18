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

#ifndef GLSLEDITOR_H
#define GLSLEDITOR_H

#include "../view.h"
#include "data/iblock.h"
#include "opengl/glslparser/glsldocument.h"

#include <QList>
#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QTabWidget>
#include <QAction>
#include <QMenu>

namespace ysm
{
	class GLSLEditorTab;

	//! \brief GLSL code editor widget. This is the main widget, that provides menu bar, tool bar and tabs.
	class GLSLEditor : public QWidget, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view.
		 */
		explicit GLSLEditor(QWidget* parentWidget, IView* parentView);

		/*!
		 * \brief Gets the editor's file menu.
		 * \returns The file menu.
		 */
		QMenu* getFileMenu() const;

		/*!
		 * \brief Gets the editor's tools menu.
		 * \returns The tools menu.
		 */
		QMenu* getToolsMenu() const;

	public slots:

		//! \brief Create a new tab.
		void newTab();

		//! \brief Open an existing file.
		void open();

		//! \brief Save the file.
		void save();

		//! \brief Save the file as new file.
		void saveAs();

		//! \brief Connect/disconnect file and block.
		void setBlock();

		//! \brief Close the file.
		void close();

		//! \brief Close all files.
		void closeAll();

		/*!
		 * \brief Close a specific tab.
		 * \param tabIndex The tab index.
		 */
		void closeTab(int tabIndex);

		//! \brief Shows the code of the given block.
		void open(Document* document, IBlock* block);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Window was closed, check for unsaved data.
		 * \param event The event args.
		 */
		void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Release the tabs that belong to the given block.
		 * \param block The block that was removed.
		 */
		void releaseTabs(IBlock* block);

		/*!
		 * \brief Document changed, update the tabs.
		 * \param document The document that changed.
		 */
		void updateTabs(GLSLDocument* document);

	private slots:

		//! \brief The current tab changed.
		void currentTabChanged(int);

	private:

		//! \brief Menu bar presented by this widget.
		QMenuBar* _menuBar;

		//! \brief The file menu, displayed in the menu bar.
		QMenu* _fileMenu;

		//! \brief The tools menu, displayed in the menu bar.
		QMenu* _toolsMenu;

		//! \brief The tab widget presented by this widget.
		QTabWidget* _tabWidget;

		//! \brief The current editor tab.
		GLSLEditorTab* _currentTab;

		/*!
		 * \brief Actions, displayed in menu bar.
		 * Part of the 'file' menu, and handled by the editor (in a global scope).
		 */
		QAction* _newTab;
		QAction* _open;
		QAction* _save;
		QAction* _saveAs;
		QAction* _setBlock;
		QAction* _close;
		QAction* _closeAll;

		/*!
		 * \brief Actions, displayed in menu bar.
		 * Part of the 'tools' menu, and handled by the editor (in a global scope).
		 */
		QAction* _settings;
	};
}

#endif // GLSLEDITOR_H
