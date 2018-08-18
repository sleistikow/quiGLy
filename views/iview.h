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

#ifndef IVIEW_H
#define IVIEW_H

#include "data/iserializable.h"
#include "commands/ichangeable.h"
#include "commands/iuicommand.h"

#include <QList>
#include <QDebug>

namespace ysm
{
	class Document;
	class IPipelineItem;

	//! \brief Base interface for all views that display changeable data.
	//! Provides a changeable document and active item and forwards the changes to all children.
	//! Can be notified about changes using the updateView() method.
	//! SERIALIZATION: A view is serializable, but does not forward serialization to it's children. A view should use
	//! Document::attach() to attach specific objects to the document's serialization. In case a child view would then
	//! use this method to attach itself to the document, too, the child view would be serialized twice.
	class IView : public ISerializable
	{

	public:

		//! \brief Destruct instance.
		virtual ~IView() { }

		/*!
		 * \brief Get the parent view.
		 * \return The parent view.
		 */
		virtual IView* getParentView() const = 0;

		/*!
		 * \brief Sets a new parent view.
		 * \param parentView The parent view.
		 */
		virtual void setParentView(IView* parentView) = 0;

		/*!
		 * \brief Get the active document.
		 * \return The active document.
		 */
		virtual Document* getActiveDocument() const = 0;

		/*!
		 * \brief Set the active document.
		 * \param document The view's active document.
		 */
		virtual void setActiveDocument(Document* document) = 0;

		/*!
		 * \brief Checks wether the active document should be inherited and updated by the parent.
		 * \return True, if the parent should forward active document changes.
		 */
		virtual bool inheritsActiveDocument() const = 0;

		/*!
		 * \brief Gets the active pipeline item.
		 * \return The active pipeline item.
		 */
		virtual IPipelineItem* getActiveItem() const = 0;

		/*!
		 * \brief Sets the active pipeline item.
		 * \param pipelineItem The view's active pipeline item.
		 */
		virtual void setActiveItem(IPipelineItem* pipelineItem) = 0;

		/*!
		 * \brief Checks wether the active item should be inherited and updated by the parent.
		 * \return True, if the parent should forward active item changes.
		 */
		virtual bool inheritsActiveItem() const = 0;

		/*!
		 * \brief Add a new child view.
		 * \param childView The new child view.
		 */
		virtual void addChildView(IView* childView) = 0;

		/*!
		 * \brief Remove a child view.
		 * \param childView The child view.
		 */
		virtual void removeChildView(IView* childView) = 0;

		/*!
		 * \brief Called whenever the underlying data might have changed.
		 * \param document The changed document.
		 * \param changedObjects List of changed data objects.
		 * \param operation changeOperation executed.
		 */
		virtual void updateView(Document* document, const QList<IChangeable*>& changedObjects,
								IChangeable::Operation operation)  = 0;

		/*!
		 * \brief Called whenever the underlying data might have changed, but the change cannot be exactly defined.
		 * \param document The changed document.
		 */
		virtual void resetView(Document* document) = 0;

		/*!
		 * \brief Retrieve the command queue by iterating up the view tree.
		 * \return The command queue to operate on.
		 */
		virtual IUICommandQueue* getCommandQueue() const = 0;

		/*!
		 * \brief Execute the given UI command by raising it up the view tree.
		 * \param command The executed command.
		 */
		virtual void executeCommand(IUICommand* command) = 0;

	protected:

		//! \brief Initialize new instance.
		IView() { }

	};
}

#endif // IVIEW_H
