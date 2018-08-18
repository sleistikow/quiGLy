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

#ifndef GEOMETRYDATASOURCE_H
#define GEOMETRYDATASOURCE_H

#include "datasource.h"

namespace ysm
{
	/**
	 * @brief Base class for all geometry data sources
	 */
	class GeometryDataSource : public DataSource
	{
	public:
		// Types
		/**
		 * @brief The various attribute types a data source can provide
		 */
		enum
		{
			VertexPositions = 0x0001,
			VertexNormals = 0x0002,
			VertexTangents = 0x0004,
			VertexBitangents = 0x0008,
			VertexColors = 0x0010,
			TextureCoordinates = 0x0020,
			IndexList = 0x0040,
		};

	public:
		explicit GeometryDataSource(Pipeline* pipeline, Block* block);

	public:
		// Data access
		/**
		 * @brief Retrieves the vertex positions
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec3Data& getVertexPositions();

		/**
		 * @brief Retrieves the vertex normals
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec3Data& getVertexNormals();

		/**
		 * @brief Retrieves the vertex tangents
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec3Data& getVertexTangents();

		/**
		 * @brief Retrieves the vertex bitangents
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec3Data& getVertexBitangents();

		/**
		 * @brief Retrieves the vertex colors
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec4Data& getVertexColors();

		/**
		 * @brief Retrieves the texture coordinates
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const Vec3Data& getTextureCoordinates();

		/**
		 * @brief Retrieves the index list for indexed rendering
		 * If the data source doesn't provide this output, an exception is thrown.
		 */
		virtual const UIntData& getIndexList();
	};
}

#endif
