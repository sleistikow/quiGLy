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

#include "texturesamplerblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glwrapper.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/evaluationutils.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationexception.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/property.h"

namespace ysm
{

	TextureSamplerBlockEvaluator::TextureSamplerBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TextureSamplerBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::TextureSampler;
	}

	QList<BlockType> TextureSamplerBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Texture;
	}

	void TextureSamplerBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLWrapper* wrapper = getEvaluator()->getEvaluatedData(block);
		if(!wrapper)
		{
			GLuint sampler;
			f->glGenSamplers(1, &sampler);

			// Check for errors
			if(!sampler)
				throw EvaluationException("Sampler could not be created", block);

			// Wrap the result
			wrapper = new GLWrapper(BlockType::TextureSampler, sampler);
			getEvaluator()->setEvaluatedData(block, wrapper);

			// set the texture parameter via property
			//min/mag Filter
			f->glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, EvaluationUtils::mapFilterTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_MinFilter)->getValue()));
			f->glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, EvaluationUtils::mapFilterTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_MagFilter)->getValue()));
			//minlod/maxlod
			f->glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, block->getProperty<FloatProperty>(PropertyID::TextureBase_MinLOD)->getValue());
			f->glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, block->getProperty<FloatProperty>(PropertyID::TextureBase_MaxLOD)->getValue());
			//wrapping
			f->glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeR)->getValue()));
			f->glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeS)->getValue()));
			f->glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeT)->getValue()));
			//bordercolor
			QVector4D color = block->getProperty<ColorProperty>(PropertyID::TextureBase_BorderColor)->getValue();
			float colorAsArray[] = {color.x(), color.y(), color.z(), color.w()};
			f->glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, colorAsArray);
			//compareMode
			bool compMode = block->getProperty<BoolProperty>(PropertyID::TextureBase_CompareMode)->getValue();
			if(compMode)
				f->glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			else
				f->glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			//compare function
			f->glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, EvaluationUtils::mapFunctionTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_CompareFunc)->getValue()));

			// check anisotropy level
			float actualMaxAnisotropy = 0.0f;
			f->glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &actualMaxAnisotropy);
			float desiredMaxAnisotropy = block->getProperty<FloatProperty>(PropertyID::TextureBase_Anisotropy)->getValue();

			if(desiredMaxAnisotropy > actualMaxAnisotropy || desiredMaxAnisotropy < 0)
				getEvaluator()->addWarning({QString("Anisotropy is not in the valid range [0, %1]").arg(actualMaxAnisotropy), block});

			// set, if desired value can be applied
			f->glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, desiredMaxAnisotropy);
		}

		// Add a binding for each connected texture
		for(IConnection* textureConnection : pass->getOutConnections(block))
		{
			// Get connected texture or texture view
			GLTextureWrapper* texture = getEvaluator()->getEvaluatedData<GLTextureWrapper>(textureConnection->getDest());
			if(!texture)
				throw EvaluationException("Texture has not been evaluated yet", block);

			// Set this sampler to be used in the connected texture
			texture->setSampler(wrapper->getValue());
		}

	}

}
