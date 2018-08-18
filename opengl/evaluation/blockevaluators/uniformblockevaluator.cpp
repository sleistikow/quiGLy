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

#include "uniformblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/property.h"

#include <QOpenGLShaderProgram>

namespace ysm
{

	UniformBlockEvaluator::UniformBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> UniformBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::Uniform_Int
								  << BlockType::Uniform_UInt
								  << BlockType::Uniform_Float
								  << BlockType::Uniform_Double
								  << BlockType::Uniform_Vec2
								  << BlockType::Uniform_Vec3
								  << BlockType::Uniform_Vec4
								  << BlockType::Uniform_Mat3x3
								  << BlockType::Uniform_Mat4x4;
	}

	QList<BlockType> UniformBlockEvaluator::getDependencies() const
	{
		// Shader needs to be linked before
		return BlockEvaluator::getDependencies() << BlockType::VertexPuller;
	}

	void UniformBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Look, if the block has already been evaluated
		// Get shaderprogram
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);

		// Iterate over all outgoing connections
		for(IConnection* connection : pass->getOutConnections(block))
		{
			// In case the uniform is connected to a buffer, we don't need to setup OpenGL
			if(connection->getDest()->getType() == BlockType::Buffer ||
				connection->getDest()->getType() == BlockType::Mixer)
				continue;

			// Get uniform location
			GLint location = -1;
			if(connection->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)->getValue())
				connection->getProperty<UIntProperty>(PropertyID::Uniform_Location)->getValue();
			else
				location = program->uniformLocation(*connection->getProperty<StringProperty>(PropertyID::Uniform_Name));

			switch(block->getType())
			{
			case BlockType::Uniform_Int:
				program->setUniformValue(location, *block->getProperty<IntProperty>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_UInt:
				program->setUniformValue(location, *block->getProperty<UIntProperty>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Double:
				//keine Double Uniforms in OpenGL
			case BlockType::Uniform_Float:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Vec2:
				program->setUniformValue(location, *block->getProperty<Vec2Property>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Vec3:
				program->setUniformValue(location, *block->getProperty<Vec3Property>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Vec4:
				program->setUniformValue(location, *block->getProperty<Vec4Property>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Mat3x3:
				program->setUniformValue(location, *block->getProperty<Mat3x3Property>(PropertyID::Uniform_Value));
				break;
			case BlockType::Uniform_Mat4x4:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::Uniform_Value));
				break;
			default:
				throw EvaluationException("Unhandled uniform found!", block);
			}
		}

	}

}
