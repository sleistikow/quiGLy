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

#ifndef WIZARDCOLLECTIONSWITCH_H
#define WIZARDCOLLECTIONSWITCH_H

#include "wizarditemcollection.h"
#include "wizardsourceselect.h"

#include <QComboBox>
#include <QStackedWidget>

namespace ysm
{
	//! \brief Dynamically loads multiple configuration widgets from XML and allows to switch between them.
	class WizardCollectionStack : public QObject
	{
		Q_OBJECT

	public:

		//! \brief Constant to access the type field.
		static const QString TypeField;

	public:

		//! \brief Helper structure, contains information about a single item type.
		struct ItemType
		{
			//! \brief The type's item collection.
			WizardItemCollection* _itemCollection;

			//! \brief The widget that contains the collection.
			QWidget* _collectionWidget;

			//! \brief The type's attributes.
			QString _name, _nameSource;
		};

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent window.
		 */
		WizardCollectionStack(WizardWindow* parent);

		//! \brief Destruct instance.
		virtual ~WizardCollectionStack();

		/*!
		 * \brief Gets the active item type.
		 * \return The item type.
		 */
		ItemType getCurrentType() const;

		/*!
		 * \brief Sets the active item type.
		 * \param type The item type.
		 */
		void setCurrentType(const QString& type);

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
		 * \brief Gets the item select caption.
		 * \return The item select caption.
		 */
		QString getCaption() const;

		/*!
		 * \brief Sets the item select caption.
		 * \param caption The item select caption.
		 */
		void setCaption(const QString& caption);

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
		 * \brief Generate the layout from the given DOM element.
		 * \param layout The layout to fill.
		 * \param rootElement The root DOM element.
		 */
		virtual void generateLayout(QFormLayout* layout, QDomElement rootElement);

		/*!
		 * \brief Checks wether the widget is complete.
		 * \return True, if widget is complete.
		 */
		virtual bool isComplete() const;

	public slots:

		//! \brief Initialize the dynamic items.
		virtual void initializeItems();

	signals:

		//! \brief Emitted when the current type changes.
		void currentTypeChanged();

	protected:

		/*!
		 * \brief Register a new item collection type.
		 * \param typeElement The XML element.
		 * \return The created widget.
		 */
		virtual QWidget* registerItemCollection(QDomElement typeElement);

	protected slots:

		//! \brief Type combo box changed, update the widget.
		void updateCurrentType();

	private:

		//! \brief The type combo box.
		QComboBox* _typeSelect;

		//! \brief The type select caption.
		QString _typeCaption;

		//! \brief The widget that holds the collection item widgets.
		QStackedWidget* _stackedWidget;

		//! \brief The item types by unique identifier.
		QMap<QString, ItemType> _itemTypes;

		//! \brief The delegate.
		IWizardDynamicDelegate* _delegate;

		//! \brief The parent window.
		WizardWindow* _window;
	};

}

#endif // WIZARDCOLLECTIONSWITCH_H
