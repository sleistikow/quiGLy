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

#ifndef VIEW_H
#define VIEW_H

#include "abstractview.h"
#include "commands/iuicommand.h"

#include <QList>

namespace ysm
{

	//! \brief Basic implementation of the view interface.
	//! Provides changeable document and active item.
	class View : public AbstractView
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent view.
		 */
		View(IView* parent);

		//! \brief Destruct instance.
		virtual ~View();

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

		//! \brief Should be called at the end of subclasses constructors, before subview initialization.
		//! TODO: Maybe this can be done better.
		void initialize();
	};

}

#endif // VIEW_H
