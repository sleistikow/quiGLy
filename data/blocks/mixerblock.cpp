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

#include "mixerblock.h"
#include "portlist.h"
#include "port.h"
#include "datasourceblock.h"
#include "uniforms/uniformbaseblock.h"
#include "data/properties/propertylist.h"
#include "data/properties/mixerlayoutproperty.h"
#include "data/types/bufferstreamingcontext.h"
#include "data/common/dataexceptions.h"
#include "data/iconnection.h"

namespace ysm
{
	MixerBlock::MixerBlock(Pipeline* parent) : Block(parent, block_type, "Mixer")
	{

	}

	void MixerBlock::initBlock()
	{
		Block::initBlock();

		// Connect to self to receive disconnection events
		connect(this, SIGNAL(blockDisconnected(Connection*)), this, SLOT(onConnectionRemoved(Connection*)));
	}

	void MixerBlock::streamToBuffer(BufferStreamingContext* ctx) const
	{
		MixerLayout layout{_mixerLayout->getValue()};
		bool getAsStruct = layout.getEntriesAsStruct();

		ctx->getBufferData()->clear();
		ctx->getBufferData()->append(getStreamData(ctx, getAsStruct, getAsStruct));
	}

	MixerLayoutProperty* MixerBlock::getMixerLayout()
	{
		return _mixerLayout;
	}

	Port* MixerBlock::getDataInPort()
	{
		return _dataInPort;
	}

	Port* MixerBlock::getDataOutPort()
	{
		return _dataOutPort;
	}

	void MixerBlock::createProperties()
	{
		Block::createProperties();

		_mixerLayout = _properties->newProperty<MixerLayoutProperty>(PropertyID::Mixer_Layout, "Layout");
	}

	void MixerBlock::createPorts()
	{
		Block::createPorts();

		_dataInPort = _ports->newPort(PortType::Data_In, PortDirection::In, "Data In");
		_dataOutPort = _ports->newPort(PortType::Data_Out, PortDirection::Out, "Data Out");
	}

    bool MixerBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _dataOutPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Mixer, PortType::Data_In);
            conPoints << qMakePair(BlockType::Buffer, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Mixer output must be connected to a Mixer or Buffer block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }

	void MixerBlock::onConnectionRemoved(Connection* con)
	{
		_mixerLayout->removeConnectionEntries(con);
	}

	const QByteArray MixerBlock::getNextBufferStreamEntry(BufferStreamingContext* ctx, const bool overrideAsStruct) const
	{
		MixerLayout layout{_mixerLayout->getValue()};
		bool getAsStruct = (layout.getEntriesAsStruct() || overrideAsStruct);
		QString layoutError;		
		QByteArray data;

		// Verify our layout first, so we don't need to do that when actually getting the data
		if (!layout.verifyLayout(layoutError))
			return data;

		// For performance reasons, we have separate implementations for struct and block data
		if (getAsStruct)
		{
			data = getNextBufferStreamEntry_Struct(ctx);
			ctx->advanceDataIndex(this);
		}
		else
			data = getNextBufferStreamEntry_Block(ctx);

		return data;
	}

	const QByteArray MixerBlock::getNextBufferStreamEntry_Struct(BufferStreamingContext* ctx) const
	{
		MixerLayout layout{_mixerLayout->getValue()};
		QVector<QByteArray> dataEntries;
		bool isDataEmpty = true;
		unsigned int totalDataSize = 0;

		// Each entry is first inserted into a vector of QByteArrays; this is needed to fill up missing entries in an otherwise non-empty struct
		// Afterwards, all entries are combined to form one single byte array
		for (const MixerLayout::MixerLayoutEntry& entry : *layout.getEntries())
		{
			QByteArray curData;
			unsigned int outputSize = 0;
			IBlock* block = entry.dataConnection->getSource();

			// See what type of block we're connected to
			if (dynamic_cast<MixerBlock*>(block))
				curData = retrieveData_Mixer(entry, ctx, true, &outputSize);
			else if (dynamic_cast<UniformBaseBlock*>(block))
				curData = retrieveData_Uniform(entry, ctx, &outputSize);
			else if (dynamic_cast<DataSourceBlock*>(block))
				curData = retrieveData_DataSource(entry, ctx, true, &outputSize);

			// Create an all-zero data entry when no data has been returned
			if (curData.size() == 0 && outputSize > 0)
				curData = QByteArray(outputSize, 0);
			else
				isDataEmpty = false;

			totalDataSize += curData.size();

			dataEntries.append(std::move(curData));
		}

		// Combine all data entries
		QByteArray data;

		if (!isDataEmpty && totalDataSize > 0)
		{
			data.reserve(totalDataSize);

			while (!dataEntries.isEmpty())
			{
				// Add first entry from dataEntries, remove it immediately afterwards (to restrain memory usage)
				data += std::move(dataEntries[0]);
				dataEntries.remove(0);
			}
		}

		return data;
	}

	const QByteArray MixerBlock::getNextBufferStreamEntry_Block(BufferStreamingContext* ctx) const
	{
		MixerLayout layout{_mixerLayout->getValue()};
		QByteArray data;

		for (const MixerLayout::MixerLayoutEntry& entry : *layout.getEntries())
		{
			IBlock* block = entry.dataConnection->getSource();

			if (dynamic_cast<MixerBlock*>(block))
				data += retrieveData_Mixer(entry, ctx, false, nullptr);
			else if (dynamic_cast<UniformBaseBlock*>(block))
				data += retrieveData_Uniform(entry, ctx, nullptr);
			else if (dynamic_cast<DataSourceBlock*>(block))
				data += retrieveData_DataSource(entry, ctx, false, nullptr);
		}

		return data;
	}

	const QByteArray MixerBlock::getStreamData(BufferStreamingContext* ctx, const bool getEntireData, const bool getAsStruct) const
	{
		QByteArray mixerData;

		if (getEntireData)
		{
			// Get stream entries from this mixer until ran dry
			while (true)
			{
				QByteArray data = getNextBufferStreamEntry(ctx, getAsStruct);

				if (data.length() == 0)
					break;

				mixerData.append(std::move(data));
			}
		}
		else
		{
			// Either get a single data entry or the entire data as a block
			mixerData = getNextBufferStreamEntry(ctx, getAsStruct);
		}

		return mixerData;
	}

	const QByteArray MixerBlock::retrieveData_Mixer(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, const bool getAsStruct, unsigned int* outputSize) const
	{
		MixerBlock* src = dynamic_cast<MixerBlock*>(layoutEntry.dataConnection->getSource());
		MixerLayout layout{src->_mixerLayout->getValue()};

		if (outputSize)			
			*outputSize = src->getOutputSize();

		return src->getStreamData(ctx, layout.getEntriesAsStruct() && !getAsStruct, getAsStruct);
	}

	const QByteArray MixerBlock::retrieveData_DataSource(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, const bool getAsStruct, unsigned int* outputSize) const
	{
		DataSourceBlock* src = dynamic_cast<DataSourceBlock*>(layoutEntry.dataConnection->getSource());

		if (!src)
			throw std::runtime_error{"Thought I had a data source connection, but I was wrong"};

		DataSource::DataSourceOutput output = src->findOutputByPort(layoutEntry.dataConnection->getSourcePort());

		if (outputSize)
			*outputSize = src->getOutputSize(output, &layoutEntry.typeConversion);

		QByteArray data;

		if (getAsStruct)
			data = src->retrieveOutput(output, layoutEntry.typeConversion, ctx->getCurrentDataIndex(this));
		else
			data = src->retrieveOutput(output, layoutEntry.typeConversion, -1);

		return data;
	}

	const QByteArray MixerBlock::retrieveData_Uniform(const MixerLayout::MixerLayoutEntry& layoutEntry, BufferStreamingContext* ctx, unsigned int* outputSize) const
	{
		Q_UNUSED(ctx);

		UniformBaseBlock* src = dynamic_cast<UniformBaseBlock*>(layoutEntry.dataConnection->getSource());

		if (!src)
			throw std::runtime_error{"Thought I had a uniform connection, but I was wrong"};

		if (outputSize)
			*outputSize = src->getOutputSize(layoutEntry.dataConnection->getSourcePort());

        if (ctx->getCurrentDataIndex(this) != 0)
            return QByteArray();

		return src->retrieveUniformData(layoutEntry.dataConnection->getSourcePort());
	}

	unsigned int MixerBlock::getOutputSize()
	{
		MixerLayout layout{_mixerLayout->getValue()};
		unsigned int size = 0;

		for (const MixerLayout::MixerLayoutEntry& entry : *layout.getEntries())
		{
			IBlock* block = entry.dataConnection->getSource();

			if (dynamic_cast<MixerBlock*>(block))
			{
				MixerBlock* src = dynamic_cast<MixerBlock*>(entry.dataConnection->getSource());

				size += src->getOutputSize();
			}
			else if (dynamic_cast<UniformBaseBlock*>(block))
			{
				UniformBaseBlock* src = dynamic_cast<UniformBaseBlock*>(entry.dataConnection->getSource());

				size += src->getOutputSize(entry.dataConnection->getSourcePort());
			}
			else if (dynamic_cast<DataSourceBlock*>(block))
			{
				DataSourceBlock* src = dynamic_cast<DataSourceBlock*>(entry.dataConnection->getSource());
				DataSource::DataSourceOutput output = src->findOutputByPort(entry.dataConnection->getSourcePort());

				size += src->getOutputSize(output, &entry.typeConversion);
			}
		}

		return size;
	}
}
