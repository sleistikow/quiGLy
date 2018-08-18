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

#include "transformfeedbackblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glrenderpass.h"
#include "opengl/glwrapper.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/evaluationutils.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/varyingsproperty.h"

#include <QOpenGLShaderProgram>

namespace ysm
{

	TransformFeedbackBlockEvaluator::TransformFeedbackBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TransformFeedbackBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::TransformFeedback;
	}

	void TransformFeedbackBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);

		QVector<const GLchar*> feedbackVaryings;

		Varyings varyings{block->getProperty<VaryingsProperty>(PropertyID::Varyings)->getValue()};
		for(const Varyings::VaryingEntry& entry : *varyings.getEntries())
			feedbackVaryings.append(static_cast<const GLchar*>(entry.name.toStdString().c_str()));

		GLenum bufferMode = EvaluationUtils::mapBufferModeToOpenGL(*block->getProperty<EnumProperty>(PropertyID::TransformFeedback_BufferMode));

		f->glTransformFeedbackVaryings(program->programId(),
									   feedbackVaryings.size(),
									   feedbackVaryings.data(),
									   bufferMode
									   );

		for(IConnection* connection : pass->getOutConnections(block))
		{
			if(connection->getDest()->getType() != BlockType::Buffer)
				continue;

			GLWrapper* buffer = getEvaluator()->getEvaluatedData(connection->getDest());
			if(!buffer)
				throw EvaluationException("Buffer has not been evaluated yet", block);

			unsigned int index = *connection->getProperty<UIntProperty>(PropertyID::TransformFeedback_Index);
			f->glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer->getValue());
		}

	}

}

