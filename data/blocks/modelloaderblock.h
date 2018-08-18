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

#ifndef MODELLOADERBLOCK_H
#define MODELLOADERBLOCK_H

#include "geometrydatasourceblock.h"
#include "data/properties/filenameproperty.h"
#include "data/types/modeldatasource.h"

namespace ysm
{
	/**
	 * @brief Data source block for loading model files
	 */
	class ModelLoaderBlock : public GeometryDataSourceBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::ModelLoader};

	public:
		// Construction
		explicit ModelLoaderBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the image file
		 */
		FilenameProperty* getModelFile();

	public:
		void applyPropertyChanges(IProperty* prop) override;

	protected:
		void createProperties() override;

		void reloadDataSource() override;

	private:
		// The data source
		ModelDataSource _modelDataSource;

		// Properties
		FilenameProperty* _modelFile{nullptr};

		UIntProperty* _meshCount{nullptr};
		StringListProperty* _meshNames{nullptr};

		BoolProperty* _combineMeshes{nullptr};
		UIntProperty* _activeMesh{nullptr};
		UIntProperty* _activeMeshOutputs{nullptr};
		UIntProperty* _activeMeshVertexCount{nullptr};
		UIntProperty* _activeMeshElementCount{nullptr};
	};
}

#endif
