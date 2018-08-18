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

#ifndef FILESELECT_H
#define FILESELECT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace ysm
{

	//! \brief Simple file select widget consisting of text edit and browse button.
	class FileSelect : public QWidget
	{
		Q_OBJECT

		Q_PROPERTY(QString filename READ getFilename WRITE setFilename NOTIFY filenameChanged)

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		FileSelect(QWidget* parent);

		/*!
		 * \brief Gets the selected file.
		 * \return The selected file.
		 */
		QString getFilename() const;

		/*!
		 * \brief Sets the selected file.
		 * \param selectedFile The selected file.
		 */
		void setFilename(const QString& selectedFile);

		/*!
		 * \brief Checks the read only state.
		 * \return True, if read only.
		 */
		bool isReadOnly() const;

		/*!
		 * \brief Sets the read only state.
		 * \param isReadOnly True, if read only.
		 */
		void setReadOnly(bool isReadOnly);

	signals:

		//! \brief Emitted when the file changes.
		void filenameChanged();

		//! \brief Emitted when the file browser should open.
		void browseFile();

	protected slots:

		//! \brief Emits the filename changed signal.
		void emitFilenameChanged();

		//! \brief Emits the file browser signal.
		void emitBrowseFile();

	private:

		//! \brief The line edit.
		QLineEdit* _lineEdit;

		//! \brief The browse button.
		QPushButton* _browseButton;
	};

}

#endif // FILESELECT_H
