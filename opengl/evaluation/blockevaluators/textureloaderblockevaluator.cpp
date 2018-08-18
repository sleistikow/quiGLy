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

#include "textureloaderblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glrenderpass.h"
#include "opengl/glwrapper.h"
#include "opengl/gli.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/textureproperty.h"

#include <QOpenGLFunctions_4_2_Core>

namespace ysm
{

	TextureLoaderBlockEvaluator::TextureLoaderBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TextureLoaderBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::TextureLoader;
	}

	QList<BlockType> TextureLoaderBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Texture;
	}

	void TextureLoaderBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		// Once, get the image data
		const gli::texture* texture = block->getProperty<TextureProperty>(PropertyID::Texture_Texture)->getValue();
		if(!texture || texture->empty())
			return;

		// Fill all connected textures with the image data
		for(IConnection* connection : pass->getOutConnections(block))
		{
			if(connection->getDest()->getType() != BlockType::Texture)
				continue;

			IBlock* textureBlock = connection->getDest();

			//texture format
			gli::gl GL(gli::gl::PROFILE_GL33);
			gli::gl::format const format = GL.translate(texture->format(), texture->swizzles());

			//target
			GLenum target = GL.translate(texture->target());

			// Bind the connected texture
			GLTextureWrapper* textureName = getEvaluator()->getEvaluatedData<GLTextureWrapper>(textureBlock);
			if(!textureName)
				throw EvaluationException("Texture has not been evaluated yet", block);

			f->glBindTexture(target, textureName->getValue());

			glm::tvec3<GLsizei> const extent(texture->extent());
			GLsizei const faceTotal = static_cast<GLsizei>(texture->layers() * texture->faces());

			//handle texture storage (available since OpenGL Version 4.2)
			QOpenGLFunctions_4_2_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_2_Core>();
			if(functions)
			{
				switch(texture->target())
				{
					case gli::TARGET_1D:
						functions->glTexStorage1D(
										target, static_cast<GLint>(texture->levels()), format.Internal, extent.x);

						break;
					case gli::TARGET_1D_ARRAY:
					case gli::TARGET_2D:
					case gli::TARGET_CUBE:
						functions->glTexStorage2D(
										target, static_cast<GLint>(texture->levels()), format.Internal,
										extent.x, texture->target() == gli::TARGET_2D ? extent.y : faceTotal);
						break;
					case gli::TARGET_2D_ARRAY:
					case gli::TARGET_3D:
					case gli::TARGET_CUBE_ARRAY:
						functions->glTexStorage3D(
										target, static_cast<GLint>(texture->levels()), format.Internal,
										extent.x, extent.y,
										texture->target() == gli::TARGET_3D ? extent.z : faceTotal);
						break;
					default:
						assert(0);
						break;
				}

				for(std::size_t layer = 0; layer < texture->layers(); ++layer)
				for(std::size_t face = 0; face < texture->faces(); ++face)
				for(std::size_t level = 0; level < texture->levels(); ++level)
				{
					GLsizei const layerGL = static_cast<GLsizei>(layer);
					glm::tvec3<GLsizei> extent(texture->extent(level));
					target = gli::is_target_cube(texture->target())
							? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
							: target;


					switch(texture->target())
					{
						case gli::TARGET_1D:
								if(gli::is_compressed(texture->format()))
									f->glCompressedTexSubImage1D(
														target, static_cast<GLint>(level), 0, extent.x,
														format.Internal, static_cast<GLsizei>(texture->size(level)),
														texture->data(layer, face, level));
								else
									f->glTexSubImage1D(
														target, static_cast<GLint>(level), 0, extent.x,
														format.External, format.Type,
														texture->data(layer, face, level));

							break;
						case gli::TARGET_1D_ARRAY:
						case gli::TARGET_2D:
						case gli::TARGET_CUBE:
								if(gli::is_compressed(texture->format()))
									f->glCompressedTexSubImage2D(
														target, static_cast<GLint>(level),
														0, 0,
														extent.x,
														texture->target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
														format.Internal, static_cast<GLsizei>(texture->size(level)),
														texture->data(layer, face, level));
								else
									f->glTexSubImage2D(
														target, static_cast<GLint>(level),
														0, 0,
														extent.x,
														texture->target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
														format.External, format.Type,
														texture->data(layer, face, level));
							break;
						case gli::TARGET_2D_ARRAY:
						case gli::TARGET_3D:
						case gli::TARGET_CUBE_ARRAY:
								if(gli::is_compressed(texture->format()))
									f->glCompressedTexSubImage3D(
														target, static_cast<GLint>(level),
														0, 0, 0,
														extent.x, extent.y,
														texture->target() == gli::TARGET_3D ? extent.z : layerGL,
														format.Internal, static_cast<GLsizei>(texture->size(level)),
														texture->data(layer, face, level));
								else
									f->glTexSubImage3D(
														target, static_cast<GLint>(level),
														0, 0, 0,
														extent.x, extent.y,
														texture->target() == gli::TARGET_3D ? extent.z : layerGL,
														format.External, format.Type,
														texture->data(layer, face, level));
							break;
						default:
								assert(0);
							break;
					}
				}
			}
			//use Storage via glTexImage, loading just image at level 0 and generating mipmaps
			else
			{
				glm::tvec3<GLsizei> extent(texture->extent(0));

				switch(texture->target())
				{
					case gli::TARGET_1D:
						f->glTexImage1D(
										target,
										0,
										format.Internal,
										extent.x,
										0, //border not needed
										format.External,
										format.Type,
										texture->data(0, 0, 0)
										);
						break;
					case gli::TARGET_1D_ARRAY:
					case gli::TARGET_2D:
					case gli::TARGET_CUBE:
						f->glTexImage2D(
										target,
										0,
										format.Internal,
										extent.x,
										texture->target() == gli::TARGET_2D ? extent.y : faceTotal,
										0, //border not needed
										format.External,
										format.Type,
										texture->data(0, 0, 0)
										);
						break;
					case gli::TARGET_2D_ARRAY:
					case gli::TARGET_3D:
					case gli::TARGET_CUBE_ARRAY:
						f->glTexImage3D(
										target,
										0,
										format.Internal,
										extent.x,
										extent.y,
										texture->target() == gli::TARGET_3D ? extent.z : faceTotal,
										0, //border not needed
										format.External,
										format.Type,
										texture->data(0, 0, 0)
										);
						break;
					default:
						assert(0);
						break;
				}
				f->glGenerateMipmap(target);
			}
			f->glBindTexture(target, 0);

		}// end for
	}

}
