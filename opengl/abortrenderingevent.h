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

#ifndef ABORTRENDERINGEVENT_H
#define ABORTRENDERINGEVENT_H

#include <QEvent>
#include <QString>

namespace ysm
{

	class GLRenderView;

	/**
	 * This is a user defined event to abort rendering which will be processed as late as possible in the event loop.
	 * The main reason for this is, that the signal-slot structure in combination with the QOpenGLWidgets update behaviour
	 * will crash the program, otherwise.
	 */
	class AbortRenderingEvent : public QEvent
	{
	public:
		// The user defined Event-type
		static const QEvent::Type EventType = QEvent::User;

		/// @brief Constructs the event with the event type above.
		AbortRenderingEvent(GLRenderView* view, const QString& reason, const QString& log = "");

		/// @brief Returns the view, the error occurd on
		GLRenderView* getView() const;

		/// @brief Gets the reason for sending the event.
		const QString& getReason() const;

		/// @brief Gets additional log
		const QString& getLog() const;

	private:

		GLRenderView* _view;
		QString _reason;
		QString _log;
	};

}

#endif // ABORTRENDERINGEVENT_H
