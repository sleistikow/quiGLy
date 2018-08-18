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

#ifndef WIZARDSOURCESELECT_H
#define WIZARDSOURCESELECT_H

#include "wizarditemcollection.h"

#include <QSet>

namespace ysm
{
	class WizardComboBoxItem;

	//! \brief Provides a source selection item.
	class WizardSourceSelect : public WizardItemCollection
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param type The source's type name.
		 * \param disable Wether the source can be disabled.
		 * \param parent The parent window.
		 */
		WizardSourceSelect(QString type, bool disable, WizardWindow* parent);

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

		/*!
		 * \brief Checks wether the source is currently diabled.
		 * \return True, if source is disabled.
		 */
		bool isDisabled() const;

		/*!
		 * \brief Checks wether the source is currently referencing.
		 * \return True, if source is referencing another source.
		 */
		bool isReferencing() const;

	public slots:

		//! \brief Initialize the dynamic items.
		void initializeItems() Q_DECL_OVERRIDE;

		//! \brief Update dynamic items.
		void updateItems() Q_DECL_OVERRIDE;

	private:

		//! \brief The soruce's type name.
		QString _type;

		//! \brief The disabled option.
		QString _disabled;

		//! \brief True, if source must be specified.
		bool _hasDisableOption;

		//! \brief List of all available references.
		QSet<QString> _references;

		//! \brief The primary combo box.
		WizardComboBoxItem* _comboBox;
	};

}

#endif // WIZARDSOURCESELECT_H
