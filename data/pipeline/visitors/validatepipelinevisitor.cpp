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

#include "validatepipelinevisitor.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"

#include "data/rendercommands/clearrendercommand.h"
#include "data/rendercommands/drawrendercommand.h"

#include "data/blocks/mixerblock.h"
#include "data/blocks/bufferblock.h"
#include "data/blocks/vertexpullerblock.h"
#include "data/blocks/vertexarrayobjectblock.h"
#include "data/blocks/displayblock.h"
#include "data/blocks/texturesamplerblock.h"
#include "data/blocks/textureblock.h"
#include "data/blocks/textureviewblock.h"
#include "data/blocks/framebufferobjectblock.h"
#include "data/blocks/renderbufferblock.h"
#include "data/blocks/renderbufferblock.h"
#include "data/blocks/codegeneratorblock.h"
#include "data/blocks/shader/fragmentshaderblock.h"
#include "data/blocks/shader/geometryshaderblock.h"
#include "data/blocks/shader/tessellationcontrolshaderblock.h"
#include "data/blocks/shader/tessellationevaluationshaderblock.h"
#include "data/blocks/shader/vertexshaderblock.h"
#include "data/blocks/cameracontrolblock.h"
#include "data/blocks/uniforms/modelviewprojectionblock.h"

#include "data/properties/mixerlayoutproperty.h"
#include "data/properties/vaolayoutproperty.h"

namespace ysm
{
	ValidatePipelineVisitor::ValidatePipelineVisitor(Pipeline* pipeline) : _pipeline{pipeline}
	{
		if (!pipeline)
			throw std::invalid_argument{"pipeline may not be null"};
	}

	PipelineItemStatus ValidatePipelineVisitor::getStatus() const
	{
		return _status;
	}

	bool ValidatePipelineVisitor::blockVisited(Block* block)
	{
		switch (block->getType())
		{
		case BlockType::Mixer:
			verifyBlock_Mixer(dynamic_cast<MixerBlock*>(block));
			break;
		case BlockType::Buffer:
			verifyBlock_Buffer(dynamic_cast<BufferBlock*>(block));
			break;
		case BlockType::VertexPuller:
			verifyBlock_VertexPuller(dynamic_cast<VertexPullerBlock*>(block));
			break;
		case BlockType::VertexArrayObject:
			verifyBlock_VertexArrayObject(dynamic_cast<VertexArrayObjectBlock*>(block));
			break;
		case BlockType::Display:
			verifyBlock_Display(dynamic_cast<DisplayBlock*>(block));
			break;
		case BlockType::TextureSampler:
			verifyBlock_TextureSampler(dynamic_cast<TextureSamplerBlock*>(block));
			break;
		case BlockType::Texture:
			verifyBlock_Texture(dynamic_cast<TextureBlock*>(block));
			break;
		case BlockType::TextureView:
			verifyBlock_TextureView(dynamic_cast<TextureViewBlock*>(block));
			break;
		case BlockType::FrameBufferObject:
			verifyBlock_FrameBufferObject(dynamic_cast<FrameBufferObjectBlock*>(block));
			break;
		case BlockType::RenderBuffer:
			verifyBlock_RenderBuffer(dynamic_cast<RenderBufferBlock*>(block));
			break;
		case BlockType::CodeGenerator:
			verifyBlock_CodeGenerator(dynamic_cast<CodeGeneratorBlock*>(block));
			break;
		case BlockType::Shader_Fragment:
			verifyBlock_FragmentShader(dynamic_cast<FragmentShaderBlock*>(block));
			break;
		case BlockType::Shader_Geometry:
			verifyBlock_GeometryShader(dynamic_cast<GeometryShaderBlock*>(block));
			break;
		case BlockType::Shader_Vertex:
			verifyBlock_VertexShader(dynamic_cast<VertexShaderBlock*>(block));
			break;
		case BlockType::Shader_TessellationControl:
			verifyBlock_TessellationControlShader(dynamic_cast<TessellationControlShaderBlock*>(block));
			break;
		case BlockType::Shader_TessellationEvaluation:
			verifyBlock_TessellationEvaluationShader(dynamic_cast<TessellationEvaluationShaderBlock*>(block));
			break;
		case BlockType::CameraControl:
			verifyBlock_CameraControl(dynamic_cast<CameraControlBlock*>(block));
			break;
		case BlockType::ModelViewProjection:
			verifyBlock_ModelViewProjection(dynamic_cast<ModelViewProjectionBlock*>(block));
			break;

		case BlockType::Rasterization:
		case BlockType::FragmentTests:
		case BlockType::TransformFeedback:
		case BlockType::TessellationPrimitiveGenerator:
			verifyBlockGroup_FixedFunction(block);
			break;

		default:
			// Do nothing; some blocks will set their status on their own
			break;
		}

		updateStatus(block);
		return true;
	}

	bool ValidatePipelineVisitor::portVisited(Port* port)
	{
		updateStatus(port);
		return true;
	}

	bool ValidatePipelineVisitor::connectionVisited(Connection* connection)
	{
		updateStatus(connection);
		return true;
	}

	bool ValidatePipelineVisitor::renderCommandVisited(RenderCommand* command)
	{
		updateStatus(command);
		return true;
	}

	void ValidatePipelineVisitor::updateStatus(const IPipelineItem* item)
	{
		// Only update the current status if the new one is worse than the old one
		if (_status < item->getStatus())
			_status = item->getStatus();
	}

	void ValidatePipelineVisitor::verifyBlock_CameraControl(CameraControlBlock* block)
	{
		if(!block)
			throw std::bad_cast{};

		if( _pipeline->getBlocks(BlockType::CameraControl).size() > 1)
			block->setStatus(PipelineItemStatus::Sick, "There must not exist more than one Camera Control Block per pipeline");
		else if(block->getGenericOutPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_ModelViewProjection(ModelViewProjectionBlock* block)
	{
		if(!block)
			throw std::bad_cast{};

		if(block->getGenericInPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "There must not be more than one Camera Control Block connected");
		else if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_Mixer(MixerBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(block->getInConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Sick, "A mixer needs at least one datasource connected");
		else if(MixerLayout{block->getMixerLayout()->getValue()}.getEntries()->isEmpty())
			block->setStatus(PipelineItemStatus::Sick, "This mixer has not been configured, yet");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_Buffer(BufferBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(block->getDataInPort()->getConnectionCount() != 1 && block->getDataOutPort()->getConnectionCount() > 0)
			block->setStatus(PipelineItemStatus::Sick, "A buffer block must have exactly one incoming connection");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_VertexPuller(VertexPullerBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		bool foundCommand = false;
		unsigned int needIBO = 0;
		for(IRenderCommand* command : _pipeline->getRenderCommands())
		{
			if( command->getCommand() == RenderCommandType::Draw && command->getAssignedBlock() == block)
			{
				foundCommand = true;

				if(*command->getProperty<EnumProperty>(PropertyID::Draw_DrawMode) == DrawRenderCommand::DrawMode_Elements)
				{
					needIBO = 1;
					break;
				}
			}
		}

		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(foundCommand && block->getIboPort()->getConnectionCount() != needIBO)
		{
			if(needIBO)
				block->setStatus(PipelineItemStatus::Sick, "The connected render command needs to have exactly one IBO connected");
			else
				block->setStatus(PipelineItemStatus::Chilled, "The connected render command needs no connected IBO");
		}
		else if(!foundCommand)
			block->setStatus(PipelineItemStatus::Chilled, "This vertex puller has no assigned draw-rendercommand");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_VertexArrayObject(VertexArrayObjectBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getGenericOutPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if (block->getGenericInPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This Vertex Array Object has no connected buffers");
		else if(VaoLayout{block->getVaoLayout()->getValue()}.getEntries()->isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This Vertex Array Object has not been configured, yet");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_Display(DisplayBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if (block->getGenericInPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This Block is orphaned");
		else
			for(IRenderCommand* command : _pipeline->getRenderCommands())
			{
				if(	command->getCommand() == RenderCommandType::Clear &&
					command->getAssignedBlock() == block &&
					_pipeline->getRenderCommands().last() == command)
				{
					block->setStatus(PipelineItemStatus::Chilled, "the assigned clear command is the last command in the execution order");
					return;
				}
			}

		// In case we reach this statement, everything is ok
		block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_TextureSampler(TextureSamplerBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getGenericOutPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_Texture(TextureBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getDataInPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A texture block must not have more than one data source connected");
		else if(block->getSamplerPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A texture block must not have more than one sampler connected");
		else if(block->getDataInPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This texture block has no data input connection");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_TextureView(TextureViewBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(block->getTexturePort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A texture view block must not have more than one texture connected");
		else if(block->getSamplerPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A texture view block must not have more than one sampler connected");
		else if(block->getTexturePort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This texture view block has no texture connected");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_FrameBufferObject(FrameBufferObjectBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(block->getGenericInPort()->getConnectionCount() == 0 && block->getTexturePort()->getConnectionCount() > 0)
			block->setStatus(PipelineItemStatus::Sick, "A FBO block needs to be connected to at least one Fragment Tests block");
		else if(block->getGenericInPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Chilled, "Connecting multiple passes to a FBO  a beta feature and not verified completly. Stay cautious.");
		else
			for(IRenderCommand* command : _pipeline->getRenderCommands())
			{
				if(	command->getCommand() == RenderCommandType::Clear &&
					command->getAssignedBlock() == block &&
					_pipeline->getRenderCommands().last() == command)
				{
					block->setStatus(PipelineItemStatus::Chilled, "the assigned clear command is the last command in the execution order");
					return;
				}
			}

		// In case we reach this statement, everything is ok
		block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_RenderBuffer(RenderBufferBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if(block->getInConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlockGroup_Shader(ShaderBlock* block)
	{

		// Default orphanity check
		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");

		// A map containing all declarations for uniforms and samplers
		QSet<QString> declarations;

		// Look for multiple sampler declarations
		QSet<unsigned int> units;
		for(IConnection* connection : block->getTextureInPort()->getInConnections())
		{
			unsigned int unit = *connection->getProperty<UIntProperty>(PropertyID::Texture_Unit);
			if(units.contains(unit))
				block->setStatus(PipelineItemStatus::Chilled, "At least one unit of the connected texture units is set more than once");
			else
				units.insert(unit);

			QString name = *connection->getProperty<StringProperty>(PropertyID::Texture_Name);
			if(declarations.contains(name))
			{
				block->setStatus(PipelineItemStatus::Sick, "At least one uniform or sampler name is set more than once");

				// We can return here, because it can't become worse
				return;
			}
			else
				declarations.insert(name);
		}

		// Look for multiple uniform declarations
		QSet<int> locations;
		for(IConnection* connection : block->getUniformInPort()->getInConnections())
		{
			int location = *connection->getProperty<UIntProperty>(PropertyID::Uniform_Location);
			if(connection->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)->getValue() && locations.contains(location))
			{
				block->setStatus(PipelineItemStatus::Sick, "At least one location of the connected uniform is set more than once");

				// We can return here, because it can't become worse
				return;
			}
			else
				locations.insert(location);

			QString name = *connection->getProperty<StringProperty>(PropertyID::Uniform_Name);
			if(declarations.contains(name))
			{
				block->setStatus(PipelineItemStatus::Sick, "At least one uniform or sampler name is set more than once");

				// We can return here, because it can't become worse
				return;
			}
			else
				declarations.insert(name);
		}

		if(block->getGenericInPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A shader block generic in port must not have more than one incoming connection");
		else if(block->getGenericInPort()->getConnectionCount() == 0)
			block->setStatus(PipelineItemStatus::Chilled, "This shader block has no incoming connection");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlock_FragmentShader(FragmentShaderBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		verifyBlockGroup_Shader(block);

		if(block->getStatus() == PipelineItemStatus::Healthy)
		{
			// TODO: Check for connected buffers
		}
	}

	void ValidatePipelineVisitor::verifyBlock_VertexShader(VertexShaderBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		verifyBlockGroup_Shader(block);
	}

	void ValidatePipelineVisitor::verifyBlock_GeometryShader(GeometryShaderBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		verifyBlockGroup_Shader(block);
	}

	void ValidatePipelineVisitor::verifyBlock_TessellationControlShader(TessellationControlShaderBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		verifyBlockGroup_Shader(block);
	}

	void ValidatePipelineVisitor::verifyBlock_TessellationEvaluationShader(TessellationEvaluationShaderBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		verifyBlockGroup_Shader(block);
	}

	void ValidatePipelineVisitor::verifyBlock_CodeGenerator(CodeGeneratorBlock* block)
	{
		if (!block)
			throw std::bad_cast{};

		if (!block->getGenericInPort()->isConnectedTo(BlockType::FragmentTests, PortType::GenericOut))
			block->setStatus(PipelineItemStatus::Sick, "A code generator block must be connected to a fragment tests block");
		else if (block->getGenericInPort()->getConnectionCount() > 1)
			block->setStatus(PipelineItemStatus::Sick, "A code generator block may only have a single connection");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}

	void ValidatePipelineVisitor::verifyBlockGroup_FixedFunction(IBlock* block)
	{
		if(block->getOutConnections().isEmpty())
			block->setStatus(PipelineItemStatus::Chilled, "This block is orphaned");
		else if(block->getInConnections().size() != 1)
			block->setStatus(PipelineItemStatus::Sick, "This block needs exactly one incoming connection");
		else
			block->setStatus(PipelineItemStatus::Healthy);
	}
}
