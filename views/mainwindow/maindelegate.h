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

#ifndef MAINDELEGATE_H
#define MAINDELEGATE_H

#include <QObject>

namespace ysm
{
	class MainWindow;

	//! \brief Class that manages the main window's actions.
	//! By outsourcing the actions to a seperate class, main menu and main tool bar can use the same actions, without
	//! polluting the main window class itself.
	class MainDelegate : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param mainWindow Reference to the main window.
		 */
		MainDelegate(MainWindow* mainWindow);

	public slots:

		//! \brief File actions.
		void onNew();
		void onSinglePass();
		void onMultiPass();
		void onOpen();
		void onSave();
		void onSaveAs();
		void onImport();
		void onExport();
		void onRecent();
		void onClose();
		void onCloseAll();

		//! \brief Editing actions.
		void onUndo();
		void onRedo();

		//! \brief Pipeline actions.
		void onAutoLayout();

		//! \brief Rendering actions.
		void onRender();
		void onValidate();

		//! \brief Advanced actions.
		void onOptions();
		void onVersion();
		void onQuit();

	public:

		/*!
		 * \brief Shows the open dialog.
		 * \return The selected file.
		 */
		QString showOpenDialog();

		/*!
		 * \brief Shows the save as dialog.
		 * \return The selected file.
		 */
		QString showSaveAsDialog();

		/*!
		 * \brief Shows the import dialog.
		 * \return The selected file.
		 */
		QString showImportDialog();

		/*!
		 * \brief Shows the export dialog.
		 * \return The selected file.
		 */
		QString showExportDialog();

	private:

		//! \brief Reference to the main window.
		MainWindow* _mainWindow;

	};

}

#endif // MAINDELEGATE_H
