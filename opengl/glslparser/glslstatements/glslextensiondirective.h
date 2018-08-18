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

#ifndef GLSLEXTENSIONDIRECTIVE_H
#define GLSLEXTENSIONDIRECTIVE_H

#include "glslstatement.h"

namespace ysm
{

	//! \brief Contains information about a the GLSL extension directive.
	class GLSLExtensionDirective : public GLSLStatement
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Tries to parse the given statement.
		 * \param statement The statement text.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 * \return NULL, if statement does not match.
		 */
		static GLSLExtensionDirective* createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent);

		/*!
		 * \brief Initialize new instance.
		 * \param statement The statement.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		GLSLExtensionDirective(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		/*!
		 * \brief Gets the statement's type.
		 * \return The type.
		 */
		GLSLStatementType getType() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the key.
		 * \return The key.
		 */
		QString getKey() const;

		/*!
		 * \brief Gets the value.
		 * \return The value.
		 */
		QString getValue() const;

	private:

		//! \brief Regular expression used to parse statements.
		static QRegularExpression* _directiveExpression;

		//! \brief The key.
		QString _key;

		//! \brief The value.
		QString _value;
	};

}

#endif // GLSLEXTENSIONDIRECTIVE_H
