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

#include "ziparchive.h"

#include <QDirIterator>
#include <QDebug>

#define NAME_BUFFER_SIZE	1024

namespace ysm
{
	ZipArchive::ZipArchive()
	{

	}

	ZipArchive::~ZipArchive()
	{
		closeZip();
	}

	bool ZipArchive::openZip(QString zipFile, ArchiveMode mode)
	{
		// Close any previously opened archive
		closeZip();

		if (mode == ModePack)
		{
			// Open the archive for storing
			_zipFile = zipOpen(qPrintable(zipFile), APPEND_STATUS_CREATE);
			return (_zipFile != nullptr);
		}
		else if (mode == ModeUnpack)
		{
			// Open the archive for unpacking
			_unzipFile = unzOpen(qPrintable(zipFile));
			return (_unzipFile != nullptr);
		}

		// Unknown mode
		return false;
	}

	void ZipArchive::closeZip()
	{
		if (_zipFile)
		{
			zipClose(_zipFile, nullptr);
			_zipFile = nullptr;
		}

		if (_unzipFile)
		{
			unzClose(_unzipFile);
			_unzipFile = nullptr;
		}
	}

	bool ZipArchive::addFile(QString fileName, QString zipDir)
	{
		// Not opened for storing?
		if (!_zipFile)
			return false;

		QFileInfo fi{fileName};
		QString nameInArchive = (zipDir.isEmpty() ? fi.fileName() : QFileInfo(QDir(zipDir), fi.fileName()).filePath());
		zip_fileinfo zipInfo;

		memset(&zipInfo, 0, sizeof(zipInfo));

		// Create a new file in the archive
		if (zipOpenNewFileInZip(_zipFile, qPrintable(nameInArchive), &zipInfo, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_BEST_COMPRESSION) == ZIP_OK)
		{
			QFile file{fileName};
			bool ret = false;

			// Write the contents to the archive file
			if (file.open(QIODevice::ReadOnly))
			{
				QByteArray contents = file.readAll();

				if (zipWriteInFileInZip(_zipFile, contents.data(), contents.size()) == ZIP_OK)
					ret = true;
			}

			zipCloseFileInZip(_zipFile);
			return ret;
		}

		return false;
	}

	bool ZipArchive::addDirectory(QString dirName, QString baseDir)
	{
		// Not opened for storing?
		if (!_zipFile)
			return false;

		if (baseDir.isEmpty())
			baseDir = dirName;

		QDirIterator it(dirName, QDirIterator::Subdirectories);
		QDir rootDir{baseDir};

		// Iterate over all files & folders in the specified directory and add them to the archive
		while (it.hasNext())
		{
			QString file = it.next();
			QFileInfo fi = it.fileInfo();

			if (fi.isDir())
				continue;

			// Get the relative directory
			QDir dir = fi.dir();
			QString relDir = rootDir.relativeFilePath(dir.absolutePath());

			if (!addFile(file, relDir))
				return false;
		}

		return true;
	}

	bool ZipArchive::unpackContents(QString destDir)
	{
		// Not opened for unpacking?
		if (!_unzipFile)
			return false;

		bool ret = true;

		// Extract all files found in the archive
		if (unzGoToFirstFile(_unzipFile) == UNZ_OK)
		{
			do
			{
				unz_file_info unzipInfo;
				char nameBuffer[NAME_BUFFER_SIZE];

				memset(&unzipInfo, 0, sizeof(unzipInfo));
				memset(nameBuffer, 0, NAME_BUFFER_SIZE * sizeof(char));

				// Get information about the current file
				if (unzGetCurrentFileInfo(_unzipFile, &unzipInfo, nameBuffer, NAME_BUFFER_SIZE, nullptr, 0, nullptr, 0) == UNZ_OK)
				{
					// Finally, open the file and read its contents
					if (unzOpenCurrentFile(_unzipFile) == UNZ_OK)
					{
						if (!unpackFile(destDir, nameBuffer, unzipInfo.uncompressed_size))
							ret = false;

						unzCloseCurrentFile(_unzipFile);
					}
					else
						ret = false;
				}
				else
					ret = false;
			} while (ret && unzGoToNextFile(_unzipFile) == UNZ_OK);
		}

		return ret;
	}

	bool ZipArchive::unpackFile(QString destDir, QString fileName, unsigned int fileSize)
	{
		QFileInfo fullPath{QDir{destDir}, fileName};
		QFile file(fullPath.absoluteFilePath());
		bool ret = false;

		// Create the destination directory
		fullPath.dir().mkpath(".");

		// Open our destination file
		if (file.open(QIODevice::WriteOnly|QIODevice::Truncate))
		{
			if (fileSize > 0)
			{
				QByteArray data;

				// Read the compressed data into a buffer...
				data.resize(fileSize);
				int copied = unzReadCurrentFile(_unzipFile, data.data(), fileSize);

				// ... and write it to the destination (but only if we got all data)
				if (copied == (int)fileSize)
				{
					file.write(data.data(), copied);
					file.flush();

					ret = true;
				}
			}
			else
				ret = true;

			file.close();
		}

		return ret;
	}
}
