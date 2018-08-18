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

#include "datasourceblock.h"
#include "data/types/datasource.h"
#include "data/types/typeutils.h"
#include "connection.h"

namespace ysm
{
	DataSourceBlock::DataSourceBlock(DataSource* dataSource, Pipeline* parent, BlockType type, const QString& name) : Block(parent, type, name), _dataSource{dataSource}
	{
		if (!dataSource)
			throw std::invalid_argument{"dataSource may not be null"};
	}

	DataSourceBlock::~DataSourceBlock()
	{

	}

	void DataSourceBlock::initBlock()
	{
		Block::initBlock();

		reloadDataSource();
		rebuildOutputUnits();
	}

	void DataSourceBlock::applyPropertyChanges(IProperty* prop)
	{
		Block::applyPropertyChanges(prop);

		if (prop != _statusMessage)
		{
			reloadDataSource();

			// Data properties and ports depend on the data source, so recreate them
			rebuildOutputUnits();
		}
	}

	DataSource::DataSourceOutput DataSourceBlock::findOutputByPort(IPort* port)
	{
		for (auto it = _outputUnits.cbegin(); it != _outputUnits.cend(); ++it)
		{
			if (it.value().port == port)
				return it.key();
		}

		return DataSource::NoOutput;
	}

	unsigned int DataSourceBlock::getOutputSize(DataSource::DataSourceOutput output, const TypeConversion::ConversionOptions* typeConversion)
	{
		unsigned int size = 0;

		if (_outputUnits.contains(output))
		{
			size = _outputUnits[output].outputSize;

			// If we got a type conversion, we should return OpenGL type sizes in all cases
			if (typeConversion)
			{
				size = DataTypeUtils::getTypeSize(_outputUnits[output].outputType, true);

				if (typeConversion->targetType != DataType::NoType)
				{
					if (TypeConversion::canConvertBetweenTypes(_outputUnits[output].outputType, typeConversion->targetType))
						size = DataTypeUtils::getTypeSize(typeConversion->targetType, true);
				}
			}
		}

		return size;
	}

	bool DataSourceBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		bool isOutputUnit = false;

		for (auto output : _outputUnits)
		{
			if (output.port == src)
			{
				isOutputUnit = true;
				break;
			}
		}

		if (isOutputUnit)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Mixer, PortType::Data_In)
					  << qMakePair(BlockType::Buffer, PortType::Data_In)
					  << qMakePair(BlockType::Texture, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Data Source output must be connected to a Mixer, Buffer or Texture block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	DataSource* DataSourceBlock::getDataSource()
	{
		return _dataSource;
	}

	UIntProperty* DataSourceBlock::getDataOutputs() const
	{
		return _dataOutputs;
	}

	const IProperty* DataSourceBlock::getOutput(DataSource::DataSourceOutput output) const
	{
		if (_outputUnits.contains(output))
			return _outputUnits[output].property;

		return nullptr;
	}

	Port* DataSourceBlock::getOutputPort(DataSource::DataSourceOutput output)
	{
		if (_outputUnits.contains(output))
			return _outputUnits[output].port;

		return nullptr;
	}

	bool DataSourceBlock::isOverridingStatus() const
	{
		return true;
	}

	void DataSourceBlock::createProperties()
	{
		Block::createProperties();

		_dataOutputs = _properties->newProperty<UIntProperty>(PropertyID::Data_Outputs, "Data Outputs", true);
		_dataOutputs->setSerializable(false);
		_dataOutputs->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _dataSource->getOutputs(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });
	}	

	void DataSourceBlock::prepareConnection(Connection* con)
	{
		DataSource::DataSourceOutput output = findOutputByPort(con->getSourcePort());

		// Check if the connection source port belongs to one of our data outputs
		if (output != DataSource::NoOutput)
		{
			if (_outputUnits.contains(output))
			{
				// Add a property to the connection carrying the output data type
				EnumProperty* outputType = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::Data_OutputType, "Data Type");

				*outputType = static_cast<int>(_outputUnits[output].outputType);
				outputType->setSerializable(false);
				outputType->setReadOnly();
			}
		}
	}

	bool DataSourceBlock::hasCacheKeyChanged(bool clear)
	{
		// Check if key should be cleared
		QString cacheKey = _dataSource->getCacheKey(true);
		if(clear)
			_cacheKey = cacheKey;

		// Check for change.
		return _cacheKey != cacheKey;
	}
}
