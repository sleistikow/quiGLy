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

#ifndef GLSLDOCUMENTLIST_H
#define GLSLDOCUMENTLIST_H

#include "../iserializable.h"
#include "data/iblock.h"

#include <QVector>

namespace ysm
{
	class Document;
	class GLSLDocument;
	class GLSLCodeBlock;

	//! \brief Serializable list of GLSL documents.
	//! As long as a document is part of the document list, it's retained by it.
	class GLSLDocumentList : public ISerializable
	{
	public:

		//! \brief Initialize new instance.
		GLSLDocumentList();

		/*!
		 * \brief Create copy of instance.
		 * \param src The source instance.
		 */
		GLSLDocumentList(const GLSLDocumentList& src);

		//! \brief Destruct instance.
		virtual ~GLSLDocumentList();

		/*!
		 * \brief Checks, wether the generate input flag is set.
		 * \return True, if input should be generated.
		 */
		bool hasGenerateInput() const;

		/*!
		 * \brief Generate the input blocks for this document.
		 * \param codeBlock The code block to generate input blocks for.
		 */
		void generateInputBlocks(GLSLCodeBlock* codeBlock);

		/*!
		 * \brief Serialize list.
		 * \param xmlElement The xml target element.
		 * \param ctx The context.
		 */
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;

		/*!
		 * \brief Deserialize list.
		 * \param xmlElement The xml source element.
		 * \param ctx The context.
		 */
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

		/*!
		 * \brief Assign a document list to another document list.
		 * \param src The source instance.
		 * \return The target instance.
		 */
		GLSLDocumentList& operator =(const GLSLDocumentList& src);

		/*!
		 * \brief Compare another instance to this instance.
		 * \param src The source instance.
		 * \return True if equal.
		 */
		bool operator ==(const GLSLDocumentList& src) const;

		/*!
		 * \brief Compare another instance to this instance.
		 * \param src The source instance.
		 * \return False if equal.
		 */
		bool operator !=(const GLSLDocumentList& src) const;

		/*!
		 * \brief Gets the list of all documents.
		 * \return
		 */
		QVector<GLSLDocument*> getDocuments() const;

		/*!
		 * \brief Adds a document.
		 * \param document The document or NULL for a new one to be created.
		 * \param index The index or -1 to append.
		 * \return The added document
		 */
		GLSLDocument* addDocument(GLSLDocument* document = NULL, int index = -1);

		/*!
		 * \brief Adds a document.
		 * \param plainText The plain text.
		 * \param targetBlock The block to assign to the document.
		 * \param index The index or -1 to append.
		 * \return The added document
		 */
		GLSLDocument* addDocument(QString plainText, IBlock* targetBlock, int index = -1);

		/*!
		 * \brief Removes a document.
		 * \param document The document.
		 */
		void removeDocument(GLSLDocument* document);

		/*!
		 * \brief Removes a document.
		 * \param documentIndex The document's index.
		 */
		void removeDocument(int documentIndex);

	private:

		//! \brief Holds all documents.
		QVector<GLSLDocument*> _documents;

		//! \brief True, if input blocks should be generated.
		bool _generateInput;
	};

}

#endif // GLSLDOCUMENTLIST_H
