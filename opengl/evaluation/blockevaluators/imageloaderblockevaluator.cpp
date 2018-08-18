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

#include "imageloaderblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/evaluationutils.h"
#include "opengl/glwrapper.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/property.h"
#include "data/blocks/texturebaseblock.h"

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions_4_3_Core>
#include <QtMath>

namespace ysm
{

	ImageLoaderBlockEvaluator::ImageLoaderBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> ImageLoaderBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::ImageLoader;
	}

	QList<BlockType> ImageLoaderBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Texture;
	}

	void ImageLoaderBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		for(IConnection* connection : pass->getOutConnections(block))
		{
			IBlock* textureBlock = connection->getDest();

			if(textureBlock->getType() != BlockType::Texture)
				continue;

			// Bind the connected texture
			GLenum target = EvaluationUtils::mapTextureTargetToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::Texture_TargetType));

			GLTextureWrapper* texture = getEvaluator()->getEvaluatedData<GLTextureWrapper>(textureBlock);
			if(!texture)
				throw EvaluationException("Texture has not been evaluated yet", block);

			f->glBindTexture(target, texture->getValue());

			// Get the size of the image
			QSize imgSize = *block->getProperty<SizeProperty>(PropertyID::Img_Size);

			//calc and set number of possible mipmap levels
			int levels = qFloor(qLn(qMax(imgSize.width(),imgSize.height()))/qLn(2)) + 1;

			QOpenGLFunctions_4_2_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_2_Core>();

			// set the actual data
			switch (*textureBlock->getProperty<EnumProperty>(PropertyID::Texture_TargetType)) {
			case TextureBaseBlock::Target_Proxy1D:
			case TextureBaseBlock::Target_1D:
				if(functions)
					functions->glTexStorage1D(target,
											  levels,
											  EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
											  imgSize.width());
				else
					f->glTexImage1D(target,
									0,
									//internal format set in textureblock
									EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
									imgSize.width(),
									0,				//border not needed
									EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
									EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
									0);
				//load the data
				f->glTexSubImage1D(target,
								   0, 0,
								   imgSize.width(),
								   EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
								   EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
								   &(block->getProperty<Vec4DataProperty>(PropertyID::Data_TexelColors)->getValue().data()[0]));
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
											  EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
											  imgSize.width(),
											  imgSize.height());
				else
					f->glTexImage2D(target,
									0,
									EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
									imgSize.width(),
									imgSize.height(),
									0,				//border not needed
									EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
									EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
									0);

					//load the data
				f->glTexSubImage2D(target,
								   0, 0, 0,
								   imgSize.width(),
								   target == TextureBaseBlock::Target_1DArray ? 0 : imgSize.height(),
								   EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
								   EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
								   &(block->getProperty<Vec4DataProperty>(PropertyID::Data_TexelColors)->getValue().data()[0])
								   );
				break;
			case TextureBaseBlock::Target_2DMS:
			case TextureBaseBlock::Target_Proxy2DMS:
				/*
				f->glTexImage2DMultisample(target,
										   4,
										   EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
										   imgSize.width(),
										   imgSize.height(),
										   GL_TRUE);
										   */

				break;
			case TextureBaseBlock::Target_2DMSArray:
			case TextureBaseBlock::Target_Proxy2DMSArray:
				//glTexImage3DMultisample
				break;
			case TextureBaseBlock::Target_Proxy2DArray:
			case TextureBaseBlock::Target_Proxy3D:
			case TextureBaseBlock::Target_2DArray:
			case TextureBaseBlock::Target_3D:
				if(functions)
					functions->glTexStorage3D(target,
											  levels,
											  EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
											  imgSize.width(),
											  imgSize.height(),
											  1); //depth
				else
					f->glTexImage3D(target,
									0,
									EvaluationUtils::mapInternalFormatToOpenGL(*textureBlock->getProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat)),
									imgSize.width(),
									imgSize.height(),
									1,				//depth
									0,				//border not needed
									EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
									EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
									0);

				//load the data
				f->glTexSubImage3D(target,
								   0, 0, 0, 0,
								   imgSize.width(),
								   imgSize.height(),
								   0,
								   EvaluationUtils::mapPixelDataFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataFormat)),
								   EvaluationUtils::mapPixelDataTypeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::Img_PixelDataType)),
								   &(block->getProperty<Vec4DataProperty>(PropertyID::Data_TexelColors)->getValue().data()[0]));
				break;
			default:
				break;
			}

			// Enable Mipmapping (set in Textureblock)
			bool mipMapEnabled = textureBlock->getProperty<BoolProperty>(PropertyID::Texture_Mipmaps)->getValue();
			if(mipMapEnabled)
					f->glGenerateMipmap(target);

			//Base and Max Level Mipmap (set in Textureblock)
			f->glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, textureBlock->getProperty<IntProperty>(PropertyID::Texture_BaseLevel)->getValue());
			f->glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, textureBlock->getProperty<IntProperty>(PropertyID::Texture_MaxLevel)->getValue());

			f->glBindTexture(target, 0);
		}


	}

}
