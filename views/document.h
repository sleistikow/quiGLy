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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "commands/ichangeable.h"

#include <QObject>

namespace ysm
{
	class ISerializable;
	class IUICommandQueue;
	class IPipeline;
	class GLController;
	class Registry;
	class IPipelineManager;

	//! \brief Class that stores document data and provides change notifications.
	class Document : public QObject, public IChangeable
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Create new document.
		 * \param parent The parent object.
		 */
		Document(QObject* parent);

		/*!
		 * \brief Create document from existing file.
		 * Note: To load the file, call load() after attaching serializables.
		 * \param filename The filename.
		 * \param parent The parent object.
		 */
		Document(const QString& filename, QObject* parent);

		//! \brief Destruct instance.
		virtual ~Document();

		/*!
		 * \brief Gets the document's current name (filename or placeholder).
		 * \return The document's name.
		 */
		QString getName() const;

		/*!
		 * \brief Gets the document's filename.
		 * \return The document's file.
		 */
		QString getFile() const;

		/*!
		 * \brief Set a new filename for the document.
		 * \param filename The filename.
		 */
		void setFile(const QString& filename);

		/*!
		 * \brief Gets the document's command queue.
		 * \return The command queue.
		 */
		IUICommandQueue* getCommandQueue() const;

		/*!
		 * \brief Gets the document's pipeline.
		 * \return The pipeline.
		 */
		IPipeline* getPipeline() const;

		/*!
		 * \brief Gets the document's OpenGL controller.
		 * \return The OpenGL controller.
		 */
		GLController* getGLController() const;

		/*!
		 * \brief Gets the document's interface registry.
		 * \return The registry.
		 */
		Registry* getRegistry() const;

		/*!
		 * \brief Load the project from it's file.
		 * \param messages Any messages that might have occured.
		 * \return True on success.
		 */
		bool load(QStringList& messages);

		/*!
		 * \brief Imports a document from file into this document.
		 * \param filename The file name.
		 * \param messages Any messages that might have occured.
		 * \return True on success.
		 */
		bool import(const QString& filename, QStringList& messages);

		/*!
		 * \brief Saves the document.
		 * \param messages Any messages that might have occured.
		 * \param assets True if assets should be exported.
		 * \param compress True if document should be stored as archive.
		 * \return True on success.
		 */
		bool save(QStringList& messages, bool assets = false, bool compress = false);

		/*!
		 * \brief Exports the document.
		 * \param filename The target file. This is NOT set as document file.
		 * \param assets True if assets should be exported.
		 * \param compress True if document should be stored as archive.
		 * \param messages Any messages that might have occured.
		 * \return True on success.
		 */
		bool save(const QString& filename, QStringList& messages, bool assets = false, bool compress = false);

		/*!
		 * \brief Checks wether the document has unsaved changes.
		 * \return True, if document has unsaved changes.
		 */
		bool hasUnsavedChanges();

		/*!
		 * \brief Wether the document is currently being rendered.
		 * \return True, if the document is currently being rendered.
		 */
		bool isRendering() const;

		/*!
		 * \brief Checks wether rendering is supported.
		 * \return True, if rendering is supported.
		 */
		bool isRenderingSupported() const;

		/*!
		 * \brief Set the document's rendering state.
		 * \param isRendering True if rendering.
		 */
		void setRendering(bool isRendering);

		/*!
		 * \brief Attach additional serializable data.
		 * \param attachment The attachment data.
		 */
		void attach(ISerializable* attachment);

		/*!
		 * \brief Detach additional serializable data.
		 * \param attachment The attachment data.
		 */
		void detach(ISerializable* attachment);

	signals:

		//! \brief The document's settings changed.
		void settingsChanged();

		//! \brief The document's state changed.
		void unsavedChangesChanged(Document*);

		//! \brief The document's rendering state changed.
		void renderingChanged();

	protected slots:

		//! \brief Emit unsaved changes.
		void emitChanges();

	private:

		//! \brief The command queue.
		IUICommandQueue* _commandQueue;

		//! \brief The represented pipeline.
		IPipeline* _pipeline;

		//! \brief The OpenGL controller used for rendering.
		GLController* _openGL;

		//! \brief The document's interface registry.
		Registry* _registry;

		//! \brief The pipeline manager.
		IPipelineManager* _manager;

		//! \brief The document file.
		QString _file;

		//! \brief The document's current rendering state.
		bool _isRendering;

		//! \brief Additional serializable data.
		QList<ISerializable*> _attachments;
	};

}

#endif // DOCUMENT_H
