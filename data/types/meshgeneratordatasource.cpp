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

#include "meshgeneratordatasource.h"

#include <QVector3D>
#include <QtMath>
#include <cstring>

#include <QDebug>

namespace ysm
{
	MeshGeneratorDataSource::MeshGeneratorDataSource(Pipeline* pipeline, Block* block) : GeometryDataSource(pipeline, block)
	{
		_outputs = VertexPositions|VertexNormals|TextureCoordinates|IndexList;
	}

	void MeshGeneratorDataSource::createTriangle(const QVector2D pt1, const QVector2D pt2, const QVector2D pt3)
	{
		clearGeometry();

		// Pre-compute the cache key
		_cacheKey = QString("Triangle/%1,%2/%3,%4/%5,%6").arg(pt1.x()).arg(pt1.y()).arg(pt2.x()).arg(pt2.y()).arg(pt3.x()).arg(pt3.y());

		float xMin = std::numeric_limits<float>::max();
		float xMax = std::numeric_limits<float>::min();
		float yMin = std::numeric_limits<float>::max();
		float yMax = std::numeric_limits<float>::min();

		auto updateMinMax = [&xMin, &xMax, &yMin, &yMax](const QVector2D pt)
		{
			if (pt.x() < xMin) xMin = pt.x();
			if (pt.x() > xMax) xMax = pt.x();
			if (pt.y() < yMin) yMin = pt.y();
			if (pt.y() > yMax) yMax = pt.y();
		};

		updateMinMax(pt1);
		updateMinMax(pt2);
		updateMinMax(pt3);

		float width = qAbs(xMax - xMin);
		float height = qAbs(yMax - yMin);

		if (width <= 0.0f || height <= 0.0f)
			throw std::invalid_argument{"The triangle has no area"};

		_vertexPositions << pt1 << pt2 << pt3;
		_vertexNormals << -pt1.normalized() << -pt2.normalized() << -pt3.normalized();
		_textureCoordinates << QVector2D{(pt1.x() - xMin) / width, (pt1.y() - yMin) / height}
							<< QVector2D{(pt2.x() - xMin) / width, (pt2.y() - yMin) / height}
							<< QVector2D{(pt3.x() - xMin) / width, (pt3.y() - yMin) / height};
		_indexList << 0 << 1 << 2;
	}

	void MeshGeneratorDataSource::createRectangle(const float width, const float height, const QVector3D center)
	{
		if (width <= 0.0f || height <= 0.0f)
			throw std::invalid_argument{"The width/height of the rectangle must be > 0.0"};

		clearGeometry();

		// Pre-compute the cache key
		_cacheKey = QString("Rectangle/%1/%2/%3,%4,%5").arg(width).arg(height).arg(center.x()).arg(center.y()).arg(center.z());

		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;

		_vertexPositions << QVector3D{center.x() + halfWidth, center.y() + halfHeight, center.z()}
						 << QVector3D{center.x() - halfWidth, center.y() + halfHeight, center.z()}
						 << QVector3D{center.x() - halfWidth, center.y() - halfHeight, center.z()}
						 << QVector3D{center.x() + halfWidth, center.y() - halfHeight, center.z()};
		_vertexNormals << QVector3D{0.0f, 0.0f, 1.0f} << QVector3D{0.0f, 0.0f, 1.0f} << QVector3D{0.0f, 0.0f, 1.0f} << QVector3D{0.0f, 0.0f, 1.0f};
		_textureCoordinates << QVector3D{1.0f, 1.0f, 0.0f} << QVector3D{0.0f, 1.0f, 0.0f} << QVector3D{0.0f, 0.0f, 0.0f} << QVector3D{1.0f, 0.0f, 0.0f};
		_indexList << 0 << 1 << 2 << 0 << 2 << 3;
	}

	void MeshGeneratorDataSource::createCircle(const float radius, const unsigned int sections, const QVector3D center)
	{
		if (sections < 3)
			throw std::invalid_argument{"The number of sections for a circle must be >= 3"};

		clearGeometry();

		// Pre-compute the cache key
		_cacheKey = QString("Circle/%1,%2,%3/%4/%5").arg(center.x()).arg(center.y()).arg(center.z()).arg(radius).arg(sections);

		double curAngle = 0.0;
		double incAngle = (2 * M_PI) / sections;

		_vertexPositions << center;
		_vertexNormals << QVector3D{0.0f, 0.0f, 1.0f};
		_textureCoordinates << QVector3D{0.5f, 0.5f, 0.0f};

		for (unsigned int i = 1; i <= sections; ++i)
		{
			QVector2D v{static_cast<float>(qCos(curAngle)), static_cast<float>(qSin(curAngle))};
			QVector2D p{(v.x() * radius) + center.x(), (v.y() * radius) + center.y()};

			_vertexPositions << QVector3D{p.x(), p.y(), center.z()};
			_vertexNormals << QVector3D{0.0f, 0.0f, 1.0f}.normalized();
			_textureCoordinates << QVector3D{(v.x() + 1) * 0.5f, (v.y() + 1) * 0.5f, 0.0f};

			if (i >= 2)
				_indexList << 0 << i - 1 << i;

			curAngle += incAngle;
		}

		_indexList << 0 << sections << 1;
	}

	void MeshGeneratorDataSource::createCuboid(const float width, const float height, const float depth, bool interpolate, const QVector3D center)
	{
		if (width <= 0.0f || height <= 0.0f || depth <= 0.0f)
			throw std::invalid_argument{"The width/height/depth of the cuboid must be > 0.0"};

		clearGeometry();

		// Pre-compute the cache key
		_cacheKey = QString("Cuboid/%1,%2,%3/%4/%5/%6/%7").arg(center.x()).arg(center.y()).arg(center.z()).arg(width).arg(height).arg(depth).arg(interpolate);

		// Iterate over all 6 faces.
		QVector3D vPos, vTex, vNormal;
		for(int face = 0; face < 6; face++)
		{
			for(int corner = 0; corner < 4; corner++)
			{
				int x = corner / 2;
				int y = corner % 2;
				int z = face / 3;

				switch(face)
				{
				case 0:
				case 3:
					vPos = { (x - 0.5f) * width, (y - 0.5f) * height, (z - 0.5f) * depth };
					vNormal = { 0.0, 0.0, (z - 0.5f) * depth };
					vTex = { x * width, y * height, 0.0 };
					break;
				case 1:
				case 4:
					vPos = { (y - 0.5f) * width, (z - 0.5f) * height, (x - 0.5f) * depth };
					vNormal = { 0.0, (z - 0.5f) * height, 0.0 };
					vTex = { x * depth, y * width, 0.0 };
					break;
				case 2:
				case 5:
					vPos = { (z - 0.5f) * width, (x - 0.5f) * height, (y - 0.5f) * depth };
					vNormal = { (z - 0.5f) * width, 0.0, 0.0 };
					vTex = { x * height, y * depth, 0.0 };
					break;
				}

				_vertexPositions << vPos + center;
				_vertexNormals << (interpolate ? vPos.normalized() : vNormal);
				_textureCoordinates << vTex;
			}

			// Index list renders 6 vertices (two full triangles).
			_indexList << face * 4 + 0 << face * 4 + 1 << face * 4 + 2
					   << face * 4 + 3 << face * 4 + 2 << face * 4 + 1;
		}
	}

	void MeshGeneratorDataSource::createSphere(const float radius, const float sections, const QVector3D center)
	{
		if (radius <= 0.0f || sections <= 0.0f)
			throw std::invalid_argument{"The radius/sections of the sphere must be > 0.0"};

		clearGeometry();

		// Pre-compute the cache key
		_cacheKey = QString("Sphere/%1,%2,%3/%5/%6").arg(center.x()).arg(center.y()).arg(center.z()).arg(radius).arg(sections);

		// Calculate the angle between different points.
		float angle = 2 * M_PI / sections;

		// Create the inner triangles.
		float offset = 0.0f;
		for(int i = 1; i < sections; i++)
		{
			// Calculate new slice data.
			float prevRadius = qSin((i - 1) * angle / 2) * radius;
			float prevHeight = qCos((i - 1) * angle / 2) * radius;
			float curRadius = qSin(i * angle / 2) * radius;
			float curHeight = qCos(i * angle / 2) * radius;
			float nextRadius = qSin((i + 1) * angle / 2) * radius;
			float nextHeight = qCos((i + 1) * angle / 2) * radius;

			qDebug() << curHeight << curRadius;

			// Create one triangle "ring".
			QVector3D vPos;
			for(int j = 0; j < sections; j++)
			{
				vPos = QVector3D
				{
					(float) qSin((j + offset) * angle) * prevRadius,
					prevHeight,
					(float) qCos((j + offset) * angle) * prevRadius,
				};
				_vertexPositions << vPos + center;
				_vertexNormals << vPos.normalized();
				_textureCoordinates << QVector3D { vPos.x() / radius, vPos.y() / radius, 1.0f };

				vPos = QVector3D
				{
					(float) qSin((j - .5f + offset) * angle) * curRadius,
					curHeight,
					(float) qCos((j - .5f + offset) * angle) * curRadius
				};
				_vertexPositions << vPos + center;
				_vertexNormals << vPos.normalized();
				_textureCoordinates << QVector3D { vPos.x() / radius, vPos.y() / radius, 1.0f };

				vPos = QVector3D
				{
					(float) qSin((j + .5f + offset) * angle) * curRadius,
					curHeight,
					(float) qCos((j + .5f + offset) * angle) * curRadius
				};
				_vertexPositions << vPos + center;
				_vertexNormals << vPos.normalized();
				_textureCoordinates << QVector3D { vPos.x() / radius, vPos.y() / radius, 1.0f };

				vPos = QVector3D
				{
					(float) qSin((j + offset) * angle) * nextRadius,
					nextHeight,
					(float) qCos((j + offset) * angle) * nextRadius
				};
				_vertexPositions << vPos + center;
				_vertexNormals << vPos.normalized();
				_textureCoordinates << QVector3D { vPos.x() / radius, vPos.y() / radius, 1.0f };

				int indexOffset = ((i - 1) * sections + j) * 4;
				_indexList << indexOffset + 0 << indexOffset + 1 << indexOffset + 2
						   << indexOffset + 3 << indexOffset + 2 << indexOffset + 1;
			}

			// Adjust the offset.
			offset -= 0.5;
		}
	}

	const Vec3Data& MeshGeneratorDataSource::getVertexPositions()
	{
		return _vertexPositions;
	}

	const Vec3Data& MeshGeneratorDataSource::getTextureCoordinates()
	{
		return _textureCoordinates;
	}

	const Vec3Data& MeshGeneratorDataSource::getVertexNormals()
	{
		return _vertexNormals;
	}

	const UIntData& MeshGeneratorDataSource::getIndexList()
	{
		return _indexList;
	}

	void MeshGeneratorDataSource::clearGeometry()
	{
		_vertexPositions.clear();
		_textureCoordinates.clear();
		_vertexNormals.clear();
		_indexList.clear();

		_cacheKey = "";
	}

	CacheObject::Key MeshGeneratorDataSource::getCacheKey(bool retrieveForeignKey)
	{
		Q_UNUSED(retrieveForeignKey);

		// Use the pre-computed key
		return ("MeshGeneratorDataSource/" + _cacheKey);
	}

	CacheObject::CacheObjectData* MeshGeneratorDataSource::createCacheData()
	{
		// Never actually cache this object
		return nullptr;
	}
}
