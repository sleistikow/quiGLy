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

#include "vertexarrayobjectblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glwrapper.h"
#include "opengl/evaluation/setuprenderingevaluator.h"
#include "opengl/evaluation/evaluationexception.h"
#include "opengl/evaluation/evaluationutils.h"

#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/properties/vaolayoutproperty.h"
#include "data/types/vaolayout.h"

#include <QOpenGLShaderProgram>

namespace ysm
{

	VertexArrayObjectBlockEvaluator::VertexArrayObjectBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> VertexArrayObjectBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::VertexArrayObject;
	}

	QList<BlockType> VertexArrayObjectBlockEvaluator::getDependencies() const
	{
		return BlockEvaluator::getDependencies() << BlockType::Buffer;
	}

	void VertexArrayObjectBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		// Look, if the block has already been evaluated
		GLContextSensitiveWrapper* wrapper = getEvaluator()->getEvaluatedData<GLContextSensitiveWrapper>(block);
		if(!wrapper)
		{
			wrapper = new GLContextSensitiveWrapper(BlockType::VertexArrayObject);
			getEvaluator()->setEvaluatedData(block, wrapper);
		}

		if(!wrapper->getValue())
		{
			// Create a new vao
			GLuint vao;
			f->glGenVertexArrays(1, &vao);

			// Check for errors
			if(!vao)
				throw EvaluationException("VAO could not be created", block);

			// Wrap the result
			wrapper->setValue(vao);

			// Bind the vao
			f->glBindVertexArray(vao);

			// Bind the Buffers connected to the vao
			VaoLayout vaoLayout{block->getProperty<VaoLayoutProperty>(PropertyID::Vao_Layout)->getValue()};
			for(const VaoLayout::VaoLayoutEntry& entry : *vaoLayout.getEntries())
			{
				if(!entry.bufferConnection)
					throw EvaluationException("Connection in the VAO Layout not set correctly", block);

				// Bind connected buffer
				GLWrapper* buffer = getEvaluator()->getEvaluatedData(entry.bufferConnection->getSource());
				if(!buffer)
					throw EvaluationException("Buffer not evaluated yet", block);

				f->glBindBuffer(GL_ARRAY_BUFFER, buffer->getValue());

				// Resolve attrib location
				GLint location = entry.index;
				if(location < 0)
					location = getEvaluator()->getShaderProgram(pass)->attributeLocation(entry.name);

				// Set Attrib Pointer
				GLenum type = EvaluationUtils::mapDataTypeToOpenGL(entry.type);
				f->glEnableVertexAttribArray(location);
				f->glVertexAttribPointer(location, entry.size, type, entry.normalized, entry.stride, static_cast<char*>(0) + entry.offset);

				// Unbind buffer
				f->glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			// Unbind vao
			f->glBindVertexArray(0);
		}
	}

}
