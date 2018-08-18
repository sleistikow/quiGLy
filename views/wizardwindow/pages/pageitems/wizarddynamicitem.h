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

#ifndef WIZARDDYNAMICITEM_H
#define WIZARDDYNAMICITEM_H

#include "../wizarddependencyobject.h"

#include <QMap>
#include <QVariant>
#include <QDomElement>
#include <QLabel>

namespace ysm
{
	class WizardWindow;

	//! \brief Base interface for dynamic page items.
	class WizardDynamicItem : public QObject, public WizardDependencyObject
	{
		Q_OBJECT

	protected:

		/*!
		 * \brief Initialize new instance.
		 * \param element The element's DOM representation.
		 * \param parent The parent
		 */
		WizardDynamicItem(QDomElement element, QObject* parent);

	public:

		//! \brief Destruct instance.
		virtual ~WizardDynamicItem();

		/*!
		 * \brief Returns the field's name.
		 * \return The field's name.
		 */
		QString getName() const;

		/*!
		 * \brief Returns the element's caption.
		 * \return The caption.
		 */
		QString getCaption() const;

		/*!
		 * \brief Returns the default value.
		 * \return The default value.
		 */
		QString getDefault() const;

		/*!
		 * \brief Returns the advanced description.
		 * \return The description.
		 */
		QString getDescription() const;

	public:

		/*!
		 * \brief Checks wether the item is mandatory.
		 * \return True, if user needs to fill the item before he can proceed.
		 */
		virtual bool isMandatory() const;

		/*!
		 * \brief Check if the item is complete, if it's mandatory.
		 * \return True, if complete.
		 */
		virtual bool isComplete() const;

		/*!
		 * \brief True, if the widget wants to stretch over full height.
		 * \return True to stretch.
		 */
		virtual bool isStretching() const;

		/*!
		 * \brief Checks if the control is enabled
		 * \return True, if enabled.
		 */
		virtual bool isEnabled() const = 0;

		/*!
		 * \brief Enabled the control.
		 * \param isEnabled True, if control should be enabled.
		 */
		virtual void setEnabled(bool isEnabled) = 0;

		//! \brief Reset the field value.
		virtual void resetValue() = 0;

	protected:

		/*!
		 * \brief Creates a default label for the item.
		 * \param parent The parent widget.
		 * \return The label.
		 */
		QLabel* getLabel(WizardWindow* parent) const;

	private:

		//! \brief True if field is mandatory.
		bool _mandatory;

		//! \brief The represented field's name.
		QString _field;

		//! \brief The element's caption.
		QString _caption;

		//! \brief The default value.
		QString _default;

		//! \brief The description.
		QString _description;
	};

}

#endif // WIZARDDYNAMICITEM_H
