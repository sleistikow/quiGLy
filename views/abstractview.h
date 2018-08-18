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

#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include "iview.h"

namespace ysm
{

	//! \brief Basic implementation of the view interface.
	//! Provides changeable document and active item. See the IView interface documentation for more information.
	class AbstractView : public IView
	{

	private:

		//! \brief Possible notification types.
		enum NotificationType
		{
			Static,
			Dynamic,
			All,
		};

		//! \brief Non-generic base interface for resets.
		class IReset
		{
		public:

			//! \brief Initialize new instance.
			IReset() { }

			//! \brief Destruct instance.
			virtual ~IReset() { }

			/*!
			 * \brief Execute the actual notification handler, if the object applies to it.
			 * \param document The document that was reset.
			 */
			virtual void notify(Document* document) const = 0;
		};

		//! \brief Non-generic base interface for notifications.
		class INotification
		{
		public:

			//! \brief Initialize new instance.
			INotification() { }

			//! \brief Destruct instance.
			virtual ~INotification() { }

			/*!
			 * \brief Execute the actual notification handler, if the object applies to it.
			 * \param document The document that was changed.
			 * \param changeable The object that was changed.
			 * \param operation The operation.
			 */
			virtual void notify(Document* document, IChangeable* changeable,
								IChangeable::Operation operation) const = 0;
		};

		//! \brief Special reset interface.
		template<typename V> class Reset : public IReset
		{
		public:

			/*!
			 * \brief Initialize new instance.
			 * \param type The notification type.
			 * \param instance The instance to call the slot on.
			 * \param slot The slot.
			 */
			Reset(NotificationType type, V* instance, void (V::*slot)()) :
				_type(type),
				_instance(instance),
				_slot(slot)
			{ _document = instance->getActiveDocument(); }

			/*!
			 * \brief Execute the actual notification handler, if the object applies to it.
			 * \param document The document that was reset.
			 */
			void notify(Document* document) const Q_DECL_OVERRIDE
			{
				//Check if document matches.
				if(_type == Static && _document != document) return;
				if(_type == Dynamic && _instance->getActiveDocument() != document) return;

				//Execute the reset handler.
				(_instance->*_slot)();
			}

		private:

			//! \brief The notification type.
			NotificationType _type;

			//! \brief The instance to execute the notification on.
			V* _instance;

			//! \brief The notification to execute.
			void (V::*_slot)();

			//! \brief Document if static.
			Document* _document;
		};

		//! \brief Special notification interface.
		template<typename T, typename V> class Notification : public INotification
		{
		public:

			/*!
			 * \brief Initialize new instance.
			 * \param type The notification type.
			 * \param operation The operation to watch.
			 * \param instance The instance to call the slot on.
			 * \param slot The slot.
			 */
			Notification(NotificationType type, IChangeable::Operation operation, V* instance, void (V::*slot)(T*)) :
				_operation(operation),
				_type(type),
				_instance(instance),
				_slot(slot)
			{ _document = instance->getActiveDocument(); }

			/*!
			 * \brief Execute the actual notification handler, if the object applies to it.
			 * \param document The document that was changed.
			 * \param changeable The object that was changed.
			 * \param operation The operation.
			 */
			void notify(Document* document, IChangeable* changeable,
						IChangeable::Operation operation) const Q_DECL_OVERRIDE
			{
				//Check if document matches.
				if(_type == Static && _document != document) return;
				if(_type == Dynamic && _instance->getActiveDocument() != document) return;

				//Check if the operation matches.
				if(operation == _operation)
				{
					//Try to cast, to check for type matches.
					T* converted = dynamic_cast<T*>(changeable);
					if(converted)
						(_instance->*_slot)(converted);
				}
			}

		private:

			//! \brief The operation to look for.
			IChangeable::Operation _operation;

			//! \brief The notification type.
			NotificationType _type;

			//! \brief The instance to execute the notification on.
			V* _instance;

			//! \brief The notification to execute.
			void (V::*_slot)(T*);

			//! \brief Document if static.
			Document* _document;
		};

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent view.
		 */
		AbstractView(IView* parent);

		//! \brief Destruct instance.
		virtual ~AbstractView();

		/*!
		 * \brief Get the parent view.
		 * \return The parent view.
		 */
		IView* getParentView() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Sets a new parent view.
		 * \param parentItem The parent view.
		 */
		void setParentView(IView* parentView) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the active document.
		 * \return The view's active document.
		 */
		Document* getActiveDocument() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Set the active document.
		 * Note: To change the behavior on active document change, override the update methods rather than this one.
		 * \param document The view's active document.
		 */
		void setActiveDocument(Document* document) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the active pipeline item.
		 * \return The view's active pipeline item.
		 */
		IPipelineItem* getActiveItem() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Sets the active pipeline item.
		 * Note: To change the behavior on active item change, override the update methods rather than this one.
		 * \param pipelineItem The view's active pipeline item.
		 */
		void setActiveItem(IPipelineItem* pipelineItem) Q_DECL_OVERRIDE;

		/*!
		 * \brief Add a new child view.
		 * \param childView The new child view.
		 */
		void addChildView(IView* childView) Q_DECL_OVERRIDE;

		/*!
		 * \brief Remove a child view.
		 * \param childView The child view.
		 */
		void removeChildView(IView* childView) Q_DECL_OVERRIDE;

		/*!
		 * \brief Retrieve the command queue.
		 * \return The command queue to operate on.
		 */
		IUICommandQueue* getCommandQueue() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Execute the given UI command.
		 * \param command The command.
		 */
		void executeCommand(IUICommand* command) Q_DECL_OVERRIDE;

		/*!
		 * \brief Called whenever the underlying data might have changed.
		 * \param document The changed document.
		 * \param operation changeOperation executed.
		 * \param changedObjects List of changed data objects (ignored for reset operation).
		 */
		void updateView(Document* document, const QList<IChangeable*>& changedObjects,
						IChangeable::Operation operation) Q_DECL_OVERRIDE;

		/*!
		 * \brief Called whenever the underlying data might have changed, but the change cannot be exactly defined.
		 * \param document The changed document.
		 */
		void resetView(Document* document) Q_DECL_OVERRIDE;

		/*!
		 * \brief Serialize the view and it's children recursively.
		 * \param xmlElement The XML element.
		 * \param context The context.
		 */
		void serialize(QDomElement* xmlElement, SerializationContext* context) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Deserialize the view and it's children recursively.
		 * \param xmlElement The XML element.
		 * \param context The context.
		 */
		void deserialize(const QDomElement* xmlElement, SerializationContext* context) Q_DECL_OVERRIDE;

	protected:

		//! \brief Called whenever the selected document might have changed.
		virtual void updateDocument() = 0;

		//! \brief Called whenever the selected item might have changed.
		virtual void updateItem() = 0;

		/*!
		 * \brief Returns wether updates are currently blocked (setActive... is not allowed).
		 * This must be called in subclassed event handlers that react to active item/document changes.
		 * If the updates are blocked, the handler must not be executed.
		 * \return True, if setActive... is not allowed.
		 */
		bool tryBlockUpdates();

		//! \brief Must be called to unblock the updates after a successful tryBlockUpdates call.
		void unblockUpdates();

		/*!
		 * \brief Connects to a specific change operation on objects of type T in the document active BY NOW.
		 * \param operation The change operation that is executed.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename T, typename V>
		void notifyStatic(IChangeable::Operation operation, V* instance, void (V::*slot)(T*))
		{
			//Just create and append the notification.
			_notifications.append(new Notification<T, V>(Static, operation, instance, slot));
		}

		/*!
		 * \brief Connects to a reset operation in the document active BY NOW.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename V>
		void notifyStatic(V* instance, void(V::*slot)())
		{
			//Just create and append the reset.
			_resets.append(new Reset<V>(Static, instance, slot));
		}

		/*!
		 * \brief Connects to a specific change operation on objects of type T in the document active BY THEN.
		 * \param operation The change operation that is executed.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename T, typename V>
		void notifyDynamic(IChangeable::Operation operation, V* instance, void (V::*slot)(T*))
		{
			//Just create and append the notification.
			_notifications.append(new Notification<T, V>(Dynamic, operation, instance, slot));
		}

		/*!
		 * \brief Connects to a reset operation in the document active BY THEN.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename V>
		void notifyDynamic(V* instance, void(V::*slot)())
		{
			//Just create and append the reset.
			_resets.append(new Reset<V>(Dynamic, instance, slot));
		}

		/*!
		 * \brief Connects to a specific change operation on objects of type T in any document.
		 * \param operation The change operation that is executed.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename T, typename V>
		void notifyAll(IChangeable::Operation operation, V* instance, void (V::*slot)(T*))
		{
			//Just create and append the notification.
			_notifications.append(new Notification<T, V>(All, operation, instance, slot));
		}

		/*!
		 * \brief Connects to a specific change operation on objects of type T in any document.
		 * \param instance The instance to execute the slot on.
		 * \param slot The method to execute on operation. Will be executed for every changed object.
		 */
		template<typename V>
		void notifyAll(V* instance, void(V::*slot)())
		{
			//Just create and append the reset.
			_resets.append(new Reset<V>(All, instance, slot));
		}

	protected:

		//! \brief The active document.
		Document* _activeDocument;

		//! \brief The active item.
		IPipelineItem* _activeItem;

	private:

		//! \brief The parent view.
		IView* _parentView;

		//! \brief The child views.
		QList<IView*> _childViews;

		//! \brief List of all notifications sent.
		QList<INotification*> _notifications;

		//! \brief List of all resets sent.
		QList<IReset*> _resets;

		//! \brief Avoids call cycles during internal update.
		bool _blockUpdates;
	};

}

#endif // ABSTRACTVIEW_H
