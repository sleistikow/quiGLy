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

#ifndef WIZARDOUTPUTSELECT_H
#define WIZARDOUTPUTSELECT_H

#include "wizarditemcollection.h"

#include <QCheckBox>

namespace ysm
{
	//! \brief Provides an output selection item.
	class WizardOutputSelect : public WizardItemCollection
	{
		Q_OBJECT

	public:

		//! \brief Constant to access the enabled field.
		static const QString EnabledField;

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent window.
		 */
		WizardOutputSelect(WizardWindow* parent);

		/*!
		 * \brief Check wether output is enabled by default.
		 * \return True, if default.
		 */
		bool isDefault() const;

		/*!
		 * \brief Set wether output is enabled by default.
		 * \param isEnabled True, if default.
		 */
		void setDefault(bool isEnabled);

	public:

		/*!
		 * \brief Generate the layout from the given DOM element.
		 * \param layout The layout to fill.
		 * \param rootElement The root DOM element.
		 * \param isChild True, if this is a recursive call.
		 * \return True, if the layout want to stretch over full height.
		 */
		bool generateLayout(QFormLayout* layout, QDomElement rootElement, bool isChild = false) Q_DECL_OVERRIDE;

		/*!
		 * \brief Checks wether the widget is complete.
		 * \return True, if widget is complete.
		 */
		bool isComplete() const Q_DECL_OVERRIDE;

	public slots:

		//! \brief Initialize the dynamic items.
		void initializeItems() Q_DECL_OVERRIDE;

		//! \brief Update dynamic items.
		void updateItems() Q_DECL_OVERRIDE;

	private:

		//! \brief True, if output is enabled by default.
		bool _default;

		//! \brief Check box to enable output.
		QCheckBox* _enableOption;
	};

}

#endif // WIZARDOUTPUTSELECT_H
