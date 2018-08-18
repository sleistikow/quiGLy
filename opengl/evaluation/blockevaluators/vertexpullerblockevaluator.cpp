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

#include "vertexpullerblockevaluator.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationexception.h"

#include <QOpenGLShaderProgram>

namespace ysm
{

	VertexPullerBlockEvaluator::VertexPullerBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> VertexPullerBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::VertexPuller;
	}

	QList<BlockType> VertexPullerBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Shader_Fragment
												 << BlockType::Shader_Geometry
												 << BlockType::Shader_TessellationControl
												 << BlockType::Shader_TessellationEvaluation
												 << BlockType::Shader_Vertex;
	}

	void VertexPullerBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get the shader program
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);

		// Link our program
		if(!program->link())
			throw EvaluationException("Linking Shaderprogram failed", block, program->log());

		// Bind the program
		program->bind();
	}

}
