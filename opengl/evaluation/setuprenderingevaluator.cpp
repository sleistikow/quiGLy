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

#include "setuprenderingevaluator.h"

#include "opengl/evaluation/evaluationexception.h"
#include "opengl/glrenderpassset.h"
#include "opengl/glrenderpass.h"
#include "opengl/glwrapper.h"

#include "blockevaluators/bufferblockevaluator.h"
#include "blockevaluators/framebufferobjectblockevaluator.h"
#include "blockevaluators/imageloaderblockevaluator.h"
#include "blockevaluators/renderbufferblockevaluator.h"
#include "blockevaluators/shaderblockevaluator.h"
#include "blockevaluators/textureblockevaluator.h"
#include "blockevaluators/textureloaderblockevaluator.h"
#include "blockevaluators/texturesamplerblockevaluator.h"
#include "blockevaluators/textureviewblockevaluator.h"
#include "blockevaluators/transformfeedbackblockevaluator.h"
#include "blockevaluators/uniformblockevaluator.h"
#include "blockevaluators/vertexarrayobjectblockevaluator.h"
#include "blockevaluators/vertexpullerblockevaluator.h"

#include "blockevaluators/fragmenttestsblockevaluator.h"
#include "blockevaluators/rasterizationblockevaluator.h"
#include "blockevaluators/multiuniformblockevaluator.h"
#include "blockevaluators/tessellationprimitivegeneratorblockevaluator.h"

#include "data/iblock.h"
#include "data/ipipeline.h"
#include "data/irendercommand.h"
#include "data/properties/property.h"

#include "views/logview/logview.h"

#include <QOpenGLShaderProgram>
#include <QQueue>

namespace ysm
{

SetupRenderingEvaluator::SetupRenderingEvaluator()
{
	// Register Non-Rendertime Evaluators
	registerBlockEvaluator<BufferBlockEvaluator>();
	registerBlockEvaluator<FrameBufferObjectBlockEvaluator>();
	registerBlockEvaluator<ImageLoaderBlockEvaluator>();
	registerBlockEvaluator<RenderBufferBlockEvaluator>();
	registerBlockEvaluator<ShaderBlockEvaluator>();
	registerBlockEvaluator<TextureBlockEvaluator>();
	registerBlockEvaluator<TextureLoaderBlockEvaluator>();
	registerBlockEvaluator<TextureSamplerBlockEvaluator>();
	registerBlockEvaluator<TextureViewBlockEvaluator>();
	registerBlockEvaluator<TransformFeedbackBlockEvaluator>();
	registerBlockEvaluator<UniformBlockEvaluator>();
	registerBlockEvaluator<VertexArrayObjectBlockEvaluator>();
	registerBlockEvaluator<VertexPullerBlockEvaluator>();

	// Register Rendertime Evaluators
	registerBlockEvaluator<FragmentTestsBlockEvaluator>();
	registerBlockEvaluator<RasterizationBlockEvaluator>();
	registerBlockEvaluator<MultiUniformBlockEvaluator>();
	registerBlockEvaluator<TessellationPrimitiveGeneratorBlockEvaluator>();

	// Now, build the evaluation order
	buildEvaluationOrder();
}

SetupRenderingEvaluator::~SetupRenderingEvaluator()
{
	// Set destruct flag
	// TODO: Find out why this is neccessary
	clear(true);

	// Delete all the evaluators
	for(IBlockEvaluator* evaluator : _evaluators.values().toSet())
		delete evaluator;
}

void SetupRenderingEvaluator::buildEvaluationOrder()
{
	//TODO: generate dynamically out of IBlockEvaluator::getDependencies()
	_evaluationOrder.clear();
	_evaluationOrder << BlockType::Buffer
					 << BlockType::RenderBuffer
					 << BlockType::FrameBufferObject
					 << BlockType::TransformFeedback
					 << BlockType::VertexArrayObject
					 << BlockType::Shader_Geometry
					 << BlockType::Shader_TessellationControl
					 << BlockType::Shader_TessellationEvaluation
					 << BlockType::Shader_Fragment
					 << BlockType::Shader_Vertex
					 << BlockType::VertexPuller
					 << BlockType::Texture
					 << BlockType::ImageLoader
					 << BlockType::TextureLoader
					 << BlockType::TextureView
					 << BlockType::TextureSampler
					 << BlockType::Uniform_Double
					 << BlockType::Uniform_Float
					 << BlockType::Uniform_Int
					 << BlockType::Uniform_UInt
					 << BlockType::Uniform_Vec2
					 << BlockType::Uniform_Vec3
					 << BlockType::Uniform_Vec4
					 << BlockType::Uniform_Mat3x3
					 << BlockType::Uniform_Mat4x4;
}

const QList<SetupRenderingEvaluator::Warning>& SetupRenderingEvaluator::getWarnings() const
{
	return _warnings;
}

QOpenGLShaderProgram* SetupRenderingEvaluator::getShaderProgram(GLRenderPass* pass) const
{
	return _shaderPrograms.value(pass, nullptr);
}

void SetupRenderingEvaluator::setEvaluatedData(IBlock *block, GLWrapper *data, bool replace)
{
	if(!replace && _evaluationData.contains(block))
		return;

	_evaluationData.insert(block, data);
}

void SetupRenderingEvaluator::addWarning(const Warning& warning)
{
	_warnings.append(warning);
}

void SetupRenderingEvaluator::clear(bool destruct)
{
	// If we have never evaluated anything, skip
	if(_evaluationData.isEmpty())
		return;

	// Delete Wrappers
	for(GLWrapper* wrapper : _evaluationData)
	{
		// Extract actual GL name
		GLuint value = wrapper->getValue();

		// Get the context, the ressource was created in
		// TODO: If rendering is already disabled, the wrapper seems to return an invalid context
		QOpenGLContext* context = wrapper->getContext();

		// Skip, if no context is available
		// Destruct flag is neccessary to avoid memory problems in destructor
		// TODO: Find the cause of theese problems and handle them correctly
		if(context && !destruct)
		{
			// Get a OpenGL functions object. We use the context the ressource was created in.
			GLConfiguration::Functions* f = context->versionFunctions<GLConfiguration::Functions>();

			// Delete shareable ressources, only
			switch(wrapper->getType())
			{
			case BlockType::Buffer:				f->glDeleteBuffers(1, &value);			break;
			case BlockType::RenderBuffer:		f->glDeleteRenderbuffers(1, &value);	break;
			case BlockType::TextureSampler:		f->glDeleteSamplers(1, &value);			break;
			case BlockType::Texture:			f->glDeleteTextures(1, &value);			break;
			default: // Nothing to do
				break;
			}
		}

		// Finally, delete wrapper
		delete wrapper;
	}

	// Clear everything left
	_evaluationData.clear();

	// Delete storages
	for(QOpenGLShaderProgram* shaderProgram : _shaderPrograms)
		delete shaderProgram;

	_shaderPrograms.clear();
}

BlockType SetupRenderingEvaluator::getEvaluatedBlockType() const
{
	return BlockType::Display;
}

void SetupRenderingEvaluator::evaluate(GLRenderPassSet* renderPassSet)
{
	if(!renderPassSet)
		throw std::runtime_error("renderPassSet may not be null");

	if(renderPassSet->getOutputBlock()->getType() != getEvaluatedBlockType())
		throw std::runtime_error("A SetupRenderingEvaluator can evaluate Display Blocks, only");

	GLConfiguration::Functions* functions = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();
	if(!functions)
		throw EvaluationException("The Functions object could not be initialized correctly");

	// Set Multisampling
	if(renderPassSet->getOutputBlock()->getProperty<BoolProperty>(PropertyID::Display_MultiSample)->getValue())
		functions->glEnable(GL_MULTISAMPLE);
	else
		functions->glDisable(GL_MULTISAMPLE);

	// Enable sRGB
	if(renderPassSet->getOutputBlock()->getProperty<BoolProperty>(PropertyID::Display_GammaCorrection)->getValue())
		functions->glEnable(GL_FRAMEBUFFER_SRGB);
	else
		functions->glDisable(GL_FRAMEBUFFER_SRGB);

	// Clear warnings
	_warnings.clear();

	// Evaluate all passes contained in the set
	for(GLRenderPass* pass : renderPassSet->getRenderPasses())
	{
		// Create a shader proram for the current pass
		//TODO: find a better place
		_shaderPrograms.insert(pass, new QOpenGLShaderProgram);

		// Evaluate blocks in defined order
		for(BlockType type : _evaluationOrder)
		{
			// Find an evaluator which can handle the current blocktype
			IBlockEvaluator* blockEvaluator = _evaluators.value(type, nullptr);

			// In case we found an evaluator, we can now handle the blocks of the current type
			if(blockEvaluator)
			{
				for(IBlock* block : pass->getBlocksByType(type))
					blockEvaluator->evaluate(block, pass);
			}
			else
				LogView::log(QString("No evaluator registered for Type %1")
							 .arg(static_cast<int>(type)));
		}
	}
}

void SetupRenderingEvaluator::initializeContext(IBlock* block, GLRenderPass* pass)
{
	// Get the blocks type
	BlockType type = block->getType();

	// Find an evaluator which can handle the current blocktype
	IBlockEvaluator* blockEvaluator = _initializers.value(type, nullptr);

	// In case we found an evaluator, we can now handle the blocks of the current type
	if(blockEvaluator)
	{
		for(IBlock* block : pass->getBlocksByType(type))
			blockEvaluator->evaluate(block, pass);
	}
	else
		LogView::log(QString("No initializer registered for Type %1")
					 .arg(static_cast<int>(type)));
}


}
