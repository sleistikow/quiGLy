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

#include "fragmenttestspropertyview.h"
#include "data/blocks/fragmenttestsblock.h"

using namespace ysm;

FragmentTestsPropertyView::FragmentTestsPropertyView(IPipelineItem* pipelineItem, QWidget* parentWidget,
													 IView* parentView):
	PipelineItemPropertyView(pipelineItem, parentWidget, parentView)
{
	//Set Depthtest Group
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_DepthTest), "Depthtest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_DepthFunc), "Depthtest");
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_DepthMask), "Depthtest");
	//Set Scissortest Group
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_ScissorTest), "Scissortest");
	setPropertyGroup(pipelineItem->getProperty<Vec2Property>(PropertyID::FragmentTests_LowerLeftPosition), "Scissortest");
	setPropertyGroup(pipelineItem->getProperty<IntProperty>(PropertyID::FragmentTests_RectangleWidth), "Scissortest");
	setPropertyGroup(pipelineItem->getProperty<IntProperty>(PropertyID::FragmentTests_RectangleHeight), "Scissortest");
    //Set Blend Group
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_Blending), "Blend");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceColor), "Blend");
    setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceAlpha), "Blend");
    setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationColor), "Blend");
    setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationAlpha), "Blend");
    setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_BlendEquation), "Blend");
	//Set Stenciltest
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_StencilTest), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncFront), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncBack), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailFront), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailFront), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassFront), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailBack), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailBack), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassBack), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<IntProperty>(PropertyID::FragmentTests_StencilRef), "Stenciltest");
	setPropertyGroup(pipelineItem->getProperty<UIntProperty>(PropertyID::FragmentTests_StencilMask), "Stenciltest");
	//Set Colormask
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskRed), "Colormask");
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskGreen), "Colormask");
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskBlue), "Colormask");
	setPropertyGroup(pipelineItem->getProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskAlpha), "Colormask");


}
