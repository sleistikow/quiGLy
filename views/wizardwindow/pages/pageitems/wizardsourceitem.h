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

#ifndef WIZARDSOURCEITEM_H
#define WIZARDSOURCEITEM_H

#include "../collections/wizardsourceselect.h"
#include "../delegates/wizardprefixdelegate.h"
#include "wizarddynamicitem.h"

#include <QCheckBox>
#include <QGroupBox>

namespace ysm
{

	//! \brief Dynamic item that consists of multiple items itself.
	//! Note that the parent page must implement the delegate interface.
	class WizardSourceItem : public WizardDynamicItem, public WizardPrefixDelegate
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param collection The collection that manages the item.
		 * \param element The root DOM element.
		 * \param layout The layout where to attach.
		 */
		WizardSourceItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout);

		/*!
		 * \brief Check if the item is complete, if it's mandatory.
		 * \return True, if complete.
		 */
		bool isComplete() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Checks if the control is enabled
		 * \return True, if enabled.
		 */
		bool isEnabled() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Enabled the control.
		 * \param isEnabled True, if control should be enabled.
		 */
		void setEnabled(bool isEnabled) Q_DECL_OVERRIDE;

	public:

		/*!
		 * \brief Checks wether the item is mandatory.
		 * \return True, if user needs to fill the item before he can proceed.
		 */
		bool isMandatory() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief The widget's completion state changed.
		void notifyCompleteChanged() Q_DECL_OVERRIDE;

		//! \brief Reset the field value.
		void resetValue() Q_DECL_OVERRIDE;

	private:

		//! \brief Root element of the source file.
		QDomElement _sourceRoot;

		//! \brief The source select.
		WizardSourceSelect* _sourceSelect;

		//! \brief The parent collection.
		WizardItemCollection* _parent;
	};

}

#endif // WIZARDSOURCEITEM_H
