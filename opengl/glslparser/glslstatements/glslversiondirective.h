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

#ifndef GLSLVERSIONDIRECTIVE_H
#define GLSLVERSIONDIRECTIVE_H

#include "glslstatement.h"

namespace ysm
{

	//! \brief Contains information about a the GLSL version directive.
	class GLSLVersionDirective : public GLSLStatement
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
		static GLSLVersionDirective* createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent);

		/*!
		 * \brief Initialize new instance.
		 * \param statement The statement.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		GLSLVersionDirective(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		/*!
		 * \brief Gets the statement's type.
		 * \return The type.
		 */
		GLSLStatementType getType() const Q_DECL_OVERRIDE;

        /*!
         * \brief Returns the GLSL version defined by this directive.
         * \return The GLSL version.
         */
        int getVersion() const;

		/*!
		 * \brief Returns the GLSL version addon defined by this directive.
		 * \return The GLSL version addon.
		 */
		QString getAddon() const;

	private:

		//! \brief Regular expression used to parse statements.
		static QRegularExpression* _directiveExpression;

		//! \brief The GLSL version specified by this directive.
		unsigned int _version;

		//! \brief The GLSL version addon specified by this directive.
		QString _addon;
	};

}

#endif // GLSLVERSIONDIRECTIVE_H
