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

#ifndef VALIDATEPIPELINEVISITOR_H
#define VALIDATEPIPELINEVISITOR_H

#include "pipelinevisitor.h"
#include "data/pipeline/pipelineitemstatus.h"

namespace ysm
{
	class IBlock;
	class IPipelineItem;
	class MixerBlock;
	class BufferBlock;
	class VertexPullerBlock;
	class VertexArrayObjectBlock;
	class DisplayBlock;
	class TextureSamplerBlock;
	class TextureBlock;
	class TextureViewBlock;
	class FrameBufferObjectBlock;
	class RenderBufferBlock;
	class ShaderBlock;
	class FragmentShaderBlock;
	class VertexShaderBlock;
	class TessellationControlShaderBlock;
	class TessellationEvaluationShaderBlock;
	class GeometryShaderBlock;
	class CodeGeneratorBlock;
	class CameraControlBlock;
	class ModelViewProjectionBlock;
	class DisplayBlock;

	class ValidatePipelineVisitor : public PipelineVisitor
	{
	public:
		// Construction
		explicit ValidatePipelineVisitor(Pipeline* pipeline);

	public:
		/**
		 * @brief Gets the status of the entire pipeline
		 */
		PipelineItemStatus getStatus() const;

	public:
		bool blockVisited(Block* block) override;
		bool portVisited(Port* port) override;
		bool connectionVisited(Connection* connection) override;
		bool renderCommandVisited(RenderCommand* command) override;

	protected:
		/**
		 * @brief Updates the overall status of the pipeline, not lowering the current status
		 */
		void updateStatus(const IPipelineItem* item);

		// Single Block verification
		void verifyBlock_Mixer(MixerBlock* block);
		void verifyBlock_Buffer(BufferBlock* block);
		void verifyBlock_VertexPuller(VertexPullerBlock* block);
		void verifyBlock_VertexArrayObject(VertexArrayObjectBlock* block);
		void verifyBlock_Display(DisplayBlock* block);
		void verifyBlock_TextureSampler(TextureSamplerBlock* block);
		void verifyBlock_Texture(TextureBlock* block);
		void verifyBlock_TextureView(TextureViewBlock* block);
		void verifyBlock_FrameBufferObject(FrameBufferObjectBlock* block);
		void verifyBlock_RenderBuffer(RenderBufferBlock* block);
		void verifyBlock_FragmentShader(FragmentShaderBlock* block);
		void verifyBlock_VertexShader(VertexShaderBlock* block);
		void verifyBlock_TessellationControlShader(TessellationControlShaderBlock* block);
		void verifyBlock_TessellationEvaluationShader(TessellationEvaluationShaderBlock* block);
		void verifyBlock_GeometryShader(GeometryShaderBlock* block);
		void verifyBlock_CodeGenerator(CodeGeneratorBlock* block);
		void verifyBlock_CameraControl(CameraControlBlock* block);
		void verifyBlock_ModelViewProjection(ModelViewProjectionBlock* block);

		// Verification of blocks having the same criteria
		void verifyBlockGroup_Shader(ShaderBlock* block);
		void verifyBlockGroup_FixedFunction(IBlock* block);

	private:
		Pipeline* _pipeline{nullptr};

		PipelineItemStatus _status{PipelineItemStatus::Healthy};
	};
}

#endif
