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

#ifndef GLSLCOMPLETER_H
#define GLSLCOMPLETER_H

#include <QCompleter>
#include <QTextDocument>
#include <QKeyEvent>
#include <QListView>

#include "data/iblock.h"
#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/keywordreader.h"

namespace ysm
{
	class IGLSLCompleterDelegate;

	//! \brief The completer's underlying model.
	class GLSLCompleterModel : public QAbstractListModel
	{
		Q_OBJECT

	public:

		//! \brief Initialize new instance.
		GLSLCompleterModel(GLSLDocument* glslDocument, QObject* parent);

		/*!
		 * \brief Get the row count.
		 * \param parent The parent index.
		 * \return  The row count.
		 */
		int rowCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Get the data.
		 * \param index The model index.
		 * \param role The role.
		 * \return The data.
		 */
		QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Sets the current code block, where completion occurs.
		 * \param codeBlock The code block.
		 */
		void setCurrentBlock(GLSLCodeBlock* codeBlock);

	protected:

		/*!
		 * \brief Recursively finds global scope declarations.
		 * \param codeBlock The code block to search in.
		 */
		void findGlobalScopeDeclarations(GLSLCodeBlock* codeBlock);

		/*!
		 * \brief Recursively finds local scope declarations.
		 * \param codeBlock The code block to search in.
		 */
		void findLocalScopeDeclarations(GLSLCodeBlock* codeBlock);

	protected slots:

		//! \brief Update the internal data.
		void updateData();

	private:

		//! \brief The underlying document.
		GLSLDocument* _document;

		//! \brief The GLSL keywords.
		KeywordReader::KeywordList _glslKeywords;

		//! \brief List of all added user keyword names, to remove hidden elements.
		QList<QString> _userKeywordNames;

		//! \brief The user keywords.
		QList<GLSLDeclaration*> _userKeywords;

	};

	//! \brief The completer's visual appearance.
	class GLSLCompleterView : public QListView
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		GLSLCompleterView(QWidget* parent = 0);

	protected:

		/*!
		 * \brief Current item changed, show popup.
		 * \param current Current item.
		 * \param previous Previous item.
		 */
		void currentChanged(const QModelIndex& current, const QModelIndex& previous) Q_DECL_OVERRIDE;
	};

	//! \brief Autocomplete widget for the GLSLEditor.
	class GLSLCompleter : public QCompleter
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentDocument The parent document.
		 */
		explicit GLSLCompleter(GLSLDocument* glslDocument, QObject* parent = 0);

		/*!
		 * \brief Gets the completer's IGLSLCompleterDelegate, which is responsible for providing the data.
		 * \returns The completer's delegate.
		 */
		IGLSLCompleterDelegate* getDelegate() const;

		/*!
		 * \brief Sets the completer's IGLSLCompleterDelegate, which is responsible for providing the data.
		 * \param blocks The completer's delegate.
		 */
		void setDelegate(IGLSLCompleterDelegate* delegate);

		/*!
		 * \brief Checks, if the completer wants the given key event.
		 * \param args The event args.
		 * \returns True, if the completer handles the key event.
		 */
		bool handleKeyPress(QKeyEvent* args);

	private slots:

		//! \brief The document changed.
		void documentChanged();

		/*!
		 * \brief Calls the delegates completion method.
		 * \param completion The completion string.
		 */
		void forwardCompletion(const QString& completion);

		//! \brief Complete with delegate rect.
		void complete();

	private:

		//! \brief The completer's IGLSLCompleterDelegate, which is responsible for providing the data.
		IGLSLCompleterDelegate* _delegate;

		//! \brief Parent document.
		GLSLDocument* _glslDocument;

		//! \brief The model.
		GLSLCompleterModel* _model;
	};
}

#endif // GLSLCOMPLETER_H
