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

#ifndef NUMERICPROPERTYVIEWITEM_H
#define NUMERICPROPERTYVIEWITEM_H

#include "data/properties/property.h"
#include "genericpropertyviewitem.h"

#include <QObject>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRegExpValidator>

namespace ysm
{
	//! \brief Spinbox for any type of numeric input.
	template<typename T, typename S, typename V, int M = 0, int N = 0> class NumericPropertyViewItem :
		public GenericPropertyViewItem<T, S>
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent view item.
		 */
		explicit NumericPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem<T,S>(property, parentView)
		{
			//Set label to property name.
			this->setLabel(new QLabel(property->getName()));

			//Create spin box, adjust limits if specified.
			_spinBox = new V();
			if(N != M)
				_spinBox->setRange(M, N);
			else
				_spinBox->setRange(std::numeric_limits<S>::lowest(), std::numeric_limits<S>::max());

			//Connect to signals.
			this->setEdit(_spinBox);

			//Check if property is readonly.
			if(property->isReadOnly())
				_spinBox->setReadOnly(true);

			this->connect(_spinBox, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
		}

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		virtual S getViewValue() const Q_DECL_OVERRIDE { return _spinBox->value(); }

	protected:

		//! \brief The model value changed, update the view.
		virtual void updateViewValue() Q_DECL_OVERRIDE
		{
			_spinBox->setValue(this->getModelValue());
			_spinBox->setReadOnly(this->getProperty()->isReadOnly());
		}

	protected:

		//! \brief The spin box.
		V* _spinBox;
	};

	//! \brief Spinbox for floating point input.
	template<typename T, typename S, int M = 0, int N = 0, int P = 2> class FloatingPropertyViewItem :
		public NumericPropertyViewItem<T, S, QDoubleSpinBox, M, N>
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent view item.
		 */
		explicit FloatingPropertyViewItem(IProperty* property, IView* parentView) :
			NumericPropertyViewItem<T, S, QDoubleSpinBox, M, N>(property, parentView)
		{
			//Adjust the decimals.
			this->_spinBox->setDecimals(P);
		}
	};

	//! \brief Template for integer property view items.
	typedef NumericPropertyViewItem<IntProperty, int, QSpinBox> IntPropertyViewItem;

	//! \brief Template for unsigned integer property view items.
	typedef NumericPropertyViewItem<UIntProperty, unsigned int, QSpinBox, 0, INT_MAX> UIntPropertyViewItem;

	//! \brief Template for floating point property view items.
	typedef FloatingPropertyViewItem<FloatProperty, float> FloatPropertyViewItem;
	typedef FloatingPropertyViewItem<DoubleProperty, double, 0, 0, 6> DoublePropertyViewItem;

	//! \brief Template for adjustable floating point property view items.
	template<int M, int N, int P = 4> using PreciseFloatPropertyViewItem = FloatingPropertyViewItem<FloatProperty, float, M, N, P>;
	template<int M, int N, int P = 8> using PreciseDoublePropertyViewItem = FloatingPropertyViewItem<DoubleProperty, double, M, N, P>;
}

#endif // NUMERICPROPERTYVIEWITEM_H
