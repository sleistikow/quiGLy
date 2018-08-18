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

#ifndef GLSLHIGHLIGHTER_H
#define GLSLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

namespace ysm
{
	class GLSLDocument;

	//! \brief Class used to highlight GLSL code.
	class GLSLHighlighter : public QSyntaxHighlighter
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent document.
		 */
		GLSLHighlighter(GLSLDocument* parent);

	protected:

		/*!
		 * \brief Highlight the given text block.
		 * \param text The text to highlight.
		 */
		void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

		//! \brief Loads the highlighting rules from XML.
		void loadHighlightingRules();

	protected slots:

		//! \brief Updates the internal keyword lists.
		void updateKeywordList();

	private:

		//! \brief The highlight formats, mapped by type.
		static QMap<QString, QTextCharFormat> _highlightFormats;

		//! \brief Contains the highlighting order.
		static QList<QString> _highlightingOrder;

		//! \brief Multiline comments.
		static QPair<QRegularExpression, QRegularExpression> _multilineComment;

		//! \brief Editable blocks
		static QPair<QRegularExpression, QRegularExpression> _editableBlock;

		//! \brief The keywords, mapped by type.
		QMap<QString, QList<QRegularExpression>> _keywordLists;

		//! \brief The underlying document as GLSL document.
		GLSLDocument* _document;
	};
}

#endif // GLSLHIGHLIGHTER_H
