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

#ifndef GLCONTROLLER_H
#define GLCONTROLLER_H

#include <QObject>
#include <QSurfaceFormat>
#include <QTimer>

#ifdef QT_DEBUG
#include <QOpenGLDebugMessage>
#endif

#include "data/blocks/blocktype.h"

namespace ysm
{

	class Document;
	class IView;
	class IPipeline;
	class IBlock;
	class GLRenderView;
	class SetupRenderingEvaluator;

	/**
	 * @brief The GLController class is the core of the output,
	 * meaning all the data we can get out of our pipeline is going to be
	 * packed together in one single GLRenderView per Pipeline.
	 */
	class GLController : public QObject
	{
		Q_OBJECT
	public:

		/**
		 * @brief GLController Conctructs a new controller.
		 * @param parent parent for this controller.
		 */
		GLController(QObject* parent);

		/// @brief Destructs this controller instance.
		virtual ~GLController();

		/**
		 * @brief Retrieve the rendering evaluator.
		 * @return The setup rendering evaluator.
		 */
		SetupRenderingEvaluator* getEvaluator() const;

		/**
		 * @brief isOpenGLVersionSupported Determines, whether the specified OpenGL version is supported.
		 * @param major Major OpenGL version
		 * @param minor Minor OpenGL version
		 * @return True, if supported, false otherwise
		 */
		bool isOpenGLVersionSupported(int major, int minor);

		/**
		 * @brief getOpenGLVersion Gets the currently set OpenGL version.
		 * @return The OpenGL version
		 */
		QPair<int, int> getOpenGLVersion() const;

		/**
		 * @brief This function is called, when a view is ready to be initialized.
		 * @param sender View to be initialized
		 */
		void registerView(GLRenderView* view);

		/// @brief Called by GLRenderView, if rendering has to be aborted for any given reason.
		void signalAbortRendering(GLRenderView* view, const QString& reason, const QString& log = "");

	signals:

		/// @brief Emitted, when the messages of some blocks have changed during evaluation
		void messagesChanged();

	private:

		SetupRenderingEvaluator* _setupRenderingEvaluator;

		QSurfaceFormat _surfaceFormat;				/*!< The currently globally used format for all GLRenderView instances. */

		QTimer _timer;						/*!< This timer forces a permanent update on the widget. */

#ifdef QT_DEBUG
	private slots:

		void onMessageLogged(QOpenGLDebugMessage message);
#endif

	};
}

#endif // GLCONTROLLER_H
