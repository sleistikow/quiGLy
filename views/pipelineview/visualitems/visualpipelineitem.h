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

#ifndef VISUALPIPELINEITEM_H
#define VISUALPIPELINEITEM_H

#include "../../viewitem.h"
#include "../pipelinescenelayouter.h"
#include "data/ipipelineitem.h"
#include "commands/iuicommand.h"

#include <QObject>
#include <QGraphicsObject>
#include <QVariant>
#include <QAction>

namespace ysm
{
	class PipelineScene;

	//! \brief Base class for all visual pipeline items, rendered in a PipelineScene.
	//! NOTE: Class caches the bounding rect, due to this it's neccessary that set the initial bounding rect in their
	//!       constructor, by setting the protected _boundingRect member.
	class VisualPipelineItem : public QGraphicsObject, public ViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentGraphics The parent graphics item.
		 * \param parentView The parent view item.
		 */
		explicit VisualPipelineItem(QGraphicsItem* parentGraphics, IView* parentView);

		/*!
		 * \brief Returns the scene as pipeline scene.
		 * \return The pipeline scene.
		 */
		PipelineScene* getPipelineScene() const;

		/*!
		 * \brief Gets the displayed pipeline item.
		 * \return The pipeline item.
		 */
		virtual IPipelineItem* getPipelineItem() const = 0;

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF boundingRect() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief Restores the layout using the parent scene's layouter.
		void restoreLayout();

		//! \brief Deletes this item from the pipeline.
		virtual void deleteItem() = 0;

	protected:

		/*!
		 * \brief Show the item's context menu.
		 * \param event The event arguments.
		 */
		void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Internal method, that does the layout restore, and can be overriden by child classes.
		 * \param layouter The validated layouter.
		 */
		virtual void restoreFromLayouter(PipelineSceneLayouter* layouter);

		/*!
		 * \brief Returns all context menu actions.
		 * \return The actions. Add NULL for a seperator.
		 */
		virtual QList<QAction*> getContextActions() const;

	protected:

		/*!
		 * \brief A pipeline item changed, update this view.
		 * \param pipelineItem The pipeline item.
		 */
		void updateVisual(IPipelineItem* pipelineItem);

		/*!
		 * \brief A property changed, update this view.
		 * \param property The property.
		 */
		void updateProperty(IProperty* property);

		//! \brief Updates the bounding rect and redraws the item.
		void invalidateBoundingRect();

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		virtual QRectF calculateBoundingRect() const = 0;

		/*!
		 * \brief The item changed, forward change to the layouter, if neccessary.
		 * \param change The change.
		 * \param value The new value.
		 * \return The adjusted value.
		 */
		QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

		/**
		 * @brief Should return true, if the given property is visually represented by this view.
		 * @param propertyID The property ID.
		 * @return If true, the view will redraw if the property changes.
		 */
		virtual bool isVisualProperty(PropertyID propertyID);

	private:

		//! \brief Context menu actions.
		QAction* _deleteAction;

		//! \brief The cached bounding rect.
		mutable QRectF _boundingRect;

		//! \brief Flag that is set once the bounding rect has been calculated.
		//! Neccessary to identify bounding rect changes, which need to be forwarded to Qt.
		mutable bool _boundingRectDefined;
	};

}

#endif // VISUALPIPELINEITEM_H
