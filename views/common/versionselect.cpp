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

#include "versionselect.h"
#include "opengl/glcontroller.h"

using namespace ysm;

VersionSelect::VersionSelect(int minimumVersion, QWidget* parent) :
	QComboBox(parent)
{
	//Create a temporary GL controller for version tests.
	GLController* versionTest = NULL;
	try { versionTest = new GLController(NULL); }
	catch(std::exception) { versionTest = NULL; }

	//Retrieve all versions supported by quiGLy.
	RenderManager* manager = MainWindow::getInstance()->getRenderManager();
	QList<RenderManager::OpenGLVersion> availableVersions = manager->getSupportedVersions();

	//Iterate over all versions and generate a string representation.
	for(int i = 0; i < availableVersions.length(); i++)
	{
		//Get the current version and build the numeric version for comparison.
		RenderManager::OpenGLVersion version = availableVersions[i];
		int numericVersion = version._major * 100 + version._minor * 10;

		//Check if the current version is supported.
		if(numericVersion < minimumVersion) continue;
		bool isSupported = versionTest && versionTest->isOpenGLVersionSupported(version._major, version._minor);

		//Build the version string.
		QString versionString = QString("OpenGL %1.%2").arg(version._major).arg(version._minor);
		if(!isSupported)
			versionString += " (not supported)";

		//Store the current version.
		_versionInfo[versionString] = qMakePair(version, isSupported);
	}

	//Add the available version strings to the combo box.
	addItems(_versionInfo.keys());
}

RenderManager::OpenGLVersion VersionSelect::getCurrentVersion() const
{
	//Return the current version info.
	return _versionInfo[currentText()].first;
}

int VersionSelect::getNumericVersion() const
{
	//Return the current version info as numeric version.
	RenderManager::OpenGLVersion version = _versionInfo[currentText()].first;
	return version._major * 100 + version._minor * 10;
}
