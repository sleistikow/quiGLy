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

#ifndef WIZARDSELECTDIALOG_H
#define WIZARDSELECTDIALOG_H

#include "../wizardfile.h"

#include <QDialog>
#include <QListWidget>
#include <QTextEdit>

namespace ysm
{
	class WizardWindow;

	//! \brief Dialog that displays all available project wizards and description.
	class WizardSelectDialog : public QDialog
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param resourcePath The resource path.
		 * \param multiPass True if multi pass is allowed.
		 * \param parent The parent widget.
		 */
		WizardSelectDialog(const QString& resourcePath, bool multiPass, QWidget* parent);

		/*!
		 * \brief Returns the currently selected wizard.
		 * \return The selected wizard.
		 */
		WizardFile getCurrentWizard() const;

	protected:

		/*!
		 * \brief Loads the resources from the current resouce path.
		 * \return True, if all resources were loaded successfully.
		 */
		bool loadResources();

		/*!
		 * \brief Add a new wizard that can be selected.
		 * \param wizard The wizard to add.
		 */
		void addWizard(const WizardFile& wizard);

	protected slots:

		//! \brief The selection changed.
		void currentWizardChanged();

	private:

		//! \brief The path to the wizard configurations.
		QString _resourcePath;

		//! \brief True, if multi pass allowed.
		bool _multiPass;

		//! \brief List of all available project wizards.
		QList<WizardFile> _wizards;

		//! \brief List that displays all available wizards.
		QListWidget* _wizardList;

		//! \brief List that displays the current wizard's description.
		QTextEdit* _wizardDescription;
	};

}

#endif // WIZARDSELECTDIALOG_H
