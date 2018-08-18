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

#include "wizardcollectionstack.h"
#include "../../wizardwindow.h"

#include <QDir>

using namespace ysm;

const QString WizardCollectionStack::TypeField("type");

WizardCollectionStack::WizardCollectionStack(WizardWindow* parent) :
	QObject(parent),
	_typeSelect(NULL),
	_window(parent)
{
	//Set default caption.
	_typeCaption = tr("Type");
}

WizardCollectionStack::~WizardCollectionStack() { }

void WizardCollectionStack::generateLayout(QFormLayout* layout, QDomElement rootElement)
{
	//Check wether multiple types are available.
	if(rootElement.elementsByTagName("item").count() > 1)
	{
		//Show the type combo box if type can be selected.
		_typeSelect = new QComboBox(_window);
		layout->addRow(WizardWindow::getLabel(_typeCaption), _typeSelect);
//		layout->addItem(new QSpacerItem(0, 5));

		//Register to combo box changes.
		connect(_typeSelect, &QComboBox::currentTextChanged, this, &WizardCollectionStack::updateCurrentType);
		_delegate->registerDynamicField(TypeField, _typeSelect, "currentData");
	}

	//Create the stacked widget.
	_stackedWidget = new QStackedWidget(_window);
	_stackedWidget->setMinimumWidth(400);
	layout->addRow(_stackedWidget);

	//Iterate over all available item types.
	for(QDomElement element = rootElement.firstChildElement("item"); !element.isNull();
		element = element.nextSiblingElement("item"))
	{
		//Register the corresponding item type.
		registerItemCollection(element);
	}

	//Adjust the current size.
	updateCurrentType();
}

void WizardCollectionStack::setCurrentType(const QString& type)
{
	//Ensure type select is available.
	if(_typeSelect)
	{
		//Set the current item.
		int itemIndex = _typeSelect->findData(type);
		_typeSelect->setCurrentIndex(itemIndex);
	}
}

WizardCollectionStack::ItemType WizardCollectionStack::getCurrentType() const
{
	//Return first type, if no select available.
	if(!_typeSelect) return _itemTypes.first();

	//Access the current type name and the type.
	QString typeName = _typeSelect->currentData().toString();
	return _itemTypes[typeName];
}

void WizardCollectionStack::updateCurrentType()
{
	//Get the active item type.
	ItemType itemType = getCurrentType();

	//Ensure widget is available.
	if(!itemType._collectionWidget) return;

	//Activate the correct widget.
	//TODO: Maybe ensure that the widget exists.
	QWidget* activeWidget = itemType._collectionWidget;
	_stackedWidget->setCurrentWidget(activeWidget);
	itemType._itemCollection->initializeItems();

	//Reset the size policy of all widget, so just the active widget is handled.
	for(int i = 0; i < _stackedWidget->count(); i++)
	{
		//Ignore all inactive widget sizes.
		QWidget* currentWidget = _stackedWidget->widget(i);
		QSizePolicy::Policy sizePolicy = currentWidget == activeWidget ? QSizePolicy::Expanding : QSizePolicy::Ignored;
		currentWidget->setSizePolicy(QSizePolicy::Expanding, sizePolicy);
	}

	//Adjust the size to match the active widget.
	_stackedWidget->adjustSize();

	//Signal that the type changed.
	emit currentTypeChanged();
}

QWidget* WizardCollectionStack::registerItemCollection(QDomElement typeElement)
{
	//Retrieve the relevant data before include.
	QString typeName = typeElement.attribute("type");
	QString nameSource = typeElement.attribute("naming");

	//Check if an include file is specified.
	if(typeElement.hasAttribute("include"))
	{
		//Retrieve a path to the file.
		QString includeName = typeElement.attribute("include");

		//Get the source file.
		QString includePath = QDir(_window->getWizard().getIncludeDirectory()).filePath(includeName);
		QFile includeFile(includePath);

		//Replace the type element with the referenced file.
		if(!WizardFile::getRootElement(includeFile, typeElement)) return NULL;
	}

	//Retrieve the relevant data after include.
	QString typeCaption = WizardFile::getProperty(typeElement, "caption");

	//Create the collection widget.
	QWidget* collectionWidget = new QWidget(_window);
	QFormLayout* collectionLayout = new QFormLayout(collectionWidget);
	collectionLayout->setMargin(0);

	//Create the new item collection.
	WizardItemCollection* itemCollection = new WizardItemCollection(_window);
	itemCollection->setDelegate(_delegate);
	itemCollection->generateLayout(collectionLayout, typeElement);

	//Store the widget.
	if(_typeSelect) _typeSelect->addItem(typeCaption, typeName);
	_itemTypes[typeName] = { itemCollection, collectionWidget, typeName, nameSource };
	_stackedWidget->addWidget(collectionWidget);

	//Return the widget.
	return collectionWidget;
}

void WizardCollectionStack::initializeItems() { getCurrentType()._itemCollection->initializeItems(); }
bool WizardCollectionStack::isComplete() const { return getCurrentType()._itemCollection->isComplete(); }

IWizardDynamicDelegate* WizardCollectionStack::getDelegate() const { return _delegate; }
void WizardCollectionStack::setDelegate(IWizardDynamicDelegate* delegate) { _delegate = delegate; }

QString WizardCollectionStack::getCaption() const { return _typeCaption; }
void WizardCollectionStack::setCaption(const QString& caption) { _typeCaption = caption; }

bool WizardCollectionStack::isEnabled() const { return getCurrentType()._itemCollection->isEnabled(); }
void WizardCollectionStack::setEnabled(bool isEnabled)
{
	//Disable combo box and current stack.
	if(_typeSelect) _typeSelect->setEnabled(isEnabled);
	getCurrentType()._itemCollection->setEnabled(isEnabled);
}

