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

#include "meshgeneratorblock.h"
#include "data/properties/propertylist.h"

namespace ysm
{

	MeshGeneratorBlock::MeshGeneratorBlock(Pipeline* parent) :
		GeometryDataSourceBlock(&_meshDataSource, parent, BlockType::MeshGenerator, "Mesh Generator"),
		_meshDataSource(parent, this)
	{

	}

	QMap<int, QString> MeshGeneratorBlock::getGeometryTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Geom_Triangle] = "Triangle";
		enumNames[Geom_Rectangle] = "Rectangle";
		enumNames[Geom_Circle] = "Circle";
		enumNames[Geom_Cuboid] = "Cuboid";
        enumNames[Geom_Sphere] = "Sphere";

		return enumNames;
	}

	EnumProperty* MeshGeneratorBlock::getGeometryType()
	{
		return _geometryType;
	}

	Vec3Property* MeshGeneratorBlock::getCenter()
	{
		return _center;
	}

	Vec2Property* MeshGeneratorBlock::getVertex1()
	{
		return _vertex1;
	}

	Vec2Property* MeshGeneratorBlock::getVertex2()
	{
		return _vertex2;
	}

	Vec2Property* MeshGeneratorBlock::getVertex3()
	{
		return _vertex3;
	}

	FloatProperty* MeshGeneratorBlock::getWidth()
	{
		return _width;
	}

	FloatProperty* MeshGeneratorBlock::getHeight()
	{
		return _height;
	}

	FloatProperty* MeshGeneratorBlock::getDepth()
	{
		return _depth;
	}

	FloatProperty* MeshGeneratorBlock::getRadius()
	{
		return _radius;
	}

	UIntProperty* MeshGeneratorBlock::getSections()
	{
		return _sections;
	}

	void MeshGeneratorBlock::createProperties()
	{
		GeometryDataSourceBlock::createProperties();

		_geometryType = _properties->newProperty<EnumProperty>(PropertyID::Mesh_Type, "Geometry Type");
		*_geometryType = Geom_Rectangle;

		_center = _properties->newProperty<Vec3Property>(PropertyID::Mesh_Center, "Center");
		*_center = QVector3D{0.0f, 0.0f, 0.0f};

		_vertex1 = _properties->newProperty<Vec2Property>(PropertyID::Mesh_Vertex1, "Triangle Vertex 1");
		*_vertex1 = QVector2D{0.0f, 0.0f};

		_vertex2 = _properties->newProperty<Vec2Property>(PropertyID::Mesh_Vertex2, "Triangle Vertex 2");
		*_vertex2 = QVector2D{1.0f, 0.0f};

		_vertex3 = _properties->newProperty<Vec2Property>(PropertyID::Mesh_Vertex3, "Triangle Vertex 3");
		*_vertex3 = QVector2D{0.0f, 1.0f};

		_width = _properties->newProperty<FloatProperty>(PropertyID::Mesh_Width, "Width");
		*_width = 1.0f;

		_height = _properties->newProperty<FloatProperty>(PropertyID::Mesh_Height, "Height");
		*_height = 1.0f;

		_depth = _properties->newProperty<FloatProperty>(PropertyID::Mesh_Depth, "Depth");
		*_depth = 1.0f;

		_interpolate = _properties->newProperty<BoolProperty>(PropertyID::Mesh_InterpolateNormals, "Interpolate Normals");
		*_interpolate = false;

		_radius = _properties->newProperty<FloatProperty>(PropertyID::Mesh_Radius, "Circle Radius");
		*_radius = 1.0f;

		_sections = _properties->newProperty<UIntProperty>(PropertyID::Mesh_Sections, "Circle Sections");
		*_sections = 32;

		_meshVertexCount = _properties->newProperty<UIntProperty>(PropertyID::Mesh_VertexCount, "Vertex Count", true);
		_meshVertexCount->setSerializable(false);
		_meshVertexCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _meshDataSource.getVertexPositions().size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_meshElementCount = _properties->newProperty<UIntProperty>(PropertyID::Mesh_ElementCount, "Element Count", true);
		_meshElementCount->setSerializable(false);
		_meshElementCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _meshDataSource.getIndexList().size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });
	}

	void MeshGeneratorBlock::reloadDataSource()
	{
		setStatus(PipelineItemStatus::Healthy);

		try
		{
			// Use our Geometry 2D data source to create the actual output
			switch (*_geometryType)
			{
			case Geom_Triangle:
				_meshDataSource.createTriangle(*_vertex1, *_vertex2, *_vertex3);
				break;

			case Geom_Rectangle:
				_meshDataSource.createRectangle(*_width, *_height, *_center);
				break;

			case Geom_Circle:
				_meshDataSource.createCircle(*_radius, *_sections, *_center);
				break;

			case Geom_Cuboid:
				_meshDataSource.createCuboid(*_width, *_height, *_depth, *_interpolate, *_center);
				break;

            case Geom_Sphere:
                _meshDataSource.createSphere(*_radius, *_sections, *_center);
                break;
			}
		}
		catch (std::exception& excp)
		{
			QString msg = QString("The mesh could not be generated: %1").arg(excp.what());
			setStatus(PipelineItemStatus::Sick, msg);
		}
	}
}
