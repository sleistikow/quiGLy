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

#include "texturedatasourceblock.h"
#include "data/types/texturedatasource.h"

namespace ysm
{
	TextureDataSourceBlock::TextureDataSourceBlock(DataSource* dataSource, Pipeline* parent, BlockType type, const QString& name) : DataSourceBlock(dataSource, parent, type, name)
	{

	}

	QByteArray TextureDataSourceBlock::retrieveOutput(DataSource::DataSourceOutput output, const TypeConversion::ConversionOptions& typeConv, const int index)
	{
		if (output == DataSource::NoOutput)
			throw std::invalid_argument{"output may not be DataSource::NoOutput"};

		if (!_outputUnits.contains(output))
			throw std::invalid_argument{"The data source doesn't provide the given output type"};

		QByteArray data;

		switch (output)
		{
		case TextureDataSource::TexelColors: // Vec4 types
			data = DataSourceBlock::retrieveOutput<Vec4DataProperty>(_outputUnits[output], typeConv, index);
			break;
		}

		return data;
	}

	unsigned int TextureDataSourceBlock::retrieveOutputCount(DataSource::DataSourceOutput output)
	{
		if (output == DataSource::NoOutput)
			throw std::invalid_argument{"output may not be DataSource::NoOutput"};

		if (!_outputUnits.contains(output))
			throw std::invalid_argument{"The data source doesn't provide the given output type"};

		unsigned int count = 0;

		switch (output)
		{
		case TextureDataSource::TexelColors: // Vec4 types
			count = DataSourceBlock::retrieveOutputCount<Vec4DataProperty>(_outputUnits[output]);
			break;
		}

		return count;
	}

	void TextureDataSourceBlock::rebuildOutputUnits()
	{
		if (!_dataSource)
			throw std::runtime_error{"Trying to create output units for a data source block, but the data source is null"};

		TextureDataSource* texDataSrc = dynamic_cast<TextureDataSource*>(_dataSource);

		if (!texDataSrc)
			throw std::runtime_error{"The provided data source is no texture data source"};

		rebuildOutputUnit<Vec4DataProperty>(TextureDataSource::TexelColors, DataType::Vec4, PropertyID::Data_TexelColors,
											PortType::Data_TexelColors, "Texel Colors", std::bind(&TextureDataSource::getTexelColors, texDataSrc));
	}
}
