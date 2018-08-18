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

#ifndef GLSLDOCUMENT_H
#define GLSLDOCUMENT_H

#include "commands/ichangeable.h"
#include "glslcodeblock.h"

#include <QObject>
#include <QTextDocument>
#include <QTextCursor>

namespace ysm
{
	class GLSLDocumentInfo;
	class GLSLPipelineAdapter;
	class GLSLParser;
	class GLSLHighlighter;
	class GLSLDeclaration;

	//! \brief Custom text document, that provides futher editing features for GLSL code.
	//! A GLSL document has a retain counter, because it can be owned by multiple objects at a time.
	class GLSLDocument : public QTextDocument, public IChangeable
	{
		Q_OBJECT

	public:

		//! \brief Marks the beginning of an editable section.
		static const QString editableStart;

		//! \brief Marks the end of an editable section.
		static const QString editableEnd;

		//! \brief Marks the beginning of an editable block.
		static const QString editableBlockStart;

		//! \brief Marks the end of an editable block.
		static const QString editableBlockEnd;

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		GLSLDocument(QObject* parent = 0);

		//! \brief Destruct instance.
		virtual ~GLSLDocument();

		//! \brief Increase the retain counter.
		void retain();

		//! \brief Decrease the retain counter
		//! Deletes the document, if the counter becomes zero.
		void release();

	public:

		/*!
		 * \brief Returns the document's parser.
		 * \return The document's parser.
		 */
		GLSLParser* getParser() const;

		/*!
		 * \brief Returns the document's information.
		 * \return The document's information.
		 */
		GLSLDocumentInfo* getDocumentInfo() const;

		/*!
		 * \brief Returns the document's adapter to the quiGLy pipeline.
		 * \return The document's adapter.
		 */
		GLSLPipelineAdapter* getPipelineAdapter() const;

	public:

		/*!
		 * \brief Returns the saved content.
		 * \return The saved content.
		 */
		QString getSavedContent() const;

		/*!
		 * \brief Changes the saved content directly.
		 * \param savedContent The new content.
		 */
		void setSavedContent(QString savedContent);

		/*!
		 * \brief Returns the saved content, without any additional markers as valid GLSL code.
		 * \param keepLength If true, the text's length is validated.
		 * \return The saved content as GLSL code.
		 */
		QString getSavedCode(bool keepLength = true) const;

		/*!
		 * \brief Returns the document's content, without any additional markers as valid GLSL code.
		 * \param keepLength If true, the text's length is validated.
		 * \return The document's content as GLSL code.
		 */
		QString toPlainCode(bool keepLength = true) const;

		/*!
		 * \brief Returns the saved state root block.
		 * \return The document's root block.
		 */
		GLSLCodeBlock* getFullCodeBlock() const;

		/*!
		 * \brief Returns the modified state root block.
		 * \return The document's root block.
		 */
		GLSLCodeBlock* getWhitespaceCodeBlock() const;

	public slots:

		//! \brief Synchronize the document using the pipeline adapter.
		void synchronizeDocument();

		//! \brief Save the document.
		void saveDocument();

		//! \brief Revert the document to the last saved state.
		void revertDocument();

	signals:

		//! \brief Emitted, after code was verified.
		//! Note that the document MUST NOT BE CHANGED in corresponding slots.
		void codeVerified(GLSLCodeBlock*);

	protected slots:

		//! \brief Invalidate the current code.
		void invalidateCode();

	private:

		//! \brief The retain counter. If zero, document is deleted.
		int _retainCount;

	private:

		//! \brief Parser used to parse the document.
		GLSLParser* _parser;

		//!	\brief The highlighter.
		GLSLHighlighter* _highlighter;

		//! \brief The pipeline adapter.
		GLSLPipelineAdapter* _adapter;

		//! \brief The document's information.
		GLSLDocumentInfo* _info;

	private:

		//! \brief The last saved GLSL code as stored to data. Used to revert changes.
		QString _savedContent;

		//! \brief The current GLSL code as stored to data. Used to trace changes.
		QString _modifiedContent;

		//! \brief The root code block of the document (fully parsed).
		mutable GLSLCodeBlock* _fullCodeBlock;

		//! \brief The root code block of the document (whitespace parsed).
		mutable GLSLCodeBlock* _whitespaceCodeBlock;

		//! \brief Flags that shows wether the the document has changed after the last parse.
		mutable bool _fullCodeDirty, _whitespaceCodeDirty;

		//! \brief Flag that indentifies changes during verification.
		bool _verificationFlag;
	};

}

#endif // GLSLDOCUMENT_H
