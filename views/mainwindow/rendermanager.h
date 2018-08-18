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

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "data/iblock.h"

#include "opengl/glcontroller.h"
#include "opengl/glrenderview.h"

#include <QObject>
#include <QDockWidget>
#include <QByteArray>
#include <QMap>

namespace ysm
{
	class Document;
	class MainWindow;

	//! \brief Manages the main window's render tasks.
	class RenderManager : public QObject
	{
		Q_OBJECT

	public:

		//! \brief Internal struct to store dock widget information.
		struct RenderDockWidget
		{
			/// \brief The dock widget that contains the render view.
			QDockWidget* _dockWidget;

			/// \brief The OpenGL render view.
			GLRenderView* _renderView;

			/// \brief The view's camera position.
			QVector3D _cameraBackup;
		};

		//! \brief Struct used to store OpenGL versions.
		struct OpenGLVersion { int _major, _minor; };

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent
		 */
		RenderManager(MainWindow* parentWindow);

		/*!
		 * \brief Return all supported OpenGL versions.
		 * \return The supported versions.
		 */
		QList<OpenGLVersion> getSupportedVersions();

		/*!
		 * \brief Set the current OpenGL version.
		 * \param version The new version.
		 */
		void setVersion(RenderManager::OpenGLVersion version);

	public slots:

		//! \brief Update the visible render views.
		void updateRenderViews();

		//! \brief Re-evaluate the pipeline.
		void evaluatePipeline();

		/*!
		 * \brief Re-evaluate the pipeline of the given document by command queue notification.
		 * TODO: OpenGL controller should be able to respond to command queue notifications itself (performance).
		 * \param document The document.
		 */
		void evaluateDocument(Document* document);

	public slots:

		//! \brief Toggle the rendering state of the active document.
		void toggleRendering();

		/*!
		 * \brief Change the rendering state of the active document.
		 * \param isRendering The new state.
		 */
		void setRendering(bool isRendering);

	protected:

		/*!
		 * \brief Retrieve all display blocks from the pipeline.
		 * \param activeDocument The document to search.
		 * \return The display blocks.
		 */
		QList<IBlock*> getDisplayBlocks(Document* activeDocument) const;

		/*!
		 * \brief Retrieve or create the render widget for the given block.
		 * \param renderBlock The display block.
		 * \param tabifyWidget If set, the new widget will be tabified behind this widget.
		 * \return The render dock widget.
		 */
		RenderDockWidget recreateRenderWidget(IBlock* renderBlock, QDockWidget* tabifyWidget);

		//! \brief Recreate all active render widgets.
		void recreateRenderWidgets();

		/*!
		 * \brief Release all active render views to recreate the pipeline.
		 * \return The render views.
		 */
		QList<GLRenderView*> releaseRenderViews();

		//! \brief Release all render dock widgets without valid render view.
		void releaseRenderDocks();

	private:

		//! \brief The main window.
		MainWindow* _parentWindow;

		//! \brief Contains all active QDockWidget widgets, used for rendering.
		QMap<IBlock*, RenderDockWidget> _renderDockWidgets;

		//! \brief All supported OpenGL versions.
		QList<OpenGLVersion> _supportedVersions;
	};

}

#endif // RENDERMANAGER_H
