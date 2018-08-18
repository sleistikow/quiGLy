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

#include "framebufferobjectblockevaluator.h"
#include "opengl/glwrapper.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/evaluation/evaluationutils.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/property.h"
#include "data/blocks/framebufferobjectblock.h"

namespace ysm
{

	FrameBufferObjectBlockEvaluator::FrameBufferObjectBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> FrameBufferObjectBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::FrameBufferObject;
	}

	void FrameBufferObjectBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLContextSensitiveWrapper* wrapper = getEvaluator()->getEvaluatedData<GLContextSensitiveWrapper>(block);
		if(!wrapper)
		{
			wrapper = new GLContextSensitiveWrapper(BlockType::FrameBufferObject);
			getEvaluator()->setEvaluatedData(block, wrapper);
		}

		if(!wrapper->getValue())
		{
			// Create a new fbo
			GLuint fbo;
			f->glGenFramebuffers(1, &fbo);

			// Check for errors
			if(!fbo)
				throw EvaluationException("FBO could not be created", block);

			// Wrap the result
			wrapper->setValue(fbo);

			// Store all Color attachments here
			QVector<GLenum> colorAttachments;

			// Bind all connected textures and render buffers
			// Note, that we take all connections here, since the connections in the pass would be the same.
			for(IConnection* connection : block->getOutConnections())
			{
				// Get connected block
				IBlock* dest = connection->getDest();

				// Get the underlying OpenGL data
				GLWrapper* attachedBlock = getEvaluator()->getEvaluatedData(dest);
				if(!attachedBlock)
					throw EvaluationException("Attachment has not been evaluated yet");

				// Check for incorrect Attachments
				int attachmentType = *connection->getProperty<EnumProperty>(PropertyID::FrameBufferObject_Attachment);
				int desiredColorIndex = 0;
				if(attachmentType == FrameBufferObjectBlock::Attachment_Color0)
				{
					desiredColorIndex = *connection->getProperty<UIntProperty>(PropertyID::FrameBufferObject_ColorIndex);
					int maxColorIndex = 0;
					f->glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorIndex);

					if(desiredColorIndex >= maxColorIndex)
						getEvaluator()->addWarning({QString("Desired color buffer index is higher than possible, which is %1.").arg(maxColorIndex), block});

					colorAttachments.append(GL_COLOR_ATTACHMENT0 + desiredColorIndex);
				}

				// Bind framebuffer object
				f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

				// Get the actual attachment
				GLenum attachment = EvaluationUtils::mapAttachmentToOpenGL(*connection->getProperty<EnumProperty>(PropertyID::FrameBufferObject_Attachment), desiredColorIndex);

				if(dest->getType() == BlockType::Texture || dest->getType() == BlockType::TextureView)
				{
					GLenum target = EvaluationUtils::mapTextureTargetToOpenGL(*dest->getProperty<EnumProperty>(PropertyID::Texture_TargetType));

					unsigned int level = *connection->getProperty<UIntProperty>(PropertyID::FrameBufferObject_TextureLevel);
					unsigned int layer = *connection->getProperty<UIntProperty>(PropertyID::FrameBufferObject_TextureLayer);

					switch(target)
					{
						// handle cube maps
						case GL_TEXTURE_CUBE_MAP:
							target = EvaluationUtils::mapCubeMapSideToOpenGL(*connection->getProperty<EnumProperty>(PropertyID::FrameBufferObject_CubeMapSide));
							f->glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, attachedBlock->getValue(), level);
							break;

						// handle all layered textures
						case GL_TEXTURE_3D:
						case GL_TEXTURE_1D_ARRAY:
						case GL_TEXTURE_2D_ARRAY:
						case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
						case GL_TEXTURE_CUBE_MAP_ARRAY:
							if(!connection->getProperty<BoolProperty>(PropertyID::FrameBufferObject_AllLayers)->getValue())
								f->glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, attachedBlock->getValue(), level, layer);
							else
								f->glFramebufferTexture(GL_FRAMEBUFFER, attachment, attachedBlock->getValue(), level);
							break;

						// handle all non-layered textures
						default:
							if (target == GL_TEXTURE_RECTANGLE || target == GL_TEXTURE_BUFFER) { //TODO: did I forgot smth?
								if (level > 0) {
									getEvaluator()->addWarning({"When rendering into an unlayered target, level has not be 0", block});
									level = 0;
								}
							}
							f->glFramebufferTexture(GL_FRAMEBUFFER, attachment, attachedBlock->getValue(), level);
							break;
					}
				}
				else if(dest->getType() == BlockType::RenderBuffer)
				{
					f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, attachedBlock->getValue());
				}

				// Release Framebuffer object
				f->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

			}

			// Need to unbind fbo directly, that the default FBO is bound in case an error occurd.
			f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			//TODO: map out location to attachments, according to editor..?
			if(colorAttachments.isEmpty())
				f->glDrawBuffer(GL_NONE);
			else
				f->glDrawBuffers(colorAttachments.size(), colorAttachments.data());

			// check the FBO status...
			GLenum status = f->glCheckFramebufferStatus(GL_FRAMEBUFFER);

			// Bind default FBO to be able to throw exceptions without leaving behind the incomplete FBO.
			f->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
			if(status != GL_FRAMEBUFFER_COMPLETE)
			{
				switch(status)
				{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					throw EvaluationException("At least one attachment is not attachment complete", block);
				//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				//	throw EvaluationException("Not all attached images have the same dimension", block);
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					throw EvaluationException("No image was attached to the framebuffer", block);
				case GL_FRAMEBUFFER_UNSUPPORTED:
					throw EvaluationException("The combination of internal formats of the attached images are not valid", block);
				default:
					throw EvaluationException("FBO could not be setup completly for an unknown reason", block);
				}
			}
		}
	}

}
