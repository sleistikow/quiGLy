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

#include "ysmpalette.h"

#include <QPalette>

using namespace ysm;

QColor YSMPalette::getBlockColor(BlockType blockType)
{
	switch(blockType)
	{
	//Data source: Blue.
	case BlockType::MeshGenerator: return QColor("#52b3d9");
	case BlockType::ModelLoader: return QColor("#59abe3");
	case BlockType::ImageLoader: return QColor("#446cb3");
	case BlockType::TextureLoader: return QColor("#224A8E");
	case BlockType::Array: return QColor("#33467A");

	//Texture data: Deep purple.
	case BlockType::Texture: return QColor("#bf55ec");
	case BlockType::TextureView: return QColor("#9a12b3");
	case BlockType::TextureSampler: return QColor("#8e44ad");

	//Data storage: Green.
	case BlockType::Buffer: return QColor("#86e2d5");
	case BlockType::Mixer: return QColor("#4ecdc4");
	case BlockType::VertexArrayObject: return QColor("#66cc99");

	//Rendering: Mixed.
	case BlockType::VertexPuller: return QColor("#fde3a7");
	case BlockType::Rasterization: return QColor("#f27935");
	case BlockType::TessellationPrimitiveGenerator: return QColor("#eb9532");
	case BlockType::TransformFeedback: return QColor("#db0a5b");

	//Post rendering: Pink.
	case BlockType::FragmentTests: return QColor("#d24d57");
	case BlockType::FrameBufferObject: return QColor("#d2527f");
	case BlockType::RenderBuffer: return QColor("#e26a6a");

	//Output: Pink grey.
	case BlockType::CameraControl: return QColor("#bfb9e4");
	case BlockType::CodeGenerator: return QColor("#aea8d3");
	case BlockType::Display: return QColor("#dcc6e0");

	//Shader: Orange red.
	case BlockType::Shader_Geometry:
	case BlockType::Shader_TessellationControl:
	case BlockType::Shader_TessellationEvaluation: return QColor("#f4b350");
	case BlockType::Shader_Fragment: return QColor("#d35400");
	case BlockType::Shader_Vertex: return QColor("#f5ab35");

	//Single value uniforms: Purple grey.
	case BlockType::Uniform_Double:
	case BlockType::Uniform_Float:
	case BlockType::Uniform_Int:
	case BlockType::Uniform_UInt:
		return QColor("#913d88");

	//One dimensional uniforms: Purple grey.
	case BlockType::Uniform_Vec2:
	case BlockType::Uniform_Vec3:
	case BlockType::Uniform_Vec4:
	case BlockType::Uniform_ElapsedTime:
		return QColor("#9b59b6");

	//Two dimensional uniforms: Purple grey.
	case BlockType::Uniform_Mat3x3:
	case BlockType::Uniform_Mat4x4:
	case BlockType::ModelViewProjection:
		return QColor("#be90d4");

	//Light & Material: Pink grey.
	case BlockType::LightSource:
	case BlockType::Material:
		return QColor("#fe93d5");

	//No color available.
	default:
		return Qt::white;
	}
}

QColor YSMPalette::getRenderCommandColor(RenderCommandType commandType)
{
	switch(commandType)
	{
	case RenderCommandType::Clear:	return QColor("#34495e");
	case RenderCommandType::Draw:	return QColor("#2c3e50");

	//No color available.
	default:						return Qt::white;
	}
}

QColor YSMPalette::getPipelineItemStatusColor(PipelineItemStatus itemStatus)
{
	switch(itemStatus)
	{
	case PipelineItemStatus::Chilled:	return QColor("#e9d460");
	case PipelineItemStatus::Healthy:	return QColor("#2ecc71");
	case PipelineItemStatus::Sick:		return QColor("#e74c3c");

	//No color available.
	default:							return Qt::white;
	}
}

QColor YSMPalette::getInactiveColor() { return QPalette().color(QPalette::Inactive, QPalette::Background); }

YSMPalette::YSMPalette() { }
