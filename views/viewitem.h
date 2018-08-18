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

#ifndef VIEWITEM_H
#define VIEWITEM_H

#include "abstractview.h"

#include <Qt>

namespace ysm
{
	class Document;
	class IPipelineItem;

	//! \brief Specialization of the view interface, can be used for all views that do NOT change it's document.
	//! A view item is e.g. a single tab of a view, where the view displays one tab for each document.
	class ViewItem : public AbstractView
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentView The parent view.
		 */
		ViewItem(IView* parentView);

		//! \brief Destruct instance.
		virtual ~ViewItem();

		/*!
		 * \brief Checks wether the active document should be inherited and updated by the parent.
		 * \return True, if the parent should forward active document changes.
		 */
		bool inheritsActiveDocument() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Checks wether the active item should be inherited and updated by the parent.
		 * \return True, if the parent should forward active item changes.
		 */
		bool inheritsActiveItem() const Q_DECL_OVERRIDE;

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Should be called at the end of subclasses constructors, before subview construction.
		 * TODO: Maybe this can be done better.
		 * \param activeDocument Active document, if NULL parent document will be used.
		 */
		void initialize(Document* activeDocument = 0);
	};

}

#endif // VIEWITEM_H
