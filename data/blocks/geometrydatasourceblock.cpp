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

#include "geometrydatasourceblock.h"
#include "data/types/geometrydatasource.h"

namespace ysm
{
	GeometryDataSourceBlock::GeometryDataSourceBlock(DataSource* dataSource, Pipeline* parent, BlockType type, const QString& name) : DataSourceBlock(dataSource, parent, type, name)
	{

	}

	QByteArray GeometryDataSourceBlock::retrieveOutput(DataSource::DataSourceOutput output, const TypeConversion::ConversionOptions& typeConv, const int index)
	{
		if (output == DataSource::NoOutput)
			throw std::invalid_argument{"output may not be DataSource::NoOutput"};

		if (!_outputUnits.contains(output))
			throw std::invalid_argument{"The data source doesn't provide the given output type"};

		QByteArray data;

		switch (output)
		{
		case GeometryDataSource::VertexColors: // Vec4 types
			data = DataSourceBlock::retrieveOutput<Vec4DataProperty>(_outputUnits[output], typeConv, index);
			break;

		case GeometryDataSource::IndexList: // UInt types
			data = DataSourceBlock::retrieveOutput<UIntDataProperty>(_outputUnits[output], typeConv, index);
			break;

		default: // Vec3 types
			data = DataSourceBlock::retrieveOutput<Vec3DataProperty>(_outputUnits[output], typeConv, index);
		}

		return data;
	}

	unsigned int GeometryDataSourceBlock::retrieveOutputCount(DataSource::DataSourceOutput output)
	{
		if (output == DataSource::NoOutput)
			throw std::invalid_argument{"output may not be DataSource::NoOutput"};

		if (!_outputUnits.contains(output))
			throw std::invalid_argument{"The data source doesn't provide the given output type"};

		unsigned int count = 0;

		switch (output)
		{
		case GeometryDataSource::VertexColors: // Vec4 types
			count = DataSourceBlock::retrieveOutputCount<Vec4DataProperty>(_outputUnits[output]);
			break;

		case GeometryDataSource::IndexList: // UInt types
			count = DataSourceBlock::retrieveOutputCount<UIntDataProperty>(_outputUnits[output]);
			break;

		default: // Vec3 types
			count = DataSourceBlock::retrieveOutputCount<Vec3DataProperty>(_outputUnits[output]);
		}

		return count;
	}

	void GeometryDataSourceBlock::rebuildOutputUnits()
	{
		if (!_dataSource)
			throw std::runtime_error{"Trying to create output units for a data source block, but the data source is null"};

		GeometryDataSource* geomDataSrc = dynamic_cast<GeometryDataSource*>(_dataSource);

		if (!geomDataSrc)
			throw std::runtime_error{"The provided data source is no geometry data source"};

		rebuildOutputUnit<Vec3DataProperty>(GeometryDataSource::VertexPositions, DataType::Vec3, PropertyID::Data_VertexPositions,
											PortType::Data_VertexPositions, "Vertex Positions", std::bind(&GeometryDataSource::getVertexPositions, geomDataSrc));

		rebuildOutputUnit<Vec3DataProperty>(GeometryDataSource::VertexNormals, DataType::Vec3, PropertyID::Data_VertexNormals,
											PortType::Data_VertexNormals, "Vertex Normals", std::bind(&GeometryDataSource::getVertexNormals, geomDataSrc));

		rebuildOutputUnit<Vec3DataProperty>(GeometryDataSource::VertexTangents, DataType::Vec3, PropertyID::Data_VertexTangents,
											PortType::Data_VertexTangents, "Vertex Tangents", std::bind(&GeometryDataSource::getVertexTangents, geomDataSrc));

		rebuildOutputUnit<Vec3DataProperty>(GeometryDataSource::VertexBitangents, DataType::Vec3, PropertyID::Data_VertexBitangents,
											PortType::Data_VertexBitangents, "Vertex Bitangents", std::bind(&GeometryDataSource::getVertexBitangents, geomDataSrc));

		rebuildOutputUnit<Vec4DataProperty>(GeometryDataSource::VertexColors, DataType::Vec4, PropertyID::Data_VertexColors,
											PortType::Data_VertexColors, "Vertex Colors", std::bind(&GeometryDataSource::getVertexColors, geomDataSrc));

		rebuildOutputUnit<Vec3DataProperty>(GeometryDataSource::TextureCoordinates, DataType::Vec3, PropertyID::Data_TextureCoordinates,
											PortType::Data_TextureCoordinates, "Texture Coordinates", std::bind(&GeometryDataSource::getTextureCoordinates, geomDataSrc));

		rebuildOutputUnit<UIntDataProperty>(GeometryDataSource::IndexList, DataType::UInt, PropertyID::Data_IndexList,
										   PortType::Data_IndexList, "Index List", std::bind(&GeometryDataSource::getIndexList, geomDataSrc));
	}
}
