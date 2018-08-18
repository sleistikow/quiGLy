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

#include "pipelineprojectstream.h"
#include "pipelinemanager.h"

#include "data/common/dataexceptions.h"
#include "data/common/compr/ziparchive.h"

#include "data/blocks/connectionlist.h"
#include "data/blocks/connection.h"
#include "data/blocks/port.h"

#include "data/pipeline/visitors/assetspipelinevisitor.h"

#include <QIODevice>
#include <QFileInfo>
#include <QTemporaryDir>
#include <QXmlStreamReader>

using namespace ysm;

unsigned int PipelineProjectStream::_projectFileVersion = 110;

IPipelineManager* PipelineProjectStream::loadProject(QString filename, QStringList &messages, IPipelineManager *target,
													 QList<ISerializable *> *additionalObjects)
{
	//Open the file.
	QFile unknownFile(filename);
	if(unknownFile.open(QIODevice::ReadOnly))
	{
		//Determine the file format by the first two bytes.
		char identifierBytes[2];
		unknownFile.read(&identifierBytes[0], 2);
		unknownFile.close();

		//Extract zip archives.
		if(identifierBytes[0] == 'P' && identifierBytes[1] == 'K')
			if(!extractArchive(filename, messages))
			{
				messages << "Unable to extract project archive.";
				return NULL;
			}

		//Access the correct target file (might have changed during archive extraction).
		QFile sourceFile(filename);
		QFileInfo sourceInfo(sourceFile);

		//Open the new project file.
		if(sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			//Try to create the XML document.
			int errorLine;
			QString errorMessage;
			QDomDocument xmlDocument;
			if(!xmlDocument.setContent(&sourceFile, &errorMessage, &errorLine))
			{
				messages << "Unable to parse project file: " + sourceFile.fileName();
				messages << QString("Error in line (%1): %2").arg(errorLine).arg(errorMessage);
				return NULL;
			}

			//Access root element.
			QDomElement rootElement = xmlDocument.documentElement();
			SerializationContext context(&xmlDocument, &rootElement);

			//Safely try to load the project.
			try
			{
				//Check wether new project is required.
				PipelineManager* projectManager = static_cast<PipelineManager*>(target);
				if(!projectManager)
					projectManager = new PipelineManager();

				//Initialize the context.
				context.initContext(SerializationContext::PathAdjustMode::Load, sourceInfo.dir(),
									getAssetsDirectory(filename), PipelineManager::getHighestItemID(), target);

				//Deserialize all pipelines.
				projectManager->deserialize(&rootElement, &context);
				context.processDeferredElements();
				if(additionalObjects)
					loadAdditionalObjects(&rootElement, &context, additionalObjects);

				//Success.
				return projectManager;
			}

			//Handle possible errors.
			catch(std::exception& exception)
			{
				messages << exception.what();
				return NULL;
			}
		}
	}

	//Project file could not be opened.
	messages << "Unable to open project file.";
	return NULL;
}

bool PipelineProjectStream::storeProject(IPipelineManager *source, QString filename, QStringList &messages,
										 bool exportAssets, QList<ISerializable *> *additionalObjects)
{
	//Export the assets.
	if(exportAssets) storeAssets(source, filename, messages);

	//Create the basic XML document and root element.
	QDomDocument xmlDocument;
	QDomElement rootElement = xmlDocument.createElement("YSMProject");
	xmlDocument.appendChild(rootElement);

	//Store the file version.
	rootElement.setAttribute("fileVersion", _projectFileVersion);

	//Use the correct path adjustment mode.
	SerializationContext::PathAdjustMode adjustMode = exportAssets ?
		SerializationContext::PathAdjustMode::Move :
		SerializationContext::PathAdjustMode::Keep;

	//Create the serialization context.
	SerializationContext context(&xmlDocument, &rootElement);
	context.initContext(adjustMode, QFileInfo(filename).dir(), getAssetsDirectory(filename));
	context.poolPipelineItems(source);

	//Safely try to store the project.
	try
	{
		//Serialize the pipeline.
		source->serialize(&rootElement, &context);
		if(additionalObjects)
			storeAdditionalObjects(&rootElement, &context, additionalObjects);

		//Retrieve context messages.
		messages << context.getMessages();

		//Create the project file.
		QFile targetFile(filename);
		if(!targetFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		{
			messages << "Could not create project file.";
			return false;
		}

		//Write to project file.
		QTextStream outputStream(&targetFile);
		outputStream << xmlDocument.toString(4);
	}

	//Handle possible errors.
	catch (std::exception& exception)
	{
		messages << exception.what();
		return false;
	}

	//Successfully stored.
	return true;
}

bool PipelineProjectStream::compressProject(IPipelineManager *source, QString filename, QStringList &messages,
											QList<ISerializable *> *additionalObjects)
{
	//Get archive data.
	ZipArchive zipArchive;
	QFileInfo fileInfo(filename);

	//Create a temporary directory to store the data.
	QTemporaryDir temporaryDirectory(QDir::temp().absoluteFilePath("YSMProject"));
	QString temporaryPath = temporaryDirectory.path();

	//Adjust the target to be inside the temporary folder and set the correct extension (which is *.zip otherwise).
	QFileInfo temporaryInfo(temporaryPath, fileInfo.fileName());
	QString temporaryFilename = temporaryInfo.absoluteFilePath();
	temporaryFilename.replace(fileInfo.completeSuffix(), "ysm");

	//Store the project to the temporary path.
	storeProject(source, temporaryFilename, messages, true, additionalObjects);

	//Try to open the archive.
	if(!zipArchive.openZip(filename, ZipArchive::ModePack))
	{
		messages << "Unable to create temporary archive.";
		return false;
	}

	//Compress the data.
	if(!zipArchive.addDirectory(temporaryPath))
	{
		messages << "Unable to add files to target archive.";
		return false;
	}

	//Close and create the archive.
	zipArchive.closeZip();
	return true;
}

bool PipelineProjectStream::extractArchive(QString& filename, QStringList& messages)
{
	//Get archive data.
	ZipArchive zipArchive;
	QFileInfo fileInfo(filename);

	//Try to open the archive.
	if(!zipArchive.openZip(filename, ZipArchive::ModeUnpack))
	{
		messages << "Zip archive could not be opened.";
		return false;
	}

	//Extract the archive.
	if(!zipArchive.unpackContents(fileInfo.dir().absolutePath()))
	{
		messages << "Zip archive could not be extracted.";
		return false;
	}

	//Adjust file name to operate on *.ysm from now on.
	filename.replace(fileInfo.completeSuffix(), "ysm");
	return true;
}

void PipelineProjectStream::loadAdditionalObjects(QDomElement* root, SerializationContext* context,
												  QList<ysm::ISerializable*>* additionalObjects)
{
	//Retrieve the element.
	QDomElement additionalElement = root->firstChildElement("Additional");

	//Deserialize the additional data.
	if(!additionalElement.isNull())
		for(ISerializable* serializable : *additionalObjects)
			serializable->deserialize(&additionalElement, context);
}


void PipelineProjectStream::storeAdditionalObjects(QDomElement* root, SerializationContext* context,
												   QList<ysm::ISerializable*>* additionalObjects)
{
	//Create the additional root element.
	QDomElement additionalElement = context->createElement("Additional");
	root->appendChild(additionalElement);

	//Serialize the additional data.
	for(ISerializable* serializable : *additionalObjects)
		serializable->serialize(&additionalElement, context);
}

unsigned int PipelineProjectStream::getHighestObjectID(QString filename)
{
	//Initial maximum is 0.
	unsigned int maxIdentifier = 0;

	//Open the file.
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		//Create an xml reader from the contents, then close the file.
		QXmlStreamReader xmlReader(file.readAll());
		file.close();

		//Iterate over all elements in the project file to find the highest object ID.
		for(xmlReader.readNext(); !xmlReader.atEnd(); xmlReader.readNext())
		{
			//Only handle start elements of pipeline items.
			if(!xmlReader.isStartElement()) continue;
			if(xmlReader.name().compare(QString("property"), Qt::CaseInsensitive) == 0) continue;
			if(xmlReader.name().compare(QString("pipeline"), Qt::CaseInsensitive) == 0) continue;

			//Find the ID attribute and compare to current maximum.
			for(const QXmlStreamAttribute &attribute : xmlReader.attributes())
				if(attribute.name().toString() == "id")
					maxIdentifier = qMax(attribute.value().toUInt(), maxIdentifier);
		}
	}

	//Return the highest available id.
	return maxIdentifier;
}

QDir PipelineProjectStream::getAssetsDirectory(QString filename)
{
	//Get file info and directory.
	QFileInfo fileInfo(filename);
	QDir sourcePath = fileInfo.dir();

	//Assets path consists of the project file name + ".assets".
	QString assetsPath = fileInfo.fileName();
	assetsPath.replace(" ", "");
	assetsPath += ".assets";

	//Build the final path.
	QFileInfo fullPath(sourcePath, assetsPath);
	return QDir(fullPath.absoluteFilePath());
}

bool PipelineProjectStream::storeAssets(IPipelineManager* source, QString filename, QStringList& messages)
{
	//Concrete pipeline manager is required to use visitors.
	PipelineManager* projectManager = static_cast<PipelineManager*>(source);

	//Create assets directory.
	QDir assetsDirectory = getAssetsDirectory(filename);
	assetsDirectory.mkpath(".");

	//Use a visitor to find all assets in the pipeline.
	AssetsPipelineVisitor visitor;
	projectManager->takeVisitor(&visitor);

	//Iterate over all retrieved files and store them.
	QStringList assetFileList = visitor.getAssetFiles();
	foreach(QString assetFile, assetFileList)
	{
		//Validate the file.
		if(assetFile.trimmed().isEmpty()) continue;

		//Try to copy the file to the target directory.
		if(!copyAsset(assetFile, assetsDirectory))
		{
			//Asset could not be stored, append message and return.
			messages << QString("The asset '%1' could not be copied to '%2'")
						.arg(assetFile)
						.arg(assetsDirectory.canonicalPath());
			return false;
		}
	}

	//All assets stored successfully.
	return true;
}

bool PipelineProjectStream::copyAsset(const QString& sourceAsset, const QDir& directory)
{
	//Build the asset's target path.
	QFileInfo fileInfo(sourceAsset);
	QString targetAsset = directory.absoluteFilePath(fileInfo.fileName());

	//Remove any pre-existing file with the same name.
	//TODO: Can be improved on assets with same name.
	QFile::remove(targetAsset);

	//Try to copy the asset.
	return QFile::copy(sourceAsset, targetAsset);
}
