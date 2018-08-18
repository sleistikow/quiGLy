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

#ifndef WIZARDFILEPAGE_H
#define WIZARDFILEPAGE_H

#include "views/common/versionselect.h"
#include "views/common/fileselect.h"
#include "wizardcustompage.h"

#include <QDomElement>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QWizardPage>

namespace ysm
{
	class WizardWindow;

	//! \brief Wizard page that allows to specify where to store the new project.
	class WizardFilePage : public WizardCustomPage
	{
		Q_OBJECT

	public:

		//! \brief Constant to access the filename field.
		static const QString FilenameField;

		//! \brief Constant to access the import field.
		static const QString ImportField;

		//! \brief Constant to access the version field.
		static const QString VersionField;

		//! \brief Constant to access the name field.
		static const QString NameField;

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param rootElement The XML configuration.
		 * \param parent The parent.
		 */
		WizardFilePage(QDomElement rootElement, WizardWindow* parent);

		/*!
		 * \brief Initialize new instance.
		 * \param allowImport True if import option is enabled.
		 * \param minimumVersion The minimal required version.
		 * \param parent The parent.
		 */
		WizardFilePage(bool allowImport, int minimumVersion, QWizard* parent);

	public slots:

		//! \brief Show the file browser.
		void browseFile();

		//! \brief Update the file widget.
		void updatePage();

	protected:

		/*!
		 * \brief Check wether the page is filled correctly.
		 * \return True, if user can continue.
		 */
		bool isComplete() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Create the page.
		 * \param isMultiPass If true, page shows only name.
		 * \return Return the form layout.
		 */
		QFormLayout* createPage(bool isMultiPass);

	private:

		//! \brief Configuration options.
		FileSelect* _fileSelect;
		QCheckBox* _importOption;
		VersionSelect* _versionSelect;
		QLineEdit* _nameEdit;

		//! \brief XML data.
		bool _allowImport;
		int _minimumVersion;
	};

}

#endif // WIZARDFILEPAGE_H
