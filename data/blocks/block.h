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

#ifndef BLOCK_H
#define BLOCK_H

#include "data/pipeline/pipelineitem.h"
#include "data/iblock.h"

namespace ysm
{
	class PortList;

	/**
	 * @brief Represents a single block in a pipeline
	 */
	class Block : public PipelineItem<IBlock>
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Undefined};

	public:
		// Construction
		explicit Block(Pipeline* parent, const BlockType type, const QString& name = "");
		virtual ~Block();

	public:
		// Initialization
		/**
		 * @brief Initializes this block (creates properties and ports etc.)
		 * If something goes wrong during initialization, an exception is thrown.
		 */
		virtual void initBlock();

	public:
		// Object access
		/**
		 * @brief Access to the block's port list
		 */
		PortList* getPortList();

		// Visitors
		/**
		 * @brief Takes a visitor and iterates through all pipelines, blocks, ports and connections
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public: // IBlock
		// General attributes
		BlockType getType() const override;

		// Connections
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

		QVector<IConnection*> getInConnections() const override;
		QVector<IConnection*> getOutConnections() const override;

		// Ports
		QVector<IPort*> getInPorts() const override;
		QVector<IPort*> getOutPorts() const override;
		IPort* getPort(PortType type) override;

	public: // ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	signals:
		/**
		 * @brief Emitted when this block has been connected to another block
		 * This signal is always emitted for both involved blocks
		 */
		void blockConnected(Connection* con);

		/**
		 * @brief Emitted when this block has been disconnected from another block
		 * This signal is always emitted for both involved blocks
		 */
		void blockDisconnected(Connection* con);

		/**
		 * @brief Emitted before this block will be removed
		 */
		void blockRemoved();

	protected:
		/**
		 * @brief Creates all ports of this block
		 * Any existing ports will be removed when calling this function.
		 * Throws an exception when something goes wrong.
		 */
		virtual void createPorts();

	protected:
		// Connection helpers
		using ConnectionPoints = QVector<QPair<BlockType, PortType>>;

		/**
		 * @brief Checks whether @p port is a valid connection point
		 * A connection point is a specific port of a specific block (represented by BlockType/PortType pair).
		 */
		bool checkConnectionPoints(IPort* port, const ConnectionPoints& validPoints);

	protected slots:
		/**
		 * @brief Called when a new connection to or from this block has been established
		 */
		virtual void prepareConnection(Connection* con);

	private:
		/**
		 * @brief Disables all connection events on this block
		 */
		void disableConnectionEvents();

	protected:
		BlockType _type{BlockType::Undefined};

	protected:
		PortList* _ports{nullptr};
	};
}

#endif
