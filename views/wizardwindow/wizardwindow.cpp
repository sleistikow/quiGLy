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

#include "wizardwindow.h"
#include "pages/wizardfilepage.h"
#include "pages/collections/wizardoutputselect.h"

#include "views/document.h"
#include "views/mainwindow/documentmanager.h"
#include "views/mainwindow/mainwindow.h"

#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QTemporaryDir>
#include <QDir>

using namespace ysm;

QVariant WizardWindow::Source::getField(const QString& field) const
{
	//Retrieve the source's field.
	QString prefixedField = QString("%1.%2").arg(_name).arg(field);
	return _parent->field(prefixedField);
}

QString WizardWindow::Source::getUserName() const
{
	//Build expression to evaluate naming.
	QRegularExpression expression("\\[:(?<field>[a-zA-Z\\.]*)\\]");
	QRegularExpressionMatch match = expression.match(_naming);

	//Evaluate the naming.
	QString name = _naming;
	while(match.hasMatch())
	{
		//Retrieve the current field value.
		QString field = getField(match.captured("field")).toString();
		field = QFileInfo(field).fileName();

		//Replace current field and evaluate next.
		name.replace(match.capturedStart(), match.capturedLength(), field);
		match = expression.match(name);
	}

	//Prefix with parent name, if available.
	QString parent = _parent->getUserName();
	return parent.isEmpty() ? name : QString("%1: %2").arg(parent).arg(name);
}

QString WizardWindow::Source::getPrimary() const { return getField(_primary).toString(); }
QString WizardWindow::Source::getID() const { return QString("#%1.%2").arg(_parent->getID()).arg(_name); }
bool WizardWindow::Source::isReference() const { return getPrimary().startsWith("#"); }
bool WizardWindow::Source::isEnabled() const { return getPrimary() != _disable; }

QVariant WizardWindow::Output::getField(const QString& field) const
{
	//Retrieve the output's field.
	QString prefixedField = QString("%1.%2").arg(getTypeName()).arg(field);
	return _parent->field(prefixedField);
}

QString WizardWindow::Output::getUserName() const
{
	//Prefix with parent name, if available.
	QString parent = _parent->getUserName();
	return parent.isEmpty() ? _naming : QString("%1: %2").arg(parent).arg(_naming);
}

QString WizardWindow::Output::getTypeName() const { return QString("%1.%2").arg(_name).arg(_type); }
QString WizardWindow::Output::getID() const { return QString("#%1.%2").arg(_parent->getID()).arg(getTypeName()); }
bool WizardWindow::Output::isEnabled() const { return getField(WizardOutputSelect::EnabledField).toBool(); }

int WizardWindow::_nextID = 0;

WizardWindow::WizardWindow(const WizardFile& wizardFile, bool singlePass, QWidget* parent) :
	QWizard(parent),
	_identifier(++_nextID),
	_wizardFile(wizardFile),
	_multiPass(!singlePass)
{
	//Initialize the wizard.
	setWindowTitle(_wizardFile.getProperty(WizardFile::Name));
	setWizardStyle(ModernStyle);

	//Generate the pages.
	createPages();
}

WizardFile WizardWindow::getWizard() const { return _wizardFile; }

bool WizardWindow::getImportOption() const { return field(WizardFilePage::ImportField).toBool(); }
QString WizardWindow::getTargetFile() const { return field(WizardFilePage::FilenameField).toString(); }
QString WizardWindow::getUserName() const { return field(WizardFilePage::NameField).toString(); }

bool WizardWindow::isMultiPass() const { return _multiPass; }
int WizardWindow::getID() const { return _identifier; }

void WizardWindow::createPages()
{
	//Iterate over all pages of the wizard and create a matching widget.
	QList<WizardFile::Page> pageInfoList = _wizardFile.getPages();
	foreach(WizardFile::Page pageInfo, pageInfoList)
		addPage(_pageFactory.createPage(pageInfo, this));
}

void WizardWindow::createProject(MainWindow* mainWindow)
{
	//Generate temporary project directory that is available on stack.
	QTemporaryDir temporaryDirectory(QDir::temp().absoluteFilePath("YSMProject"));
	QString temporaryFile = QDir(temporaryDirectory.path()).filePath("importProject.ysm");

	//If no target file set, create temporary directory.
	QString projectFile = getTargetFile();
	if(projectFile.isEmpty())
		projectFile = temporaryFile;

	//Generate the single project.
	WizardParser wizardParser;
	wizardParser.appendWizard(this);

	//Load the project file.
	if(wizardParser.finishProject(projectFile))
	{
		//Access the main window delegates.
		Document* activeDocument = mainWindow->getActiveDocument();
		DocumentManager* manager = mainWindow->getDocumentManager();

		//Load the project file according to specified options.
		if(getImportOption())
		{
			//Ensure there is a document available.
			if(!activeDocument) activeDocument = manager->createDocument();
			manager->importDocument(activeDocument, projectFile);
		}
		else manager->loadDocument(projectFile);
	}
}

QLabel* WizardWindow::getLabel(const QString& caption)
{
	//Configure the default label.
	QLabel* defaultLabel = new QLabel(caption);
	defaultLabel->setMinimumWidth(100);

	//Return the label.
	return defaultLabel;
}

QWidget* WizardWindow::getDivider()
{
	//Configure the divider widget.
	QFrame* dividerFrame = new QFrame();
	dividerFrame->setFrameShape(QFrame::HLine);
	dividerFrame->setFrameShadow(QFrame::Sunken);

	//Return the divider.
	return dividerFrame;
}

QList<WizardWindow::Output> WizardWindow::getOutputs(const QString& output) const { return _outputs[output].values(); }
QList<QString> WizardWindow::getOutputs() const { return _outputs.keys(); }

WizardWindow::Source WizardWindow::getSource(const QString& source) const { return _sources[source]; }
QList<WizardWindow::Source> WizardWindow::getSources() const { return _sources.values(); }

QList<WizardWindow::Reference> WizardWindow::getReferences(const QString& type) const { return _references[type]; }

void WizardWindow::registerOutput(const Output& output)
{
	//Log the output registration.
	_outputs[output._name][output._type] = output;
	qDebug() << "Registered output " << output._name;
}

void WizardWindow::registerSource(const Source& source)
{
	//Log the source registration.
	_sources[source._name] = source;
	qDebug() << "Registered source " << source._name;
}

void WizardWindow::registerReference(const Source& source)
{
	//Register a reference to the source.
	_references[source._type].append({ source.getUserName(), source.getID(), false });
}

void WizardWindow::registerReference(const Output& output)
{
	//Register a reference to the output.
	_references[output._type].append({ output.getUserName(), output.getID(), true });
}

int WizardWindow::getItemCount(const QString& name) { return _items[name].count(); }
void WizardWindow::removeItem(const QString& name, const QString& item) { _items[name].removeOne(item); }

QString WizardWindow::appendItem(const QString& name)
{
	//Check if item has been registered.
	if(!_itemIDs.contains(name)) _itemIDs[name] = 0;

	//Store the new item's identifier.
	QString item = QString("%1[%2]").arg(name).arg(_itemIDs[name]++);
	_items[name].append(item);

	//Return the item's identifier.
	return item;
}

bool WizardWindow::nextItem(QString& name, QString& item, int& index) const
{
	//Ensure index is valid.
	if(index >= _items[name].count()) return false;

	//Return the iteration data.
	item = _items[name][index++];
	return true;
}

