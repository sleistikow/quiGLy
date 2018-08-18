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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "data/pipeline/pipelineitem.h"
#include "data/iconnection.h"
#include "port.h"

namespace ysm
{
	/**
	 * @brief Represents a connection between two block ports
	 * Connections are always owned by their source ports. If a port gets deleted,
	 * any connections pointing to that port will be automatically deleted.
	 */
	class Connection : public PipelineItem<IConnection>
	{
		Q_OBJECT

	public:
		enum ConnectionSignalReceivers
		{
			SignalReceiver_None = 0,
			SignalReceiver_Source = 0x01,
			SignalReceiver_Target = 0x02,
			SignalReceiver_Both = SignalReceiver_Source|SignalReceiver_Target,
		};

	public:
		// Construction
		/**
		 * @param src The source connection port
		 * @param dest The destination connection port
		 */
		explicit Connection(Pipeline* parent, Port* src, Port* dest);

	public:
		// Connection events
		/**
		 * @brief Called when the connection has been established
		 */
		void connected();

		/**
		 * @brief Called when the connection has been removed
		 */
		void disconnected();

		/**
		 * @brief Gets which receivers should be notified about connection events
		 */
		int getConnectionEventsReceivers() const;

		/**
		 * @brief Sets which receivers should be notified about connection events
		 */
		void setConnectionEventsReceivers(int receivers);

		// Visitors
		/**
		 * @brief Takes a visitor and iterates through all pipelines, blocks, ports and connections
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public: // IConnection
		// Connected ports
		Port* getSourcePort() override;
		Port* getDestPort() override;

		// Connected blocks
		Block* getSource() override;
		Block* getDest() override;

	public: // ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;	

	public:
		bool canRename() const override;

	protected:
		Port* _sourcePort{nullptr};
		Port* _destPort{nullptr};

	private:
		int _connectionSignalReceivers{SignalReceiver_Both};
	};
}

#endif
