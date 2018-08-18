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

#ifndef PIPELINEPROJECT_H
#define PIPELINEPROJECT_H

#include <QString>
#include <QFile>
#include <QDir>

class QDomElement;

namespace ysm
{
	class IPipeline;
	class ISerializable;
	class IPipelineManager;
	class SerializationContext;

	//! \brief Class that provides methods to load and store pipeline projects.
	class PipelineProjectStream
	{
	public:

		//! \brief Supported project file version.
		static unsigned int _projectFileVersion;

		/*!
		 * \brief Loads a project file and returns the created pipeline manager.
		 * \param filename The source file.
		 * \param messages Returns messages that might have occured during loading.
		 * \param target Target pipelinemanager, if not null the file is imported instead of loaded.
		 * \param additionalObjects Additional serializable data.
		 * \return The created pipeline, or target, if specified.
		 */
		static IPipelineManager* loadProject(QString filename, QStringList& messages, IPipelineManager* target = NULL,
											 QList<ISerializable*>* additionalObjects = nullptr);

		/*!
		 * \brief Stores an existing pipeline manager to file.
		 * \param source The pipeline to store.
		 * \param filename The target file.
		 * \param messages Returns messages that might have occured during saving.
		 * \param exportAssets True, if assets should be included in export.
		 * \param additionalObjects Additional serializable data.
		 * \return True on success.
		 */
		static bool storeProject(IPipelineManager* source, QString filename, QStringList& messages,
								 bool exportAssets = false, QList<ISerializable*>* additionalObjects = nullptr);

		/*!
		 * \brief Compresses an existing pipeline manager to zip archive.
		 * \param source The pipeline to store.
		 * \param filename The target file.
		 * \param messages Returns messages that might have occured during saving.
		 * \param exportAssets True, if assets should be included in export.
		 * \param additionalObjects Additional serializable data.
		 * \return True on success.
		 */
		static bool compressProject(IPipelineManager* source, QString filename, QStringList& messages,
									QList<ISerializable*>* additionalObjects = nullptr);

	private:

		/*!
		 * \brief Loads the additional serializable data.
		 * \param root The root XML element.
		 * \param context The context.
		 * \param additionalObjects The additional objects.
		 */
		static void loadAdditionalObjects(QDomElement* root, SerializationContext* context,
										  QList<ysm::ISerializable*>* additionalObjects);

		/*!
		 * \brief Stores the additional serializable data.
		 * \param root The root XML element.
		 * \param context The context.
		 * \param additionalObjects The additional objects.
		 */
		static void storeAdditionalObjects(QDomElement* root, SerializationContext* context,
										   QList<ysm::ISerializable*>* additionalObjects);

		/*!
		 * \brief Retrieves the highest object ID from the given file.
		 * \param filename The filename.
		 * \return The highest object ID.
		 */
		static unsigned int getHighestObjectID(QString filename);

		/*!
		 * \brief Get the assets directory from the given filename.
		 * \param filename The filename.
		 * \return The assets directory.
		 */
		static QDir getAssetsDirectory(QString filename);

		/*!
		 * \brief Store the project assets.
		 * \param source The pipeline to store.
		 * \param filename The target file.
		 * \param messages Possible messages that occured.
		 * \return True on success.
		 */
		static bool storeAssets(IPipelineManager *source, QString filename, QStringList& messages);

		/*!
		 * \brief Store a single project asset.
		 * \param sourceAsset The asset.
		 * \param directory The directory.
		 * \return True on success.
		 */
		static bool copyAsset(const QString& sourceAsset, const QDir& directory);

		/*!
		 * \brief Extract an archive that holds a project.
		 * \param filename The archive's filename, which is replaced by the project's filename.
		 * \param messages Possible error messages.
		 * \return True on success.
		 */
		static bool extractArchive(QString& filename, QStringList& messages);
	};
}

#endif
