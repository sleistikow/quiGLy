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

#include "wizardsourcepage.h"
#include "../wizardwindow.h"
#include "../wizardfile.h"

#include <QVBoxLayout>
#include <QDir>

using namespace ysm;

WizardSourcePage::WizardSourcePage(QDomElement rootElement, WizardWindow* parent) :
	WizardPageDelegate(parent),
	WizardDependencyObject(rootElement),
	_sourceSelect(NULL)
{
	//Retrieve the relevant data.
	_prefix = rootElement.attribute("name");
	_source = rootElement.attribute("source");

	//Create the basic form layout.
	QFormLayout* pageLayout = new QFormLayout(this);

	//Get the source file.
	QString sourceFile = QString("%1.xml").arg(_source);
	QString sourcePath = QDir(parent->getWizard().getSourcesDirectory()).filePath(sourceFile);
	QFile configurationFile(sourcePath);

	//Load the source file.
	if(WizardFile::getRootElement(configurationFile, _sourceRoot))
	{
		//Initialize the main widget.
		_sourceSelect = new WizardSourceSelect(_source, false, parent);
		_sourceSelect->setDelegate(this);
		_sourceSelect->generateLayout(pageLayout, _sourceRoot);

		//Store the source.
		QString primary = _sourceRoot.attribute("primary");
		QString disable = _sourceRoot.attribute("disable");
		QString naming = WizardFile::getProperty(_sourceRoot, "caption");
		parent->registerSource({ _prefix, _source, primary, disable, naming, parent });
	}

	//Configure the page.
	setTitle(WizardFile::getProperty(rootElement, "title"));
	setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
}

QString WizardSourcePage::dynamicName(const QString& name) const
{
	//Prefix the name by the source name.
	return QString("%1.%2").arg(_prefix).arg(name);
}

bool WizardSourcePage::isComplete() const { return !_sourceSelect  || _sourceSelect->isComplete(); }

void WizardSourcePage::initializePage()
{
	//Initialize the source select.
	if(_sourceSelect) _sourceSelect->initializeItems();
}
