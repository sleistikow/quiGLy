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

#include "modelloaderblock.h"

namespace ysm
{
	ModelLoaderBlock::ModelLoaderBlock(Pipeline* parent) :
		GeometryDataSourceBlock(&_modelDataSource, parent, BlockType::ModelLoader, "Model Loader"),
		_modelDataSource{parent, this}
	{

	}

	FilenameProperty* ModelLoaderBlock::getModelFile()
	{
		return _modelFile;
	}

	void ModelLoaderBlock::createProperties()
	{
		GeometryDataSourceBlock::createProperties();

		_modelFile = _properties->newProperty<FilenameProperty>(PropertyID::Model_FileName, "Model File");

		_meshCount = _properties->newProperty<UIntProperty>(PropertyID::Model_MeshCount, "Mesh Count", true);
		_meshCount->setSerializable(false);
		_meshCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _modelDataSource.getMeshCount(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_meshNames = _properties->newProperty<StringListProperty>(PropertyID::Model_MeshNames, "Mesh Names", true);
		_meshNames->setSerializable(false);
		_meshNames->delegateValue(
					[this]()->const QStringList& { static QStringList __ret; __ret = _modelDataSource.getMeshNames(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_combineMeshes = _properties->newProperty<BoolProperty>(PropertyID::Model_CombineMeshes, "Combine Meshes");
		*_combineMeshes = false;

		_activeMesh = _properties->newProperty<UIntProperty>(PropertyID::Model_ActiveMesh, "Active Mesh");
		*_activeMesh = 0;

		_activeMeshOutputs = _properties->newProperty<UIntProperty>(PropertyID::Model_ActiveMeshOutputs, "Mesh Outputs", true);
		_activeMeshOutputs->setSerializable(false);
		_activeMeshOutputs->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _modelDataSource.getMeshOutputs(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });


		_activeMeshVertexCount = _properties->newProperty<UIntProperty>(PropertyID::Model_ActiveMeshVertexCount, "Vertex Count", true);
		_activeMeshVertexCount->setSerializable(false);
		_activeMeshVertexCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _modelDataSource.getVertexPositions().size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });

		_activeMeshElementCount = _properties->newProperty<UIntProperty>(PropertyID::Model_ActiveMeshElementCount, "Element Count", true);
		_activeMeshElementCount->setSerializable(false);
		_activeMeshElementCount->delegateValue(
					[this]()->const unsigned int& { static unsigned int __ret = 0; __ret = _modelDataSource.getIndexList().size(); return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });
	}

	void ModelLoaderBlock::reloadDataSource()
	{
		QString modelFile = *_modelFile;

		setStatus(PipelineItemStatus::Healthy);

		// Set the file name for our model data source and let it load the model immediately
		_modelDataSource.setModelFile(modelFile, *_combineMeshes, true);
	}

	void ModelLoaderBlock::applyPropertyChanges(IProperty* prop)
	{
		GeometryDataSourceBlock::applyPropertyChanges(prop);

		// Set the selected (active) mesh index of the model
		if (!prop || prop == _activeMesh)
			_modelDataSource.setMeshIndex(*_activeMesh);
	}
}
