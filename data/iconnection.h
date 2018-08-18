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

#ifndef ICONNECTION_H
#define ICONNECTION_H

#include "ipipelineitem.h"

namespace ysm
{
	class IPort;
	class IBlock;

	/// @brief Interface for all connection items.
	class IConnection : public IPipelineItem
	{
	public:

		/// @brief Destruct instance.
		virtual ~IConnection() { }

	public:

		/**
		 * @brief Gets the source port.
		 * @return The source port.
		 */
		virtual IPort* getSourcePort() = 0;

		/**
		 * @brief Gets the destination port.
		 * @return The destination port.
		 */
		virtual IPort* getDestPort() = 0;

		/**
		 * @brief Gets the source block.
		 * @param The source block.
		 */
		virtual IBlock* getSource() = 0;

		/**
		 * @brief Gets the destination block.
		 * @return The destination block.
		 */
		virtual IBlock* getDest() = 0;

	protected:

		/// @brief Initialize new instance.
		IConnection() { }
	};
}

#endif
