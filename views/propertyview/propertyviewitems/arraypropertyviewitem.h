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

#ifndef ARRAYPROPERTYVIEWITEM_H
#define ARRAYPROPERTYVIEWITEM_H

#include "genericpropertyviewitem.h"

#include <QFontDatabase>

namespace ysm
{
	/**
	 * @brief Base class for array properties
	 */
	template<typename T, typename S>
	class ArrayPropertyViewItem : public GenericPropertyViewItem<T, S>
	{
	public:
		// Construction
		explicit ArrayPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem<T, S>(property, parentView)
		{
			//Set label to property name.
			this->setLabel(new QLabel(property->getName()));

			//Create edit widget.
			const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

			_lineEdit = new QLineEdit();
			_lineEdit->setFont(fixedFont);

			this->setEdit(_lineEdit);

			//Check if property is readonly.
			_lineEdit->setReadOnly(property->isReadOnly());

			//Connect to Slots
			this->connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
		}

	public:
		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		S getViewValue() const Q_DECL_OVERRIDE
		{
			QString text = _lineEdit->text();

			text.replace(",", "|");
			text.replace(" ", "|");

			T prop;
			prop.fromString(text);

			return prop.getValue();
		}

	protected:
		//! \brief The model value changed, update the view.
		void updateViewValue() Q_DECL_OVERRIDE
		{
			S val = this->getModelValue();
			T prop; prop.setValue(val);
			QString text = prop.toString();

			text.replace("|", ",");

			_lineEdit->setText(text);
			_lineEdit->setReadOnly(this->getProperty()->isReadOnly());
		}

	private:
		//! \brief The line edit.
		QLineEdit* _lineEdit;
	};

	// Array property view items
	typedef ArrayPropertyViewItem<FloatDataProperty, FloatData> FloatDataPropertyViewItem;
	typedef ArrayPropertyViewItem<IntDataProperty, IntData> IntDataPropertyViewItem;
	typedef ArrayPropertyViewItem<UIntDataProperty, UIntData> UIntDataPropertyViewItem;
}

#endif // ARRAYPROPERTYVIEWITEM_H
