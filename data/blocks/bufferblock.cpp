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

#include "bufferblock.h"
#include "port.h"
#include "portlist.h"
#include "connection.h"
#include "datasourceblock.h"
#include "data/properties/propertylist.h"
#include "data/properties/mixerlayoutproperty.h"
#include "data/types/bufferstreamingcontext.h"

namespace ysm
{
	BufferBlock::BufferBlock(Pipeline* parent, BlockType type, const QString& name, CachePool* pool) : Block(parent, type, name), CacheableObject(pool)
	{

	}

	BufferBlock::BufferBlock(Pipeline* parent) : Block(parent, block_type, "Buffer"), CacheableObject(parent->getManager()->getCachePool())
	{

	}

	ByteArrayProperty* BufferBlock::getBufferData() const
	{
		return _bufferData;
	}

	UIntProperty* BufferBlock::getBufferSize() const
	{
		return _bufferSize;
	}

	UIntProperty*BufferBlock::getBufferEntryCount() const
	{
		return _bufferEntryCount;
	}

	EnumProperty* BufferBlock::getUsageFrequency()
	{
		return _usageFrequency;
	}

	EnumProperty* BufferBlock::getUsageAccess()
	{
		return _usageAccess;
	}

	Port* BufferBlock::getDataInPort()
	{
		return _dataInPort;
	}

	Port* BufferBlock::getDataOutPort()
	{
		return _dataOutPort;
	}

	void BufferBlock::createProperties()
	{
		Block::createProperties();

		_bufferData = _properties->newProperty<ByteArrayProperty>(PropertyID::Buffer_Data, "Data", true);
		_bufferData->setSerializable(false);
		_bufferData->delegateValue(
					[this]()->const QByteArray& { return getBufferData()->data; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_bufferSize = _properties->newProperty<UIntProperty>(PropertyID::Buffer_Size, "Size", true);
		_bufferSize->setSerializable(false);
		_bufferSize->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = getBufferData()->data.size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_bufferEntryCount = _properties->newProperty<UIntProperty>(PropertyID::Buffer_EntryCount, "Entry Count", true);
		_bufferEntryCount->setSerializable(false);
		_bufferEntryCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = getBufferData()->elementCount; return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_usageFrequency = _properties->newProperty<EnumProperty>(PropertyID::Buffer_UsageFrequency, "Usage Frequency");
		*_usageFrequency = Usage_Static;

		_usageAccess = _properties->newProperty<EnumProperty>(PropertyID::Buffer_UsageAccess, "Usage Access");
		*_usageAccess = Usage_Draw;
	}

	void BufferBlock::createPorts()
	{
		Block::createPorts();

		_dataInPort = _ports->newPort(PortType::Data_In, PortDirection::In, "Data In");
		_dataOutPort = _ports->newPort(PortType::Data_Out, PortDirection::Out, "Data Out");
	}

	bool BufferBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _dataOutPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::VertexArrayObject, PortType::Data_In);
			conPoints << qMakePair(BlockType::VertexPuller, PortType::VertexPuller_IndexList);
			conPoints << qMakePair(BlockType::Shader_Fragment, PortType::Shader_SSBOIn);
			conPoints << qMakePair(BlockType::Shader_Fragment, PortType::Shader_SSBOOut);
			conPoints << qMakePair(BlockType::Shader_Fragment, PortType::Shader_UBO);
			conPoints << qMakePair(BlockType::Shader_Fragment, PortType::Shader_AtomicCounterIn);
			conPoints << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_SSBOIn);
			conPoints << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_SSBOOut);
			conPoints << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_UBO);
			conPoints << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_AtomicCounterIn);
			conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_SSBOIn);
			conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_SSBOOut);
			conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_UBO);
			conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_AtomicCounterIn);
			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_SSBOIn);
			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_SSBOOut);
			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_UBO);
			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_AtomicCounterIn);
			conPoints << qMakePair(BlockType::Shader_Geometry, PortType::Shader_SSBOIn);
			conPoints << qMakePair(BlockType::Shader_Geometry, PortType::Shader_SSBOOut);
			conPoints << qMakePair(BlockType::Shader_Geometry, PortType::Shader_UBO);
			conPoints << qMakePair(BlockType::Shader_Geometry, PortType::Shader_AtomicCounterIn);
			conPoints << qMakePair(BlockType::Texture, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Buffer output must be connected to a Vertex Array Object, Vertex Puller, Texture or any Shader block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	void BufferBlock::prepareConnection(Connection* con)
	{
		// If we're connecting to a Shader, add needed properties to connection
		if ((con->getSourcePort()->getType() == PortType::Data_Out && con->getDestPort()->getType() == PortType::Shader_SSBOIn)
			|| (con->getSourcePort()->getType() == PortType::Data_Out && con->getDestPort()->getType() == PortType::Shader_AtomicCounterIn)
			|| (con->getSourcePort()->getType() == PortType::Data_Out && con->getDestPort()->getType() == PortType::Shader_UBO)
			|| (con->getSourcePort()->getType() == PortType::Shader_SSBOOut && con->getDestPort()->getType() == PortType::Data_In))
		{
			StringProperty* name = con->getPropertyList()->newProperty<StringProperty>(PropertyID::Buffer_Name, "Name");
			*name = getName().replace(" ", "");

			UIntProperty* binding = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::Buffer_Binding, "Binding");
			*binding = 0;
		}
	}

	CacheObject::Key BufferBlock::getCacheKey(bool retrieveForeignKey)
	{
		Q_UNUSED(retrieveForeignKey);

		CacheObject::Key key;

		// Key consists of class qualifier + everything describing the layout of the buffer
		key = "BufferBlock/";

		// Get incoming connections; must be exactly one
		QVector<Connection*> inCons = _dataInPort->assembleConnections(PortDirection::In);

		if (inCons.size() == 1)
		{
			QString conKey;
			Block* block = inCons[0]->getSource();

			// Data is only buffered if not connected to a TFB
			if (block->getType() != BlockType::TransformFeedback)
			{
				// Process the connected block

				auto processMixer = [&](MixerBlock* mixer) {
					conKey = getCacheKey_Mixer(mixer);
				};

				auto processDataSource = [&](DataSourceBlock* dataSrc) {
					conKey = getCacheKey_DataSource(dataSrc);
				};

				auto processUniform = [&](UniformBaseBlock* uniform) {
					conKey = getCacheKey_Uniform(uniform);
				};

				processConnectedBlock(block, processMixer, processDataSource, processUniform);
			}
			else
				conKey = "TFB";

			key += QString("%1->%2[%3]").arg(inCons[0]->getSourcePort()->getID()).arg(inCons[0]->getDestPort()->getID()).arg(conKey);
		}

		return key;
	}

	CacheObject::CacheObjectData* BufferBlock::createCacheData()
	{
		// Data must be created on the heap, will be managed by the cache pool
		BufferData* data{nullptr};

		// Get incoming connections; must be exactly one
		QVector<Connection*> inCons = _dataInPort->assembleConnections(PortDirection::In);

		if (inCons.size() == 1)
		{
			Connection* con = inCons[0];
			Block* block = con->getSource();

			// Data is only buffered if not connected to a TFB
			if (block->getType() != BlockType::TransformFeedback)
			{
				unsigned int outputSize = 0;

				// Data must be created on the heap, will be managed by the cache pool
				data = new BufferData;

				// Process the connected block

				auto processMixer = [&](MixerBlock* mixer) {
					BufferStreamingContext ctx{&data->data};
					mixer->streamToBuffer(&ctx);

					outputSize = mixer->getOutputSize();
				};

				auto processDataSource = [&](DataSourceBlock* dataSrc) {
					DataSource::DataSourceOutput output = dataSrc->findOutputByPort(con->getSourcePort());
					TypeConversion::ConversionOptions typeConv;

					// Do not convert the data
					typeConv.targetType = DataType::NoType;

					data->data.append(dataSrc->retrieveOutput(output, typeConv, -1));

					outputSize = dataSrc->getOutputSize(output, &typeConv);
				};

				auto processUniform = [&](UniformBaseBlock* uniform) {
					data->data.append(uniform->retrieveUniformData(con->getSourcePort()));

					uniform->getOutputSize(con->getSourcePort());
				};

				processConnectedBlock(block, processMixer, processDataSource, processUniform);

				data->elementCount = (outputSize > 0 ? data->data.size() / outputSize : 0);
			}
		}

		return data;
	}

	const BufferBlock::BufferData* BufferBlock::getBufferData()
	{
		const BufferData* data = getCachedData<BufferData>();

		if (!data)
		{
			// No data cached (should actually never happen); return an empty dummy
			data = &_emptyData;
		}

		return data;
	}

	QString BufferBlock::getCacheKey_DataSource(DataSourceBlock* dataSrc)
	{
		if (!dataSrc)
			throw std::runtime_error{"dataSrc may not be null"};

		// Try and see if the data source is cacheable
		ICacheable* cacheable = dynamic_cast<ICacheable*>(dataSrc->getDataSource());

		if (cacheable)
			return cacheable->getCacheKey(true);

		return "DataSource";
	}

	QString BufferBlock::getCacheKey_Mixer(MixerBlock* mixer)
	{
		if (!mixer)
			throw std::runtime_error{"mixer may not be null"};

		MixerLayout layout = *mixer->getMixerLayout();
		QString mixerKey = "MixerBlock/";

		if (layout.getEntriesAsStruct())
			mixerKey += "asStruct:";
		else
			mixerKey += "asBlock:";

		for (const MixerLayout::MixerLayoutEntry& entry : *layout.getEntries())
		{
			QString str;

			// Encode the type conversion
			str += QString("%1,%2,%3,%4,%5;").arg(static_cast<int>(entry.typeConversion.targetType))
					.arg(static_cast<int>(entry.typeConversion.swizzlingX)).arg(static_cast<int>(entry.typeConversion.swizzlingY)).arg(static_cast<int>(entry.typeConversion.swizzlingZ)).arg(static_cast<int>(entry.typeConversion.swizzlingW));

			// Encode the connection
			if (entry.dataConnection)
			{
				str += QString("%1->%2").arg(entry.dataConnection->getSourcePort()->getID()).arg(entry.dataConnection->getDestPort()->getID());

				// Recursively follow the connection and add it to the current key

				str += "[";

				IBlock* block = entry.dataConnection->getSource();

				// Process the connected block

				auto processMixer = [&](MixerBlock* mixer) {
					str += getCacheKey_Mixer(mixer);
				};

				auto processDataSource = [&](DataSourceBlock* dataSrc) {
					str += getCacheKey_DataSource(dataSrc);
				};

				auto processUniform = [&](UniformBaseBlock* uniform) {
					str += getCacheKey_Uniform(uniform);
				};

				processConnectedBlock(block, processMixer, processDataSource, processUniform);

				str += "]";
			}
			else
				str += "NoConnection;";

			mixerKey += str + "/";
		}

		mixerKey += "]";

		return mixerKey;
	}

	QString BufferBlock::getCacheKey_Uniform(UniformBaseBlock *uniform)
	{
		if (!uniform)
			throw std::runtime_error{"uniform may not be null"};

		// Try and see if the uniform is cacheable
		ICacheable* cacheable = dynamic_cast<ICacheable*>(uniform);

		if (cacheable)
			return cacheable->getCacheKey(true);

		return "Uniform";
	}

	bool BufferBlock::hasCacheKeyChanged(bool clear)
	{
		// Check if key should be cleared
		QString cacheKey = getCacheKey(true);
		if(clear)
			_cacheKey = cacheKey;

		// Check for change.
		return _cacheKey != cacheKey;
	}


}
