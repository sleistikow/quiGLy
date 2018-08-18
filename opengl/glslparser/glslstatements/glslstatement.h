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

#ifndef GLSLSTATEMENT_H
#define GLSLSTATEMENT_H

#include "../glslquickfix.h"

#include <QObject>
#include <QTextCursor>
#include <QAction>

namespace ysm
{
	class GLSLCodeBlock;
	class GLSLDocument;

	//! \brief All known statement types.
	enum GLSLStatementType
	{
		Unknown = 0x00,
		Declaration = 0x01,
		VersionDirective = 0x02,
		ExtensionDirective = 0x03,
	};

	//! \brief Contains information about a GLSL statement error.
	struct GLSLStatementError
	{
		//! \brief The user message.
		QString userMessage;

		//! \brief The statement.
		GLSLStatement* statement;

		//! \brief Available quick fixes.
		QList<IGLSLQuickFix*> quickFixes;

		//! \brief Fix by generating inputs.
		IGLSLQuickFix* generateInputs;
	};

	//! \brief Base interface for all GLSL statements.
	class GLSLStatement : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Connects to statements, that should follow each other.
		 * \param predecessor The leading statement.
		 * \param successor The trailing statement.
		 */
		static void connectStatements(GLSLStatement* predecessor, GLSLStatement* successor);

		/*!
		 * \brief Initialize new instance.
		 * \param statement The statement's text.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		GLSLStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		/*!
		 * \brief Gets the statement's text.
		 * \return The text.
		 */
		QString getText() const;

		/*!
		 * \brief Gets the document.
		 * \return The document.
		 */
		GLSLDocument* getDocument() const;

		/*!
		 * \brief Returns text cursor marking the whole statement, including semicolon, excluding brackets.
		 * \return Text cursor marking the whole block.
		 */
		QTextCursor getCompleteStatement() const;

		/*!
		 * \brief Sets the beginning of the statement (including whitespaces).
		 * \param cursorPosition Position of the cursor in the document.
		 */
		void setStatementAnchor(int cursorPosition);

		/*!
		 * \brief Sets the ending of the statement, (including whitespaces, excluding terminating character).
		 * \param cursorPosition Position of the cursor in the document.
		 */
		void setStatementPosition(int cursorPosition);

		/*!
		 * \brief Gets the following statement.
		 * \return The statement.
		 */
		GLSLStatement* getNextStatement() const;

		/*!
		 * \brief Gets the preceeding statement.
		 * \return The statement.
		 */
		GLSLStatement* getPreviousStatement() const;

		/*!
		 * \brief Gets the termination character ('{', '}', ';').
		 * \return The termination character.
		 */
		QChar getTerminator() const;

		/*!
		 * \brief Sets the termination character ('{', '}', ';').
		 * \param terminator The termination character.
		 */
		void setTerminator(const QChar& terminator);

		/*!
		 * \brief Gets the statement's type.
		 * \return The type.
		 */
		virtual GLSLStatementType getType() const;

		/*!
		 * \brief Gets the statement's language error messages (internal checks).
		 * \return The error messages.
		 */
		virtual QList<GLSLStatementError> getGLSLErrors();

		/*!
		 * \brief Gets the statement's pipeline error messages (external checks).
		 * \return The error messages.
		 */
		QList<GLSLStatementError> getPipelineErrors() const;

		/*!
		 * \brief Gets the statement's concenated error messages.
		 * \return The error messages.
		 */
		QList<GLSLStatementError> getErrors();

		/*!
		 * \brief Adds and error to the statement's error messages.
		 * \param error The error.
		 */
		void addPipelineError(GLSLStatementError error);

		/*!
		 * \brief Returns the statement's body.
		 * \return The body.
		 */
		GLSLCodeBlock* getBody() const;

		/*!
		 * \brief Sets the body.
		 * \param body The body.
		 */
		void setBody(GLSLCodeBlock* body);

	private:

		//! \brief The document.
		GLSLDocument* _document;

		//! \brief Cursor marking the while block, including the brackets.
		QTextCursor _statementCursor;

		//! \brief The statement's raw text.
		QString _statement;

		//! \brief Pointer to the next statement.
		GLSLStatement* _nextStatement;

		//! \brief Pointer to the previous statement.
		GLSLStatement* _previousStatement;

		//! \brief Termination character.
		QChar _terminator;

		//! \brief Error messages targeting this statement.
		QList<GLSLStatementError> _errors;

		//! \brief The statement's body.
		GLSLCodeBlock* _body;
	};

}

#endif // GLSLSTATEMENT_H
