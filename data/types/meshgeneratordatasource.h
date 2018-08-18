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

#ifndef MESHGENERATORDATASOURCE_H
#define MESHGENERATORDATASOURCE_H

#include <QVector2D>
#include <QVector3D>

#include "geometrydatasource.h"

namespace ysm
{
	/**
	 * @brief Simple Mesh generator
	 */
	class MeshGeneratorDataSource : public GeometryDataSource
	{
	public:
		explicit MeshGeneratorDataSource(Pipeline* pipeline, Block* block);

	public:
		// 2D Geometry generation
		/**
		 * @brief Creates a triangle based on three vertices
		 */
		void createTriangle(const QVector2D pt1, const QVector2D pt2, const QVector2D pt3);

		/**
		 * @brief Creates a rectangle of size @p width x @p height at @p center
		 */
		void createRectangle(const float width, const float height, const QVector3D center = QVector3D(0.0f, 0.0f, 0.0f));

		/**
		 * @brief Creates a circle of radius @p radius at @p center, consisting of @p sides sides
		 */
		void createCircle(const float radius, const unsigned int sections, const QVector3D center = QVector3D(0.0f, 0.0f, 0.0f));

		// 3D Geometry generation
		/**
		 * @brief Creates a cuboid of size @p width x @p height x @p depth at @p center and @interpolate the normals.
		 */
		void createCuboid(const float width, const float height, const float depth, bool interpolate, const QVector3D center = QVector3D(0.0f, 0.0f, 0.0f));

        /**
         * @brief Creates a sphere using @p radius x @p sections at @p center
         */
        void createSphere(const float radius, const float sections, const QVector3D center = QVector3D(0.0f, 0.0f, 0.0f));

	public:
		// Data access
		const Vec3Data& getVertexPositions() override;
		const Vec3Data& getVertexNormals() override;
		const Vec3Data& getTextureCoordinates() override;
		const UIntData& getIndexList() override;

	public:
		// ICacheable
		CacheObject::Key getCacheKey(bool retrieveForeignKey) override;
		CacheObject::CacheObjectData*createCacheData() override;

	private:
		void clearGeometry();

	private:
		Vec3Data _vertexPositions;
		Vec3Data _vertexNormals;
		Vec3Data _textureCoordinates;        
		UIntData _indexList;

		CacheObject::Key _cacheKey;
	};
}

#endif
