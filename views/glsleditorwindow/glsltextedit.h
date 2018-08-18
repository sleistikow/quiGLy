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

#ifndef GLSLTEXTEDIT_H
#define GLSLTEXTEDIT_H

#include "opengl/glslparser/glslcodeblock.h"

#include "glsllinebar.h"
#include "glsllinebarblock.h"
#include "iglsllinebardelegate.h"
#include "iglslcompleterdelegate.h"

#include <QWidget>
#include <QPlainTextEdit>
#include <QCompleter>

namespace ysm
{
	class GLSLHighlighter;
	class GLSLCompleter;
	class GLSLDocument;
	class GLSLStatement;
	class GLSLCodeBlock;

	/*!
	 * \brief A custom QPlainTextEdit widget, that improves support for GLSL.
	 * Provides line numbers, syntax highlighting, autocomplete, GLslang parser and more.
	 */
	class GLSLTextEdit : public QPlainTextEdit, public IGLSLLineBarDelegate, public IGLSLCompleterDelegate
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent widget.
		 * \param document The document.
		 */
		explicit GLSLTextEdit(GLSLDocument *document, QWidget* parent = 0);

		/*!
		 * \brief Returns the GLSL document.
		 * \return The GLSL document.
		 */
		GLSLDocument* getGLSLDocument() const;

		/*!
		 * \brief Returns the line bar's block count, which matches the block count of this QPlainTextEdit.
		 * \param lineBar The caller.
		 * \return The block count.
		 */
		int getLineBarBlockCount(const GLSLLineBar*) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns the line bar's blocks, which match the blocks of this QPlainTextEdit.
		 * \param lineBar The caller.
		 * \return The line bar's blocks.
		 */
		QVector<GLSLLineBarBlock> getLineBarBlocks(const GLSLLineBar*) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the prefix for the given completer.
		 * \param completer The completer.
		 * \return The completer's prefix.
		 */
		QString getCompleterPrefix(const GLSLCompleter* completer) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the bounds for the given completer.
		 * \param completer The completer.
		 * \return The completer's bounds. If the width is 0, it will be automatically calculated.
		 */
		QRect getCompleterRect(const GLSLCompleter* completer) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets code block, where to complete
		 * \param completer The completer.
		 * \return The code block, that provides local scope declarations.
		 */
		GLSLCodeBlock* getCompleterBlock(const GLSLCompleter* completer) const Q_DECL_OVERRIDE;

	public slots:

		/*!
		 * \brief Handle a completer's completion.
		 * \param completion The completion data.
		 */
		 void completerCompleted(const GLSLCompleter* completer, const QString& completion) Q_DECL_OVERRIDE;

		 //! \brief Update the code markers.
		 void updateCodeMarkers(GLSLCodeBlock* codeBlock);

	signals:

		 //! \brief Editor lost focus.
		 void editingFinished();

		//! \brief Emits, when the line bar blocks changed.
		void lineBarBlocksChanged() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Editor focussed out, fire signal.
		 * \param focusEvent The event.
		 */
		void focusOutEvent(QFocusEvent* focusEvent) Q_DECL_OVERRIDE;

		/*!
		 * \brief A key was pressed, check if the completer wants to handle it.
		 * \param args The event args.
		 */
		void keyPressEvent(QKeyEvent* args) Q_DECL_OVERRIDE;

		/*!
		 * \brief Filters and stores all code markers of statements in the given block.
		 * Recursive, call through updateCodeMakers().
		 * \param codeBlock The block.
		 */
		void addCodeMarkers(GLSLCodeBlock* codeBlock);

	private slots:

		//! \brief Invoked, when the widget is scrolled or updated. Updates the editor's line bar.
		void emitLineBarBlocksChanged(const QRect&, int);

		//! \brief Invoked, when the line block count changes. Updates the editor's line bar.
		void emitLineBarBlocksChanged(int);

		//! \brief Invoked, when the current line changes. Highlights the current line.
		void updateCurrentLine();

	private:

		//! \brief Completion widget used by the editor.
		GLSLCompleter* _completer;

		//! \brief The GLSL document.
		GLSLDocument* _glslDocument;

		//! \brief Extra selections, that highlight code.
		QList<QTextEdit::ExtraSelection> _codeMarkers;

		//! \brief The latest cursor, used to identify movements.
		QTextCursor _latestCursor;
	};
}

#endif // GLSLTEXTEDIT_H
