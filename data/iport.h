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

#ifndef IPORT_H
#define IPORT_H

#include <QVector>
#include "ipipelineitem.h"
#include "blocks/porttype.h"

namespace ysm
{
	class IConnection;
	class IBlock;

	/// @brief Interface for all ports.
	class IPort : public IPipelineItem
	{
	public:

		/// @brief Destruct instance.
		virtual ~IPort() { }

	public:

		/**
		 * @brief Retrieves all incoming connections to this port.
		 * @return List of all incoming connections.
		 */
		virtual QVector<IConnection*> getInConnections() const = 0;

		/**
		 * @brief Retrieves all outgoig connections from this port.
		 * @return List of all outgoing connections.
		 */
		virtual QVector<IConnection*> getOutConnections() const = 0;

	public:

		/**
		 * @brief Gets the block this port belongs to.
		 * @return The block.
		 */
		virtual IBlock* getBlock() = 0;

		/**
		 * @brief Gets the type (in/out/both) of this port.
		 * @return The type.
		 */
		virtual PortType getType() const = 0;

		/**
		 * @brief Checks if this port accepts incoming connections.
		 * @return True, if type is in or both.
		 */
		virtual bool isIn() const = 0;

		/**
		 * @brief Checks if this port can be connected to some other port.
		 * @return True, if type is out or both.
		 */
		virtual bool isOut() const = 0;

	protected:

		/// @brief Initialize new instance.
		IPort() { }
	};
}

#endif
