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

#include "clearrendercommand.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	ClearRenderCommand::ClearRenderCommand(Pipeline* parent) : RenderCommand(parent, RenderCommandType::Clear, "Clear Buffers")
	{

	}

	BoolProperty* ClearRenderCommand::getClearColorEnabled()
	{
		return _clearColorEnabled;
	}

	ColorProperty* ClearRenderCommand::getClearColor()
	{
		return _clearColor;
	}

	BoolProperty* ClearRenderCommand::getClearDepthEnabled()
	{
		return _clearDepthEnabled;
	}

	DoubleProperty* ClearRenderCommand::getClearDepth()
	{
		return _clearDepth;
	}

	BoolProperty* ClearRenderCommand::getClearStencilEnabled()
	{
		return _clearStencilEnabled;
	}

	IntProperty* ClearRenderCommand::getClearStencil()
	{
		return _clearStencil;
	}

	Vec2Property* ClearRenderCommand::getLowerLeftCorner()
	{
		return _lowerLeftCorner;
	}

	UIntProperty* ClearRenderCommand::getWidth()
	{
		return _width;
	}

	UIntProperty* ClearRenderCommand::getHeight()
	{
		return _height;
	}

	BoolProperty* ClearRenderCommand::getAutoSize()
	{
		return _autoSize;
	}

	bool ClearRenderCommand::canAcceptBlockAssignment(IBlock* block, QString& denialReason)
	{
		if (block->getType() == BlockType::FrameBufferObject || block->getType() == BlockType::Display)
			return true;

		denialReason = "Clear Render Command can only be assigned to an FBO or Display block";
		return false;
	}

	void ClearRenderCommand::createProperties()
	{
		RenderCommand::createProperties();

		_clearColorEnabled = _properties->newProperty<BoolProperty>(PropertyID::Clear_ColorEnabled, "Clear Color Enabled");
		*_clearColorEnabled = true;

		_clearColor = _properties->newProperty<ColorProperty>(PropertyID::Clear_Color, "Clear Color");
		*_clearColor = QVector4D{1.0f, 1.0f, 1.0f, 1.0f};

		_clearDepthEnabled = _properties->newProperty<BoolProperty>(PropertyID::Clear_DepthEnabled, "Clear Depth Enabled");
		*_clearDepthEnabled = true;

		_clearDepth = _properties->newProperty<DoubleProperty>(PropertyID::Clear_Depth, "Clear Depth");
		*_clearDepth = 1.0;

		_clearStencilEnabled = _properties->newProperty<BoolProperty>(PropertyID::Clear_StencilEnabled, "Clear Stencil Enabled");
		*_clearStencilEnabled = true;

		_clearStencil = _properties->newProperty<IntProperty>(PropertyID::Clear_Stencil, "Clear Stencil");
		*_clearStencil = 0;

		_lowerLeftCorner = _properties->newProperty<Vec2Property>(PropertyID::Clear_ViewportLowerLeftCorner, "Lower Left Corner");
		*_lowerLeftCorner = QVector2D(0, 0);

		_width = _properties->newProperty<UIntProperty>(PropertyID::Clear_ViewportWidth, "Width");
		*_width = 250;

		_height = _properties->newProperty<UIntProperty>(PropertyID::Clear_ViewportHeight, "Height");
		*_height = 250;

		_autoSize = _properties->newProperty<BoolProperty>(PropertyID::Clear_ViewportAutoSize, "Auto Size");
		*_autoSize = true;
	}
}
