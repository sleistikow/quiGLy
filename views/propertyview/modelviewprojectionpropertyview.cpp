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

#include "modelviewprojectionpropertyview.h"
#include "data/properties/property.h"

namespace ysm
{
	ModelViewProjectionPropertyView::ModelViewProjectionPropertyView(IPipelineItem* pipelineItem, QWidget* parentWidget,
																	 IView* parentView) :
		PipelineItemPropertyView(pipelineItem, parentWidget, parentView)
	{
		//Set Hidden Things
		setPropertyHidden(pipelineItem->getProperty<Mat4x4Property>(PropertyID::MVP_MatM));
		setPropertyHidden(pipelineItem->getProperty<Mat4x4Property>(PropertyID::MVP_MatV));
		setPropertyHidden(pipelineItem->getProperty<Mat4x4Property>(PropertyID::MVP_MatP));
		setPropertyHidden(pipelineItem->getProperty<Mat4x4Property>(PropertyID::MVP_MatMV));
		setPropertyHidden(pipelineItem->getProperty<Mat4x4Property>(PropertyID::MVP_MatMVP));
		setPropertyHidden(pipelineItem->getProperty<Mat3x3Property>(PropertyID::MVP_MatNormal));

		//Set Model Group
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_ModelPosition), "Model");
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_ModelRotationAxis), "Model");
		setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::MVP_ModelRotationAngle), "Model");
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_ModelScaling), "Model");

		//Set Camera Group
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_CameraPosition), "Camera");
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_CameraTarget), "Camera");
		setPropertyGroup(pipelineItem->getProperty<Vec3Property>(PropertyID::MVP_CameraUpVector), "Camera");

		//Set Projection Group
		setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::MVP_ProjVerticalAngle), "Perspective");
		setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::MVP_ProjAspectRation), "Perspective");
		setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::MVP_ProjNear), "Perspective");
		setPropertyGroup(pipelineItem->getProperty<FloatProperty>(PropertyID::MVP_ProjFar), "Perspective");
	}
}
