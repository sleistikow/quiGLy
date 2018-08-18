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

#ifndef WIZARDNUMERICITEM_H
#define WIZARDNUMERICITEM_H

#include "wizarddynamicitem.h"
#include "../collections/wizarditemcollection.h"
#include "../../wizardwindow.h"

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

namespace ysm
{

	//! \brief Numeric item for dynamic wizard pages.
	template<typename T, typename S> class WizardNumericItem : public WizardDynamicItem
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param collection The collection that manages the item.
		 * \param element The root DOM element.
		 * \param layout The layout where to attach.
		 */
		WizardNumericItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout) :
			WizardDynamicItem(element, collection)
		{
			//Create the item's widget.
			_widget = new T(collection->getWindow());
			_widget->setRange(std::numeric_limits<S>::lowest(), std::numeric_limits<S>::max());
			_widget->setToolTip(getDescription());

			//Register the new item.
			collection->getDelegate()->registerDynamicField(getName(), _widget, "value");
			connect(_widget, &T::editingFinished, collection, &WizardItemCollection::updateItems);
			layout->addRow(getLabel(collection->getWindow()), _widget);
		}

		/*!
		 * \brief Checks if the control is enabled
		 * \return True, if enabled.
		 */
		bool isEnabled() const Q_DECL_OVERRIDE { return _widget->isEnabled(); }

		/*!
		 * \brief Enabled the control.
		 * \param isEnabled True, if control should be enabled.
		 */
		void setEnabled(bool isEnabled) Q_DECL_OVERRIDE { _widget->setEnabled(isEnabled); }

		//! \brief Reset the field value.
		void resetValue() Q_DECL_OVERRIDE
		{
			//Ensure value is only reset if neccessary.
			S defaultValue = static_cast<S>(getDefault().toDouble());
			if(defaultValue != _widget->value())
				_widget->setValue(defaultValue);
		}

	private:

		//! \brief The internal widget.
		T* _widget;
	};

}

#endif // WIZARDNUMERICITEM_H
