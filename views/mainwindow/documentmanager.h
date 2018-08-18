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

#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include "commands/iuicommandqueue.h"

#include <QObject>
#include <QThread>
#include <QSignalMapper>

namespace ysm
{
	class IChangeable;
	class Document;
	class MainWindow;

	//! \brief Document manager for the main window.
	class DocumentManager : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWindow The parent window.
		 */
		DocumentManager(MainWindow* parentWindow);

		/*!
		 * \brief Returns all documents.
		 * \return List of all documents.
		 */
		QList<Document*> getDocuments() const;

	public slots:

		//! \brief Creates a new document.
		Document* createDocument();

		/*!
		 * \brief Load a document from file.
		 * \param filename The file name.
		 */
		Document* loadDocument(QString filename);

		/*!
		 * \brief Save the document.
		 * \param document The document to save.
		 */
		void saveDocument(Document* document);

		/*!
		 * \brief Load the given file into the given document.
		 * \param document The base document.
		 * \param filename The file to import.
		 */
		void importDocument(Document* document, QString filename);

		/*!
		 * \brief Export the document.
		 * \param document The document to save.
		 * \param filename The file to export to.
		 * \param assets True if assets should be stored.
		 * \param compress True if document should be compressed.
		 */
		void exportDocument(Document* document, QString filename, bool assets, bool compress);

		/*!
		 * \brief Closes the given document.
		 * \param document The document.
		 * \return True if document was closed.
		 */
		bool closeDocument(Document* document);

		/*!
		 * \brief Close all documents.
		 * \return True if all documents were closed.
		 */
		bool closeAll();

	protected:

		/*!
		 * \brief Add the document to the list of documents.
		 * \param document The new document.
		 */
		void addDocument(Document* document);

		/*!
		 * \brief Notify about a document's change.
		 * \param document The new document.
		 */
		void notifyDocumentChanged(Document* document);

	protected slots:

		/*!
		 * \brief Data was removed, reacts to UI command queue.
		 * \param document Document that was changed or NULL to apply to all documents.
		 * \param changes List of removed objects.
		 */
		void willRemoveData(Document* document, const QList<IChangeable*>& changes);

		/*!
		 * \brief Data will be changed, reacts to UI command queue.
		 * \param document Document that was changed or NULL to apply to all documents.
		 * \param changes List of removed objects.
		 */
		void didChangeData(Document* document, const QList<IChangeable*>& changes);

		/*!
		 * \brief Data will be added, reacts to UI command queue.
		 * \param document Document that was changed or NULL to apply to all documents.
		 * \param changes List of removed objects.
		 */
		void didAddData(Document* document, const QList<IChangeable*>& changes);

	private:

		//! \brief List of all documents.
		QList<Document*> _documents;

		//! \brief The parent window.
		MainWindow* _parentWindow;
	};

}

#endif // DOCUMENTMANAGER_H
