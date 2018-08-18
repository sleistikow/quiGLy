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

#include "glwrapper.h"
#include "glrenderpass.h"

#include <QOpenGLContext>
#include <QOpenGLShader>

namespace ysm
{

GLWrapper::GLWrapper(BlockType type, GLuint value)
	: _type(type),
	  _value(value),
	  _context(nullptr)
{
	if(value)
		_context = QOpenGLContext::currentContext();
}

GLWrapper::~GLWrapper()
{
}

BlockType GLWrapper::getType() const
{
	return _type;
}

GLuint GLWrapper::getValue() const
{
	return _value;
}

QOpenGLContext* GLWrapper::getContext() const
{
	return _context;
}

GLContextSensitiveWrapper::GLContextSensitiveWrapper(BlockType type, GLuint value)
	: GLWrapper(type, 0)
{
	setValue(value);
}

GLContextSensitiveWrapper::~GLContextSensitiveWrapper()
{
}

void GLContextSensitiveWrapper::setValue(GLuint value)
{
	if(value)
		_values[QOpenGLContext::currentContext()] = value;
}

GLuint GLContextSensitiveWrapper::getValue() const
{
	return _values.value(QOpenGLContext::currentContext());
}

QOpenGLContext* GLContextSensitiveWrapper::getContext() const
{
	return nullptr;
}

GLShaderWrapper::GLShaderWrapper()
	: GLWrapper(BlockType::Undefined)
{
}

const QList<QOpenGLShader*>& GLShaderWrapper::getShaders() const
{
	return _shaders;
}

void GLShaderWrapper::addShader(QOpenGLShader* shader)
{
	_shaders.append(shader);
}

GLuint GLShaderWrapper::getValue() const
{
	return 0;
}

GLTextureWrapper::GLTextureWrapper(GLuint value, GLenum target)
	: GLBindingWrapper(BlockType::Texture, value),
	  _target(target),
	  _sampler(0)
{
}

void GLTextureWrapper::setSampler(GLuint sampler)
{
	_sampler = sampler;
}

GLenum GLTextureWrapper::getTarget() const
{
	return _target;
}

GLuint GLTextureWrapper::getSampler() const
{
	return _sampler;
}

}
