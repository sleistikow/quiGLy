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

#ifndef PIPELINETAB_H
#define PIPELINETAB_H

#include "../viewitem.h"
#include "data/ipipeline.h"
#include "visualitems/visualpipelineitemfactory.h"
#include "data/iserializable.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QSignalMapper>
#include <QToolBox>
#include <QAction>
#include <QMouseEvent>
#include <QTimer>

namespace ysm
{
	class IPipelineItem;
	class IBlock;

	class PipelineView;
	class PipelineScene;
	class PipelineSceneLayouter;
	class VisualPipelineItemFactory;

	//! \brief QWidget that is capable of presenting IPipeline to the user.
	class PipelineTab : public QGraphicsView, public ViewItem
	{
		Q_OBJECT

	private:

		//! \brief Possible modes, that define the behavior on user action.
		enum Mode
		{
			Default,
			Ignore,
			DraggingPossible,
			Dragging,
			PanningPossible,
			Panning,
		};

	public:

		/*!
		 * \brief Initialize a new instance.
		 * \param parentView The parent pipeline item.
		 * \param document The displayed document.
		 */
		PipelineTab(PipelineView* parentView, Document* document);

		/*!
		 * \brief Returns the current pipeline scene.
		 * \return The current pipelineScene
		 */
		PipelineScene* getPipelineScene() const;

	public slots:

		/*!
		 * \brief requestEditor Emits the editor requested signal for the given block.
		 * \param shaderBlock The block.
		 */
		void requestEditor(IBlock* shaderBlock);

		//! \brief Update messages beneath all blocks.
		void updateMessages();

        //! \brief Update the scene rect. Usually no need to call manually.
		void updateSceneRect();

	protected:

		/*!
		 * \brief Mouse wheel is used for zooming.
		 * \param event The event arguments.
		 */
		void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Key was pressed, react.
		 * \param event The event arguments.
		 */
		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Key was released, react.
		 * \param event The event arguments.
		 */
		void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Mouse key was pressed, react.
		 * \param event The event arguments.
		 */
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Mouse key was released, react.
		 * \param event The event arguments.
		 */
		void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Mouse was moved, react.
		 * \param event The event arguments.
		 */
		void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Context menu is required, react.
		 * \param event The event arguments.
		 */
		void contextMenuEvent(QContextMenuEvent* event) Q_DECL_OVERRIDE;

	private:

		//! \brief The parent view.
		PipelineView* _parentView;

		//! \brief The currently displayed scene.
		PipelineScene* _pipelineScene;

		//! \brief The current zoom level.
        float _zoomLevel;

		//! \brief The current and latest mode.
		Mode _mode, _latestMode;

		//! \brief Latest mouse position on key press and on move.
		QPointF _mouseDownPosition, _mouseMovePosition;

		//! \brief Item that was selected during last mouse key press.
		VisualPipelineItem* _mouseDownItem;
	};

}
#endif // PIPELINETAB_H
