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

#ifndef GLSLPARSER_H
#define GLSLPARSER_H

#include <QObject>
#include <QTextDocument>
#include <QVector>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextStream>

namespace ysm
{
	class GLSLCodeBlock;
	class GLSLStatement;
	class GLSLStatementFactory;
	class GLSLDocument;

	//! \brief Provides methods to parse and format a QTextDocument, that contains GLSL code.
	class GLSLParser : public QObject
	{
		Q_OBJECT
		Q_ENUMS(WordType)
		Q_FLAGS(ParserState)

	public:

		//! \brief Possible word types in GLSL.
		enum class WordType
		{
			Unknown,
			Datatype,
			Qualifier,
			Structural,
			BuiltInVariable,
			BuiltInConstant,
			BuiltInFunction,
			Variable,
			Function,
		};

		//! \brief Possible parser states.
		enum States
		{
			Default = 0x00000,

			ParsingSingleLineComment = 0x00001,
			ParsingMultiLineComment = 0x00002,
			ParsingPreprocessor = 0x00004,

			ClearNonWhitespaceStack = 0x00010,
			ClearStatementStack = 0x00020,

			NewBlock = 0x00100,
			NewLine = 0x00200,

			Success = 0x40000,
			Error = 0x80000,
		};
		Q_DECLARE_FLAGS(State, States)

		/*!
		 * \brief Initialize new instance.
		 * \param codeDocument The code document.
		 * \param parent The parent.
		 */
		explicit GLSLParser(GLSLDocument *codeDocument);

	public slots:

		/*!
		 * \brief Start to parse the whole document.
		 * \param indentationOnly If true, statements are not parsed into detail.
		 * \returns The root block.
		 */
		GLSLCodeBlock* parseDocument(bool indentationOnly = false);

	private:

		/*!
		 * \brief Parse a new code block, beginning at the current stream position.
		 * \param indentationOnly If true, no statements are parsed.
		 * \return Newly created code block.
		 */
		GLSLCodeBlock* parseCodeBlock(bool indentationOnly = false);

		//! \brief Parses the current character as a statement character.
		void parseStatementCharacter();

		//! \brief Parses the current character as a single line comment character.
		void parseSingleLineCommentCharacter();

		//! \brief Parses the current character as a multi line comment character.
		void parseMultiLineCommentCharacter();

		/*!
		 * \brief Clears the statement stack and creates a statement from it's current content.
		 * \param indentationOnly If true, statements are not parsed into detail.
		 * \param endNonWhitespace If true, the statement ends at the last non whitespace anchor.
		 */
		void clearStatementStack(bool indentationOnly, bool endNonWhitespace = false);

	private:

		//! \brief The code document to operate on.
		GLSLDocument* _codeDocument;

		//! \brief The parser's current state.
		State _currentState;

		//! \brief The current character beeing parsed.
		QChar _currentCharacter;

		//! \brief The current non whitespace word (split by whitespace). Used as a minimal stack.
		QString _currentNonWhitespaceWord;

		//! \brief The current statement (split by semicolon).
		QString _currentStatement;

		//! \brief Anchor of the current statement.
		int _currentStatementAnchor;

		//! \brief Anchor of the last non whitespace character.
		int _currentNonWhitespaceAnchor;

		//! \brief Currently parsed block.
		GLSLCodeBlock* _currentBlock;

		//! \brief The stream used to read the current data.
		QTextStream* _currentStream;

		//! \brief The statement factory.
		GLSLStatementFactory* _statementFactory;

		//! \brief Last parsed statement.
		GLSLStatement* _lastStatement;
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(GLSLParser::State)
}

#endif // GLSLPARSER_H
