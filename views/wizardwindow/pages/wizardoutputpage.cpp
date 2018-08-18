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

#include "wizardoutputpage.h"
#include "../wizardwindow.h"
#include "../wizardfile.h"

#include <QFormLayout>
#include <QDir>

using namespace ysm;

WizardOutputPage::WizardOutputPage(QDomElement rootElement, WizardWindow* parent) :
	WizardPageDelegate(parent),
	WizardDependencyObject(rootElement)
{
	//Retrieve the relevant data.
	_prefix = rootElement.attribute("name");

	//Create the basic form layout.
	QFormLayout* pageLayout = new QFormLayout(this);

	//Iterate over all available outputs.
	for(QDomElement element = rootElement.firstChildElement("output"); !element.isNull();
		element = element.nextSiblingElement("output"))
	{
		//Retrieve the output's data.
		QString output = element.attribute("output");
		bool isEnabled = element.attribute("default") == "true";

		//Get the output file.
		QString outputFile = QString("%1.xml").arg(output);
		QString outputPath = QDir(parent->getWizard().getOutputsDirectory()).filePath(outputFile);
		QFile configurationFile(outputPath);

		//Load the output file.
		QDomElement outputRoot;
		if(WizardFile::getRootElement(configurationFile, outputRoot))
		{
			//Initialize the output widget.
			WizardPrefixDelegate* outputDelegate = new WizardPrefixDelegate(this, output);
			WizardOutputSelect* outputSelect = new WizardOutputSelect(parent);
			outputSelect->setDelegate(outputDelegate);
			outputSelect->setDefault(isEnabled);
			outputSelect->generateLayout(pageLayout, outputRoot);

			//Retrieve additional data.
			QString source = outputRoot.attribute("source");
			QString naming = WizardFile::getProperty(outputRoot, "caption");

			//Store the output.
			parent->registerOutput({ _prefix, output, source, naming, parent });
			_outputSelects.append(outputSelect);
			_delegates.append(outputDelegate);
		}

		//Append a spacer.
		pageLayout->addItem(new QSpacerItem(0, 5));
	}

	//Configure the page.
	setTitle(WizardFile::getProperty(rootElement, "title"));
	setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
}

WizardOutputPage::~WizardOutputPage() { qDeleteAll(_outputSelects); qDeleteAll(_delegates); }

QString WizardOutputPage::dynamicName(const QString& name) const
{
	//Prefix the name by the output name.
	return QString("%1.%2").arg(_prefix).arg(name);
}

bool WizardOutputPage::isComplete() const
{
	//Check all output selects.
	foreach(WizardOutputSelect* outputSelect, _outputSelects)
		if(!outputSelect->isComplete())
			return false;

	//All outputs checked.
	return true;
}

void WizardOutputPage::initializePage()
{
	//Initialize all output selects.
	foreach(WizardOutputSelect* outputSelect, _outputSelects)
		outputSelect->initializeItems();
}
