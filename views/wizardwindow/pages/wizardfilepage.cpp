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

#include "wizardfilepage.h"
#include "../wizardwindow.h"
#include "../wizardfile.h"

#include "views/mainwindow/mainwindow.h"
#include "views/mainwindow/maindelegate.h"

#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>

using namespace ysm;

const QString WizardFilePage::FilenameField("project.file");
const QString WizardFilePage::ImportField("project.import");
const QString WizardFilePage::VersionField("project.version");
const QString WizardFilePage::NameField("project.name");

WizardFilePage::WizardFilePage(QDomElement rootElement, WizardWindow* parent) :
	WizardCustomPage(rootElement, parent, true),
	_fileSelect(NULL),
	_importOption(NULL),
	_versionSelect(NULL),
	_nameEdit(NULL)
{
	//Extract the XML data.
	_allowImport = WizardFile::getAttribute(rootElement, "allowImport", "true") == "true";
	_minimumVersion = WizardFile::getAttribute(rootElement, "minimumVersion", "330").toInt();

	//Create the page.
	QFormLayout* pageLayout = createPage(parent->isMultiPass());
	createItemCollection(pageLayout, rootElement);

	//Configure the page.
	setTitle(WizardFile::getProperty(rootElement, "title"));
	setSubTitle(WizardFile::getProperty(rootElement, "subtitle"));
}

WizardFilePage::WizardFilePage(bool allowImport, int minimumVersion, QWizard* parent) :
	WizardCustomPage(parent),
	_fileSelect(NULL),
	_importOption(NULL),
	_versionSelect(NULL),
	_nameEdit(NULL),
	_allowImport(allowImport),
	_minimumVersion(minimumVersion)
{
	//Create the page
	createPage(false);

	//Configure the page.
	setTitle(tr("Project"));
	setSubTitle(tr("Adjust the basic project settings."));
}

QFormLayout* WizardFilePage::createPage(bool isMultiPass)
{
	//Create the basic form layout.
	QFormLayout* pageLayout = new QFormLayout(this);

	//Check if import option is allowed.
	if(_allowImport && !isMultiPass)
	{
		//Create the import option.
		_importOption = new QCheckBox(tr("Import into existing project"), this);
		_importOption->setChecked(false);

		//Register the import option.
		pageLayout->addRow(NULL, _importOption);
		connect(_importOption, &QCheckBox::toggled, this, &WizardFilePage::updatePage);
		registerField(ImportField, _importOption);
	}

	//Check if file field is allowed.
	if(!isMultiPass)
	{
		//Create the file widget.
		_fileSelect = new FileSelect(this);

		//Register the file widget.
		pageLayout->addRow(WizardWindow::getLabel(tr("Project File")), _fileSelect);
		connect(_fileSelect, &FileSelect::browseFile, this, &WizardFilePage::browseFile);
		registerField(FilenameField, _fileSelect, "filename");
	}

	//Check if version field is allowed.
	if(!isMultiPass)
	{
		//Create the version select field.
		_versionSelect = new VersionSelect(_minimumVersion, this);

		//Register the version select field.
		pageLayout->addRow(WizardWindow::getLabel(tr("OpenGL Version")), _versionSelect);
		registerField(VersionField, _versionSelect, "numericVersion");
	}

	//Check if name field is allowed.
	if(isMultiPass)
	{
		//Create the name edit.
		_nameEdit = new QLineEdit(this);

		//Register the version select field.
		pageLayout->addRow(WizardWindow::getLabel(tr("Render Pass Name")), _nameEdit);
		connect(_nameEdit, &QLineEdit::textChanged, this, &WizardFilePage::updatePage);
		registerField(NameField, _nameEdit);
	}

	//Return the layout.
	return pageLayout;
}

void WizardFilePage::browseFile()
{
	//Show file dialog using the main delegate.
	MainDelegate* mainDelegate = MainWindow::getInstance()->getDelegate();
	QString projectFile = mainDelegate->showSaveAsDialog();

	//Store the file name.
	_fileSelect->setFilename(projectFile);
	emit completeChanged();
}

void WizardFilePage::updatePage()
{
	//Disable the widgets, if import option is selected.
	if(_fileSelect) _fileSelect->setEnabled(!_importOption || !_importOption->isChecked());
	if(_versionSelect) _versionSelect->setEnabled(!_importOption || !_importOption->isChecked());
	emit completeChanged();
}

bool WizardFilePage::isComplete() const
{
	//Run the base checks first.
	if(!QWizardPage::isComplete()) return false;

	//The project must be correctly specified.
	return
		(_importOption && _importOption->isChecked()) ||
		(_fileSelect && !_fileSelect->getFilename().isEmpty()) ||
		(_nameEdit && !_nameEdit->text().isEmpty());
}
