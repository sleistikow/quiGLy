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

#include "textureblockevaluator.h"
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

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QtMath>

namespace ysm
{

	TextureBlockEvaluator::TextureBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TextureBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::Texture;
	}

	QList<BlockType> TextureBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Buffer;
	}

	void TextureBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
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
				throw EvaluationException("Texture could not be created", block);

			// Get the target the texture should be bound to
			GLenum target = EvaluationUtils::mapTextureTargetToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_TargetType)->getValue());

			// Wrap the result
			wrapper = new GLTextureWrapper(texture, target);
			getEvaluator()->setEvaluatedData(block, wrapper);

			// Bind the texture
			f->glBindTexture(target, texture);

			// Get internal Format (used by texture and texture view)
			GLenum sizedInternalFormat = EvaluationUtils::mapInternalFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat));

			// In case the datasource is a buffer, connect the texture to it
			QVector<IConnection*> dataCon = block->getPort(PortType::Data_In)->getInConnections();
			if(dataCon.size() == 1)
			{
				IBlock* dataBlock = dataCon[0]->getSource();
				if(dataBlock->getType() == BlockType::Buffer)
				{
					GLWrapper* buffer = getEvaluator()->getEvaluatedData(dataBlock);
					if(!buffer)
						throw EvaluationException("Buffer has not been evaluated yet", block);

					// Setup Texture Buffer
					f->glBindBuffer(GL_TEXTURE_BUFFER, buffer->getValue());
					f->glTexBuffer(target, sizedInternalFormat, buffer->getValue());
					f->glBindBuffer(GL_TEXTURE_BUFFER, 0);
				}
				else if(dataBlock->getType() == BlockType::FrameBufferObject)
				{
					unsigned int width, height;
					IConnection* fboToTex = dataCon[0];

					// retrieve size from corresponding block
					if(block->getProperty<BoolProperty>(PropertyID::Texture_RenderBufferAutoSize)->getValue())
					{
						width = dataBlock->getProperty<UIntProperty>(PropertyID::RenderBuffer_Width)->getValue();
						height = dataBlock->getProperty<UIntProperty>(PropertyID::RenderBuffer_Height)->getValue();
					}
					else
					{
						width = block->getProperty<UIntProperty>(PropertyID::Texture_Width)->getValue();
						height = block->getProperty<UIntProperty>(PropertyID::Texture_Height)->getValue();
					}

					//calc and set number of possible mipmap levels
					int levels = qFloor(qLn(qMax(width, height))/qLn(2)) + 1;

					QOpenGLFunctions_4_2_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_2_Core>();

					// set the actual data
					switch (*block->getProperty<EnumProperty>(PropertyID::Texture_TargetType))
					{
					case TextureBaseBlock::Target_Proxy1D:
					case TextureBaseBlock::Target_1D:
						if(functions)
							functions->glTexStorage1D(target,
													  levels,
													  sizedInternalFormat,
													  width);
						else
							f->glTexImage1D(target,
											0,
											//internal format set in textureblock
											sizedInternalFormat,
											width,
											0,				//border not needed
											EvaluationUtils::mapPixelDataFormatToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
											EvaluationUtils::mapPixelDataTypeToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
											0);
						break;
					case TextureBaseBlock::Target_1DArray:
					case TextureBaseBlock::Target_2DRect:
					case TextureBaseBlock::Target_Proxy2DRect:
					case TextureBaseBlock::Target_2D:
					case TextureBaseBlock::Target_Proxy2D:
					case TextureBaseBlock::Target_Proxy1DArray:
					case TextureBaseBlock::Target_CubeMapPosX:
					case TextureBaseBlock::Target_CubeMapNegX:
					case TextureBaseBlock::Target_CubeMapPosY:
					case TextureBaseBlock::Target_CubeMapNegY:
					case TextureBaseBlock::Target_CubeMapPosZ:
					case TextureBaseBlock::Target_CubeMapNegZ:
					case TextureBaseBlock::Target_ProxyCubeMap:
						if(functions)
							functions->glTexStorage2D(target,
													  levels,
													  sizedInternalFormat,
													  width,
													  height);
						else
							f->glTexImage2D(target,
											0,
											sizedInternalFormat,
											width,
											height,
											0,				//border not needed
											EvaluationUtils::mapPixelDataFormatToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
											EvaluationUtils::mapPixelDataTypeToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
											0);
						break;
					case TextureBaseBlock::Target_2DMS:
					case TextureBaseBlock::Target_Proxy2DMS:
						break;
					case TextureBaseBlock::Target_2DMSArray:
					case TextureBaseBlock::Target_Proxy2DMSArray:
						break;
					case TextureBaseBlock::Target_Proxy2DArray:
					case TextureBaseBlock::Target_Proxy3D:
					case TextureBaseBlock::Target_2DArray:
					case TextureBaseBlock::Target_3D:
						if(functions)
							functions->glTexStorage3D(target,
													  levels,
													  sizedInternalFormat,
													  width,
													  height,
													  1); //depth
						else
							f->glTexImage3D(target,
											0,
											sizedInternalFormat,
											width,
											height,
											1,				//depth
											0,				//border not needed
											EvaluationUtils::mapPixelDataFormatToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
											EvaluationUtils::mapPixelDataTypeToOpenGL(*fboToTex->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
											0);
						break;
					default:
						break;
					}
				}
			}

			//settings available only in texture
#ifndef Q_OS_MAC
			//Texturemode available since 4.3 Version
			QOpenGLFunctions_4_3_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
			if(functions)
				functions->glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, EvaluationUtils::mapTextureModeTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Texture_DepthStencilMode)));
#endif

			//Swizzling
			GLint swizzleMask[] = {EvaluationUtils::mapSwizzleTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_SwizzleRed)->getValue()),
								   EvaluationUtils::mapSwizzleTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_SwizzleGreen)->getValue()),
								   EvaluationUtils::mapSwizzleTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_SwizzleBlue)->getValue()),
								   EvaluationUtils::mapSwizzleTypeToOpenGL(block->getProperty<EnumProperty>(PropertyID::Texture_SwizzleAlpha)->getValue())
								  };
			f->glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

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
			// Skip connection, if going from texture to texture view
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
