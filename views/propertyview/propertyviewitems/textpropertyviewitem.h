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

#ifndef NUMBERPROPERTYVIEW_H
#define NUMBERPROPERTYVIEW_H

#include "data/properties/property.h"
#include "genericpropertyviewitem.h"

#include <QLineEdit>
#include <QRegExpValidator>

namespace ysm
{
	//! \brief Textbox for any type of text input, validated by regex.
	template<typename T, typename S, const char* R> class TextPropertyViewItem : public GenericPropertyViewItem<T, S>
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent view item.
		 */
		explicit TextPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem<T,S>(property, parentView)
		{
			//Set label to property name.
			this->setLabel(new QLabel(property->getName()));

			//Create edit and connect signal.
			_lineEdit = new QLineEdit();
			_lineEdit->setValidator(new QRegExpValidator(QRegExp(R)));
			this->setEdit(_lineEdit);

			//Check if property is readonly.
            _lineEdit->setReadOnly(property->isReadOnly());

            this->connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(updateModelValue()));
		}

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		virtual S getViewValue() const Q_DECL_OVERRIDE
		{
			//Use stream to read type independent.
			S value;
			QString string = _lineEdit->text();
			QTextStream(&string) >> value;
			return value;
		}

	protected:

		//! \brief The model value changed, update the view.
		virtual void updateViewValue() Q_DECL_OVERRIDE
		{
			//Use stream to write type independent.
			S value = this->getModelValue();
			QString string;
			QTextStream(&string) << value;

			_lineEdit->setText(string);
            _lineEdit->setReadOnly(this->getProperty()->isReadOnly());
		}

	private:

		//! \brief The line edit.
		QLineEdit* _lineEdit;
	};

	//! \brief Template for single character property view items.
	extern "C" const char _CharPropertyViewItem[];
	typedef TextPropertyViewItem<CharProperty, QChar, _CharPropertyViewItem> CharPropertyViewItem;

	//! \brief Template for string property view items.
	extern "C" const char _StringPropertyViewItem[];
	typedef TextPropertyViewItem<StringProperty, QString, _StringPropertyViewItem> StringPropertyViewItem;
}

#endif // NUMBERPROPERTYVIEW_H
