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

#include "clearcommandpropertyview.h"
#include "propertyviewfactory.h"
#include "propertyviewitems/colourpropertyviewitem.h"

using namespace ysm;

void ClearCommandPropertyView::registerPropertyItems(PropertyViewFactory *factory)
{
    //Register default property items.
    PipelineItemPropertyView::registerPropertyItems(factory);

    //Tell factory to use ColourPropertyViewItem
    factory->setDefaultRenderCommandItemConstructor<ColourPropertyViewItem>(RenderCommandType::Clear,
                                                                            PropertyID::Clear_Color);
}

ClearCommandPropertyView::ClearCommandPropertyView(IPipelineItem *pipelineItem, QWidget* parentWidget,
												   IView* parentView) :
	PipelineItemPropertyView(pipelineItem, parentWidget, parentView)
{
    //Set Layout for Color, Depth and Stencil
    setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::Clear_ColorEnabled), "Clear Color");
	setPropertyGroup(pipelineItem->getProperty<ColorProperty>(PropertyID::Clear_Color), "Clear Color");

    setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::Clear_DepthEnabled), "Clear Depth");
    setPropertyGroup(pipelineItem->getProperty<DoubleProperty>(PropertyID::Clear_Depth), "Clear Depth");

    setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::Clear_StencilEnabled), "Clear Stencil");
    setPropertyGroup(pipelineItem->getProperty<IntProperty>(PropertyID::Clear_Stencil), "Clear Stencil");

	setPropertyGroup(pipelineItem->getProperty<Vec2Property>(PropertyID::Clear_ViewportLowerLeftCorner), "Viewport");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Clear_ViewportWidth), "Viewport");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::Clear_ViewportHeight), "Viewport");
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::Clear_ViewportAutoSize), "Viewport");
}
