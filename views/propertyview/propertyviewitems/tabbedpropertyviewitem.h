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

#ifndef TABBEDPROPERTYVIEWITEM_H
#define TABBEDPROPERTYVIEWITEM_H

#include "data/properties/property.h"
#include "genericpropertyviewitem.h"

#include <QTabWidget>
#include <QFormLayout>
#include <QPushButton>
#include <QDebug>

namespace ysm
{
	//! \brief Tabbed property view item template.
	template<typename T, typename S, typename V> class TabbedPropertyViewItem :
		public GenericPropertyViewItem<T, S>
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param stretches If true, the tab widget stretches to full available size.
		 * \param parentView The parent item.
		 */
		TabbedPropertyViewItem(IProperty* property, IView* parentView) :
			TabbedPropertyViewItem(property, false, parentView)
		{ }

		/*!
		 * \brief Initialize new instance.
		 * \param property The property.
		 * \param stretches If true, the tab widget stretches to full available size.
		 * \param parentView The parent item.
		 */
		explicit TabbedPropertyViewItem(IProperty* property, bool stretches, IView* parentView) :
			GenericPropertyViewItem<T, S>(property, stretches, parentView)
		{
			//No label.
			this->setLabel(NULL);

			//Create the tab widget.
			_tabWidget = new QTabWidget();

			//Create the edit widget.
			QWidget* editWidget = new QWidget();
			_layout = new QGridLayout(editWidget);
			this->setEdit(editWidget);

			//Adjust the vertical stretch.
			if(stretches)
			{
				//Edit widget.
				QSizePolicy sizePolicy = editWidget->sizePolicy();
				sizePolicy.setVerticalStretch(1);
				editWidget->setSizePolicy(sizePolicy);

				//Tab widget.
				sizePolicy = _tabWidget->sizePolicy();
				sizePolicy.setVerticalStretch(1);
				_tabWidget->setSizePolicy(sizePolicy);
			}

			//Adjust the layout.
			_layout->addWidget(_tabWidget, 0, 0, 1, 2, static_cast<Qt::Alignment>(stretches ? 0 : Qt::AlignTop));
			_layout->setSizeConstraint(QLayout::SetNoConstraint);
			_layout->setColumnStretch(0, 1);
			_layout->setColumnStretch(1, 1);
			_layout->setMargin(0);
		}

		/*!
		 * \brief Returns the current view value.
		 * \return The value.
		 */
		virtual S getViewValue() const Q_DECL_OVERRIDE { return _cachedValue; }

	protected:

		//! \brief Returns the number of entries.
		virtual int getEntryCount() = 0;

		/*!
		 * \brief Creates a new tab.
		 * \param layout The layout to operate on.
		 * \param entry The entry.
		 * \returns The tab item.
		 */
		virtual V* createTab(QFormLayout* layout, int entry) = 0;

		/*!
		 * \brief Adds an additional widget to the outer grid.
		 * \param widget The widget.
		 * \param row The row.
		 * \param col The column or -1 to span all columns.
		 */
		void addWidget(QWidget* widget, int row, int col = -1)
		{
			//Just add the widget.
			_layout->addWidget(widget, row, col > -1 ? col : 0, 1, col > -1 ? 1 : 2, Qt::AlignTop);
		}

		/*!
		 * \brief Updates an existing tab.
		 * \param item The tab item.
		 * \param entry The entry.
		 */
		virtual void updateTab(V* item, int entry) = 0;

		//! \brief The model value changed, update the view.
		void updateViewValue() Q_DECL_OVERRIDE
		{
			//Reload cached value.
			_cachedValue = this->getModelValue();

			//Remove obsolete tabs.
			for(int i = _tabWidget->count(); i > getEntryCount(); i--)
			{
				_tabWidget->removeTab(i - 1);
				_tabItems.removeAt(i - 1);
			}

			//Update remaining tabs.
			for(int i = 0; i < _tabWidget->count(); i++)
				updateTab(_tabItems[i], i);

			//Add missing tabs.
			for(int i = _tabWidget->count(); i < getEntryCount(); i++)
			{
				//Create the new tab item.
				QWidget* entryWidget = new QWidget();
				QFormLayout* formLayout = new QFormLayout(entryWidget);
				formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
				formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

				_tabWidget->addTab(entryWidget, (this->getProperty()->getName() + " %1").arg(i + 1));
				_tabItems.append(createTab(formLayout, i));
			}
		}

		/*!
		 * \brief Returns the interally cached value, to change it.
		 * \return Reference to the value.
		 */
		S& getCachedValue() { return _cachedValue; }

		/*!
		 * \brief Set the cached value.
		 * \param cachedValue The cached value.
		 */
		void setCachedValue(S& cachedValue) { _cachedValue = cachedValue; }

		/*!
		 * \brief Returns the item at the given index.
		 * \param entry The entry.
		 * \return The item.
		 */
		V* getItem(int entry) const { return _tabItems.value(entry); }

		/*!
		 * \brief Gets the current entry.
		 * \return The tab widget.
		 */
		int getCurrentEntry() const { return _tabWidget->currentIndex(); }

		//! \brief Switches to the last tab (when adding new items)
		void switchToLastTab() { _tabWidget->setCurrentIndex(_tabWidget->count() - 1); }

	private:

		//! \brief The tab widget.
		QTabWidget* _tabWidget;

		//! \brief Contains all information about the tabs.
		QList<V*> _tabItems;

		//! \brief The outer layout.
		QGridLayout* _layout;

		//! \brief The cached view value.
		S _cachedValue;
	};
}

#endif // TABBEDPROPERTYVIEWITEM_H
