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

#include "mixerlayout.h"
#include "data/blocks/connection.h"
#include "data/blocks/mixerblock.h"
#include "data/blocks/datasourceblock.h"
#include "data/blocks/uniforms/uniformblock.h"
#include "data/common/dataexceptions.h"

namespace ysm
{
	MixerLayout::MixerLayout()
	{

	}

	MixerLayout::MixerLayout(const MixerLayout& src)
	{
		copyMixerLayout(src);
	}

	MixerLayout& MixerLayout::operator =(const MixerLayout& src)
	{
		copyMixerLayout(src);
		return *this;
	}

	bool MixerLayout::operator ==(const MixerLayout& src) const
	{
		if (_entriesAsStruct != src._entriesAsStruct)
			return false;

		if (_entries.size() != src._entries.size())
			return false;

		for (int i = 0; i < _entries.size(); ++i)
		{
			const MixerLayoutEntry& entry = _entries[i];
			const MixerLayoutEntry& entrySrc = src._entries[i];

			if (!compareEntries(entry, entrySrc))
				return false;
		}

		return true;
	}

	bool MixerLayout::operator !=(const MixerLayout& src) const
	{
		return !(*this == src);
	}

	bool MixerLayout::verifyLayout(QString& error) const
	{
		for (const MixerLayoutEntry& entry : _entries)
		{
			if (!entry.dataConnection)
			{
				error = "The mixer contains an invalid layout entry";
				return false;
			}

			IBlock* block = entry.dataConnection->getSource();

			if (dynamic_cast<DataSourceBlock*>(block))
			{
				// Try to cast to a data source
				DataSourceBlock* src = dynamic_cast<DataSourceBlock*>(entry.dataConnection->getSource());

				// Find the output type belonging to this connection
				DataSource::DataSourceOutput output = src->findOutputByPort(entry.dataConnection->getSourcePort());

				if (output == DataSource::NoOutput)
				{
					error = "The data source does not provide any data on the given port";
					return false;
				}
			}
			else
			{
				if (!dynamic_cast<MixerBlock*>(block) && !dynamic_cast<UniformBlock*>(block))
				{
					error = "The connected block must either be a Data Source, Mixer or Uniform block";
					return false;
				}
			}
		}

		return true;
	}

	void MixerLayout::addEntry(IConnection* con, const QString& name, const int index)
	{
		TypeConversion::ConversionOptions typeConv;

		typeConv.targetType = DataType::NoType;
		addEntry(con, typeConv, name, index);
	}

	void MixerLayout::addEntry(IConnection* con, const TypeConversion::ConversionOptions& typeConv, const QString& name, const int index)
	{
		MixerLayoutEntry entry;

		entry.dataConnection = con;
		entry.typeConversion = typeConv;
		entry.name = name;

		if (index == -1)
			_entries << entry;
		else
			_entries.insert(index, entry);
	}

	bool MixerLayout::autoConfigureLayout(IBlock* block)
	{
		if (!block || block->getType() != BlockType::Mixer)
			throw std::invalid_argument{"block may not be null and must be a Mixer block"};

		_entries.clear();

		IPort* inPort = block->getPort(PortType::Data_In);

		// Create entries for each connection
		for (IConnection* con : inPort->getInConnections())
		{
			QString name = con->getSourcePort()->getName().replace(" ", "");
			addEntry(con, name);
		}

		return true;
	}

	QVector<MixerLayout::MixerLayoutEntry>* MixerLayout::getEntries()
	{
		return &_entries;
	}

	bool MixerLayout::getEntriesAsStruct() const
	{
		return _entriesAsStruct;
	}

	void MixerLayout::setEntriesAsStruct(bool entriesAsStruct)
	{
		_entriesAsStruct = entriesAsStruct;
	}

	void MixerLayout::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		xmlElement->setAttribute("asStruct", _entriesAsStruct);

		for (const MixerLayoutEntry& entry : _entries)
		{
			QDomElement elem = ctx->createElement("MixerLayoutEntry");

			elem.setAttribute("connection", ctx->getObjectID(entry.dataConnection));
			elem.setAttribute("name", entry.name);
			elem.setAttribute("targetType", static_cast<int>(entry.typeConversion.targetType));
			elem.setAttribute("swizzlingX", static_cast<int>(entry.typeConversion.swizzlingX));
			elem.setAttribute("swizzlingY", static_cast<int>(entry.typeConversion.swizzlingY));
			elem.setAttribute("swizzlingZ", static_cast<int>(entry.typeConversion.swizzlingZ));
			elem.setAttribute("swizzlingW", static_cast<int>(entry.typeConversion.swizzlingW));

			xmlElement->appendChild(elem);
		}
	}

	void MixerLayout::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		_entriesAsStruct = (xmlElement->attribute("asStruct", QString("%1").arg(_entriesAsStruct)).toInt() ? true : false);

		_entries.clear();

		for (QDomElement& elem : ctx->getElements(xmlElement, "MixerLayoutEntry"))
		{
			if (!ctx->assertAttributes(&elem, QStringList() << "connection"))
				continue;

			unsigned int conID = elem.attribute("connection").toUInt();
			Connection* con = dynamic_cast<Connection*>(ctx->findObject(conID));

			if (!con)
			{
				QString msg = QString("Connection #%1 for the mixer layout doesn't exist").arg(conID);
				ctx->addMessage(msg);

				throw SerializationException{"The mixer layout entry connection doesn't exist"};
			}

			MixerLayoutEntry entry;

			entry.dataConnection = con;
			entry.name = elem.attribute("name");
			entry.typeConversion.targetType = static_cast<DataType>(elem.attribute("targetType", QString("%1").arg(static_cast<int>(DataType::NoType))).toInt());
			entry.typeConversion.swizzlingX = static_cast<VectorComponent>(elem.attribute("swizzlingX", QString("%1").arg(static_cast<int>(VectorComponent::X))).toInt());
			entry.typeConversion.swizzlingY = static_cast<VectorComponent>(elem.attribute("swizzlingY", QString("%1").arg(static_cast<int>(VectorComponent::Y))).toInt());
			entry.typeConversion.swizzlingZ = static_cast<VectorComponent>(elem.attribute("swizzlingZ", QString("%1").arg(static_cast<int>(VectorComponent::Z))).toInt());
			entry.typeConversion.swizzlingW = static_cast<VectorComponent>(elem.attribute("swizzlingW", QString("%1").arg(static_cast<int>(VectorComponent::W))).toInt());

			_entries << entry;
		}
	}

	void MixerLayout::removeConnectionEntries(const Connection* con)
	{
		for (int i = _entries.size() - 1; i >= 0; --i)
		{
			if (_entries[i].dataConnection == con)
				_entries.remove(i);
		}
	}

	bool MixerLayout::hasConnectionEntries(const Connection* con) const
	{
		//Iterate over all entrys and check if one uses the connection.
		for (int i = 0; i < _entries.size(); i++)
			if(_entries[i].dataConnection == con)
				return true;

		//Connection not used.
		return false;
	}

	bool MixerLayout::compareEntries(const MixerLayout::MixerLayoutEntry& entry1, const MixerLayout::MixerLayoutEntry& entry2) const
	{
		if (entry1.dataConnection != entry2.dataConnection)
			return false;

		if (entry1.name != entry2.name)
			return false;

		if (entry1.typeConversion.targetType != entry2.typeConversion.targetType)
			return false;

		if (entry1.typeConversion.swizzlingX != entry2.typeConversion.swizzlingX || entry1.typeConversion.swizzlingY != entry2.typeConversion.swizzlingY ||
			entry1.typeConversion.swizzlingZ != entry2.typeConversion.swizzlingZ || entry1.typeConversion.swizzlingW != entry2.typeConversion.swizzlingW)
		{
			return false;
		}

		return true;
	}

	void MixerLayout::copyMixerLayout(const MixerLayout& src)
	{
		_entries = src._entries;
		_entriesAsStruct = src._entriesAsStruct;
	}
}
