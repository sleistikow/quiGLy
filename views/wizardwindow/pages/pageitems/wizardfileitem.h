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

#ifndef WIZARDFILEITEM_H
#define WIZARDFILEITEM_H

#include "../collections/wizarditemcollection.h"
#include "wizarddynamicitem.h"

#include "views/common/fileselect.h"

#include <QDomElement>
#include <QFormLayout>
#include <QLabel>

namespace ysm
{

	//! \brief File item for dynamic wizard pages.
	class WizardFileItem : public WizardDynamicItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param collection The collection that manages the item.
		 * \param element The root DOM element.
		 * \param layout The layout where to attach.
		 */
		WizardFileItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout);

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

		//! \brief Reset the field value.
		void resetValue() Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Browse for a file.
		void browseFile();

	private:

		//! \brief The internal widget.
		FileSelect* _widget;
	};

}


#endif // WIZARDFILEITEM_H
