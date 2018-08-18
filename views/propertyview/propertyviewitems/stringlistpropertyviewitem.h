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

#ifndef STRINGLISTPROPERTYVIEW_H
#define STRINGLISTPROPERTYVIEW_H

#include "data/properties/property.h"
#include "genericpropertyviewitem.h"

#include <QCheckBox>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>

namespace ysm
{
	//! \brief String list editor for string list properties.
	class StringListPropertyViewItem : public GenericPropertyViewItem<StringListProperty, QStringList>
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		explicit StringListPropertyViewItem(IProperty* property, IView* parentView);

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		QStringList getViewValue() const Q_DECL_OVERRIDE;

	protected:

		//! \brief The model value changed, update the view.
		void updateViewValue() Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Adds a new entry.
		void addEntry();

		//! \brief Deletes an entry.
		void deleteEntry();

	private:

		//! \brief The table model.
		QStringListModel* _tableModel;

		//! \brief The table view.
		QListView* _listView;

        //! \brief The add button
        QPushButton* addButton;

        //! \brief the delete button
        QPushButton* deleteButton;

	};
}

#endif // STRINGLISTPROPERTYVIEW_H
