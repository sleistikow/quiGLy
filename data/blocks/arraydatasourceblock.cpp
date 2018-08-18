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

#include "arraydatasourceblock.h"
#include "data/properties/propertylist.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"
#include "data/types/gltypes.h"
#include "data/types/typeutils.h"

namespace ysm
{

	QMap<int, QString> ArrayDataSourceBlock::getDataTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[static_cast<int>(DataType::Int)] = "Integer";
		enumNames[static_cast<int>(DataType::UInt)] = "Unsigned Integer";
		enumNames[static_cast<int>(DataType::Float)] = "Float";

		return enumNames;
	}

	ArrayDataSourceBlock::ArrayDataSourceBlock(Pipeline* parent) : UniformBaseBlock(parent, block_type, "Array Data Source")
	{

	}

	UIntProperty* ArrayDataSourceBlock::getElementCount()
	{
		return _elementCount;
	}

	UIntProperty* ArrayDataSourceBlock::getByteCount()
	{
		return _byteCount;
	}

	EnumProperty* ArrayDataSourceBlock::getDataType()
	{
		return _dataType;
	}

	FloatDataProperty* ArrayDataSourceBlock::getData()
	{
		return _data;
	}

	Port* ArrayDataSourceBlock::getGenericOutPort()
	{
		return _outPort;
	}

	void ArrayDataSourceBlock::createPorts()
	{
		UniformBaseBlock::createPorts();

		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

	void ArrayDataSourceBlock::createProperties()
	{
		UniformBaseBlock::createProperties();

		_dataType = _properties->newProperty<EnumProperty>(PropertyID::Array_DataType, "Data Type");
		*_dataType = static_cast<int>(DataType::Float);

		_data = _properties->newProperty<FloatDataProperty>(PropertyID::Array_Data, "Data");
		*_data = FloatData() << 1.0f << 2.0f << 3.0f << 4.0f; // Add sample data

		_elementCount = _properties->newProperty<UIntProperty>(PropertyID::Array_ElementCount, "Element Count", true);
		_elementCount->setSerializable(false);
		_elementCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret; __ret = _data->getValue().size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return !clear && _data->hasChanged(); });

		_byteCount = _properties->newProperty<UIntProperty>(PropertyID::Array_Byte, "Bytes", true);
		_byteCount->setSerializable(false);
		_byteCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret; __ret = getOutputSize(nullptr); return __ret; },
					nullptr,
					[this](bool clear)->bool { return !clear && (_data->hasChanged() || _dataType->hasChanged()); });
	}

	bool ArrayDataSourceBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		ConnectionPoints conPoints;

		conPoints << qMakePair(BlockType::Buffer, PortType::Data_In);

		if (!checkConnectionPoints(dest, conPoints))
		{
			denialReason = "Array output must be connected to a Buffer block";
			return false;
		}

		return true;
	}

	void ArrayDataSourceBlock::prepareConnection(Connection* con)
	{
		// Simply ignore UniformBaseBlock::prepareConnection()
		Q_UNUSED(con);
	}

	unsigned int ArrayDataSourceBlock::getOutputSize(IPort* port) const
	{
		Q_UNUSED(port);

		return _data->getValue().size() * DataTypeUtils::getTypeSize(static_cast<DataType>(_dataType->getValue()));
	}

	QByteArray ArrayDataSourceBlock::retrieveUniformData(IPort* port) const
	{
		QByteArray output;

		const FloatData& data = *_data;
		if(static_cast<DataType>(_dataType->getValue()) == DataType::Float)
		{
			// No need to cast here
			output.append(reinterpret_cast<const char*>(data.data()), getOutputSize(port));
		}
		else if(static_cast<DataType>(_dataType->getValue()) == DataType::Int)
		{
			IntData v;
			for(int i=0; i<data.size(); i++)
				v << static_cast<int>(data[i]);

			output.append(reinterpret_cast<const char*>(v.data()), getOutputSize(port));
		}
		else if(static_cast<DataType>(_dataType->getValue()) == DataType::UInt)
		{
			UIntData v;
			for(int i=0; i<data.size(); i++)
				v << static_cast<unsigned int>(data[i]);

			output.append(reinterpret_cast<const char*>(v.data()), getOutputSize(port));
		}

		return output;
	}
}
