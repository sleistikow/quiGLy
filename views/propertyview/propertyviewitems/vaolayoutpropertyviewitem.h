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

#ifndef VAOLAYOUTPROPERTYVIEW_H
#define VAOLAYOUTPROPERTYVIEW_H

#include "data/properties/property.h"
#include "data/properties/vaolayoutproperty.h"
#include "tabbedpropertyviewitem.h"
#include "views/common/enumcombobox.h"
#include "views/common/connectioncombobox.h"

#include <QCheckBox>
#include <QTableView>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSignalMapper>
#include <QPushButton>

namespace ysm
{
	//! \brief Tab entry, contains all widget provided by a tab.
	struct VaoLayoutPropertyViewItemTab
	{
		ConnectionComboBox* _connection;
		QSpinBox* _index;
		QLineEdit* _name;
		QCheckBox* _normalized;
		QSpinBox* _offset;
		QSpinBox* _size;
		QSpinBox* _stride;
		EnumComboBox<GLDataType>* _type;
		EnumComboBox<GLSLDataType>* _glslType;
	};

	//! \brief VAO layouter for VAO layout properties.
	class VaoLayoutPropertyViewItem :
			public TabbedPropertyViewItem<VaoLayoutProperty, VaoLayout, VaoLayoutPropertyViewItemTab>
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		explicit VaoLayoutPropertyViewItem(IProperty* property, IView* parentView);

	protected:

		//! \brief Returns the number of entries.
		int getEntryCount() Q_DECL_OVERRIDE;

		/*!
		 * \brief Creates a new tab.
		 * \param layout The layout to operate on.
		 * \param entry The entry.
		 * \returns The tab item.
		 */
		VaoLayoutPropertyViewItemTab* createTab(QFormLayout* layout, int entry) Q_DECL_OVERRIDE;

		/*!
		 * \brief Updates an existing tab.
		 * \param item The tab item.
		 * \param layout The layout to operate on.
		 * \param entry The entry.
		 */
		void updateTab(VaoLayoutPropertyViewItemTab* item, int entry) Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Adds a new entry.
		void addEntry();

		//! \brief Deletes an entry.
		void deleteEntry();

		//! \brief Deletes an entry.
		void autoConfigure();

		//! \brief The view value changed, update the model.
		void updateModelValue() Q_DECL_OVERRIDE;

	private:

		//! \brief Internally used signal mapper.
		QSignalMapper* _signalMapper;
	};
}

#endif // VAOLAYOUTPROPERTYVIEW_H
