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

#ifndef MATRIXPROPERTYVIEWITEM_H
#define MATRIXPROPERTYVIEWITEM_H

#include "genericpropertyviewitem.h"

#include <QPlainTextEdit>

namespace ysm
{
	/**
	 * @brief Base class for matrix properties
	 */
	template<typename T, typename S, unsigned int rows>
	class MatrixPropertyViewItem : public GenericPropertyViewItem<T, S>
	{
	public:
		// Construction
		explicit MatrixPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem<T, S>(property, parentView)
		{
			//Set label to property name.
			this->setLabel(new QLabel(property->getName()));

			//Create edit widget.
			const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

			_textEdit = new QPlainTextEdit();
			_textEdit->setFont(fixedFont);

			QFontMetrics m(fixedFont);
			int RowHeight = m.lineSpacing() ;
			_textEdit->setFixedHeight((rows + 1) * RowHeight);

			this->setEdit(_textEdit);

			//Check if property is readonly.
			_textEdit->setReadOnly(property->isReadOnly());

			//Connect to Slots
			this->connect(_textEdit, SIGNAL(textChanged()), this, SLOT(updateModelValue()));
		}

	public:
		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		S getViewValue() const Q_DECL_OVERRIDE
		{
			QString text = _textEdit->toPlainText();

			text.replace("\n", "|");
			text.replace(" ", ";");

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

			text.replace(";", " ");
			text.replace("|", "\n");

			_textEdit->setPlainText(text);
			_textEdit->setReadOnly(this->getProperty()->isReadOnly());
		}

	private:
		//! \brief The line edit.
		QPlainTextEdit* _textEdit;
	};

	// Vector property view items

	typedef MatrixPropertyViewItem<Mat3x3Property, QMatrix3x3, 3> Mat3x3PropertyViewItem;
	typedef MatrixPropertyViewItem<Mat4x4Property, QMatrix4x4, 4> Mat4x4PropertyViewItem;
}

#endif
