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

#ifndef GLSLCODEBLOCK_H
#define GLSLCODEBLOCK_H

#include "glslstatements/glslstatement.h"

#include <QObject>
#include <QTextCursor>
#include <QSet>
#include <QTextDocument>

namespace ysm
{
	class GLSLDocument;

	//! \brief Contains information about a code block (block inside curly braces: { }). Used by the GLSLParser class.
	class GLSLCodeBlock : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		explicit GLSLCodeBlock(GLSLDocument* codeDocument, GLSLCodeBlock* parent = 0);

		/*!
		 * \brief Gets the GLSL document.
		 * \return The GLSL document.
		 */
		GLSLDocument* getDocument() const;

		/*!
		 * \brief Returns text cursor marking the opening bracket.
		 * \return Text cursor marking the opening bracket.
		 */
		QTextCursor getOpeningBracket() const;

		/*!
		 * \brief Returns text cursor marking the closing bracket.
		 * \return Text cursor marking the closing bracket.
		 */
		QTextCursor getClosingBracket() const;

		/*!
		 * \brief Returns text cursor marking the whole block, excluding the brackets.
		 * \return Text cursor marking the whole block.
		 */
		QTextCursor getCompleteBlock() const;

		/*!
		 * \brief Sets the beginning of the block, which must be right behind the block's opening bracket.
		 * \param cursorPosition Position of the cursor in the document.
		 */
		void setBlockAnchor(int cursorPosition);

		/*!
		 * \brief Sets the ending of the block, which must be right before the block's closing bracket.
		 * \param cursorPosition Position of the cursor in the document.
		 */
		void setBlockPosition(int cursorPosition);

		/*!
		 * \brief Returns the parent block.
		 * \return The parent block.
		 */
		GLSLCodeBlock* getParentBlock() const;

		/*!
		 * \brief Returns the parent statement.
		 * \return The parent statement.
		 */
		GLSLStatement* getParentStatement() const;

		/*!
		 * \brief Sets the parent statement.
		 * \param parentStatement The parent statement.
		 */
		void setParentStatement(GLSLStatement* parentStatement);

		/*!
		 * \brief Returns all blocks, that are direct children of this block.
		 * \return List containing all direct child blocks.
		 */
		QSet<GLSLCodeBlock*> getChildBlocks() const;

		/*!
		 * \brief Gets the block's statements.
		 * \param recusrive If true, child block statements are included.
		 * \return The block's statements.
		 */
		QList<GLSLStatement*> getStatements(bool recursive = false) const;

		/*!
		 * \brief Gets the block's statements of the given type.
		 * \param type The statement type.
		 * \param recusrive If true, child block statements are included.
		 * \return The statements.
		 */
		template<typename T> QList<T*> getStatements(const GLSLStatementType type, bool recursive = false) const
		{
			//Iterate and select all statements of the given type.
			QList<T*> statementsOfType;
			foreach(GLSLStatement* statement, _statements)
				if(statement->getType() == type)
					statementsOfType.append(qobject_cast<T*>(statement));

			//Do recursive search.
			if(recursive)
				foreach(GLSLCodeBlock* codeBlock, _childBlocks)
					statementsOfType.append(codeBlock->getStatements<T>(type, true));

			//Return all found statements.
			return statementsOfType;
		}

		/*!
		 * \brief Gets the block's statements of the given type, that match the given function.
		 * \param match Criteria that the statement must fulfill.
		 * \param recusrive If true, child block statements are included.
		 * \return The statements.
		 */
		template<typename T> QList<T*> getStatements(bool(*match)(T*), bool recursive = false) const
		{
			//Iterate and select all statements with match.
			QList<T*> statementsWithMatch;
			foreach(GLSLStatement* statement, _statements)
			{
				//Convert first, then execute the matcher.
				T* converted = qobject_cast<T*>(statement);
				if(converted && match(converted))
					statementsWithMatch.append(converted);
			}

			//Do recursive search.
			if(recursive)
				foreach(GLSLCodeBlock* codeBlock, _childBlocks)
					statementsWithMatch.append(codeBlock->getStatements<T>(match, true));

			//Return all found statements.
			return statementsWithMatch;
		}

		/*!
		 * \brief Add a statement to the block.
		 * \param statement The statement.
		 */
		void addStatement(GLSLStatement* statement);

		/*!
		 * \brief Adds a new child block to the block.
		 * \param childBlock The child block.
		 */
		void addChildBlock(GLSLCodeBlock* childBlock);

		/*!
		 * \brief Removes a child block from the block.
		 * \param childBlock The child block.
		 */
		void removeChildBlock(GLSLCodeBlock* childBlock);

		/*!
		 * \brief Gets the block's overall indentation level (+1 of the parent block).
		 * \return The indentation level.
		 */
		int getIndentationLevel() const;

		/*!
		 * \brief Gets the block's indentation level at the given cursor position.
		 * The position must be given, in order to indentify the statement at the given position.
		 * \param textCursor The text cursor.
		 * \return The indentation level.
		 */
		int getIndentationLevel(const QTextCursor& textCursor) const;

		/*!
		 * \brief Wether the block is valid (has a matching closing bracket).
		 * \return True, if the block is valid.
		 */
		bool isValid() const;

		/*!
		 * \brief Specify wether the block is valid.
		 * \param isValid True, if the block is valid.
		 */
		void setValid(const bool isValid);

		/*!
		 * \brief Gets the statement at the given cursor position. Does not search child blocks.
		 * \param textCursor The text cursor.
		 * \return The statement, or NULL if not found.
		 */
		GLSLStatement* findStatement(const QTextCursor& textCursor) const;

		/*!
		 * \brief Tries to find the deepest child block, that completly contains the text cursor.
		 * \param textCursor The text cursor.
		 * \return The child block, this or null if the cursor exceeds this block itself.
		 */
		GLSLCodeBlock* findChildBlock(const QTextCursor& textCursor);

	private:

		//! \brief Cursor marking the while block, including the brackets.
		QTextCursor _blockCursor;

		//! \brief List containing all direct child blocks.
		QSet<GLSLCodeBlock*> _childBlocks;

		//! \brief The parent block.
		GLSLCodeBlock* _parentBlock;

		//! \brief The block's statements.
		QList<GLSLStatement*> _statements;

		//! \brief The underlying document.
		GLSLDocument* _codeDocument;

		//! \brief The parent statement, (that introduced this code block).
		//! This code block is body of that statement.
		GLSLStatement* _parentStatement;

		//! \brief Wether the block is valid or not.
		bool _isValid;
	};
}

#endif // GLSLCODEBLOCK_H
