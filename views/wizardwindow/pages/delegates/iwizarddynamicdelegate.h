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

#ifndef IWIZARDDYNAMICDELEGATE_H
#define IWIZARDDYNAMICDELEGATE_H

#include <QWidget>

namespace ysm
{

	//! \brief Delegate class for the widget, that provides access to required methods.
	class IWizardDynamicDelegate
	{

	protected:

		//! \brief Initialize new instance.
		IWizardDynamicDelegate() { }

	public:

		//! \brief Destruct instance.
		virtual ~IWizardDynamicDelegate() { }

		/*!
		 * \brief Grants public access to field registration.
		 * \param name The field's name.
		 * \param widget The field's widget.
		 * \param property The widget's property.
		 */
		virtual void registerDynamicField(const QString& name, QWidget* widget, const char* property = NULL) = 0;

		/*!
		 * \brief Returns the field's value.
		 * \param name The field's name.
		 * \return The field's value.
		 */
		virtual QVariant dynamicField(const QString& name) const = 0;

		/*!
		 * \brief Returns the prefixed name.
		 * \param name The name.
		 * \return The prefixed name.
		 */
		virtual QString dynamicName(const QString& name) const = 0;

		//! \brief The widget's completion state changed.
		virtual void notifyCompleteChanged() = 0;
	};

}

#endif // IWIZARDDYNAMICDELEGATE_H
