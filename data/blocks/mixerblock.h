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

#ifndef MIXERBLOCK_H
#define MIXERBLOCK_H

#include "block.h"
#include "data/types/mixerlayout.h"

namespace ysm
{
	class MixerLayoutProperty;
	class BufferStreamingContext;

	/**
	 * @brief The almighty Mixer block
	 */
	class MixerBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Mixer};

	public:
		// Construction
		explicit MixerBlock(Pipeline* parent);

	public:
		// Initialization
		void initBlock() override;

		// Data mixing
		/**
		 * @brief Streams the entire data this mixer produces into the streaming context @p ctx
		 * If something goes wrong, an exception is thrown
		 */
		void streamToBuffer(BufferStreamingContext* ctx) const;

		/**
		 * @brief Calculates the output size of a single data element
		 */
		unsigned int getOutputSize();

	public:
		// Property access
		/**
		 * @brief Gets the mixer layout
		 */
		MixerLayoutProperty* getMixerLayout();

		// Port access
		/**
		 * @brief Gets the data in-port
		 */
		Port* getDataInPort();

		/**
		 * @brief Gets the data out-port
		 */
		Port* getDataOutPort();

    public:
        bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	private slots:
		/**
		 * @brief Called when a connection to or from this block has been removed
		 */
		void onConnectionRemoved(Connection* con);

	private:
		// Buffer stream retrieval
		/**
		 * @brief Gets the data from this buffer
		 */
		const QByteArray getStreamData(BufferStreamingContext* ctx, const bool getEntireData, const bool getAsStruct) const;

		// Buffer entry retrieval
		/**
		 * @brief Fetches the next data entry when streaming data to a buffer
		 */
		const QByteArray getNextBufferStreamEntry(BufferStreamingContext* ctx, const bool overrideAsStruct) const;

		/**
		 * @brief Fetches the next data entry as a struct
		 */
		const QByteArray getNextBufferStreamEntry_Struct(BufferStreamingContext* ctx) const;

		/**
		 * @brief Fetches the next data entry as a block
		 */
		const QByteArray getNextBufferStreamEntry_Block(BufferStreamingContext* ctx) const;

		// Data retrieval
		/**
		 * @brief Get data from a mixer
		 */
		const QByteArray retrieveData_Mixer(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, const bool getAsStruct, unsigned int* outputSize) const;

		/**
		 * @brief Get data from a data source
		 */
		const QByteArray retrieveData_DataSource(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, const bool getAsStruct, unsigned int* outputSize) const;

		/**
		 * @brief Get data from a uniform
		 */
		const QByteArray retrieveData_Uniform(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, unsigned int* outputSize) const;		

	private:
		// Properties
		MixerLayoutProperty* _mixerLayout{nullptr};

		// Ports
		Port* _dataInPort{nullptr};
		Port* _dataOutPort{nullptr};			
	};
}

#endif
