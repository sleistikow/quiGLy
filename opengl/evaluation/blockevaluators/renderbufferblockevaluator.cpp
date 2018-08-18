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

#include "renderbufferblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glwrapper.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationutils.h"

#include "data/iblock.h"
#include "data/properties/property.h"


namespace ysm
{

	RenderBufferBlockEvaluator::RenderBufferBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> RenderBufferBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::RenderBuffer;
	}

	void RenderBufferBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLWrapper* wrapper = getEvaluator()->getEvaluatedData(block);
		if(!wrapper)
		{
			// Create a new renderbuffer
			GLuint renderbuffer;
			f->glGenRenderbuffers(1, &renderbuffer);

			// Wrap the result
			wrapper = new GLWrapper(BlockType::RenderBuffer, renderbuffer);
			getEvaluator()->setEvaluatedData(block, wrapper);

			// Bind the renderbuffer
			f->glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

			// Set the params
			GLenum format = EvaluationUtils::mapInternalFormatToOpenGL(*block->getProperty<EnumProperty>(PropertyID::RenderBuffer_InternalFormat));
			unsigned int samples = *block->getProperty<UIntProperty>(PropertyID::RenderBuffer_Samples);
			unsigned int width = *block->getProperty<UIntProperty>(PropertyID::RenderBuffer_Width);
			unsigned int height = *block->getProperty<UIntProperty>(PropertyID::RenderBuffer_Height);

			if(samples)
				getEvaluator()->addWarning({"Multisampling currently not supported for Renderbuffers", block});

			f->glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);

			// Release Renderbuffer
			f->glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

}
