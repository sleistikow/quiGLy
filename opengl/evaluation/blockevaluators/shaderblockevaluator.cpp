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

#include "shaderblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/glwrapper.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/glslparser/glsldocument.h"

#include "data/iblock.h"
#include "data/iport.h"
#include "data/iconnection.h"
#include "data/properties/glsldocumentlistproperty.h"

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions_4_3_Core>

namespace ysm
{

	ShaderBlockEvaluator::ShaderBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> ShaderBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::Shader_Fragment
								  << BlockType::Shader_Geometry
								  << BlockType::Shader_TessellationControl
								  << BlockType::Shader_TessellationEvaluation
								  << BlockType::Shader_Vertex;
	}

	QList<BlockType> ShaderBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Buffer;
	}

	void ShaderBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Get the type of the shader, depending on the given blocks type
		QOpenGLShader::ShaderType shaderType;
		switch(block->getType())
		{
		case BlockType::Shader_Vertex:					shaderType = QOpenGLShader::Vertex;					break;
		case BlockType::Shader_Geometry:				shaderType = QOpenGLShader::Geometry;				break;
		case BlockType::Shader_TessellationControl:		shaderType = QOpenGLShader::TessellationControl;	break;
		case BlockType::Shader_TessellationEvaluation:	shaderType = QOpenGLShader::TessellationEvaluation;	break;
		case BlockType::Shader_Fragment:				shaderType = QOpenGLShader::Fragment;				break;
		default:										throw EvaluationException("Unsupported shader found", block);
		}

		// Get the currently active program
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);
		GLShaderWrapper* wrapper = getEvaluator()->getEvaluatedData<GLShaderWrapper>(block);
		if(wrapper)
		{
			// in case the shaderblock has already been evaluated, append the cached shaders
			for(QOpenGLShader* shader : wrapper->getShaders())
				program->addShader(shader);
		}
		else
		{
			// Create a new shader wrapper
			wrapper = new GLShaderWrapper();
			getEvaluator()->setEvaluatedData(block, wrapper);

			const GLSLDocumentList& shaderCodes = *block->getProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code);
			for(GLSLDocument* code : shaderCodes.getDocuments())
			{
				// Create a shader wrapper
				QOpenGLShader* shader = new QOpenGLShader(shaderType);

				// Add it to the program
				program->addShader(shader);

				// Try to compile the code
				if(!shader->compileSourceCode(code->getSavedCode()))
					throw EvaluationException("Compiling failed", block, shader->log());

				// Add the shader to the wrapper
				wrapper->addShader(shader);
			}
		}

		//UBO binding
		for(IConnection* connection : block->getPort(PortType::Shader_UBO)->getInConnections())
		{
			GLWrapper* buffer = getEvaluator()->getEvaluatedData(connection->getSource());
			if(!buffer)
				throw EvaluationException("Buffer has not been evaluated yet", block);

			QString uniformBlockName = *connection->getProperty<StringProperty>(PropertyID::Buffer_Name);

			f->glLinkProgram(program->programId());
			GLuint index = f->glGetUniformBlockIndex(program->programId(), uniformBlockName.toStdString().c_str());
			GLint binding = *connection->getProperty<UIntProperty>(PropertyID::Buffer_Binding);
			GLint maxBinding = 0;
			f->glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBinding);

			if(binding > maxBinding)
				getEvaluator()->addWarning({QString("Binding point is not in the valid range [0, %1]").arg(maxBinding), block});

			f->glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer->getValue());

			f->glUniformBlockBinding(program->programId(), index, binding);

		}

		//SSBO binding
#ifndef Q_OS_MAC
		QVector<IConnection*> outConnections;
		outConnections.append(pass->getOutConnections(block, PortType::Shader_SSBOOut).toVector());
		outConnections.append(block->getPort(PortType::Shader_SSBOIn)->getInConnections());

		for(IConnection* connection : outConnections)
		{
			QOpenGLFunctions_4_3_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
			if(!functions)
				throw EvaluationException("SSBOs need at least OpenGL 4.3", block);

			GLWrapper* buffer = getEvaluator()->getEvaluatedData(connection->getSource());
			if(!buffer)
				throw EvaluationException("Buffer has not been evaluated yet", block);

			QString bufferBlockName = *connection->getProperty<StringProperty>(PropertyID::Buffer_Name);

			f->glLinkProgram(program->programId());
			GLuint index = functions->glGetProgramResourceIndex(program->programId(), GL_SHADER_STORAGE_BLOCK, bufferBlockName.toStdString().c_str());
			GLint binding = *connection->getProperty<UIntProperty>(PropertyID::Buffer_Binding);
			GLint maxBinding = 0;
			functions->glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxBinding);

			if(binding > maxBinding)
				getEvaluator()->addWarning({QString("Binding point is not in the valid range [0, %1]").arg(maxBinding), block});

			functions->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer->getValue());

			functions->glShaderStorageBlockBinding(program->programId(), index, binding);
		}

		for(IConnection* connection : block->getPort(PortType::Shader_AtomicCounterIn)->getInConnections())
		{
			QOpenGLFunctions_4_2_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_2_Core>();
			if(!functions)
				throw EvaluationException("Atomic Counter needs at least OpenGL 4.2 Version", block);

			GLWrapper* buffer = getEvaluator()->getEvaluatedData(connection->getSource());
			if(!buffer)
				throw EvaluationException("Buffer has not been evaluated yet", block);

			f->glLinkProgram(program->programId());
			GLint binding = *connection->getProperty<UIntProperty>(PropertyID::Buffer_Binding);
			GLint maxBinding = 0;
			functions->glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &maxBinding);

			if(binding > maxBinding)
				getEvaluator()->addWarning({QString("Binding point is not in the valid range [0, %1]").arg(maxBinding), block});

			functions->glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, binding, buffer->getValue());
		}
#endif
	}
}
