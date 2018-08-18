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

#ifndef ZIPARCHIVE_H
#define ZIPARCHIVE_H

#include <minizip/zip.h>
#include <minizip/unzip.h>

#include <QString>

namespace ysm
{
	/**
	 * @brief Small helper class to work with Zip archives
	 */
	class ZipArchive
	{
	public:
		enum ArchiveMode
		{
			ModePack,
			ModeUnpack,
		};

	public:
		// Construction
		explicit ZipArchive();
		virtual ~ZipArchive();

	public:
		// Archive handling
		/**
		 * @brief Opens the specified archive, creating it if necessary
		 */
		bool openZip(QString zipFile, ArchiveMode mode);

		/**
		 * @brief Closes the currently opened zip file
		 */
		void closeZip();

		// File handling
		/**
		 * @brief Adds the specified file to the archive
		 */
		bool addFile(QString fileName, QString zipDir);

		/**
		 * @brief Adds the specified directory to the archive
		 */
		bool addDirectory(QString dirName, QString baseDir = "");

		/**
		 * @brief Extracts the contents of the archive to @p destDir
		 * Note: Existing files will be overwritten
		 */
		bool unpackContents(QString destDir);

	private:
		/**
		 * @brief Unpacks the current file to @p destDir
		 */
		bool unpackFile(QString destDir, QString fileName, unsigned int fileSize);

	private:
		zipFile _zipFile{nullptr};
		unzFile _unzipFile{nullptr};
	};
}

#endif
