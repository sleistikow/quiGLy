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

#ifndef IBLOCK_H
#define IBLOCK_H

#include <QVector>
#include "ipipelineitem.h"
#include "blocks/blocktype.h"
#include "blocks/porttype.h"

namespace ysm
{
	class IConnection;
	class IPort;

	/// @brief Interface for all pipeline blocks.
	class IBlock : public IPipelineItem
	{
	public:

		/// @brief Destruct instance.
		virtual ~IBlock() { }

	public:

		/**
		 * @brief Gets the type of this block.
		 * @return The block type.
		 */
		virtual BlockType getType() const = 0;

		/**
		 * @brief Called to check if this block can accept a connection.
		 * @param src The source port.
		 * @param dest The destination port.
		 * @param denialReason The denial reason.
		 */
		virtual bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) = 0;

		/**
		 * @brief Retrieves all incoming connections to this block.
		 * @return List of all incoming connections.
		 */
		virtual QVector<IConnection*> getInConnections() const = 0;

		/**
		 * @brief Retrieves all outgoing connections from this block.
		 * @return List of all outgoing connections.
		 */
		virtual QVector<IConnection*> getOutConnections() const = 0;

		/**
		 * @brief Retrieves all ports that accept connections.
		 * @return List of all incoming ports.
		 */
		virtual QVector<IPort*> getInPorts() const = 0;

		/**
		 * @brief Retrieves all ports that can be connected to some other port.
		 * @param List of all outgoing ports.
		 */
		virtual QVector<IPort*> getOutPorts() const = 0;

		/**
		 * @brief Retrieves the port if exists (null otherwise).
		 * @param type The port type.
		 * @return The port or null.
		 */
		virtual IPort* getPort(PortType type) = 0;

	protected:

		/// @brief Initialize new instance.
		IBlock() { }
	};
}

#endif
