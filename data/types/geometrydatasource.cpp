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

#include "geometrydatasource.h"

namespace ysm
{
	GeometryDataSource::GeometryDataSource(Pipeline* pipeline, Block* block) : DataSource(pipeline, block)
	{

	}

	const Vec3Data& GeometryDataSource::getVertexPositions()
	{
		throw std::runtime_error{"The data source does not provide vertex positions"};
	}

	const Vec3Data& GeometryDataSource::getVertexNormals()
	{
		throw std::runtime_error{"The data source does not provide vertex normals"};
	}

	const Vec3Data& GeometryDataSource::getVertexTangents()
	{
		throw std::runtime_error{"The data source does not provide vertex tangents"};
	}

	const Vec3Data& GeometryDataSource::getVertexBitangents()
	{
		throw std::runtime_error{"The data source does not provide vertex bitangents"};
	}

	const Vec4Data& GeometryDataSource::getVertexColors()
	{
		throw std::runtime_error{"The data source does not provide vertex colors"};
	}

	const Vec3Data& GeometryDataSource::getTextureCoordinates()
	{
		throw std::runtime_error{"The data source does not provide texture coordinates"};
	}

	const UIntData& GeometryDataSource::getIndexList()
	{
		throw std::runtime_error{"The data source does not provide an index list"};
	}
}
