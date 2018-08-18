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

#include "registry.h"

#include "propertyview/modelloaderpropertyview.h"
#include "propertyview/imageloaderpropertyview.h"
#include "propertyview/mixerpropertyview.h"
#include "propertyview/shaderpropertyview.h"
#include "propertyview/vaopropertyview.h"
#include "propertyview/bufferpropertyview.h"
#include "propertyview/fragmenttestspropertyview.h"
#include "propertyview/meshgeneratorpropertyview.h"
#include "propertyview/texturepropertyview.h"
#include "propertyview/texturesamplerpropertyview.h"
#include "propertyview/modelviewprojectionpropertyview.h"
#include "propertyview/clearcommandpropertyview.h"
#include "propertyview/textureviewpropertyview.h"
#include "propertyview/rasterizationpropertyview.h"
#include "propertyview/timeuniformpropertyview.h"
#include "propertyview/varyingspropertyview.h"

#include "pipelineview/visualitems/visualpipelineitem.h"
#include "pipelineview/visualitems/visualpipelineitemfactory.h"
#include "pipelineview/visualitems/visualimageloaderblock.h"
#include "pipelineview/visualitems/visualshaderblock.h"
#include "pipelineview/visualitems/visualuniformblock.h"
#include "pipelineview/visualitems/visualconnection.h"
#include "pipelineview/visualitems/visualoutputblock.h"
#include "pipelineview/visualitems/visualvertexpullerblock.h"
#include "pipelineview/visualitems/visualrendercommand.h"

using namespace ysm;

Registry::Registry(QObject* parent) :
	QObject(parent)
{
	//Initialize the factories.
	_visualFactory = new VisualPipelineItemFactory(this);
	_viewFactory = new PropertyViewFactory(this);

	//Data source blocks.
	registerBlockType<VisualBlock, ModelLoaderPropertyView>(BlockType::ModelLoader, "Model Loader", "Data Source");
	registerBlockType<VisualBlock, MeshGeneratorPropertyView>(BlockType::MeshGenerator, "Mesh Generator", "Data Source");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::Array, "Array", "Data Source");
	registerBlockType<VisualImageLoaderBlock, ImageLoaderPropertyView>(BlockType::ImageLoader, "Image Loader", "Data Source");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::TextureLoader, "Texture Loader", "Data Source");

	//Data processing blocks.
	registerBlockType<VisualBlock, BufferPropertyView>(BlockType::Buffer, "Buffer", "Data Processing");
	registerBlockType<VisualBlock, MixerPropertyView>(BlockType::Mixer, "Mixer", "Data Processing");
	registerBlockType<VisualBlock, VaoPropertyView>(BlockType::VertexArrayObject, "Vertex Array Object", "Data Processing");

	//Texture blocks.
	registerBlockType<VisualBlock, TexturePropertyView>(BlockType::Texture, "Texture", "Texture");
	registerBlockType<VisualBlock, TextureSamplerPropertyView>(BlockType::TextureSampler, "Texture Sampler", "Texture");
	registerBlockType<VisualBlock, TextureViewPropertyView>(BlockType::TextureView, "Texture View", "Texture");

	//Fixed Function blocks.
	registerBlockType<VisualBlock, RasterizationPropertyView>(BlockType::Rasterization, "Rasterization", "Fixed Function");
	registerBlockType<VisualBlock, FragmentTestsPropertyView>(BlockType::FragmentTests, "Fragment Tests", "Fixed Function");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::TessellationPrimitiveGenerator, "Tesselation Primitive Generator", "Fixed Function");

	//Rendering blocks.
	registerBlockType<VisualVertexPullerBlock, PipelineItemPropertyView>(BlockType::VertexPuller, "Vertex Puller", "Rendering");
	registerBlockType<VisualOutputBlock, PipelineItemPropertyView>(BlockType::FrameBufferObject, "Frame Buffer Object", "Rendering");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::RenderBuffer, "Render Buffer", "Rendering");
	registerBlockType<VisualBlock, VaryingsPropertyView>(BlockType::TransformFeedback, "Transform Feedback", "Rendering");

	//Shader blocks.
	registerBlockType<VisualShaderBlock, ShaderPropertyView>(BlockType::Shader_Vertex, "Vertex Shader", "Shader");
	registerBlockType<VisualShaderBlock, ShaderPropertyView>(BlockType::Shader_TessellationControl, "Tessellation Control Shader", "Shader");
	registerBlockType<VisualShaderBlock, ShaderPropertyView>(BlockType::Shader_TessellationEvaluation, "Tesselation Evaluation Shader", "Shader");
	registerBlockType<VisualShaderBlock, ShaderPropertyView>(BlockType::Shader_Geometry, "Geometry Shader", "Shader");
	registerBlockType<VisualShaderBlock, ShaderPropertyView>(BlockType::Shader_Fragment, "Fragment Shader", "Shader");

	//Uniform blocks.
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_UInt, "uint Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Int, "int Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Float, "float Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Double, "double Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Vec2, "vec2 Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Vec3, "vec3 Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Vec4, "vec4 Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Mat3x3, "mat3 Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, PipelineItemPropertyView>(BlockType::Uniform_Mat4x4, "mat4 Uniform", "Uniform");
	registerBlockType<VisualUniformBlock, TimeUniformPropertyView>(BlockType::Uniform_ElapsedTime, "Elapsed Time Uniform", "Uniform");
	registerBlockType<VisualBlock, ModelViewProjectionPropertyView>(BlockType::ModelViewProjection, "Model View Projection", "Uniform");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::LightSource, "Light Source", "Uniform");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::Material, "Material", "Uniform");

	//Output / Input blocks.
	//registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::CodeGenerator, "Code Generator", "User In/Out");
	registerBlockType<VisualOutputBlock, PipelineItemPropertyView>(BlockType::Display, "Display", "User In/Out");
	registerBlockType<VisualBlock, PipelineItemPropertyView>(BlockType::CameraControl, "Camera Control", "User In/Out");

	//Render commands.
	registerRenderCommandType<VisualRenderCommand, ClearCommandPropertyView>(RenderCommandType::Clear);
	registerRenderCommandType<VisualRenderCommand, PipelineItemPropertyView>(RenderCommandType::Draw);

	//Connection.
	_viewFactory->setDefaultConnectionConstructor<PipelineItemPropertyView>();
}

VisualPipelineItemFactory* Registry::getVisualFactory() const { return _visualFactory; }
PropertyViewFactory* Registry::getViewFactory() const { return _viewFactory; }
ToolView::Tool Registry::getTool(BlockType blockType) const { return _toolMap[blockType]; }

QList<BlockType> Registry::getBlocks() const { return _toolMap.keys(); }
