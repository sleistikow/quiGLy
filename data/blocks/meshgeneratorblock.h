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

#ifndef MESHGENERATORBLOCK_H
#define MESHGENERATORBLOCK_H

#include "geometrydatasourceblock.h"
#include "data/types/meshgeneratordatasource.h"

namespace ysm
{
	/**
	 * @brief Data source block for generating simple geometry
	 */
	class MeshGeneratorBlock : public GeometryDataSourceBlock
	{
		Q_OBJECT

	public:
		// Types
		/**
		 * @brief The Geometry type to generate
		 */
		enum GeometryType
		{
			Geom_Triangle,
			Geom_Rectangle,
			Geom_Circle,
			Geom_Cuboid,
            Geom_Sphere,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getGeometryTypeNames();

	public:
		static const BlockType block_type{BlockType::MeshGenerator};

	public:
		// Construction
		explicit MeshGeneratorBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the geometry type
		 */
		EnumProperty* getGeometryType();

		/**
		 * @brief Gets the rectangle/circle center
		 */
		Vec3Property* getCenter();

		/**
		 * @brief Gets the first triangle vertex
		 */
		Vec2Property* getVertex1();

		/**
		 * @brief Gets the second triangle vertex
		 */
		Vec2Property* getVertex2();

		/**
		 * @brief Gets the third triangle vertex
		 */
		Vec2Property* getVertex3();

		/**
		 * @brief Gets the rectangle/cuboid width
		 */
		FloatProperty* getWidth();

		/**
		 * @brief Gets the rectangle/cuboid height
		 */
		FloatProperty* getHeight();

		/**
		 * @brief Gets the cuboid depth
		 */
		FloatProperty* getDepth();

		/**
		 * @brief Gets the circle radius
		 */
		FloatProperty* getRadius();

		/**
		 * @brief Gets the circle section count
		 */
		UIntProperty* getSections();

	protected:
		void createProperties() override;

		void reloadDataSource() override;

	private:
		// The data source
		MeshGeneratorDataSource _meshDataSource;

		// Properties
		EnumProperty* _geometryType{nullptr};
		Vec3Property* _center{nullptr};
		Vec2Property* _vertex1{nullptr};
		Vec2Property* _vertex2{nullptr};
		Vec2Property* _vertex3{nullptr};
		FloatProperty* _width{nullptr};
		FloatProperty* _height{nullptr};
		FloatProperty* _depth{nullptr};
		FloatProperty* _radius{nullptr};
		UIntProperty* _sections{nullptr};
		BoolProperty* _interpolate{nullptr};

		UIntProperty* _meshVertexCount{nullptr};
		UIntProperty* _meshElementCount{nullptr};
	};
}

#endif
