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

#include "fragmenttestsblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	FragmentTestsBlock::FragmentTestsBlock(Pipeline* parent) : Block(parent, block_type, "Fragment Tests")
	{

	}

	QMap<int, QString> FragmentTestsBlock::getTestFuncNames()
	{
		QMap<int, QString> enumNames;

		enumNames[TestFunc_Never] = "Never";
		enumNames[TestFunc_Less] = "Less";
		enumNames[TestFunc_Equal] = "Equal";
		enumNames[TestFunc_LEqual] = "Less or Equal";
		enumNames[TestFunc_Greater] = "Greater";
		enumNames[TestFunc_NotEqual] = "Not Equal";
		enumNames[TestFunc_GEqual] = "Greater or Equal";
		enumNames[TestFunc_Always] = "Always";

		return enumNames;
	}

	QMap<int, QString> FragmentTestsBlock::getBlendFuncNames()
	{
		QMap<int, QString> enumNames;

		enumNames[BlendFunc_Zero] = "Zero";
		enumNames[BlendFunc_One] = "One";
		enumNames[BlendFunc_SourceColor] = "Source Color";
		enumNames[BlendFunc_OneMinusSourceColor] = "One minus Source Color";
		enumNames[BlendFunc_DestinationColor] = "Destination Color";
		enumNames[BlendFunc_OneMinusDestinationColor] = "One minus Destination Color";
		enumNames[BlendFunc_SourceAlpha] = "Source Alpha";
		enumNames[BlendFunc_OneMinusSourceAlpha] = "One minus Source Alpha";
		enumNames[BlendFunc_DestinationAlpha] = "Destination Alpha";
		enumNames[BlendFunc_OneMinusDestinationAlpha] = "One minus Destination Alpha";
		enumNames[BlendFunc_ConstantColor] = "Constant Color";
		enumNames[BlendFunc_OneMinusConstantColor] = "One minus Constant Color";
		enumNames[BlendFunc_ConstantAlpha] = "Constant Alpha";
		enumNames[BlendFunc_OneMinusConstantAlpha] = "One minus Constant Alpha";
		enumNames[BlendFunc_SourceAlphaSaturate] = "Source Alpha Saturate";
		enumNames[BlendFunc_Source1Color] = "Source1 Color";
		enumNames[BlendFunc_OneMinusSource1Color] = "One minus Source1 Color";
		enumNames[BlendFunc_Source1Alpha] = "Source1 Alpha";
		enumNames[BlendFunc_OneMinusSource1Alpha] = "One minus Source1 Alpha";

		return enumNames;
	}

	QMap<int, QString> FragmentTestsBlock::getBlendEquationNames()
	{
		QMap<int, QString> enumNames;

		enumNames[BlendEqu_Add] = "Add";
		enumNames[BlendEqu_Sub] = "Subtract";
		enumNames[BlendEqu_RevSub] = "Reverse Subtract";

		return enumNames;
	}

	QMap<int, QString> FragmentTestsBlock::getStencilOperationNames()
	{
		QMap<int, QString> enumNames;

		enumNames[StencilOp_Keep] = "Keep";
		enumNames[StencilOp_Zero] = "Zero";
		enumNames[StencilOp_Replace] = "Replace";
		enumNames[StencilOp_Incr] = "Incr";
		enumNames[StencilOp_IncrWrap] = "Incr_Wrap";
		enumNames[StencilOp_Decr] = "Decr";
		enumNames[StencilOp_DecrWrap] = "Decr_Wrap";
		enumNames[StencilOp_Invert] = "Invert";

		return enumNames;
	}

	void FragmentTestsBlock::createProperties()
	{
		Block::createProperties();

		_depthTestEnabled = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_DepthTest, "Depthtest");
		*_depthTestEnabled = true;

		_depthFunc = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_DepthFunc, "Depth Function");
		*_depthFunc = TestFunc_Less;

		_depthMask = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_DepthMask, "write into depthbuffer");
		*_depthMask = true;

		_stencilEnabled = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_StencilTest, "Stenciltest");
		*_stencilEnabled = false;

		_stencilFuncFront = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncFront, "Stencil Front-Function");
		*_stencilFuncFront = TestFunc_Always;

		_stencilFuncBack = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilFuncBack, "Stencil Back-Function");
		*_stencilFuncBack = TestFunc_Always;

		_stencilRef = _properties->newProperty<IntProperty>(PropertyID::FragmentTests_StencilRef, "Stencilreference");
		*_stencilRef = 0;

		_stencilMask = _properties->newProperty<UIntProperty>(PropertyID::FragmentTests_StencilMask, "Stencilmask");
		*_stencilMask = 1;

		_stencilOpSFailFront = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailFront, "Stenciloperation Front sfail");
		*_stencilOpSFailFront = StencilOp_Keep;

		_stencilOpDpFailFront = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailFront, "Stenciloperation Front dpfail");
		*_stencilOpDpFailFront = StencilOp_Keep;

		_stencilOpDpPassFront = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassFront, "Stenciloperation Front dppass");
		*_stencilOpDpPassFront = StencilOp_Keep;

		_stencilOpSFailBack = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpSFailBack, "Stenciloperation Back sfail");
		*_stencilOpSFailBack = StencilOp_Keep;

		_stencilOpDpFailBack = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpFailBack, "Stenciloperation Back dpfail");
		*_stencilOpDpFailBack = StencilOp_Keep;

		_stencilOpDpPassBack = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_StencilOpDpPassBack, "Stenciloperation Back dppass");
		*_stencilOpDpPassBack = StencilOp_Keep;

		_blendingEnabled = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_Blending, "Blending");
		*_blendingEnabled = false;

		_blendFuncSourceColor = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceColor, "Blend Source-Color");
		*_blendFuncSourceColor = BlendFunc_One;

		_blendFuncSourceAlpha = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncSourceAlpha, "Blend Source-Alpha");
		*_blendFuncSourceAlpha = BlendFunc_One;

		_blendFuncDestinationColor = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationColor, "Blend Destination-Color");
		*_blendFuncDestinationColor = BlendFunc_Zero;

		_blendFuncDestinationAlpha = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_BlendFuncDestinationAlpha, "Blend Destination-Alpha");
		*_blendFuncDestinationAlpha = BlendFunc_Zero;

		_blendEquation = _properties->newProperty<EnumProperty>(PropertyID::FragmentTests_BlendEquation, "Blend Equation");
		*_blendEquation = BlendEqu_Add;

		_scissorTestEnabled = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_ScissorTest, "Scissortest");
		*_scissorTestEnabled = false;

		_lowerLeftPosition = _properties->newProperty<Vec2Property>(PropertyID::FragmentTests_LowerLeftPosition, "Lower-Left Corner");
		*_lowerLeftPosition = QVector2D(0,0);

		_rectangleWidth = _properties->newProperty<IntProperty>(PropertyID::FragmentTests_RectangleWidth, "Width of rectangle");
		*_rectangleWidth = 50;

		_rectangleHeight = _properties->newProperty<IntProperty>(PropertyID::FragmentTests_RectangleHeight, "Height of rectangle");
		*_rectangleHeight = 50;

		_colorMaskRed = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskRed, "Red");
		*_colorMaskRed = true;

		_colorMaskGreen = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskGreen, "Green");
		*_colorMaskGreen = true;

		_colorMaskBlue = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskBlue, "Blue");
		*_colorMaskBlue = true;

		_colorMaskAlpha = _properties->newProperty<BoolProperty>(PropertyID::FragmentTests_ColorMaskAlpha, "Alpha");
		*_colorMaskAlpha = true;
	}

	Port* FragmentTestsBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* FragmentTestsBlock::getGenericOutPort()
	{
		return _outPort;
	}

	void FragmentTestsBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

    bool FragmentTestsBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Display, PortType::GenericIn);
			conPoints << qMakePair(BlockType::CodeGenerator, PortType::GenericIn);
			conPoints << qMakePair(BlockType::FrameBufferObject, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Fragment Tests output must be connected to a Display or Code Generator block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }

	BoolProperty* FragmentTestsBlock::getDepthTestEnabled()
	{
		return _depthTestEnabled;
	}

	BoolProperty* FragmentTestsBlock::getStencilEnabled()
	{
		return _stencilEnabled;
	}

	UIntProperty* FragmentTestsBlock::getStencilMask()
	{
		return _stencilMask;
	}

	IntProperty* FragmentTestsBlock::getStencilRef()
	{
		return _stencilRef;
	}

	BoolProperty* FragmentTestsBlock::getBlendingEnabled()
	{
		return _blendingEnabled;
	}

	BoolProperty* FragmentTestsBlock::getScissorTestEnabled()
	{
		return _scissorTestEnabled;
	}

	Vec2Property* FragmentTestsBlock::getLowerLeftPosition()
	{
		return _lowerLeftPosition;
	}

	IntProperty* FragmentTestsBlock::getRectangleWidth()
	{
		return _rectangleWidth;
	}

	IntProperty* FragmentTestsBlock::getRectangleHeight()
	{
		return _rectangleHeight;
	}

	BoolProperty* FragmentTestsBlock::getDepthMask()
	{
		return _depthMask;
	}

	BoolProperty* FragmentTestsBlock::getColorMaskRed()
	{
		return _colorMaskRed;
	}

	BoolProperty* FragmentTestsBlock::getColorMaskGreen()
	{
		return _colorMaskGreen;
	}

	BoolProperty* FragmentTestsBlock::getColorMaskBlue()
	{
		return _colorMaskBlue;
	}

	BoolProperty* FragmentTestsBlock::getColorMaskAlpha()
	{
		return _colorMaskAlpha;
	}
}

