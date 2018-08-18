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

#ifndef WIZARDOVERVIEWDIALOG_H
#define WIZARDOVERVIEWDIALOG_H

#include "../wizardwindow.h"

#include <QDialog>
#include <QListWidget>
#include <QGridLayout>
#include <QWizard>

namespace ysm
{

	//! \brief Root dialog for multi pass wizards.
	class WizardOverviewDialog : public QWizard
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent widget.
		 */
		WizardOverviewDialog(QWidget* parent);

		//! \brief Destruct instance.
		~WizardOverviewDialog();

	public slots:

		/*!
		 * \brief Generate the project file and load the project.
		 * \param mainWindow The main window.
		 */
		void createProject(MainWindow* mainWindow);

	protected:

		/*!
		 * \brief Create a list widget.
		 * \param title The list's title.
		 * \param listWidget Returns the list widget.
		 * \return The widget that embeds the list.
		 */
		QWidget* createListWidget(const QString& title, QListWidget** listWidget);

		/*!
		 * \brief Initialize the given page.
		 * \param id The page.
		 */
		void initializePage(int id) Q_DECL_OVERRIDE;

	protected slots:

		/*!
		 * \brief Store the given render pass data.
		 * \param wizardWindow The completed wizard window.
		 */
		void storeRenderPass(WizardWindow* wizardWindow);

		//! \brief Add a new render pass.
		void addRenderPass();

	private:

		//! \brief Available lists.
		QListWidget* _sourcesList;
		QListWidget* _passesList;
		QListWidget* _outputsList;

		//! \brief Available data.
		QList<WizardWindow*> _passes;

		//! \brief The page IDs.
		int _fileId;
		int _overviewId;
	};

}

#endif // WIZARDOVERVIEWDIALOG_H
