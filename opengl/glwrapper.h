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

#ifndef GLWRAPPER_H
#define GLWRAPPER_H

#include "glconfiguration.h"
#include "data/blocks/blocktype.h"

#include <QOffscreenSurface>

QT_BEGIN_NAMESPACE
class QOpenGLShader;
class QOpenGLContext;
QT_END_NAMESPACE

namespace ysm
{

	class GLRenderPass;

	/**
	 * @brief The GLWrapper class is a convinient wrapper for OpenGL names.
	 */
	class GLWrapper
	{
	public:

		/// @brief Constructs this wrapper and initializes the values
		GLWrapper(BlockType type, GLuint value = 0);

		/// @brief Destructor
		virtual ~GLWrapper();

		/// @brief Returns the BlockType, the wrapper was created for.
		virtual BlockType getType() const;

		/// @brief Returns the actual value, stored by this wrapper.
		virtual GLuint getValue() const;

		/// @brief Returns the context, the wrapper was created in.
		virtual QOpenGLContext* getContext() const;

	protected:

		BlockType _type;	/*!< The BlockType, the wrapper was created for. */
		GLuint _value;		/*!< Actual value stored by this wrapper. */

		QOpenGLContext* _context;

	};

	/**
	 * @brief This Wrapper is used for GL Objects which are not shareable.
	 */
	class GLContextSensitiveWrapper : public GLWrapper
	{
	public:

		/// @brief Contructs a new wrapper and sets the value for the currently active context
		GLContextSensitiveWrapper(BlockType type, GLuint value = 0);

		/// @brief Destructs this instance
		virtual ~GLContextSensitiveWrapper();

		/// @brief Sets the value for the currently active context
		virtual void setValue(GLuint value);

		/// @brief Gets the value for the currently active context
		virtual GLuint getValue() const Q_DECL_OVERRIDE;

		/// @brief Returns always null, since the context depends on the actual value
		virtual QOpenGLContext* getContext() const Q_DECL_OVERRIDE;

	private:
		QMap<QOpenGLContext*, GLuint> _values;
	};

	/**
	 * @brief The GLShaderWrapper class is a convinient wrapper class for an OpenGL shader.
	 */
	class GLShaderWrapper : public GLWrapper
	{
	public:
		/// @brief Initializes the underlying QOpenGLShader of the given type
		GLShaderWrapper();

		/// @brief Returns the underlying shaderobject
		const QList<QOpenGLShader*>& getShaders() const;

		/// @brief Adds a shader to this wrapper
		void addShader(QOpenGLShader* shader);

		/// @brief Unused, returns 0
		GLuint getValue() const Q_DECL_OVERRIDE;

	private:

		QList<QOpenGLShader*> _shaders;

	};


	// All Binding parameters at least have to contain the pass
	struct BindingParameter
	{
		GLRenderPass* pass;
	};

	/**
	 * This Wrapper is used, when the underlying object might be bound to multiple targets.
	 */
	template<typename T>
	class GLBindingWrapper : public GLWrapper
	{
	public:

		/// @brief Constructs a new wrapper with no bindings.
		GLBindingWrapper(BlockType type, GLuint value) : GLWrapper(type, value) {}

		/// @brief Adds a new binding to this wrapper.
		bool addBinding(T newBinding)
		{
			for(T binding : _bindings)
				if(binding == newBinding)
					return false;

			_bindings.append(newBinding);
			return true;
		}

		/// @brief Returns all Bindings of this wrapper, may filtered by pass.
		QList<T> getBindings(GLRenderPass* pass = nullptr) const
		{
			if(!pass)
				return _bindings;

			QList<T> filteredBindings;

			for(T binding : _bindings)
				if(binding.pass == pass)
					filteredBindings.append(binding);

			return filteredBindings;
		}

	private:

		QList<T> _bindings;
	};

	// Texture Binding Parameter
	struct TextureBindingParameter : BindingParameter
	{
		GLint location;
		unsigned int unit;

		bool operator==(const TextureBindingParameter& other) const
		{
			return	pass == other.pass &&
					location == other.location &&
					unit == other.unit;
		}
	};

	class GLTextureWrapper : public GLBindingWrapper<TextureBindingParameter>
	{
	public:

		GLTextureWrapper(GLuint value, GLenum target);
		void setSampler(GLuint sampler);

		GLenum getTarget() const;
		GLuint getSampler() const;

	private:
		GLenum _target;
		GLuint _sampler;
	};

} // end namespace ysm

#endif // GLWRAPPER_H
