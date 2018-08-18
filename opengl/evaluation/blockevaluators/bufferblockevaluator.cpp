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

#include "bufferblockevaluator.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/evaluationutils.h"
#include "opengl/glwrapper.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/blocks/bufferblock.h"
#include "data/properties/varyingsproperty.h"

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>

namespace ysm
{

	BufferBlockEvaluator::BufferBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> BufferBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::Buffer;
	}

	void BufferBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLWrapper* wrapper = getEvaluator()->getEvaluatedData(block);
		if(!wrapper)
		{
			// Look for a valid data source
			QVector<IConnection*> dataInCon = block->getPort(PortType::Data_In)->getInConnections();
			if(dataInCon.size() != 1)
				throw EvaluationException("This Buffer needs exactly one data source connected", block);

			// Create the buffer
			GLuint buffer;
			f->glGenBuffers(1, &buffer);

			// Check for errors
			if(!buffer)
				throw EvaluationException("Buffer could not be created", block);

			// Wrap the result
			wrapper = new GLWrapper(BlockType::Buffer, buffer);
			getEvaluator()->setEvaluatedData(block, wrapper);

			// Get the usage pattern
			GLenum usage = EvaluationUtils::mapUsagePatternToOpenGL(block->getProperty<EnumProperty>(PropertyID::Buffer_UsageFrequency)->getValue(),
																	block->getProperty<EnumProperty>(PropertyID::Buffer_UsageAccess)->getValue());

			IBlock* dataSourceBlock = dataInCon[0]->getSource();
			if(dataSourceBlock->getType() == BlockType::TransformFeedback)
			{
				// Calculate size
				unsigned int size = dataSourceBlock->getProperty<VaryingsProperty>(PropertyID::Varyings)->getValue().getSize();

				// Setup Transform Feedback Buffer
				f->glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
				f->glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, size, nullptr, usage);
				f->glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
			}
			else if(dataInCon[0]->getSourcePort()->getType() == PortType::Shader_SSBOOut)
			{
				QOpenGLFunctions_4_3_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
				if(!functions)
					throw EvaluationException("Shader Storage Buffer needs at least OpenGL 4.2 Version", block);

				// Calculate size
				unsigned int size = dataInCon[0]->getProperty<VaryingsProperty>(PropertyID::Varyings)->getValue().getSize();

				// Setup Transform Feedback Buffer
				functions->glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
				functions->glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, usage);
				functions->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
			//Atomic Counter Buffer
			else if(dataInCon[0]->getSourcePort()->getType() == PortType::Shader_AtomicCounterIn)
			{
				QOpenGLFunctions_4_2_Core* functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_2_Core>();
				if(!functions)
					throw EvaluationException("Atomic Counter needs at least OpenGL 4.2 Version", block);

				//TODO: replace hardcoded size
				//TODO: fill or reset bufferdata
				const GLuint data = 0;
				// Set the data once, using any unspecific target
				functions->glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffer);
				functions->glBufferData(GL_ATOMIC_COUNTER_BUFFER, 16 * sizeof(GLuint), &data, usage);
				functions->glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
			}
			else
			{
				// Get the actual data
				const QByteArray& data = block->getProperty<ByteArrayProperty>(PropertyID::Buffer_Data)->getValue();

				// Set the data once, using any unspecific target
				f->glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
				f->glBufferData(GL_COPY_WRITE_BUFFER, data.size(), reinterpret_cast<const void*>(data.constData()), usage);
				f->glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
			}
		}
	}

}
