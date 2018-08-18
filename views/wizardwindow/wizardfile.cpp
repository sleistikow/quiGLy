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

#include "wizardfile.h"
#include "views/logview/logview.h"

#include <QApplication>
#include <QDir>

#define BASE_DIR "/resource/wizards/"

#define WIZARDS_DIR "wizards/"
#define SOURCES_DIR "sources/"
#define OUTPUTS_DIR "outputs/"
#define INCLUDE_DIR "include/"

#define WIZARD_FILE "wizard.xml"
#define TEMPLATE_FILE "project.xmt"

using namespace ysm;

//Property keys.
const QString WizardFile::Name = "name";
const QString WizardFile::Description = "description";

//Invalid instance.
const WizardFile WizardFile::Invalid;

WizardFile::WizardFile(const QString& baseDirectory, const QString& wizardName) :
	_baseDirectory(baseDirectory),
	_wizardName(wizardName),
	_isValid(false)
{
	//Load the configuration.
	parseConfiguration();
}

WizardFile::WizardFile(const WizardFile& wizardFile) :
	_baseDirectory(wizardFile._baseDirectory),
	_wizardName(wizardFile._wizardName),
	_isValid(wizardFile._isValid),
	_rootElement(wizardFile._rootElement)
{ }

WizardFile::WizardFile() :
	_isValid(false)
{ }

WizardFile& WizardFile::operator=(const WizardFile& wizardFile)
{
	//Ensure no self-assignment.
	if(this != &wizardFile)
	{
		//Copy the values.
		_baseDirectory = wizardFile._baseDirectory;
		_wizardName = wizardFile._wizardName;
		_isValid = wizardFile._isValid;
		_rootElement = wizardFile._rootElement;
	}

	//Return self-reference.
	return *this;
}

bool WizardFile::isValid() const { return _isValid; }
QString WizardFile::getTemplate() const
{
	//The template file resides in the wizards directory.
	QDir wizardDirectory(getWizardsDirectory(_baseDirectory));
	return QDir(wizardDirectory.filePath(_wizardName)).filePath(TEMPLATE_FILE);
}

QList<WizardFile::Page> WizardFile::getPages() const
{
	//Ensure that the pages container tag is specified correctly.
	QList<Page> pageList;
	QDomElement pagesElement = _rootElement.firstChildElement("pages");
	if(!pagesElement.isNull())
	{
		//Iterate over the pages inside the container.
		for(QDomElement pageElement = pagesElement.firstChildElement("page"); !pageElement.isNull();
			pageElement = pageElement.nextSiblingElement("page"))
		{
			//Retrieve the page's name and store the data.
			QString pageType = pageElement.attribute("type");
			pageList.append({ pageType, pageElement });
		}
	}

	//Return the retrieved pages.
	return pageList;
}

void WizardFile::parseConfiguration()
{
	//The configuration file resides in the wizards directory.
	QDir wizardDirectory(getWizardsDirectory(_baseDirectory));
	QFile configurationFile(QDir(wizardDirectory.filePath(_wizardName)).filePath(WIZARD_FILE));

	//Try to retrieve the root element.
	_isValid = getRootElement(configurationFile, _rootElement);
}

bool WizardFile::getRootElement(QFile& configurationFile, QDomElement& rootElement)
{
	//Ensure the configuration file is available.
	if(!configurationFile.open(QIODevice::ReadOnly)) return false;

	//Build the parser and try to load the file.
	int errorLine;
	QString errorMessage;
	QDomDocument configurationDocument(configurationFile.fileName());
	bool contentSet = configurationDocument.setContent(&configurationFile, &errorMessage, &errorLine);

	//Ensure file was loaded correctly.
	//TODO: Maybe logging can be done differently to split clearly from UI.
	configurationFile.close();
	if(!contentSet)
	{
		//Log the occured error.
		LogView::log("Could not parse wizard file: " + configurationFile.fileName());
		LogView::log(QString("Error in line (%1): %2").arg(errorLine).arg(errorMessage));

		//Do not proceed.
		return false;
	}

	//Get the root element.
	rootElement = configurationDocument.documentElement();
	return true;
}

QString WizardFile::getProperty(QDomElement rootElement, const QString& propertyKey,
										 const QString& defaultValue)
{
	//Access the matching elements.
	QDomElement propertyElement = rootElement.firstChildElement(propertyKey);
	if(!propertyElement.isNull())
		return propertyElement.text();

	//Element not available.
	return defaultValue;
}

QString WizardFile::getAttribute(QDomElement rootElement, const QString& propertyKey,
										  const QString& defaultValue, const QString& attributeName)
{
	//Access the matching elements.
	QDomElement propertyElement = rootElement.firstChildElement(propertyKey);
	if(!propertyElement.isNull())
		return propertyElement.attribute(attributeName, defaultValue);

	//Element not available.
	return defaultValue;
}

QString WizardFile::getBaseDirectory()
{
	//Access the project wizard directory.
	QString wizardPath = YSM_BASE_PATH;
	wizardPath += BASE_DIR;
	return QDir::cleanPath(wizardPath);
}

QString WizardFile::getWizardsDirectory(const QString& baseDirectory)
{
	//Append the wizards directory.
	return QDir(baseDirectory).filePath(WIZARDS_DIR);
}

QString WizardFile::getSourcesDirectory(const QString& baseDirectory)
{
	//Append the sources directory.
	return QDir(baseDirectory).filePath(SOURCES_DIR);
}

QString WizardFile::getOutputsDirectory(const QString& baseDirectory)
{
	//Append the outputs directory.
	return QDir(baseDirectory).filePath(OUTPUTS_DIR);
}

QString WizardFile::getIncludeDirectory(const QString& baseDirectory)
{
	//Append the includes directory.
	return QDir(baseDirectory).filePath(INCLUDE_DIR);
}

QString WizardFile::getProperty(const QString& propertyKey) const { return getProperty(_rootElement, propertyKey); }

QString WizardFile::getWizardsDirectory() const { return getWizardsDirectory(_baseDirectory); }
QString WizardFile::getSourcesDirectory() const { return getSourcesDirectory(_baseDirectory); }
QString WizardFile::getOutputsDirectory() const { return getOutputsDirectory(_baseDirectory); }
QString WizardFile::getIncludeDirectory() const { return getIncludeDirectory(_baseDirectory); }

