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

#ifndef VECTORPROPERTYVIEWITEM_H
#define VECTORPROPERTYVIEWITEM_H

#include "genericpropertyviewitem.h"
#include "views/common/vectoredit.h"

namespace ysm
{
	/**
	 * @brief Base class for vector properties
	 */
	template<typename T, typename S, int N>
	class VectorPropertyViewItem : public GenericPropertyViewItem<T, S>
	{

	public:

		// Construction
		explicit VectorPropertyViewItem(IProperty* property, IView* parentView) :
			GenericPropertyViewItem<T, S>(property, parentView)
		{
			//Set label to property name.
			this->setLabel(new QLabel(property->getName()));

			//Create edit widget.
			_lineEdit = new VectorEdit(N);
			_lineEdit->setDivider(';');
			this->setEdit(_lineEdit);

			//Check if property is readonly.
			_lineEdit->setReadOnly(property->isReadOnly());

			//Connect to Slots
			this->connect(_lineEdit, SIGNAL(vectorChanged()), this, SLOT(updateModelValue()));
		}

	public:

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		S getViewValue() const Q_DECL_OVERRIDE
		{
			//Convert from variant.
			return S(_lineEdit->getVector());
		}

	protected:

		//! \brief The model value changed, update the view.
		void updateViewValue() Q_DECL_OVERRIDE
		{
			//Update the line edit.
			_lineEdit->setVector(this->getModelValue());
			_lineEdit->setReadOnly(this->getProperty()->isReadOnly());
		}

	private:

		//! \brief The vector edit.
		VectorEdit* _lineEdit;
	};

	// Vector property view items

	typedef VectorPropertyViewItem<Vec2Property, QVector2D, 2> Vec2PropertyViewItem;
	typedef VectorPropertyViewItem<Vec3Property, QVector3D, 3> Vec3PropertyViewItem;
	typedef VectorPropertyViewItem<Vec4Property, QVector4D, 4> Vec4PropertyViewItem;
}

#endif
