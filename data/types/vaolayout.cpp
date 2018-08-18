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

#include "vaolayout.h"
#include "data/blocks/connection.h"
#include "data/blocks/connectionlist.h"
#include "data/blocks/port.h"
#include "data/blocks/bufferblock.h"
#include "data/blocks/mixerblock.h"
#include "data/blocks/datasourceblock.h"
#include "data/types/gltypes.h"
#include "data/properties/mixerlayoutproperty.h"
#include "data/common/dataexceptions.h"

namespace ysm
{
	VaoLayout::VaoLayout()
	{

	}

	VaoLayout::VaoLayout(const VaoLayout &src)
	{
		copyVaoLayout(src);
	}

	VaoLayout& VaoLayout::operator =(const VaoLayout& src)
	{
		copyVaoLayout(src);
		return *this;
	}

	bool VaoLayout::operator ==(const VaoLayout& src) const
	{
		if (_entries.size() != src._entries.size())
			return false;

		for (int i = 0; i < _entries.size(); ++i)
		{
			const VaoLayoutEntry& entry = _entries[i];
			const VaoLayoutEntry& entrySrc = src._entries[i];

			if (!compareEntries(entry, entrySrc))
				return false;
		}

		return true;
	}

	bool VaoLayout::operator !=(const VaoLayout& src) const
	{
		return !(*this == src);
	}

	void VaoLayout::addEntry(IConnection *con, QString name, int index, int size, GLDataType type, bool normalized, int stride, int offset, GLSLDataType glslType, int entryIndex)
	{
		VaoLayoutEntry entry;
		entry.bufferConnection = con;
		entry.name = name;
		entry.index = index;
		entry.size = size;
		entry.type = type;
		entry.glslType = glslType;
		entry.normalized = normalized;
		entry.stride = stride;
		entry.offset = offset;

		if(entryIndex == -1)
			_entries << entry;
		else
			_entries.insert(entryIndex, entry);
	}


	QVector<VaoLayout::VaoLayoutEntry>* VaoLayout::getEntries()
	{
		return &_entries;
	}

	void VaoLayout::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		for (const VaoLayoutEntry& entry : _entries)
		{
			QDomElement elem = ctx->createElement("VaoLayoutEntry");

			elem.setAttribute("connection", ctx->getObjectID(entry.bufferConnection));
			elem.setAttribute("name", entry.name);
			elem.setAttribute("index", entry.index);
			elem.setAttribute("size", entry.size);
			elem.setAttribute("type", static_cast<int>(entry.type));
			elem.setAttribute("glsltype", static_cast<int>(entry.glslType));
			elem.setAttribute("normalized", entry.normalized);
			elem.setAttribute("stride", entry.stride);
			elem.setAttribute("offset", entry.offset);

			xmlElement->appendChild(elem);
		}
	}

	void VaoLayout::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		//Check if reset option is specified.
		if(xmlElement->hasAttribute("reset"))
		{
			//Find the parent block.
			unsigned int blockID = xmlElement->attribute("reset").toUInt();
			IBlock* parentBlock = dynamic_cast<IBlock*>(ctx->findObject(blockID));

			//Automatically configure the layout.
			if(parentBlock)
				autoConfigureLayout(parentBlock);

			//Skip the rest of deserialization.
			return;
		}

		_entries.clear();

		//Configuration specified in XML.
		for(QDomElement& elem : ctx->getElements(xmlElement, "VaoLayoutEntry"))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "connection"))
				continue;

			unsigned int conID = elem.attribute("connection").toUInt();
			Connection* con = dynamic_cast<Connection*>(ctx->findObject(conID));

			if(!con)
			{
				QString msg = QString("Connection #%1 for the VAO layout doesn't exist").arg(conID);
				ctx->addMessage(msg);

				throw SerializationException{"The VAO layout entry connection does not exist"};
			}

			VaoLayoutEntry entry;
			entry.bufferConnection = con;
			entry.name = elem.attribute("name");
			entry.index = elem.attribute("index").toInt();
			entry.size = elem.attribute("size").toInt();
			entry.type = static_cast<GLDataType>(elem.attribute("type").toInt());
			entry.glslType = static_cast<GLSLDataType>(elem.attribute("glsltype").toInt());
            entry.normalized = elem.attribute("normalized").toInt() ? true : false;
			entry.stride = elem.attribute("stride").toInt();
			entry.offset = elem.attribute("offset").toInt();

			_entries << entry;
		}
	}

	void VaoLayout::removeConnectionEntries(Connection *con)
	{
		for(int i = _entries.size() - 1; i >= 0; --i)
		{
			if(_entries[i].bufferConnection == con)
				_entries.remove(i);
		}
	}

	bool VaoLayout::hasConnectionEntries(const Connection* con) const
	{
		//Iterate over all entrys and check if one uses the connection.
		for (int i = 0; i < _entries.size(); i++)
			if(_entries[i].bufferConnection == con)
				return true;

		//Connection not used.
		return false;
	}

	bool VaoLayout::compareEntries(const VaoLayoutEntry& entry1, const VaoLayoutEntry& entry2) const
	{
		if(entry1.bufferConnection != entry2.bufferConnection)
			return false;
		if(entry1.name != entry2.name)
			return false;
		if(entry1.index != entry2.index)
			return false;
		if(entry1.size != entry2.size)
			return false;
		if(entry1.type != entry2.type)
			return false;
		if(entry1.glslType != entry2.glslType)
			return false;
		if(entry1.normalized != entry2.normalized)
			return false;;
		if(entry1.stride != entry2.stride)
			return false;
		if(entry1.offset != entry2.offset)
			return false;

		return true;
	}

	void VaoLayout::copyVaoLayout(const VaoLayout& src)
	{
		_entries = src._entries;
	}

	bool VaoLayout::autoConfigureLayout(IBlock* block)
	{
		if (!block || block->getType() != BlockType::VertexArrayObject)
			throw std::invalid_argument{"block may not be null and must be a VAO block"};

		_entries.clear();

		IPort* inPort = block->getPort(PortType::Data_In);

		// Create entries for each connected buffer
		for (IConnection* con : inPort->getInConnections())
		{
			if (con->getSource()->getType() == BlockType::Buffer)
			{
				BufferBlock* block = dynamic_cast<BufferBlock*>(con->getSource());

				if (!block)
					throw std::runtime_error{"Thought I had a connection to a Buffer, but I was wrong"};

				if (!autoConfigureLayout(block, con))
				{
					_entries.clear();
					return false;
				}
			}
		}

		// Update the index of each entry
		for (int i = 0; i < _entries.size(); ++i)
			_entries[i].index = i;

		return true;
	}

	bool VaoLayout::autoConfigureLayout(BufferBlock* block, IConnection* conVaoIn)
	{
		// Get incoming connections of the buffer; must be exactly one
		QVector<Connection*> inCons = block->getDataInPort()->assembleConnections(PortDirection::In);
		bool ret = false;

		if (inCons.size() == 1)
		{
			Connection* inCon = inCons[0];
			Block* block = inCon->getSource();

			// We can only process a Mixer or direct DataSource here
			if (dynamic_cast<MixerBlock*>(block))
			{
				MixerBlock* mixer = dynamic_cast<MixerBlock*>(block);
				int offset = 0;
				int stride = 0;

				ret = autoConfigureLayout_Mixer(conVaoIn, mixer, inCon, offset, stride);

				if (ret)
				{
					MixerLayout layout = *mixer->getMixerLayout();

					// Update the stride size if the layout is a struct
					if (layout.getEntriesAsStruct())
					{
						for (VaoLayoutEntry& entry : _entries)
							entry.stride = stride;
					}
				}

				return ret;
			}
			else if (dynamic_cast<DataSourceBlock*>(block))
			{
				DataSourceBlock* dataSource = dynamic_cast<DataSourceBlock*>(block);

				ret = autoConfigureLayout_DataSource(conVaoIn, dataSource, inCon, nullptr);
			}
		}

		// The connected block type isn't supported
		return ret;
	}

	bool VaoLayout::autoConfigureLayout_Mixer(IConnection* conVaoIn, MixerBlock* mixer, IConnection* conData, int& offset, int& stride)
	{
		Q_UNUSED(conData);

		MixerLayout layout = *mixer->getMixerLayout();

		for (MixerLayout::MixerLayoutEntry& entry : *layout.getEntries())
		{
			// We must be connected to a Mixer or DataSource
			if (dynamic_cast<MixerBlock*>(entry.dataConnection->getSource()))
			{
				MixerBlock* mixerBlock = dynamic_cast<MixerBlock*>(entry.dataConnection->getSource());

				if (!autoConfigureLayout_Mixer(conVaoIn, mixerBlock, entry.dataConnection, offset, stride))
					return false;
			}
			else if (dynamic_cast<DataSourceBlock*>(entry.dataConnection->getSource()))
			{
				DataSourceBlock* dataSource = dynamic_cast<DataSourceBlock*>(entry.dataConnection->getSource());

				if (autoConfigureLayout_DataSource(conVaoIn, dataSource, entry.dataConnection, &entry.typeConversion))
				{
					// Set the offset and stride of the newly added entry
					_entries.last().offset = offset;
					_entries.last().stride = stride;

					// Use the name specified in the mixer
					_entries.last().name = entry.name;

					DataSource::DataSourceOutput output = dataSource->findOutputByPort(entry.dataConnection->getSourcePort());

					// We now must update the offset and stride values for the next entry
					if (layout.getEntriesAsStruct())
					{
						int outputSize = dataSource->getOutputSize(output, &entry.typeConversion);

						offset += outputSize;
						stride += outputSize; // Will be updated correctly later
					}
					else
					{
						int elementCount = dataSource->retrieveOutputCount(output);

						offset += dataSource->getOutputSize(output, &entry.typeConversion) * elementCount;
						stride = 0;
					}
				}
				else
					return false;
			}
		}

		return true;
	}

	bool VaoLayout::autoConfigureLayout_DataSource(IConnection* conVaoIn, DataSourceBlock* dataSource, IConnection* conData, TypeConversion::ConversionOptions* convOptions)
	{
		Q_UNUSED(dataSource);

		GLDataType glType = GLDataType::NoType;
		int count = 0;
		GLSLDataType glslType = GLSLDataType::NoType;

		if (getConnectionGLTypes(conData, glType, count, glslType, convOptions))
		{
			QString name = conData->getSourcePort()->getName().replace(" ", "");

			addEntry(conVaoIn, name, 0, count, glType, false, 0, 0, glslType);
			return true;
		}

		// For some reason, the data type cannot be converted to GL (shouldn't happen)
		return false;
	}

	bool VaoLayout::getConnectionGLTypes(IConnection* con, GLDataType& glType, int& count, GLSLDataType& glslType, TypeConversion::ConversionOptions* convOptions)
	{
		glType= GLDataType::NoType;
		count = 0;
		glslType = GLSLDataType::NoType;

		EnumProperty* prop = con->getProperty<EnumProperty>(PropertyID::Data_OutputType);

		if (!prop)
			return false;

		DataType outputType = static_cast<DataType>(prop->getValue());

		// Take type conversions into account
		if (convOptions && convOptions->targetType != DataType::NoType)
			outputType = convOptions->targetType;

		if (outputType == DataType::NoType)
			return false;

		// Get the corresponding GL values of the given output type
		GLTypes::convertDataTypeToGLTypes(outputType, glType, count, glslType);

		// Check if the GL types are valid
		if (glType == GLDataType::NoType || count == 0 || glslType == GLSLDataType::NoType)
			return false;

		return true;
	}
}
