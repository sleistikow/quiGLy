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

#ifndef WIZARDDYNAMICWIDGET_H
#define WIZARDDYNAMICWIDGET_H

#include "../delegates/iwizarddynamicdelegate.h"
#include "../wizarditemfactory.h"
#include "../../wizardfile.h"

#include <QWizardPage>
#include <QDomElement>
#include <QFormLayout>
#include <QWidget>

namespace ysm
{
	class WizardWindow;
	class WizardDynamicItem;

	//! \brief Dynamically loads a configuration widget from XML and keeps track of the widgets.
	class WizardItemCollection : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param window The parent window.
		 */
		WizardItemCollection(WizardWindow* parent);

		//! \brief Destruct instance.
		virtual ~WizardItemCollection();

		/*!
		 * \brief Gets the parent window.
		 * \return The wizard window.
		 */
		WizardWindow* getWindow() const;

		/*!
		 * \brief Gets the control's delegate.
		 * \return The delegate.
		 */
		IWizardDynamicDelegate* getDelegate() const;

		/*!
		 * \brief Sets the control's delegate.
		 * \param delegate The delegate.
		 */
		void setDelegate(IWizardDynamicDelegate* delegate);

		/*!
		 * \brief Gets the overall enabled state.
		 * \return True, if the items are enabled.
		 */
		bool isEnabled() const;

		/*!
		 * \brief Sets the enabled state on the items.
		 * \param isEnabled True, if enabled.
		 */
		void setEnabled(bool isEnabled);

	public:

		/*!
		 * \brief Retrieve a list of all items.
		 * \return The items.
		 */
		QList<WizardDynamicItem*> getItems() const;

		/*!
		 * \brief Retrieve a specific item by name.
		 * \param name The item's name.
		 * \return The item.
		 */
		WizardDynamicItem* getItem(const QString& name) const;

	public:

		/*!
		 * \brief Generate the layout from the given DOM element.
		 * \param layout The layout to fill.
		 * \param rootElement The root DOM element.
		 * \param isChild True, if this is a recursive call.
		 * \return True, if the layout want to stretch over full height.
		 */
		virtual bool generateLayout(QFormLayout* layout, QDomElement rootElement, bool isChild = false);

		/*!
		 * \brief Checks wether the widget is complete.
		 * \return True, if widget is complete.
		 */
		virtual bool isComplete() const;

	public slots:

		//! \brief Initialize the dynamic items.
		virtual void initializeItems();

		//! \brief Update dynamic items.
		virtual void updateItems();

	private:

		//! \brief The item factory.
		WizardItemFactory _itemFactory;

		//! \brief All items in the widget by it's name.
		QMap<QString, WizardDynamicItem*> _dynamicItems;

		//! \brief The delegate.
		IWizardDynamicDelegate* _delegate;

		//! \brief The parent window.
		WizardWindow* _window;

		//! \brief Overall enabled state.
		bool _isEnabled;
	};

}

#endif // WIZARDDYNAMICWIDGET_H
