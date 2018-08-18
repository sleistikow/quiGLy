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

#include "textureviewblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glwrapper.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/evaluationutils.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

#include "data/iblock.h"
#include "data/iport.h"
#include "data/iconnection.h"
#include "data/blocks/texturebaseblock.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>

namespace ysm
{

	TextureViewBlockEvaluator::TextureViewBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TextureViewBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::TextureView;
	}

	QList<BlockType> TextureViewBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Texture;
	}

	void TextureViewBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// In case the given block was evaluated before because of a dependency, skip it
		if(_preEvaluated.contains(block))
		{
			// Don't forget to remove it, since the set could grow to much otherwise
			_preEvaluated.remove(block);
			return;
		}

		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLTextureWrapper* wrapper = getEvaluator()->getEvaluatedData<GLTextureWrapper>(block);
		if(!wrapper)
		{
			// if wrapper was not found, create a new texture
			GLuint texture;
			f->glGenTextures(1, &texture);

			// Check for errors
			if(!texture)
				throw EvaluationException("Texture View could not be created", block);

			// Get the target the texture should be bound to
			GLenum target = EvaluationUtils::mapTextureTargetToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_TargetType)->getValue());

			// Wrap the result
			wrapper = new GLTextureWrapper(texture, target);
			getEvaluator()->setEvaluatedData(block, wrapper);

			// Get internal Format (used by texture and texture view)
			GLenum sizedInternalFormat = EvaluationUtils::mapInternalFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat));

			// Try to get the texture, the connection leads to
			QVector<IConnection*> textureCon = block->getPort(PortType::TextureView_Texture)->getInConnections();

			if(textureCon.size() != 1)
				throw EvaluationException("The TextureView may be connected to one texture only", block);

			IBlock* textureBlock = textureCon[0]->getSource();
			if(textureBlock->getType() == BlockType::Texture || textureBlock->getType() == BlockType::TextureView)
			{
				GLTextureWrapper* origTexture = getEvaluator()->getEvaluatedData<GLTextureWrapper>(textureBlock);
				if(!origTexture)
				{
					if(textureBlock->getType() == BlockType::Texture)
						throw EvaluationException("The connected Texture or Texture View has not been evaluated yet", block);
					else
					{
						// In case the orig texture view has not been evaluated yet, we just do it the favour and evaluate it right now.
						// We can do it fine, because no block is evaluated twice and the renderpassset guarantees that no cycles exist.
						evaluate(textureBlock, pass);

						// Insert the block afterwards(!)
						_preEvaluated.insert(textureBlock);

						// Once more get the evaluated data, now being evaluated
						origTexture = getEvaluator()->getEvaluatedData<GLTextureWrapper>(textureBlock);
					}
				}

				QOpenGLFunctions_4_3_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
				if(!functions)
					throw EvaluationException("Texture Views need OpenGL 4.3, which is not supported by your system", block);

				// Setup texture view
				functions->glTextureView(texture,
										 target,
										 origTexture->getValue(),
										 sizedInternalFormat,
										 block->getProperty<UIntProperty>(PropertyID::TextureView_MinimumLevel)->getValue(),
										 block->getProperty<UIntProperty>(PropertyID::TextureView_LevelCount)->getValue(),
										 block->getProperty<UIntProperty>(PropertyID::TextureView_MinimumLayer)->getValue(),
										 block->getProperty<UIntProperty>(PropertyID::TextureView_LayerCount)->getValue()
										 );
			}
			else
				throw EvaluationException("Invalid data source connected", block);

			//if Texturesampler is connected to this Texture -> use settings of sampler
			if(block->getPort(PortType::Texture_Sampler)->getInConnections().isEmpty())
			{
				//min/mag Filter
				f->glTexParameteri(target, GL_TEXTURE_MIN_FILTER, EvaluationUtils::mapFilterTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_MinFilter)->getValue()));
				f->glTexParameteri(target, GL_TEXTURE_MAG_FILTER, EvaluationUtils::mapFilterTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_MagFilter)->getValue()));
				//minlod/maxlod
				f->glTexParameterf(target, GL_TEXTURE_MIN_LOD, block->getProperty<FloatProperty>(PropertyID::TextureBase_MinLOD)->getValue());
				f->glTexParameterf(target, GL_TEXTURE_MAX_LOD, block->getProperty<FloatProperty>(PropertyID::TextureBase_MaxLOD)->getValue());
				//wrapping
				f->glTexParameteri(target, GL_TEXTURE_WRAP_R, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeR)->getValue()));
				f->glTexParameteri(target, GL_TEXTURE_WRAP_S, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeS)->getValue()));
				f->glTexParameteri(target, GL_TEXTURE_WRAP_T, EvaluationUtils::mapWrapModeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_WrapModeT)->getValue()));
				//bordercolor
				QVector4D color = block->getProperty<ColorProperty>(PropertyID::TextureBase_BorderColor)->getValue();
				float colorAsArray[] = {color.x(), color.y(), color.z(), color.w()};
				f->glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, colorAsArray);
				//compareMode
				bool compMode = block->getProperty<BoolProperty>(PropertyID::TextureBase_CompareMode)->getValue();
				if(compMode)
					f->glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
				else
					f->glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
				//compare function
				f->glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, EvaluationUtils::mapFunctionTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::TextureBase_CompareFunc)->getValue()));

				// check anisotropy level
				float actualMaxAnisotropy = 0.0f;
				f->glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &actualMaxAnisotropy);
				float desiredMaxAnisotropy = block->getProperty<FloatProperty>(PropertyID::TextureBase_Anisotropy)->getValue();

				if(desiredMaxAnisotropy > actualMaxAnisotropy || desiredMaxAnisotropy < 0)
					getEvaluator()->addWarning({QString("Anisotropy is not in the valid range [0, %1]").arg(actualMaxAnisotropy), block});

				// set, if desired value can be applied
				f->glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, desiredMaxAnisotropy);
			}

			// Release texture
			f->glBindTexture(target, 0);
		}

		// Get currently active program
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);

		// Find all neccessary bindings
		for(IConnection* connection : pass->getOutConnections(block))
		{
			// Skip connections targeting other texture views
			if(connection->getDest()->getType() == BlockType::TextureView)
				continue;

			// Get texture unit
			unsigned int unit = connection->getProperty<UIntProperty>(PropertyID::Texture_Unit)->getValue();

			// Get binding/location
			GLint location = -1;

			// if explicit binding is active, no need to get location
			if(!connection->getProperty<BoolProperty>(PropertyID::Texture_Binding)->getValue())
				location = program->uniformLocation(connection->getProperty<StringProperty>(PropertyID::Texture_Name)->getValue());

			// Encapsulate the data and store them
			TextureBindingParameter params;
			params.pass		= pass;
			params.location	= location;
			params.unit		= unit;
			wrapper->addBinding(params);
		}
	}

}
