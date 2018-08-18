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

#ifndef MODELDATASOURCE_H
#define MODELDATASOURCE_H

#include "geometrydatasource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ysm
{
	class Block;

	/**
	 * @brief Model loader
	 */
	class ModelDataSource : public GeometryDataSource
	{
	public:
		// Construction
		explicit ModelDataSource(Pipeline* pipeline, Block* block);

	public:
		// Model loading
		/**
		 * @brief Sets the model file to load; if @p loadImmediately is set to true, the model is immediately loaded
		 */
		void setModelFile(const QString& modelFile, bool combineMeshes, bool loadImmediately = false);

		/**
		 * @brief Sets the mesh index (used when returning mesh data)
		 */
		void setMeshIndex(unsigned int index);

	public:
		// General attributes
		/**
		 * @brief Gets the number of meshes in the loaded model
		 */
		unsigned int getMeshCount();

		/**
		 * @brief Gets a list of all mesh names
		 */
		QStringList getMeshNames();

		// Mesh attributes
		/**
		 * @brief Gets the outputs of the mesh at index @p meshIndex (if set to -1, the active mesh will be used)
		 */
		unsigned int getMeshOutputs(int meshIndex = -1);

	public:
		const Vec3Data& getVertexPositions() override;
		const Vec3Data& getVertexNormals() override;
		const Vec3Data& getVertexTangents() override;
		const Vec3Data& getVertexBitangents() override;
		const Vec4Data& getVertexColors() override;
		const Vec3Data& getTextureCoordinates() override;
		const UIntData& getIndexList() override;

	public:
		// ICacheable
		CacheObject::Key getCacheKey(bool retrieveForeignKey) override;
		CacheObject::CacheObjectData* createCacheData() override;

	private:
		struct ModelData : CacheObject::CacheObjectData
		{
			// Mesh data
			struct MeshData
			{
				QString name;
				unsigned int outputs{NoOutput};

				Vec3Data vertexPositions;
				Vec3Data vertexNormals;
				Vec3Data vertexTangents;
				Vec3Data vertexBitangents;
				Vec4Data vertexColors;
				Vec3Data textureCoordinates;
				UIntData indexList;
			};

			QVector<MeshData> meshes;
		} _emptyData;

		/**
		 * @brief Gets the cached model data
		 * If no data could be cached, _emptyData is returned.
		 */
		const ModelData* getModelData();

		/**
		 * @brief Gets the currently selected cached mesh data
		 */
		const ModelData::MeshData* getMeshData(int meshIndex = -1);

	private:
		/**
		 * @brief Loads the set model file
		 * Throws an exception if something didn't work.
		 */
		void loadModel(ModelData* data);

		/**
		 * @brief Processes a single mesh
		 */
		void processMesh(const aiMesh* mesh, ModelData::MeshData* meshData);

	private:
		QString _modelFile;
		bool _combineMeshes{false};

		unsigned int _meshIndex{0};
	};
}

#endif
