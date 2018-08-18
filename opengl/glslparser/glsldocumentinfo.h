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

#ifndef GLSLDOCUMENTINFO_H
#define GLSLDOCUMENTINFO_H

#include <QMap>

namespace ysm
{
	class GLSLDocument;
	class GLSLDeclaration;

	//! \brief Contains GLSL document information that is preserved during reparse.
	class GLSLDocumentInfo
	{

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent document.
		 */
		GLSLDocumentInfo(GLSLDocument* parent);

		/*!
		 * \brief Returns the GLSL version of this document.
		 * \return The version as integer: e.g. 330 and it's addon as string: e.g. 'core'.
		 */
		QPair<int, QString> getVersion() const;

		/*!
		 * \brief Sets the GLSL version of this document.
		 * \param version The version as integer: e.g. 330 and it's addon as string: e.g. 'core'.
		 */
		void setVersion(QPair<int, QString> version);

		/*!
		 * \brief Returns the extensions enabled/disabled in this document.
		 * \return Values mapped to their extension names.
		 */
		QMap<QString, QString> getExtensions() const;

		/*!
		 * \brief Sets the extensions enabled/disabled in this document.
		 * \param extensions Values mapped to their extension names.
		 */
		 void setExtensions(QMap<QString, QString> extensions);

		 /*!
		  * \brief Returns the declaration data mapped to specific pipeline items.
		  * That data is preserved during reparse.
		  * \return Item declaration data.
		  */
		 QMap<int, GLSLDeclaration*> getDeclarations() const;

		 /*!
		  * \brief Sets the declaration data mapped to specific pipeline items.
		  * \param declarations Item declaration data.
		  */
		 void setDeclarations(QMap<int, GLSLDeclaration*> declarations);

	private:

		 //! \brief The parent document.
		 GLSLDocument* _document;

		 //! \brief The document's GLSL version and addon (e.g. 'core').
		 QPair<int, QString> _version;

		 //! \brief Extensions and their respective values.
		 QMap<QString, QString> _extensions;

		 //! \brief Global declarations and their respective data, mapped by pipeline item id.
		 QMap<int, GLSLDeclaration*> _declarations;
	};

}

#endif // GLSLDOCUMENTINFO_H
