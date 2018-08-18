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

#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include "data/pipeline/pipelineitemlist.h"

namespace ysm
{
	class Connection;
	class Port;

	class ConnectionList : public PipelineItemList<Connection>
	{
	public:
		// Construction
		explicit ConnectionList(Pipeline* pipeline, Port* parent);

	public:
		// Connection handling
		/**
		 * @brief Adds an existing connection to the list
		 */
		void addConnection(const Connection* con);

		/**
		 * @brief Creates a new connection from @p _port to the given destination port @p dest
		 * If no connection could be created, an exception is thrown.
		 * @return The newly created connection
		 */
		Connection* addConnectionTo(Port* dest);

		/**
		 * @brief Removes a connection to the given destination port @p dest (there can always only be one such connection)
		 * If the connection could not be removed, an exception is thrown.
		 */
		void removeConnectionTo(Port* dest, bool keepObject = false);

		/**
		 * @brief Searches for a connection to the given destination port @p dest
		 * @return The connection to @p dest if one exists
		 */
		Connection* findConnection(const Port* dest) const;

	public: // ISerialization
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		Port* _port{nullptr};
	};
}

#endif
