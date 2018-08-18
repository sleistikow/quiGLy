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

#ifndef STANDARDPROPERTYVIEW_H
#define STANDARDPROPERTYVIEW_H

#include "../../viewitem.h"
#include "../../document.h"
#include "../pipelineitempropertyview.h"
#include "ipropertyviewitem.h"
#include "data/iproperty.h"
#include "data/properties/property.h"
#include "commands/pipeline/change/updatepropertiescommand.h"
#include "commands/iuicommandqueue.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFontMetrics>

namespace ysm
{
	class PipelineItemPropertyView;

	//! \brief Contains slots for the generic property view item class.
	class GenericPropertyViewObject : public QObject, public ViewItem, public IPropertyViewItem
	{
		Q_OBJECT

	protected:

		/*!
		 * \brief Initialize new instance.
		 * \param parentView The parent item.
		 */
		GenericPropertyViewObject(IView* parentView) :
			ViewItem(parentView)
		{ }

	protected slots:

		//! \brief The view value changed, update the model.
		virtual void updateModelValue() = 0;

	};

	//! \brief Generic base class for all property views.
	template<typename T, typename S> class GenericPropertyViewItem : public GenericPropertyViewObject
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param stretches If true, the tab widget stretches to full available size.
		 * \param parentView The parent item.
		 */
		explicit GenericPropertyViewItem(IProperty* property, bool stretches, IView* parentView) :
			GenericPropertyViewObject(parentView),
			_property(dynamic_cast<T*>(property)),
			_synchronizingData(false),
			_stretches(stretches)
		{
			//Connect to the model.
			notifyStatic(IChangeable::Change, this, &GenericPropertyViewItem::updateView);
		}

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		explicit GenericPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem(property, false, parentView)
		{ }

		/*!
		 * \brief Returns the current model value.
		 * \return The value.
		 */
		virtual S getModelValue() const { return _property->getValue(); }

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		virtual S getViewValue() const = 0;

		/*!
		 * \brief Returns the label widget.
		 * \return The widget, or null.
		 */
		QWidget* getLabel() const Q_DECL_OVERRIDE { return _label; }

		/*!
		 * \brief Returns the edit widget.
		 * \return The widget.
		 */
		QWidget* getEdit() const Q_DECL_OVERRIDE { return _edit; }

		/*!
		 * \brief If true, the item view stretches to available size.
		 * \return True, if stretches.
		 */
		bool isStretching() const Q_DECL_OVERRIDE { return _stretches; }

		/*!
		 * \brief Gets the underlying property.
		 * \return The property.
		 */
		T* getProperty() const { return _property; }

		/*!
		 * \brief Gets the parent property view.
		 * \return The property view.
		 */
		PipelineItemPropertyView* getPropertyView() const Q_DECL_OVERRIDE { return _propertyView; }

		/*!
		 * \brief Sets the parent property view.
		 * \param propertyView The property view.
		 */
		void setPropertyView(PipelineItemPropertyView* propertyView) Q_DECL_OVERRIDE
		{
			//Set the new property view, then update.
			_propertyView = propertyView;
			updateView();
		}

		/**
		 * @brief Update this view by notification.
		 * @param property The changed item.
		 */
		void updateView(IChangeable* property)
		{
			//If the property matches update the view.
			if(_property == property)
				updateView();
		}

		//! \brief Update this view and all child views.
		void updateView() Q_DECL_OVERRIDE
		{
			//Avoid call cycles.
			if(!_synchronizingData && _propertyView)
			{
				_synchronizingData = true;
				updateViewValue();
				_synchronizingData = false;
			}
		}

	protected:

		//! \brief The view value changed, update the model.
		void updateModelValue() Q_DECL_OVERRIDE
		{
			//Avoid call cycles.
			if(!_synchronizingData && getModelValue() != getViewValue())
			{
				_synchronizingData = true;
				executeCommand(new UpdatePropertiesCommand(_property->getOwner(), _property, getViewValue()));
				_synchronizingData = false;
			}
		}

		//! \brief The model value changed, update the view.
		virtual void updateViewValue() = 0;

		/*!
		 * \brief Sets the label widget.
		 * \param labelWidget The label widget.
		 */
		void setLabel(QLabel* labelWidget)
		{
			//If a label is set, adjust it's appearance.
			_label = labelWidget;
			if(_label)
			{
				//Adjust the label to match default size.
				_label->setMinimumWidth(100);

				//Shorten the label text if neccessary.
				QString caption = _label->text();
				QFontMetrics metrics(_label->font());
				caption = metrics.elidedText(caption, Qt::ElideMiddle, 94);

				//Check if label was adjusted.
				if(caption != _label->text())
				{
					//Show debug for such labels, so they can be adjusted.
					qDebug() << "Label too long: " << _label->text();
					_label->setText(caption);
				}
			}
		}

		/*!
		 * \brief Sets the edit widget.
		 * \param editWidget The edit widget.
		 */
		void setEdit(QWidget* editWidget) { _edit = editWidget; }

	private:

		//! \brief The parent property view.
		PipelineItemPropertyView* _propertyView;

		//! \brief The underlying property.
		T* _property;

		//! \brief The label widget.
		QLabel* _label;

		//! \brief The edit widget.
		QWidget* _edit;

		//! \brief Flag to prevent call cycles.
		bool _synchronizingData;

		//! \brief Defines the stretching policy.
		bool _stretches;
	};
}

#endif // STANDARDPROPERTYVIEW_H
