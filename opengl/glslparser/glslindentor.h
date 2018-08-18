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

#ifndef GLSLINDENTOR_H
#define GLSLINDENTOR_H

#include <QTextCursor>
#include <QTextBlock>

namespace ysm
{
	class GLSLDocument;

	//! \brief Helper class that allows easy indentation on a given document.
	class GLSLIndentor
	{

	public:

		/*!
		 * \brief Create new instance.
		 * \param parentDocument The GLSL document.
		 * \param indentationCursor The cursor used for the operations.
		 */
		GLSLIndentor(GLSLDocument* parentDocument, QTextCursor& indentationCursor);

		/*!
		 * \brief Retrieves the indentation level of one single line, given by the cursor's position.
		 * \return The indentation level.
		 */
		int getIndentationLevel();

		/*!
		 * \brief Set the indentation level for one single line, given by the cursor's position.
		 * \param indentationLevel The indentation level.
		 */
		void setIndentationLevel(int indentationLevel);

		//! \brief Automatically indent one single document block at the cursor's position.
		void autoindentDocumentBlock();

		//! \brief Automatically indents the document blocks marked by the cursor.
		void autoindentDocumentBlocks();

		//! \brief Automatically indents all document blocks inside the code block at the cursor's position.
		void autoindentCodeBlock();

		/*!
		 * \brief Adjust the indentation level of multiple document blocks marked by the cursor.
		 * \param value The value to adjust by (positive means increase indentation, negative means decrease).
		 */
		void adjustIndentationLevel(int value = 1);

	private:

		//! \brief The GLSL document.
		GLSLDocument* _parentDocument;

		//! \brief The parent document.
		QTextCursor& _indentationCursor;
	};

}

#endif // GLSLINDENTOR_H
