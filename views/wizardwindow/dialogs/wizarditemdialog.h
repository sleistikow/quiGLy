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

#ifndef WIZARDITEMDIALOG_H
#define WIZARDITEMDIALOG_H

#include "../pages/collections/wizarditemcollection.h"
#include "../pages/collections/wizardcollectionstack.h"
#include "../pages/delegates/iwizarddynamicdelegate.h"
#include "../pages/delegates/wizardforwardingdelegate.h"

#include <QDialog>
#include <QComboBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>

namespace ysm
{
	//! \brief Displays a dynamic dialog that show multiple item collections.
	class WizardItemDialog : public QDialog, public WizardForwardingDelegate
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param wizardWindow The wizard window.
		 */
		WizardItemDialog(WizardWindow* wizardWindow);

	public:

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
		 * \brief Gets the item's name.
		 * \return The name.
		 */
		QString getName() const;

		/*!
		 * \brief Sets the item's name.
		 * \return name The name.
		 */
		void setName(const QString& name);

	public:

		/*!
		 * \brief Generate the layout from the given DOM element.
		 * \param rootElement The root DOM element.
		 */
		void generateLayout(QDomElement rootElement);

	public slots:

		//! \brief The widget's completion state changed.
		void notifyCompleteChanged() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Generate the widget that contains the buttons.
		 * \return The created widget.
		 */
		QWidget* generateButtons();
		
	private:

		//! \brief True, if the item can be automatically named.
		bool _autoName;

		//! \brief The name edit.
		QLineEdit* _nameEdit;

		//! \brief The buttons.
		QPushButton* _okButton;
		QPushButton* _cancelButton;

		//! \brief The main item stack.
		WizardCollectionStack* _itemSwitch;

		//! \brief The parent window.
		WizardWindow* _parent;
	};

}

#endif // WIZARDITEMDIALOG_H
