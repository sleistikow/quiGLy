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

#ifndef ENUMCOMBOBOX_H
#define ENUMCOMBOBOX_H

#include "views/viewitem.h"

#include <QComboBox>

namespace ysm
{
	//! \brief Specialized combo box to select enum property values.
	template<typename T> class EnumComboBox : public QComboBox
	{
	public:

		//! \brief Initialize new instance.
		EnumComboBox(QMap<T, QString> sourceMap)
		{
			//Iterate over the map and add the items.
			foreach(T value, sourceMap.keys())
				addItem(sourceMap[value], QVariant(static_cast<int>(value)));
		}

	public:

		/*!
		 * \brief Set the selected item.
		 * \param value The value to select.
		 */
		void setCurrentEnum(T value)
		{
			//Block the signals, the change must not be propagated.
			blockSignals(true);

			//Find the appropriate item.
			setCurrentIndex(-1);
			for(int i = 0; i < count(); i++)
				if(itemData(i).toInt() == static_cast<int>(value))
					setCurrentIndex(i);

			//Unblock the signals.
			blockSignals(false);
		}

		/*!
		 * \brief Get the currently selected item's data or NULL.
		 * \return The item's data.
		 */
		T getCurrentEnum() const
		{
			//Retrieve the current data.
			//TODO: Re-add default value.
			return static_cast<T>(currentIndex() < 0 ? 0 : currentData().toInt());
		}
	};
}

#endif
