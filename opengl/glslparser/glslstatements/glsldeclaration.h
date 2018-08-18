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

#ifndef GLSLGLOBAL_H
#define GLSLGLOBAL_H

#include "glslstatement.h"

#include "data/iblock.h"
#include "data/blocks/textureblock.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>

namespace ysm
{

	//! \brief Contains information about a global variable.
	class GLSLDeclaration : public GLSLStatement
	{
		Q_OBJECT

	public:

		//! \brief Contains the texture data types mapped to their GLSL strings.
		static QMap<QString, TextureBlock::SamplerType> TEXTURE_DATA_TYPES;

		//! \brief Contains the uniform data types mapped to their GLSL strings.
		static QMap<QString, QList<BlockType>> UNIFORM_DATA_TYPES;

	public:

		/*!
		 * \brief Tries to parse the given statement.
		 * \param statement The statement text.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 * \return NULL, if statement does not match.
		 */
		static GLSLDeclaration* createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent);

		/*!
		 * \brief Initialize new instance.
		 * \param statement The statement.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		GLSLDeclaration(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		/*!
		 * \brief Gets the statement's type.
		 * \return The type.
		 */
		GLSLStatementType getType() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the statement's language error messages (internal checks).
		 * \return The error messages.
		 */
		QList<GLSLStatementError> getGLSLErrors() Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the declarations name.
		 * \return The name.
		 */
		QString getName() const;

		/*!
		 * \brief Returns the datatype of the declaration.
		 * \return The datatype.
		 */
		QString getDataType() const;

		/*!
		 * \brief Returns the array info of the declaration.
		 * \return The datatype.
		 */
		QString getArray() const;

		/*!
		 * \brief Returns true, if this declaration is a method.
		 * \return True, if method.
		 */
		bool isMethod() const;

		/*!
		 * \brief Returns true, if this declaration is structural.
		 * \return True, if strutural.
		 */
		bool isStructural() const;

		/*!
		 * \brief Returns true, if declaration has texture type.
		 * \return True, if declaration has texture type.
		 */
		bool hasTextureType() const;

		/*!
		 * \brief Returns true, if declaration has uniform type.
		 * \return True, if declaration has uniform type.
		 */
		bool hasUniformType() const;

		/*!
		 * \brief Checks, if the statement has qualifiers at all.
		 * \return True, if statement has qualifiers.
		 */
		bool hasQualifiers() const;

		/*!
		 * \brief Checks, wether the qualifier is specified.
		 * \param qualifier The qualifier.
		 * \return True, if specified.
		 */
		bool hasQualifier(QString qualifier) const;

		/*!
		 * \brief Checks, wether the declaration has a layout.
		 * \return True, if layout is available.
		 */
		bool hasLayout() const;

		/*!
		 * \brief Checks, wether the layout contains the given parameter.
		 * \param layoutParameter The parameter.
		 * \return True, if parameter exists.
		 */
		bool hasLayoutParameter(QString layoutParameter) const;

		/*!
		 * \brief Returns the value of the given parameter.
		 * \param layoutParameter The parameter.
		 * \return Value as string.
		 */
		QString getLayoutParameter(QString layoutParameter) const;

		/*!
		 * \brief Checks wether the declaration applies at global scope.
		 * \return True, if global scope.
		 */
		bool isGlobalScope() const;

		/*!
		 * \brief Tries to find the declaration, that declares the instance name for this declaration.
		 * This applies, if this declaration has a body, which is followed directly by another declaration.
		 * \return The instance name declaration.
		 */
		GLSLDeclaration* getInstanceDeclaration() const;

		/*!
		 * \brief Tries to find the declaration, that declares the structure for this declaration.
		 * Same as getInstanceDeclaration(), just in reverse.
		 * \return The structure name declaration.
		 */
		GLSLDeclaration* getStructureDeclaration() const;

	private:

		//! \brief Regular expression used to parse statements.
		static QRegularExpression* _declarationExpression;

		//! \brief Regular expression used to parse structural statements.
		static QRegularExpression* _structureExpression;

		//! \brief Regular expression used to parse layouts.
		static QRegularExpression* _layoutExpression;

		//! \brief Map containing the layout qualifiers and values.
		QMap<QString, QString> _layout;

		//! \brief String list containing the qualifiers.
		QStringList _qualifiers;

		//! \brief The data type.
		QString _datatype;

		//! \brief The global's name.
		QString _name;

		//! \brief Possible array declaration.
		QString _array;

		//! \brief True for method declarations.
		bool _method;
	};

}

#endif // GLSLGLOBAL_H
