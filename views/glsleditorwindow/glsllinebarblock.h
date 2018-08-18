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

#ifndef LINEBARBLOCK
#define LINEBARBLOCK

#include <QWidget>

namespace ysm
{
	//! \brief A line bar block represents a 'line' in the line bar.
	class GLSLLineBarBlock
	{
	public:

		//! \brief Initialize new instance.
		GLSLLineBarBlock();

		/*!
		 * \brief Initialize new instance.
		 * \param blockNumber The line number of this block.
		 * \param blockBoundingRect The bounding rect of this block.
		 */
		GLSLLineBarBlock(int blockNumber, const QRectF& blockBoundingRect);

		/*!
		 * \brief blockNumber Gets the line number of this block.
		 * \return The line number.
		 */
		int getBlockNumber() const;

		/*!
		 * \brief getBlockBoundingRect Gets the bounding rect of this block.
		 * \return The bounding rect.
		 */
		QRectF getBlockBoundingRect() const;

	private:

		//! \brief The line number of this block.
		int _blockNumber;

		//! \brief The bounding rect of this block.
		QRectF _blockBoundingRect;
	};
}

#endif // LINEBARBLOCK
