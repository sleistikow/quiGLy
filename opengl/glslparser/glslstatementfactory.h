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

#ifndef GLSLSTATEMENTFACTORY_H
#define GLSLSTATEMENTFACTORY_H

#include <QObject>
#include <QList>
#include <QTextDocument>

namespace ysm
{
	class GLSLStatement;
	class GLSLCodeBlock;
	class GLSLDocument;

	class GLSLStatementFactory : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		explicit GLSLStatementFactory(QObject *parent = 0);

		/*!
		 * \brief Creates the correct statement subclass using the given statement text.
		 * \param statement The statement text.
		 * \param codeDocument The code document.
		 * \param parent Parent that will be passed to the statement.
		 * \return The statement instance.
		 */
		GLSLStatement* createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		//! \brief Registers a statement type.
		template<typename T> void registerStatement() { _statements.append(parseStatement<T>); }

	protected:

		/*!
		 * \brief Tries to create a statement of of the given type using the given statement text.
		 * \param statement The statement text.
		 * \param codeDocument The code document.
		 * \param parent Parent that will be passed to the statement.
		 * \returns Null if the statement does not match, otherwise new statement.
		 */
		template<typename T> static GLSLStatement* parseStatement(QString statement, GLSLDocument* codeDocument,
																  GLSLCodeBlock* parent = 0)
		{
			//Use the static create statement method, which must be implemented by all GLSLStatements.
			return T::createStatement(statement, codeDocument, parent);
		}

	private:

		//! \brief Contains all registered statements.
		QList<GLSLStatement* (*)(QString, GLSLDocument*, GLSLCodeBlock*)> _statements;

	};

}

#endif // GLSLSTATEMENTFACTORY_H
