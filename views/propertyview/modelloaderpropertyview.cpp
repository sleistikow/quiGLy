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

#include "modelloaderpropertyview.h"
#include "propertyviewfactory.h"
#include "data/properties/property.h"
#include "propertyviewitems/defaultpropertyviewitem.h"
#include "propertyviewitems/modelactivemeshpropertyviewitem.h"

using namespace ysm;

void ModelLoaderPropertyView::registerPropertyItems(PropertyViewFactory* factory)
{
	//Register default property items.
	PipelineItemPropertyView::registerPropertyItems(factory);

	//Add specific model loader layout property view item.
	factory->setDefaultBlockItemConstructor<ModelActiveMeshPropertyViewItem>(BlockType::ModelLoader,
																			 PropertyID::Model_ActiveMesh);
}

ModelLoaderPropertyView::ModelLoaderPropertyView(IPipelineItem* pipelineItem, QWidget* parentWidget,
												 IView* parentView) :
	PipelineItemPropertyView(pipelineItem, parentWidget, parentView)
{
	//Register additional group for the mesh data.
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::Model_CombineMeshes), "Mesh");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Model_ActiveMesh), "Mesh");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Model_ActiveMeshOutputs), "Mesh");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Model_ActiveMeshVertexCount), "Mesh");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Model_ActiveMeshElementCount), "Mesh");	

	//Hide data properties.
	setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexPositions));
	setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexNormals));
	setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexTangents));
	setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexBitangents));
	setPropertyHidden(pipelineItem->getProperty<Vec4DataProperty>(PropertyID::Data_VertexColors));
	setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_TextureCoordinates));
	setPropertyHidden(pipelineItem->getProperty<UIntDataProperty>(PropertyID::Data_IndexList));
	setPropertyHidden(pipelineItem->getProperty<UIntProperty>(PropertyID::Data_Outputs));
	setPropertyHidden(pipelineItem->getProperty<UIntProperty>(PropertyID::Model_ActiveMeshOutputs));

	//Hide other unneccessary properties.
	setPropertyHidden(pipelineItem->getProperty<StringListProperty>(PropertyID::Model_MeshNames));
}
