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

#ifndef VISUALNAMEDITEM_H
#define VISUALNAMEDITEM_H

#include "data/ipipelineitem.h"
#include "visuallayouteditem.h"

#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

namespace ysm
{

	//! \brief Base class for visual items, that have a name and can be renamed.
	class VisualNamedItem : public VisualLayoutedItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentView The parent item.
		 */
		VisualNamedItem(IView* parentView);

	public slots:

		//! \brief Shows the editing field for the item.
		void beginRenameItem();

		//! \brief Renames this item.
		void endRenameItem();

		//! \brief Cancels a rename action
		void cancelRenameItem();

	protected:

		/*!
		 * \brief Block was double clicked, start rename.
		 * \param event The event args.
		 */
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Catch ESC to stop renaming.
		 * \param event The event args.
		 */
		void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the item's bounding rect.
		 * \return The bounding rect.
		 */
		QRectF calculateBoundingRect() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the content's height.
		 * \return The height.
		 */
		virtual float getContentHeight() const = 0;

		/*!
		 * \brief Get the item's full width (with padding).
		 * \return The width.
		 */
		virtual float getFullWidth() const = 0;

		/*!
		 * \brief Get the item's inner width (without padding).
		 * \return The width.
		 */
		virtual float getInnerWidth() const = 0;

		/*!
		 * \brief Returns the header's height.
		 * \return The header's height.
		 */
		float getHeaderHeight() const;

		/*!
		 * \brief Returns the status message's height.
		 * \return The status message's height.
		 */
		float getMessageHeight() const;

		/*!
		 * \brief Returns all context menu actions.
		 * \return The actions. Add NULL for a seperator.
		 */
		QList<QAction*> getContextActions() const Q_DECL_OVERRIDE;

		/**
		 * @brief Should return true, if the given property is visually represented by this view.
		 * @param propertyID The property ID.
		 * @return If true, the view will redraw if the property changes.
		 */
		bool isVisualProperty(PropertyID propertyID) Q_DECL_OVERRIDE;

		/*!
		 * \brief Checks, wether the name is currently being edited.
		 * \return True, if the name is being edited.
		 */
		bool isEditingName() const;

	private:

		//! \brief Context menu actions.
		QAction* _renameAction;

		//! \brief Edit used for renaming.
		QLineEdit* _nameEdit;

		//! \brief The graphics proxy holding the text edit.
		QGraphicsProxyWidget* _textEditWidget;

		//! \brief Flag indication wether the name is being edited.
		bool _isEditingName;
	};
}


#endif // VISUALNAMEDITEM_H
