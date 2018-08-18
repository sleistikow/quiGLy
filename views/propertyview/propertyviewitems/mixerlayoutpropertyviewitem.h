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

#ifndef MIXERLAYOUTPROPERTYVIEW_H
#define MIXERLAYOUTPROPERTYVIEW_H

#include "data/properties/property.h"
#include "data/properties/mixerlayoutproperty.h"
#include "tabbedpropertyviewitem.h"
#include "views/common/connectioncombobox.h"
#include "views/common/enumcombobox.h"
#include "views/common/swizzlebox.h"

#include <QRadioButton>
#include <QTableView>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSignalMapper>
#include <QPushButton>

namespace ysm
{
	//! \brief Tab entry, contains all widgets provided by a tab.
	struct MixerLayoutPropertyViewItemTab
	{
		ConnectionComboBox* _connection;
		QLineEdit* _name;
		EnumComboBox<DataType>* _conversion;
		SwizzleBox* _swizzling;
	};

	//! \brief Mixer layouter for mixer layout properties.
	class MixerLayoutPropertyViewItem :
			public TabbedPropertyViewItem<MixerLayoutProperty, MixerLayout, MixerLayoutPropertyViewItemTab>
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param parentView The parent item.
		 */
		explicit MixerLayoutPropertyViewItem(IProperty* property, IView* parentView);

	protected:

		//! \brief Returns the number of entries.
		int getEntryCount() Q_DECL_OVERRIDE;

		/*!
		 * \brief Creates a new tab.
		 * \param layout The layout to operate on.
		 * \param entry The entry.
		 * \returns The tab item.
		 */
		MixerLayoutPropertyViewItemTab* createTab(QFormLayout* layout, int entry) Q_DECL_OVERRIDE;

		/*!
		 * \brief Updates an existing tab.
		 * \param item The tab item.
		 * \param layout The layout to operate on.
		 * \param entry The entry.
		 */
		void updateTab(MixerLayoutPropertyViewItemTab* item, int entry) Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Adds a new entry.
		void addEntry();

		//! \brief Deletes an entry.
		void deleteEntry();

		//! \brief Automatically configures the Mixer layout.
		void autoConfigure();

		//! \brief The view value changed, update the model.
		void updateModelValue() Q_DECL_OVERRIDE;

	private:

		/*!
		 * \brief Returns the tab item's target type from connection.
		 * \param tabItem The tab item.
		 * \return The data type.
		 */
		DataType getConnectionDataType(const MixerLayoutPropertyViewItemTab* tabItem) const;

		/*!
		 * \brief Returns the tab item's source type from user selection.
		 * \param tabItem The tab item.
		 * \return The data type.
		 */
		DataType getSelectedDataType(const MixerLayoutPropertyViewItemTab* tabItem) const;

	private:

		//! \brief Layout as struct flag.
		QRadioButton* _layoutGrouped;
		QRadioButton* _layoutSequential;

		//! \brief Internally used signal mapper.
		QSignalMapper* _signalMapper;

		//! \brief Indicates whether we're adding a new item
		bool _addingNewItem{false};
	};
}

#endif // MIXERLAYOUTPROPERTYVIEW_H
