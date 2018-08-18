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

#include "pipelineobjectfactory.h"

#include "data/properties/property.h"
#include "data/properties/mixerlayoutproperty.h"
#include "data/properties/vaolayoutproperty.h"

#include "data/blocks/mixerblock.h"
#include "data/blocks/bufferblock.h"
#include "data/blocks/shader/vertexshaderblock.h"
#include "data/blocks/shader/tessellationcontrolshaderblock.h"
#include "data/blocks/shader/tessellationevaluationshaderblock.h"
#include "data/blocks/shader/geometryshaderblock.h"
#include "data/blocks/shader/fragmentshaderblock.h"
#include "data/blocks/vertexpullerblock.h"
#include "data/blocks/vertexarrayobjectblock.h"
#include "data/blocks/transformfeedbackblock.h"
#include "data/blocks/meshgeneratorblock.h"
#include "data/blocks/imageloaderblock.h"
#include "data/blocks/texturesamplerblock.h"
#include "data/blocks/textureblock.h"
#include "data/blocks/textureviewblock.h"
#include "data/blocks/rasterizationblock.h"
#include "data/blocks/tessellationprimitivegeneratorblock.h"
#include "data/blocks/fragmenttestsblock.h"
#include "data/blocks/framebufferobjectblock.h"
#include "data/blocks/renderbufferblock.h"
#include "data/blocks/displayblock.h"
#include "data/blocks/codegeneratorblock.h"
#include "data/blocks/modelloaderblock.h"
#include "data/blocks/textureloaderblock.h"
#include "data/blocks/cameracontrolblock.h"
#include "data/blocks/arraydatasourceblock.h"
#include "data/blocks/uniforms/doubleuniformblock.h"
#include "data/blocks/uniforms/floatuniformblock.h"
#include "data/blocks/uniforms/intuniformblock.h"
#include "data/blocks/uniforms/mat3x3uniformblock.h"
#include "data/blocks/uniforms/mat4x4uniformblock.h"
#include "data/blocks/uniforms/uintuniformblock.h"
#include "data/blocks/uniforms/vec2uniformblock.h"
#include "data/blocks/uniforms/vec3uniformblock.h"
#include "data/blocks/uniforms/vec4uniformblock.h"
#include "data/blocks/uniforms/elapsedtimeuniformblock.h"
#include "data/blocks/uniforms/modelviewprojectionblock.h"
#include "data/blocks/uniforms/lightsourceblock.h"
#include "data/blocks/uniforms/materialblock.h"

#include "data/rendercommands/clearrendercommand.h"
#include "data/rendercommands/drawrendercommand.h"

#define REGISTER_PROPERTY_TYPE(cls) registerPropertyType(cls::property_type, [](const PropertyID id, const QString& name, const bool isReadOnly) { return new cls(id, name, isReadOnly); });
#define REGISTER_BLOCK_TYPE(cls) registerBlockType(cls::block_type, [](Pipeline* pipeline) { return new cls(pipeline); });
#define REGISTER_RENDERCOMMAND_TYPE(cls) registerRenderCommandType(cls::command_type, [](Pipeline* pipeline) { return new cls(pipeline); });

namespace ysm
{
	PipelineObjectFactory::PipelineObjectFactory()
	{
		registerStandardProperties();
		registerStandardBlocks();
		registerStandardRenderCommands();
	}

	void PipelineObjectFactory::registerPropertyType(const PropertyType type, const property_creator_type creator)
	{
		if (type == PropertyType::Undefined)
			throw std::invalid_argument{"type may not be PropertyType::Undefined"};

		if (!creator)
			throw std::invalid_argument{"creator may not be null"};

		if (_propertyTypes.contains(type))
			throw std::invalid_argument{"The given type has already been registered"};

		_propertyTypes[type] = creator;
	}

	void PipelineObjectFactory::unregisterPropertyType(const PropertyType type)
	{
		if (type == PropertyType::Undefined)
			throw std::invalid_argument{"type may not be PropertyType::Undefined"};

		if (!_propertyTypes.contains(type))
			throw std::invalid_argument{"The given type has not been registered"};

		_propertyTypes.remove(type);
	}

	IProperty* PipelineObjectFactory::createProperty(const PropertyType type, const PropertyID id, const QString& name, const bool isReadOnly) const
	{
		if (type == PropertyType::Undefined)
			throw std::invalid_argument{"type may not be PropertyType::Undefined"};

		if (id == PropertyID::None)
			throw std::invalid_argument{"id may not be PropertyID::None"};

		if (_propertyTypes.contains(type))
		{
			property_creator_type creator = _propertyTypes[type];
			IProperty* prop = creator(id, name, isReadOnly);

			if (!prop)
				throw std::runtime_error{QString("Failed to create a property of type %1 with ID %2").arg(static_cast<int>(type)).arg(static_cast<int>(id)).toStdString()};

			return prop;
		}
		else
			throw std::invalid_argument{"The given type has not been registered"};

		return nullptr;
	}

	void PipelineObjectFactory::registerBlockType(const BlockType type, const block_creator_type creator)
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		if (!creator)
			throw std::invalid_argument{"creator may not be null"};

		if (_blockTypes.contains(type))
			throw std::invalid_argument{"The given type has already been registered"};

		_blockTypes[type] = creator;
	}

	void PipelineObjectFactory::unregisterBlockType(BlockType type)
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		if (!_blockTypes.contains(type))
			throw std::invalid_argument{"The given type has not been registered"};

		_blockTypes.remove(type);
	}

	Block* PipelineObjectFactory::createBlock(const BlockType type, Pipeline* pipeline) const
	{
		if (type == BlockType::Undefined)
			throw std::invalid_argument{"type may not be BlockType::Undefined"};

		if (!pipeline)
			throw std::invalid_argument{"pipeline may not be null"};

		if (_blockTypes.contains(type))
		{
			block_creator_type creator = _blockTypes[type];
			Block* block = creator(pipeline);

			if (!block)
				throw std::runtime_error{QString("Failed to create a block of type %1").arg(static_cast<int>(type)).toStdString()};

			return block;
		}
		else
			throw std::invalid_argument{"The given type has not been registered"};

		return nullptr;
	}

	void PipelineObjectFactory::registerRenderCommandType(const RenderCommandType type, const rendercommand_creator_type creator)
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		if (!creator)
			throw std::invalid_argument{"creator may not be null"};

		if (_renderCommandTypes.contains(type))
			throw std::invalid_argument{"The given type has already been registered"};

		_renderCommandTypes[type] = creator;
	}

	void PipelineObjectFactory::unregisterRenderCommandType(const RenderCommandType type)
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		if (!_renderCommandTypes.contains(type))
			throw std::invalid_argument{"The given type has not been registered"};

		_renderCommandTypes.remove(type);
	}

	RenderCommand* PipelineObjectFactory::createRenderCommand(const RenderCommandType type, Pipeline* pipeline) const
	{
		if (type == RenderCommandType::Noop)
			throw std::invalid_argument{"type may not be RenderCommandType::Noop"};

		if (!pipeline)
			throw std::invalid_argument{"pipeline may not be null"};

		if (_renderCommandTypes.contains(type))
		{
			rendercommand_creator_type creator = _renderCommandTypes[type];
			RenderCommand* cmd = creator(pipeline);

			if (!cmd)
				throw std::runtime_error{QString("Failed to create a render command of type %1").arg(static_cast<int>(type)).toStdString()};

			return cmd;
		}
		else
			throw std::invalid_argument{"The given type has not been registered"};

		return nullptr;
	}

	void PipelineObjectFactory::registerStandardProperties()
	{
		// Standard property types

		REGISTER_PROPERTY_TYPE(BoolProperty);
		REGISTER_PROPERTY_TYPE(IntProperty);
		REGISTER_PROPERTY_TYPE(UIntProperty);
		REGISTER_PROPERTY_TYPE(FloatProperty);
		REGISTER_PROPERTY_TYPE(DoubleProperty);

		REGISTER_PROPERTY_TYPE(CharProperty);
		REGISTER_PROPERTY_TYPE(StringProperty);
		REGISTER_PROPERTY_TYPE(StringListProperty);

		REGISTER_PROPERTY_TYPE(EnumProperty);

		REGISTER_PROPERTY_TYPE(Vec2Property);
		REGISTER_PROPERTY_TYPE(Vec3Property);
		REGISTER_PROPERTY_TYPE(Vec4Property);
		REGISTER_PROPERTY_TYPE(Mat3x3Property);
		REGISTER_PROPERTY_TYPE(Mat4x4Property);

		// Data property types

		REGISTER_PROPERTY_TYPE(Vec3DataProperty);
		REGISTER_PROPERTY_TYPE(Vec4DataProperty);
		REGISTER_PROPERTY_TYPE(IntDataProperty);
		REGISTER_PROPERTY_TYPE(ByteArrayProperty);

		// Complex property types

		REGISTER_PROPERTY_TYPE(MixerLayoutProperty);
		REGISTER_PROPERTY_TYPE(VaoLayoutProperty);
	}

	void PipelineObjectFactory::registerStandardBlocks()
	{
		// Main blocks
		REGISTER_BLOCK_TYPE(MixerBlock);
		REGISTER_BLOCK_TYPE(BufferBlock);
        REGISTER_BLOCK_TYPE(VertexShaderBlock);
        REGISTER_BLOCK_TYPE(TessellationControlShaderBlock);
        REGISTER_BLOCK_TYPE(TessellationEvaluationShaderBlock);
        REGISTER_BLOCK_TYPE(GeometryShaderBlock);
        REGISTER_BLOCK_TYPE(FragmentShaderBlock);
		REGISTER_BLOCK_TYPE(VertexPullerBlock);
		REGISTER_BLOCK_TYPE(VertexArrayObjectBlock);
		REGISTER_BLOCK_TYPE(TextureSamplerBlock);
        REGISTER_BLOCK_TYPE(TextureBlock);
		REGISTER_BLOCK_TYPE(TextureViewBlock);
		REGISTER_BLOCK_TYPE(FrameBufferObjectBlock);
		REGISTER_BLOCK_TYPE(RenderBufferBlock);
		REGISTER_BLOCK_TYPE(TransformFeedbackBlock);

		// Data sources
		REGISTER_BLOCK_TYPE(MeshGeneratorBlock);
		REGISTER_BLOCK_TYPE(ImageLoaderBlock);
		REGISTER_BLOCK_TYPE(ModelLoaderBlock);
		REGISTER_BLOCK_TYPE(TextureLoaderBlock);
		REGISTER_BLOCK_TYPE(ArrayDataSourceBlock);

		// Fixed function blocks
		REGISTER_BLOCK_TYPE(RasterizationBlock);
		REGISTER_BLOCK_TYPE(FragmentTestsBlock);
        REGISTER_BLOCK_TYPE(TessellationPrimitiveGeneratorBlock);

		// Uniform blocks
		REGISTER_BLOCK_TYPE(DoubleUniformBlock);
		REGISTER_BLOCK_TYPE(FloatUniformBlock);
		REGISTER_BLOCK_TYPE(IntUniformBlock);
		REGISTER_BLOCK_TYPE(Mat3x3UniformBlock);
		REGISTER_BLOCK_TYPE(Mat4x4UniformBlock);
		REGISTER_BLOCK_TYPE(UIntUniformBlock);
		REGISTER_BLOCK_TYPE(Vec2UniformBlock);
		REGISTER_BLOCK_TYPE(Vec3UniformBlock);
		REGISTER_BLOCK_TYPE(Vec4UniformBlock);
		REGISTER_BLOCK_TYPE(ElapsedTimeUniformBlock);

		// Other blocks
		REGISTER_BLOCK_TYPE(ModelViewProjectionBlock);
		REGISTER_BLOCK_TYPE(CameraControlBlock);
		REGISTER_BLOCK_TYPE(DisplayBlock);
		REGISTER_BLOCK_TYPE(CodeGeneratorBlock);
		REGISTER_BLOCK_TYPE(LightSourceBlock);
		REGISTER_BLOCK_TYPE(MaterialBlock);
	}

	void PipelineObjectFactory::registerStandardRenderCommands()
	{
		// Drawing commands
		REGISTER_RENDERCOMMAND_TYPE(ClearRenderCommand);
		REGISTER_RENDERCOMMAND_TYPE(DrawRenderCommand);
	}
}
