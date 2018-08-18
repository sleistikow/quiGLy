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

#ifndef PORT_H
#define PORT_H

#include "data/pipeline/pipelineitem.h"
#include "data/iport.h"
#include "block.h"

namespace ysm
{
	class Connection;
	class ConnectionList;

	/**
	 * @brief Represents a block port (connection points between two blocks)
	 */
	class Port : public PipelineItem<IPort>
	{
		Q_OBJECT

	public:
		// Construction
		explicit Port(Pipeline* pipeline, Block* parent, const PortType type, const PortDirection dir, const QString& name = "");
		virtual ~Port();

	public:
		// Connections
		/**
		 * @brief Access to the port's connection list
		 */
		ConnectionList* getConnectionList();

		/**
		 * @brief Retrieves all connections to/from this port (depending on the given direction)
		 */
		QVector<Connection*> assembleConnections(const PortDirection dir, const BlockType blockType = BlockType::Undefined) const;

		/**
		 * @brief Connects this port to the destination port; if the connection fails, an exception will be thrown
		 */
		Connection* connectTo(Port* dest);

		/**
		 * @brief Removes an existing connection to the destination port
		 * If the connection could not be removed, an exception is thrown.
		 */
		void disconnectFrom(Port* dest, bool keepObject = false);

		/**
		 * @brief Removes all connections where this port is the connection source or destination
		 * This function is called when a port gets deleted so that no orphaned connections will exist.
		 */
		void disconnectFromAll();

		/**
		 * @brief Checks whether this port is connected to a certain port of a specific block
		 */
		bool isConnectedTo(const BlockType blockType, const PortType portType, const PortDirection dir = PortDirection::Both) const;

		/**
		 * @brief Gets the number of connections from/to this port (depending on the port direction)
		 */
		unsigned int getConnectionCount() const;

		// Visitors
		/**
		 * @brief Takes a visitor and iterates through all pipelines, blocks, ports and connections
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public:
		// General attributes
		/**
		 * @brief Gets the port direction
		 */
		PortDirection getDirection() const;

		/**
		 * @brief Sets the port direction
		 */
		void setDirection(const PortDirection dir);

		// Connections
		QVector<IConnection*> getInConnections() const override;
		QVector<IConnection*> getOutConnections() const override;

	public: // IVersionable
		unsigned int getMinimumVersion() const override;
		bool isSupported() const override;

		/**
		 * @brief Sets the minimum version for this port
		 */
		void setMinimumVersion(const unsigned int minVersion);

	public: // IPort
		// General attributes
		Block* getBlock() override;

		PortType getType() const override;

		bool isIn() const override;
		bool isOut() const override;

	public: // ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	public:
		bool canRename() const override;

	private:
		Block* _block{nullptr};

		ConnectionList* _connections{nullptr};

	private:
		PortType _type{PortType::Undefined};
		PortDirection _direction{PortDirection::Dry};

		unsigned int _minimumVersion{DEFAULT_MINIMUM_VERSION};
	};
}

#endif
