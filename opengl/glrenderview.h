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

#ifndef GLRENDERVIEW_H
#define GLRENDERVIEW_H

#include <QOpenGLWidget>
#include <QMatrix4x4>

#include "glconfiguration.h"

#include "views/view.h"
#include "data/blocks/blocktype.h"
#include "data/rendercommands/rendercommandtype.h"

namespace ysm
{

	class IBlock;
	class IPipeline;
	class IRenderCommand;
	class GLRenderPass;
	class GLRenderPassSet;
	class GLController;
	class SetupRenderingEvaluator;

	/**
	 * @brief The GLRenderView class is needed for achieving visual feedback from the pipeline in terms of OpenGL.
	 * A GLRenderView instance is created for every Display-Block per Pipeline.
	 */
	class GLRenderView : public QOpenGLWidget, public View
	{
	private:
		static const int SYNC_TIMEOUT = 1000000; // 1 Second timeout should be enough

	public:

		/**
		 * @brief GLRenderView  Constructor initializing the context and so on.
		 * @param controller	Controller that created this view
		 * @param displayBlock  DisplayBlock, the GLRenderView was created for
		 */
		GLRenderView(GLController* controller, IBlock* displayBlock);

		/// @brief Destructs this instance.
		virtual ~GLRenderView();

		/// @brief Returns the underlying set of renderpasses
		GLRenderPassSet* getRenderPassSet() const;

		/// @brief gets the DisplayBlock the view was created for.
		IBlock* getDisplayBlock() const;

		/// @brief Called, when registration was successful
		void onRegistrationSuccessful();

		/// @brief Called, when rendering should be aborted for any reason
		void onRenderingAborted();

		/**
		 * @brief Retrieves the camera's positon.
		 * @return The camera position.
		 */
		QVector3D getCamera() const;

		/**
		 * @brief Sets the camera's positon.
		 * @param camera The camera position.
		 */
		void setCamera(QVector3D camera);

	protected:
		// QOpenGLWidget
		void initializeGL() Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;

		// View
		void updateDocument() Q_DECL_OVERRIDE;
		void updateItem() Q_DECL_OVERRIDE;

	protected:
		// QWidget
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

	private:

		/// @brief initializes the given pass.
		void initializePass(GLRenderPass* pass);

		/// @brief Initializes the Camera control
		void setupCameraControl();

		/// @brief Initializes the elapsed time uniform in the current program
		void updateElapsedTimeUniform(IBlock* block, GLRenderPass* pass);

		/// @brief binds all textures to their specified units and targets.
		void bindTexture(IBlock* textureBlock, GLRenderPass* pass) const;

		/// @brief Sets everything to be done for drawing actual data.
		void callDrawCommand(IRenderCommand* command, GLRenderPass* pass);

		/// @brief Calls glClear with respective Parameters according to RenderCommand Properties.
		void callClearSettings(IRenderCommand* command);

	private:

		// Attributes
		GLController* _controller;			/*!< The Controller which created this view. */
		SetupRenderingEvaluator* _evaluator;/*!< The Evaluator holding the needed data. */

		GLRenderPassSet* _renderPassSet;	/*!< The PipelineInfo instance used by this RenderView. */
		GLConfiguration::Functions* f;		/*!< The OpenGL-Functions Object to gain access to the necessary functions. */

		bool _valid;						/*!< Determines, whether view is actually ready to be rendered. */

		IBlock* _cameraControl;				/*!< A Pointer to the only camera control block, if exists. */
		QPoint _grabMousePosition;			/*!< Stores the relative position the grab event took place. */
		QVector3D _cameraTransform;			/*!< The camera position within this RenderView */
	};
}

#endif // GLRENDERVIEW_H
