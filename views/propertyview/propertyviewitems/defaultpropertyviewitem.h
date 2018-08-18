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

#ifndef DEFAULTPROPERTYVIEW
#define DEFAULTPROPERTYVIEW

#include "../../viewitem.h"
#include "genericpropertyviewitem.h"
#include "data/iproperty.h"

#include <QObject>
#include <QTextEdit>
#include <QLabel>

namespace ysm
{
	/*!
	 * \brief Failsafe property view for unknown property types.
	 * Can not be derived from generic base class, because the property type is not known.
	 */
	class DefaultPropertyViewItem : public QObject, public ViewItem, public IPropertyViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		DefaultPropertyViewItem(IProperty* property, IView* parentView);

		/*!
		 * \brief Returns the current model value.
		 * \return The value.
		 */
		QString getModelValue() const;

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		QString getViewValue() const;

		/*!
		 * \brief Returns the label widget.
		 * \return The widget, or null.
		 */
		QWidget* getLabel() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the edit widget.
		 * \return The widget.
		 */
		QWidget* getEdit() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the parent property view.
		 * \return The property view.
		 */
		PipelineItemPropertyView* getPropertyView() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Sets the parent property view.
		 * \param propertyView The property view.
		 */
		void setPropertyView(PipelineItemPropertyView* propertyView) Q_DECL_OVERRIDE;

		/*!
		 * \brief If true, the item view stretches to available size.
		 * \return True, if stretches.
		 */
		bool isStretching() const Q_DECL_OVERRIDE;

		/**
		 * @brief Update this view by notification.
		 * @param property The changed item.
		 */
		void updateView(IChangeable* property);

		//! \brief Update this view and all child views.
		void updateView() Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Updates the underlying model.
		void updateModel();

	private:

		//! \brief The parent property view.
		PipelineItemPropertyView* _propertyView;

		//! \brief The underlying property.
		IProperty* _property;

		//! \brief The line edit.
		QLineEdit* _lineEdit;

		//! \brief The label.
		QLabel* _label;

		//! \brief Flag to prevent call cycles.
		bool _synchronizingData;
	};

}
#endif // DEFAULTPROPERTYVIEW

