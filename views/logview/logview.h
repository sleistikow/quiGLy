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

#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "../view.h"

#include <QDockWidget>
#include <QPlainTextEdit>

namespace ysm
{

	//! \brief Simply log view that displays output to the user.
	class LogView : public QDockWidget, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view.
		 */
		explicit LogView(QWidget* parentWidget, IView* parentView);

		//! \brief Convenience method to clear the main window's log view.
		static void clear();

		/*!
		 * \brief Convenience method to log the given message to the main window's log view.
		 * \param log The message.
		 */
		static void log(const QString& log);

		/*!
		 * \brief Convenience method to log the given message to the main window's log view.
		 * \param log The messages.
		 */
		static void log(const QStringList& log);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

	private:

		//! \brief Simple text log.
		QPlainTextEdit* _textLog;
	};

}

#endif // LOGVIEW_H
