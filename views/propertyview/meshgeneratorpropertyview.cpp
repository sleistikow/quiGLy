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

#include "meshgeneratorpropertyview.h"
#include "data/blocks/meshgeneratorblock.h"

using namespace ysm;

MeshGeneratorPropertyView::MeshGeneratorPropertyView(IPipelineItem* pipelineItem, QWidget* parentWidget,
													 IView* parentView):
	PipelineItemPropertyView(pipelineItem, parentWidget, parentView)
{
    //Set Hidden Things
    setPropertyHidden(pipelineItem->getProperty<UIntProperty>(PropertyID::Data_Outputs));
    setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexPositions));
    setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_VertexNormals));
    setPropertyHidden(pipelineItem->getProperty<Vec3DataProperty>(PropertyID::Data_TextureCoordinates));
    setPropertyHidden(pipelineItem->getProperty<UIntDataProperty>(PropertyID::Data_IndexList));

    //Set Vertices Group
    setPropertyGroup(pipelineItem->getProperty<Vec2Property>(PropertyID::Mesh_Vertex1), "Vertices");
    setPropertyGroup(pipelineItem->getProperty<Vec2Property>(PropertyID::Mesh_Vertex2), "Vertices");
    setPropertyGroup(pipelineItem->getProperty<Vec2Property>(PropertyID::Mesh_Vertex3), "Vertices");

    //Set Measurements Group
    setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::Mesh_Type), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::Mesh_Center), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::Mesh_Width), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::Mesh_Height), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::Mesh_Depth), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::Mesh_Radius), "Measurements");
    setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Mesh_Sections), "Measurements");
}
