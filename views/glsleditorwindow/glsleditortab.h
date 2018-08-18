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

#ifndef GLSLEDITORTAB_H
#define GLSLEDITORTAB_H

#include "../viewitem.h"

#include "data/iblock.h"
#include "data/iport.h"
#include "data/iconnection.h"

#include "opengl/glslparser/glslstatements/glslstatement.h"
#include "opengl/glslparser/glslcodeblock.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QTextDocument>
#include <QFile>
#include <QList>
#include <QListWidget>

namespace ysm
{
	class GLSLTextEdit;
	class GLSLLineBar;
	class GLSLHighlighter;
	class GLSLDeclaration;
	class GLSLCodeBlock;
	class GLSLDocument;

	//! \brief Represents a single tab in the GLSLEditor.
	//! NOTE: A tab might not have an active document specified, in case the displayed document is not connected to a
	//! block. In this case, no commands can be executed, because the pipeline is undefined.
	class GLSLEditorTab : public QWidget, public ViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param document The block's document.
		 * \param block The underlying block.
		 * \param codeIndex Index of the block's code item.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view item.
		 */
		explicit GLSLEditorTab(Document* document, IBlock* block, int codeIndex,
							   QWidget* parentWidget, IView* parentView);

		/*!
		 * \brief Initialize new instance.
		 * \param file The source file.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view item.
		 */
		explicit GLSLEditorTab(QFile* file, QWidget* parent, IView* parentView);

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view item.
		 */
		explicit GLSLEditorTab(QWidget *parent, IView* parentView);

		//! \brief Destruct instance.
		virtual ~GLSLEditorTab();

		/*!
		 * \brief Gets the tab's edit menu.
		 * \returns The edit menu.
		 */
		QMenu* getEditMenu();

		/*!
		 * \brief Checks if the tab was modified.
		 * \return True, if the tab was modified.
		 */
		bool isModified();

		/*!
		 * \brief Gets the underlying block.
		 * \return The block.
		 */
		IBlock* getBlock() const;

		/*!
		 * \brief Returns the text edit, should be only used if absolutely necessary.
		 * \return The text edit.
		 */
		GLSLTextEdit* getTextEdit() const;

		/*!
		 * \brief Gets the index of the code behind the underlying block.
		 * \return The index.
		 */
		int getCodeIndex() const;

		//! \brief Reverts the data in the underlying block.
		void revertData();

		//! \brief Saves the data in the underlying block.
		void saveData();

		//! \brief Saves the data in a txt file.
		void saveDataAs();

		/*!
		 * \brief Saves the data to the selected block.
		 * The document needs to be specified, so that commands are executed on the correct pipeline.
		 * \param block The block to append to.
		 * \param document The document to append to.
		 */
		void appendToBlock(IBlock* block, Document* document);

		//! \brief Releases the document from the selected block, and deletes the block's code page.
		void releaseFromBlock();

	signals:

		/*!
		 * \brief The modified flag changed.
		 * \param modified The new state.
		 */
		void modifiedChanged(bool modified);

	protected:

		/*!
		 * \brief Create the data for a specific code block.
		 * \param block The code block.
		 */
		void addCodeData(GLSLCodeBlock* codeBlock);

	protected slots:

		//! \brief Code was verified, update the block data.
		void updateCodeData(GLSLCodeBlock* codeBlock);

		/*!
		 * \brief Set the file's modified flag.
		 * \param modified Wether the file was modified.
		 */
		void setModified(bool modified);

		/*!
		 * \brief Error was selected, highlight it.
		 * \param modelIndex The model index.
		 */
		void highlightError(QModelIndex modelIndex);

		/*!
		 * \brief Error was selected, show context menu.
		 * \param contextPoint The point to show the context menu at.
		 */
		void showErrorContext(QPoint contextPoint);

	private:

		//! \brief The edit menu, displayed in the menu bar.
		QMenu* _editMenu;

		//! \brief Line bar presented at the left side of this widget.
		GLSLLineBar* _lineBar;

		//! \brief Text edit presented by this widget.
		GLSLTextEdit* _textEdit;

		//! \brief Contains all occured errors.
		QListWidget* _errorList;

		//! \brief True, if the file was modified since last save.
		bool _modified;

		/*!
		 * \brief Actions, displayed in menu bar.
		 * Part of the 'edit' menu, and handled by the tab (in a local scope).
		 */
		QAction* _undo;
		QAction* _redo;
		QAction* _cut;
		QAction* _copy;
		QAction* _paste;

		//! \brief The underlying file, if existing.
		QString _filename;

		//! \brief The document to operate on.
		GLSLDocument* _document;

		//! \brief Current statement errors.
		QList<GLSLStatementError> _statementErrors;

		//! \brief Error context menu.
		QMenu* _errorContextMenu;
	};
}

#endif // GLSLEDITORTAB_H
