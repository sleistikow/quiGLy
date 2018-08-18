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

#ifndef ACTIVEMESHCOMBOBOX_H
#define ACTIVEMESHCOMBOBOX_H

#include "data/iblock.h"
#include "views/viewitem.h"

#include <QComboBox>

namespace ysm
{

	//! \brief Combo box that displays a selection of all meshes in one object file.
	class ActiveMeshComboBox : public QComboBox, public ViewItem
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param modelLoaderBlock The parent block, which must be a model loader block.
		 * \param parentView The parent view.
		 */
		ActiveMeshComboBox(IBlock* modelLoaderBlock, IView* parentView);

	protected:

		/*!
		 * \brief Property changed, check if combo box needs update.
		 * \param property The property.
		 */
		void updateComboBox(IProperty* property);

		//! \brief Update the combo box.
		void updateComboBox();

	private:

		//! \brief The parent block.
		IBlock* _block;
	};

}

#endif // ACTIVEMESHCOMBOBOX_H
