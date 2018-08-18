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

#include "wizardsourceitem.h"
#include "../../wizardfile.h"
#include "../../wizardwindow.h"

#include <QDir>

using namespace ysm;

WizardSourceItem::WizardSourceItem(WizardItemCollection* collection, QDomElement element, QFormLayout* layout) :
	WizardDynamicItem(element, collection),
	WizardPrefixDelegate(),
	_sourceSelect(NULL),
	_parent(collection)
{
	//Retrieve the relevant data.
	QString source = element.attribute("source");
	_prefix = element.attribute("name");
	_delegate = collection->getDelegate();

	//Access the wizard file.
	WizardWindow* parent = collection->getWindow();
	WizardFile wizardFile = parent->getWizard();

	//Get the source file.
	QString sourceFile = QString("%1.xml").arg(source);
	QString sourcePath = QDir(wizardFile.getSourcesDirectory()).filePath(sourceFile);
	QFile configurationFile(sourcePath);

	//Load the source file.
	if(WizardFile::getRootElement(configurationFile, _sourceRoot))
	{
		//Run base mandatory check.
		bool isMandatory = WizardDynamicItem::isMandatory();

		//Initialize the main widget.
		_sourceSelect = new WizardSourceSelect(source, !isMandatory, parent);
		_sourceSelect->setDelegate(this);
		_sourceSelect->generateLayout(layout, _sourceRoot);

		//Store the source.
		QString prefix = _delegate->dynamicName(_prefix);
		QString primary = _sourceRoot.attribute("primary");
		QString disable = _sourceRoot.attribute("disable");
		QString naming = WizardFile::getProperty(_sourceRoot, "caption");
		parent->registerSource({ prefix, source, primary, disable, naming, parent });
	}
}

bool WizardSourceItem::isMandatory() const { return true; }
bool WizardSourceItem::isComplete() const { return !_sourceSelect  || _sourceSelect->isComplete(); }
void WizardSourceItem::resetValue() { if(_sourceSelect) _sourceSelect->initializeItems(); }

bool WizardSourceItem::isEnabled() const { return _sourceSelect && _sourceSelect->isEnabled(); }
void WizardSourceItem::setEnabled(bool isEnabled) { if(_sourceSelect) _sourceSelect->setEnabled(isEnabled); }

void WizardSourceItem::notifyCompleteChanged()
{
	//Update the parent.
	_parent->updateItems();

	//Call the base.
	WizardPrefixDelegate::notifyCompleteChanged();
}
