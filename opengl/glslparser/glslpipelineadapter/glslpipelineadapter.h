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

#ifndef GLSLPIPELINEADAPTER_H
#define GLSLPIPELINEADAPTER_H

#include "iglslpipelinecheck.h"
#include "data/iblock.h"

#include <QObject>
#include <QTextBlock>
#include <QPair>

namespace ysm
{
	class Document;
	class GLSLDocument;
	class GLSLCodeBlock;

	class GLSLVaoDeclarationCheck;

	//! \brief Brigde between a GLSL document and the quiGLy pipeline.
	//! This class uses the parsed GLSL code to match the statements to existing pipeline items, create the fixed
	//! section of the document and highlight it.
	class GLSLPipelineAdapter : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentDocument The parent document.
		 */
		GLSLPipelineAdapter(GLSLDocument* parentDocument);

		//! \brief Destruct instance.
		virtual ~GLSLPipelineAdapter();

		/*!
		 * \brief Returns the pipeline block.
		 * \return The block to operate on.
		 */
		IBlock* getBlock() const;

		/*!
		 * \brief Sets the pipeline block.
		 * \param document The block's document.
		 * \param block The pipeline block.
		 */
		void setBlock(IBlock* block);

		/*!
		 * \brief Adds a pipeline check.
		 * \param pipelineCheck The pipeline check.
		 */
		void addCheck(IGLSLPipelineCheck* pipelineCheck);

		/*!
		 * \brief Remvoes a pipeline check.
		 * \param pipelineCheck The pipeline check.
		 */
		void removeCheck(IGLSLPipelineCheck* pipelineCheck);

	public:

		/*!
		 * \brief Checks for editing permission assuming the given key was pressed using the document's current cursor.
		 * \param pressedKey The key that was pressed.
		 * \param keyModifiers The applied modifiers.
		 * \param textCursor The current cursor used to enter the text.
		 * \return True, if the key press can be accepted.
		 */
		bool checkEditingPermission(int pressedKey, Qt::KeyboardModifiers keyModifiers, QTextCursor textCursor);

		/*!
		 * \brief Checks the cursor position for neccessary adjustments to skip editing tags automatically.
		 * \param oldCursor The old cursor position.
		 * \param newCursor The new cursor position.
		 * \return True, if the new cursor position was changed.
		 */
		bool checkCursorPosition(const QTextCursor& oldCursor, QTextCursor& newCursor);

		/*!
		 * \brief Remove any GLSL code markers from the given text.
		 * \param plainText The plain text.
		 * \param keepLength If true, the text's length is validated.
		 * \return The text as valid GLSL.
		 */
		QString& validatePlainText(QString& plainText, bool keepLength);

	public slots:

		/*!
		 * \brief Synchronize the document so that it matches the pipeline.
		 * The document's saved state must be already up to date. The method works as follows:
		 * 1. The method compares the saved state code block to the existing pipeline, which will find any code elements
		 *    that are not existing in the pipeline, yet.
		 * 2. The method recreates the header from the pipeline, keeping all elements that were found in the first step.
		 * 3. The method parses the document to update the saved state code data.
		 * \return The adjusted root block after document update.
		 */
		GLSLCodeBlock* synchronizeDocument();

	signals:

		//! \brief Emitted when the underlying block changes.
		void blockChanged(IBlock *);

	protected slots:

		/*!
		 * \brief Insert the given statements at the beginning of the document.
		 * \param statements The statements
		 */
		void insertStatements(const QList<IGLSLPipelineCheck::FixedStatement>& statements);

		/*!
		 * \brief Helper method that checks for line editing permissions. Used by checkEditingPermission.
		 * \param pressedKey The key that was pressed.
		 * \param textCursor The text cursor to operate on.
		 * \return True, if the key press can be accepted.
		 */
		bool checkLineEditingPermission(int pressedKey, QTextCursor textCursor);

		/*!
		 * \brief Helper method that checks for line editing permissions. Used by checkEditingPermission.
		 * \param textCursor The text cursor to operate on.
		 * \return True, if the key press can be accepted.
		 */
		bool checkBlockEditingPermission(QTextCursor textCursor);

	private:

		//! \brief The connected shader block.
		IBlock* _pipelineBlock;

		//! \brief The parent document.
		GLSLDocument* _parentDocument;

		//! \brief Checks executed to match code to pipeline.
		QList<IGLSLPipelineCheck*> _pipelineChecks;

		//! \brief Stores all available single line editable sections inside the fixed section.
		QList<QPair<int, int>> _editableSingleSections;

		//! \brief Stores all available multi line editable sections inside the fixed section.
		QList<QPair<int, int>> _editableMultiSections;

		//! \brief Latest fixed section ending.
		int _latestFixedSection;

		//! \brief Checks that need to be adjusted.
		GLSLVaoDeclarationCheck* _vaoCheck;
	};

}

#endif // GLSLPIPELINEADAPTER_H
