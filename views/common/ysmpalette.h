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

#ifndef PIPELINEPALETTE_H
#define PIPELINEPALETTE_H

#include "data/iblock.h"
#include "data/irendercommand.h"

#include <QColor>

namespace ysm
{

	/*!
	 * \brief Holds color information for the complete UI.
	 * Colors mainly taken from: http://www.flatuicolorpicker.com/
	 */
	class YSMPalette
	{

	public:

		/*!
		 * \brief Gets the color for the given block type.
		 * \param blockType The block type.
		 * \return The color.
		 */
		static QColor getBlockColor(BlockType blockType);

		/*!
		 * \brief Gets the color for the given render command type.
		 * \param commandType The render command type.
		 * \return The color.
		 */
		static QColor getRenderCommandColor(RenderCommandType commandType);

		/*!
		 * \brief Gets the color for the given item status.
		 * \param itemStatus The item status type.
		 * \return The color.
		 */
		static QColor getPipelineItemStatusColor(PipelineItemStatus itemStatus);

		/*!
		 * \brief Gets the default inactive color.
		 * \return The color.
		 */
		static QColor getInactiveColor();

	private:

		//! \brief No initialization required, static class.
		YSMPalette();
	};

}

#endif // PIPELINEPALETTE_H
