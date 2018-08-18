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

#ifndef BOOLPROPERTYVIEW_H
#define BOOLPROPERTYVIEW_H

#include "data/properties/property.h"
#include "genericpropertyviewitem.h"

#include <QCheckBox>

namespace ysm
{
	//! \brief Checkbox for boolean properties.
	class BoolPropertyViewItem : public GenericPropertyViewItem<BoolProperty, bool>
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		explicit BoolPropertyViewItem(IProperty* property, IView* parentView);

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		bool getViewValue() const Q_DECL_OVERRIDE;

	protected:

		//! \brief The model value changed, update the view.
		void updateViewValue() Q_DECL_OVERRIDE;

	private:

		//! \brief The check box.
		QCheckBox* _checkBox;

	};
}

#endif // BOOLPROPERTYVIEW_H
