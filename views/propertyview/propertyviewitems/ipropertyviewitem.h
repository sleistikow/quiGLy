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

#ifndef IPROPERTYVIEW_H
#define IPROPERTYVIEW_H

#include "data/iproperty.h"
#include <QWidget>

namespace ysm
{
	class PipelineItemPropertyView;

	//! \brief Base interface for property view item widgets.
	class IPropertyViewItem
	{
	public:

		//! \brief Destruct instance.
		virtual ~IPropertyViewItem() { }

		/*!
		 * \brief Returns the label widget.
		 * \return The widget, or null.
		 */
		virtual QWidget* getLabel() const = 0;

		/*!
		 * \brief Returns the edit widget.
		 * \return The widget.
		 */
		virtual QWidget* getEdit() const = 0;

		/*!
		 * \brief If true, the item view stretches to available size.
		 * \return True, if stretches.
		 */
		virtual bool isStretching() const = 0;

		/*!
		 * \brief Gets the parent property view.
		 * \return The property view.
		 */
		virtual PipelineItemPropertyView* getPropertyView() const = 0;

		/*!
		 * \brief Sets the parent property view.
		 * \param propertyView The property view.
		 */
		virtual void setPropertyView(PipelineItemPropertyView* propertyView) = 0;

		//! \brief Updates the corresponding view.
		virtual void updateView() = 0;

	protected:

		//! \brief Initialize new instance.
		IPropertyViewItem() { }
	};
}

#endif // IPROPERTYVIEW_H
