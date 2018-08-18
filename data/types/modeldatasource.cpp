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

#include "modeldatasource.h"
#include "data/pipeline/pipelinemanager.h"
#include "data/blocks/block.h"

#include <QFileInfo>

namespace ysm
{
	ModelDataSource::ModelDataSource(Pipeline* pipeline, Block* block) : GeometryDataSource(pipeline, block)
	{
		_outputs = VertexPositions|VertexNormals|VertexTangents|VertexBitangents|VertexColors|TextureCoordinates|IndexList;

		// Add an empty mesh to our empty model data
		ModelData::MeshData emptyMesh;
		_emptyData.meshes.append(std::move(emptyMesh));
	}

	void ModelDataSource::setModelFile(const QString& modelFile, bool combineMeshes, bool loadImmediately)
	{
		// No file loaded, remove anything from this source from the cache
		if (modelFile.isEmpty())
			removeFromCache();

		_modelFile = modelFile.trimmed();
		_combineMeshes = combineMeshes;

		if (loadImmediately && !_modelFile.isEmpty())
		{
			// Force loading of the model file by accessing the model data
			getModelData();
		}
	}

	void ModelDataSource::setMeshIndex(unsigned int index)
	{
		_meshIndex = index;
	}

	unsigned int ModelDataSource::getMeshCount()
	{
		return getModelData()->meshes.count();
	}

	QStringList ModelDataSource::getMeshNames()
	{
		QStringList names;
		const ModelData* data = getModelData();

		for (const ModelData::MeshData& meshData : data->meshes)
			names << meshData.name;

		return names;
	}

	unsigned int ModelDataSource::getMeshOutputs(int meshIndex)
	{
		return getMeshData(meshIndex)->outputs;
	}

	const Vec3Data& ModelDataSource::getVertexPositions()
	{		
		return getMeshData()->vertexPositions;
	}

	const Vec3Data& ModelDataSource::getVertexNormals()
	{
		return getMeshData()->vertexNormals;
	}

	const Vec3Data& ModelDataSource::getVertexTangents()
	{
		return getMeshData()->vertexTangents;
	}

	const Vec3Data& ModelDataSource::getVertexBitangents()
	{
		return getMeshData()->vertexBitangents;
	}

	const Vec4Data& ModelDataSource::getVertexColors()
	{
		return getMeshData()->vertexColors;
	}

	const Vec3Data& ModelDataSource::getTextureCoordinates()
	{
		return getMeshData()->textureCoordinates;
	}

	const UIntData& ModelDataSource::getIndexList()
	{
		return getMeshData()->indexList;
	}

	void ModelDataSource::processMesh(const aiMesh* mesh, ModelData::MeshData* meshData)
	{
		meshData->name = mesh->mName.C_Str();

		// Store the current length of the mesh data
		int indexOffset = meshData->vertexPositions.length();

		// Update the output of our model
		if (mesh->HasPositions())
			meshData->outputs |= VertexPositions;

		if (mesh->HasNormals())
			meshData->outputs |= VertexNormals;

		if (mesh->HasTangentsAndBitangents())
			meshData->outputs |= VertexTangents|VertexBitangents;

		if (mesh->HasVertexColors(0))
			meshData->outputs |= VertexColors;

		if (mesh->HasTextureCoords(0))
			meshData->outputs |= TextureCoordinates;

		// Get positions, normals, (bi)tangents, colors and texture coordinates
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			// Get vertex positions
			if (mesh->HasPositions())
			{
				aiVector3D v = mesh->mVertices[i];
				meshData->vertexPositions << QVector3D{v.x, v.y, v.z};
			}

			// Get vertex normals
			if (mesh->HasNormals())
			{
				aiVector3D v = mesh->mNormals[i];
				meshData->vertexNormals << QVector3D{v.x, v.y, v.z};
			}

			// Get vertex tangents & bitangents
			if (mesh->HasTangentsAndBitangents())
			{
				aiVector3D v = mesh->mTangents[i];
				meshData->vertexTangents << QVector3D{v.x, v.y, v.z};

				v = mesh->mBitangents[i];
				meshData->vertexBitangents << QVector3D{v.x, v.y, v.z};
			}

			// Get vertex colors
			if (mesh->HasVertexColors(0))
			{
				aiColor4D c = mesh->mColors[0][i];
				meshData->vertexColors << QVector4D{c.r, c.g, c.b, c.a};
			}

			// Get texture coordinates
			if (mesh->HasTextureCoords(0))
			{
				aiVector3D v = mesh->mTextureCoords[0][i];
				meshData->textureCoordinates << QVector3D{v.x, v.y, v.z};
			}
		}

		// Get index list
		if (mesh->HasFaces())
		{
			meshData->outputs |= IndexList;

			for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
			{
				const aiFace& face = mesh->mFaces[i];

				// We can only handle triangles, and we shouldn't get anything else anyway
				// Adjust the indices if meshData has not been empty before
				if (face.mNumIndices == 3)
					meshData->indexList
							<< face.mIndices[0] + indexOffset
							<< face.mIndices[1] + indexOffset
							<< face.mIndices[2] + indexOffset;
			}
		}
	}

	const ModelDataSource::ModelData* ModelDataSource::getModelData()
	{
		const ModelData* data = getCachedData<ModelData>();

		if (!data || data->meshes.size() == 0)
		{
			// No data cached (maybe the file could not be loaded); return an empty dummy
			data = &_emptyData;
		}

		return data;
	}

	const ModelDataSource::ModelData::MeshData* ModelDataSource::getMeshData(int meshIndex)
	{
		const ModelData* data = getModelData();

		if (meshIndex == -1)
			meshIndex = _meshIndex;

		// We should always have at least one mesh in our data
		if (meshIndex >= data->meshes.count())
			meshIndex = data->meshes.count() - 1;

		return &data->meshes[meshIndex];
	}

	void ModelDataSource::loadModel(ModelData* data)
	{
		Assimp::Importer importer;
		unsigned int flags = aiProcess_CalcTangentSpace|aiProcess_GenNormals|aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|
				aiProcess_ImproveCacheLocality|aiProcess_SortByPType|aiProcess_FindDegenerates|aiProcess_GenUVCoords;

		if (_combineMeshes)
			flags |= aiProcess_OptimizeMeshes|aiProcess_OptimizeGraph;

		// Try to load the model using Assimp
		const aiScene* scene = importer.ReadFile(qPrintable(_modelFile), flags);

		if (!scene)
			throw std::runtime_error{qPrintable(QString("The model '%1' could not be loaded (%2)").arg(_modelFile).arg(importer.GetErrorString()))};

		if (!scene->HasMeshes())
			throw std::runtime_error{qPrintable(QString("The model '%1' doesn't contain any meshes").arg(_modelFile))};

		if (_combineMeshes)
		{
			// Create a single entry for all meshes
			ModelData::MeshData meshData;

			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				const aiMesh* mesh = scene->mMeshes[i];
				processMesh(mesh, &meshData);
			}

			QFileInfo fi{_modelFile};
			meshData.name = fi.baseName();

			data->meshes.append(std::move(meshData));
		}
		else
		{
			// Create separate entries for each mesh
			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				const aiMesh* mesh = scene->mMeshes[i];
				ModelData::MeshData meshData;

				processMesh(mesh, &meshData);

				data->meshes.append(std::move(meshData));
			}
		}
	}

	CacheObject::Key ModelDataSource::getCacheKey(bool retrieveForeignKey)
	{
		CacheObject::Key key;

		// Key consists of class qualifier + model filename
		key = QString("ModelDataSource/%1,%2").arg(_modelFile).arg(_combineMeshes);

		// If we're retrieving this key for another object, append the active mesh index
		if (retrieveForeignKey)
			key += QString("[%1]").arg(_meshIndex);

		return key;
	}

	CacheObject::CacheObjectData* ModelDataSource::createCacheData()
	{
		_block->setStatus(PipelineItemStatus::Healthy);

		if (!_modelFile.isEmpty())
		{
			// Data must be created on the heap, will be managed by the cache pool
			ModelData* data = new ModelData;

			try
			{
				loadModel(data);
			}
			catch (std::exception& excp)
			{
				delete data;
				data = nullptr;

				QString msg = QString("The model failed to load: %1").arg(excp.what());
				_block->setStatus(PipelineItemStatus::Chilled, msg);
			}

			return data;
		}

		return nullptr;
	}
}
