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

#ifndef PORTLIST_H
#define PORTLIST_H

#include "data/pipeline/pipelineitemlist.h"
#include "porttype.h"

namespace ysm
{
	class Block;
	class Port;

	class PortList : public PipelineItemList<Port>
	{
	public:
		// Construction
		explicit PortList(Pipeline* pipeline, Block* parent);

	public:
		/**
		 * @brief Adds an existing port to the list
		 * If a port with the same type already exists, the passed port will be deleted and the existing one
		 * will be returned.
		 */
		Port* addPort(Port* port);

		/**
		 * @brief Creates a new port of type @p type in direction @p dir and adds it to the list
		 * If no port could be created, an exception is thrown.
		 * @return The newly created port
		 */
		Port* newPort(const PortType type, const PortDirection dir, const QString& name = "");

		/**
		 * @brief Finds the port with the given @p type (if one exists)
		 */
		Port* findPort(const PortType type) const;

		/**
		 * @brief Retrieves all ports in direction @p dir
		 */
		QVector<Port*> findPorts(const PortDirection dir) const;

	public:
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		Block* _block{nullptr};
	};
}

#endif
