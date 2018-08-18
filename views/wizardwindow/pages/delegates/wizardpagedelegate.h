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

#ifndef WIZARDPAGEDELEGATE_H
#define WIZARDPAGEDELEGATE_H

#include "iwizarddynamicdelegate.h"

#include <QWizardPage>
#include <QVariant>

namespace ysm
{

	//! \brief Implementation of the dynamic delegate, that provides default behavior for pages.
	//! Note that the delegate also handles page dependencies, as it's the base class for all pages.
	class WizardPageDelegate : public QWizardPage, public IWizardDynamicDelegate
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent widget.
		 */
		WizardPageDelegate(QWidget* parent);

		//! \brief Destruct instance.
		~WizardPageDelegate();

	public:

		/*!
		 * \brief Grants public access to field registration.
		 * This method should simply forward to the wizard page's corresponding method.
		 * \param name The field's name.
		 * \param widget The field's widget.
		 * \param property The widget's property.
		 */
		void registerDynamicField(const QString& name, QWidget* widget, const char* property = NULL) Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the field's value.
		 * This method should simply forward to the wizard page's corresponding method.
		 * \param name The field's name.
		 * \return The field's value.
		 */
		QVariant dynamicField(const QString& name) const Q_DECL_OVERRIDE;

		//! \brief The widget's completion state changed.
		void notifyCompleteChanged() Q_DECL_OVERRIDE;

		/*!
		 * \brief Find the next available page.
		 * \return The next available page.
		 */
		int nextId() const Q_DECL_OVERRIDE;
	};

}

#endif // WIZARDPAGEDELEGATE_H
