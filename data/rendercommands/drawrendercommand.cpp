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

#include "drawrendercommand.h"
#include "data/properties/propertylist.h"

namespace ysm
{	
	DrawRenderCommand::DrawRenderCommand(Pipeline* parent) : RenderCommand(parent, RenderCommandType::Draw, "Draw")
	{

	}

	QMap<int, QString> DrawRenderCommand::getDrawModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[DrawMode_Arrays] = "Arrays";
		enumNames[DrawMode_Elements] = "Elements";

		return enumNames;
	}

	QMap<int, QString> DrawRenderCommand::getPrimitiveModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[PrimitiveMode_Points] = "Points";
		enumNames[PrimitiveMode_Lines] = "Lines";
		enumNames[PrimitiveMode_LinesAdjacency] = "Lines Adjacency";
		enumNames[PrimitiveMode_LineStrip] = "Line Strip";
		enumNames[PrimitiveMode_LineStripAdjacency] = "Line Strip Adjacency";
		enumNames[PrimitiveMode_LineLoop] = "Line Loop";
		enumNames[PrimitiveMode_Triangles] = "Triangles";
		enumNames[PrimitiveMode_TrianglesAdjacency] = "Triangles Adjacency";
		enumNames[PrimitiveMode_TriangleStrip] = "Triangle Strip";
		enumNames[PrimitiveMode_TriangleStripAdjacency] = "Triangle Strip Adjacency";
		enumNames[PrimitiveMode_TriangleFan] = "Triangle Fan";
		enumNames[PrimitiveMode_Patches] = "Patches";

		return enumNames;
	}

	void DrawRenderCommand::createProperties()
	{
		RenderCommand::createProperties();

		_drawMode = _properties->newProperty<EnumProperty>(PropertyID::Draw_DrawMode, "Draw mode");
		*_drawMode = DrawMode_Arrays;

		_primitiveMode = _properties->newProperty<EnumProperty>(PropertyID::Draw_PrimitiveMode, "Primitive mode");
		*_primitiveMode = PrimitiveMode_Triangles;

		_firstIndex = _properties->newProperty<UIntProperty>(PropertyID::Draw_FirstIndex, "First index");
		*_firstIndex = 0;

		_autoElementCount = _properties->newProperty<BoolProperty>(PropertyID::Draw_AutoElementCount, "Auto-detect Element Count");
		*_autoElementCount = true;

		_elementCount = _properties->newProperty<UIntProperty>(PropertyID::Draw_ElementCount, "Element Count");
		*_elementCount = 0;

		_instanced = _properties->newProperty<BoolProperty>(PropertyID::Draw_Instanced, "Instanced Draw");
		*_instanced = false;

		_instanceCount = _properties->newProperty<UIntProperty>(PropertyID::Draw_InstanceCount, "Instance Count");
		*_instanceCount = 1;
	}

	bool DrawRenderCommand::canAcceptBlockAssignment(IBlock* block, QString& denialReason)
	{
		if (block->getType() == BlockType::VertexPuller)
			return true;

		denialReason = "Draw Render Command can only be assigned to Vertex Puller block";
		return false;
	}

	UIntProperty* DrawRenderCommand::getFirstIndex()
	{
		return _firstIndex;
	}

	BoolProperty* DrawRenderCommand::getAutoElementCount()
	{
		return _autoElementCount;
	}

	UIntProperty* DrawRenderCommand::getElementCount()
	{
		return _elementCount;
	}

	EnumProperty* DrawRenderCommand::getPrimitiveMode()
	{
		return _primitiveMode;
	}

	EnumProperty* DrawRenderCommand::getDrawMode()
	{
		return _drawMode;
	}

	BoolProperty* DrawRenderCommand::getInstanced()
	{
		return _instanced;
	}

	UIntProperty* DrawRenderCommand::getInstanceCount()
	{
		return _instanceCount;
	}
}
