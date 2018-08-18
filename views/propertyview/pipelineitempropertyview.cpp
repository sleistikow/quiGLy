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

#include "pipelineitempropertyview.h"
#include "propertyviewfactory.h"
#include "../registry.h"

#include "propertyviewitems/boolpropertyviewitem.h"
#include "propertyviewitems/enumpropertyviewitem.h"
#include "propertyviewitems/filenamepropertyviewitem.h"
#include "propertyviewitems/mixerlayoutpropertyviewitem.h"
#include "propertyviewitems/numericpropertyviewitem.h"
#include "propertyviewitems/textpropertyviewitem.h"
#include "propertyviewitems/stringlistpropertyviewitem.h"
#include "propertyviewitems/defaultpropertyviewitem.h"
#include "propertyviewitems/vectorpropertyviewitem.h"
#include "propertyviewitems/matrixpropertyviewitem.h"
#include "propertyviewitems/colourpropertyviewitem.h"
#include "propertyviewitems/varyingspropertyviewitem.h"
#include "propertyviewitems/arraypropertyviewitem.h"

using namespace ysm;

void PipelineItemPropertyView::registerPropertyItems(PropertyViewFactory* factory)
{
	//Set the default property view items.
	factory->setDefaultItemConstructor<BoolPropertyViewItem>(PropertyType::Bool);
	factory->setDefaultItemConstructor<CharPropertyViewItem>(PropertyType::Char);
	factory->setDefaultItemConstructor<EnumPropertyViewItem>(PropertyType::Enumeration);
	factory->setDefaultItemConstructor<FilenamePropertyViewItem>(PropertyType::Filename);
	factory->setDefaultItemConstructor<MixerLayoutPropertyViewItem>(PropertyType::MixerLayout);
	factory->setDefaultItemConstructor<IntPropertyViewItem>(PropertyType::Int);
	factory->setDefaultItemConstructor<UIntPropertyViewItem>(PropertyType::UInt);
	factory->setDefaultItemConstructor<FloatPropertyViewItem>(PropertyType::Float);
	factory->setDefaultItemConstructor<DoublePropertyViewItem>(PropertyType::Double);
	factory->setDefaultItemConstructor<StringListPropertyViewItem>(PropertyType::StringList);
	factory->setDefaultItemConstructor<StringPropertyViewItem>(PropertyType::String);
	factory->setDefaultItemConstructor<Vec2PropertyViewItem>(PropertyType::Vec2);
	factory->setDefaultItemConstructor<Vec3PropertyViewItem>(PropertyType::Vec3);
	factory->setDefaultItemConstructor<Vec4PropertyViewItem>(PropertyType::Vec4);
	factory->setDefaultItemConstructor<Mat3x3PropertyViewItem>(PropertyType::Mat3x3);
	factory->setDefaultItemConstructor<Mat4x4PropertyViewItem>(PropertyType::Mat4x4);
	factory->setDefaultItemConstructor<ColourPropertyViewItem>(PropertyType::Color);
	factory->setDefaultItemConstructor<VaryingsPropertyViewItem>(PropertyType::Varyings);
	factory->setDefaultItemConstructor<FloatDataPropertyViewItem>(PropertyType::DataFloat);
	factory->setDefaultItemConstructor<IntDataPropertyViewItem>(PropertyType::DataInt);
	factory->setDefaultItemConstructor<UIntDataPropertyViewItem>(PropertyType::DataUInt);
}

PipelineItemPropertyView::PipelineItemPropertyView(IPipelineItem* pipelineItem, QWidget *parentWidget,
												   IView* parentView) :
	QScrollArea(parentWidget),
	ViewItem(parentView),
	_pipelineItem(pipelineItem),
	_layout(NULL)
{
	//Create common group box.
	_defaultLayout = new QFormLayout(new QGroupBox("Common"));
	_defaultLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	_defaultLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);

	//Create root child widget. QScrollArea widget must be set using setWidget().
	QWidget* internalWidget = new QWidget();
	setWidget(internalWidget);
	setWidgetResizable(true);
	setFrameStyle(QFrame::NoFrame);

	//Always hide the message log.
	setPropertyHidden(pipelineItem->getProperty<StringProperty>(PropertyID::MessageLog));

	//Register for property updates.
	notifyStatic(this, &PipelineItemPropertyView::recreatePropertyViewItems);
	notifyStatic(IChangeable::Add, this, &PipelineItemPropertyView::createPropertyItemView);
	notifyStatic(IChangeable::Remove, this, &PipelineItemPropertyView::deletePropertyItemView);
}

void PipelineItemPropertyView::createPropertyItemView(IProperty* property)
{
	//Ensure the property belongs to the correct pipeline item.
	if(property->getOwner() != _pipelineItem) return;

	//Check wether the property is visible at all.
	if(_propertySettings.contains(property) && _propertySettings[property].isHidden) return;

	//Get the property view factory.
	PropertyViewFactory* viewFactory = getActiveDocument()->getRegistry()->getViewFactory();

	//Create the property view item.
	IPropertyViewItem* propertyViewItem = viewFactory->createPropertyViewItem(property, this);
	if(!propertyViewItem)
		propertyViewItem = new DefaultPropertyViewItem(property, this);
	propertyViewItem->setPropertyView(this);

	//Ensure the edit field is available.
	if(propertyViewItem->getEdit())
	{
		//Get the property's group, or set to NULL if not grouped.
		QFormLayout* boxLayout = _defaultLayout;
		if(_propertySettings.contains(property) && !_propertySettings[property].groupName.isEmpty())
			boxLayout = _boxLayouts[_propertySettings[property].groupName];

		//Adjust the layout if neccessary.
		if(propertyViewItem->isStretching())
		{
			QSizePolicy sizePolicy = boxLayout->parentWidget()->sizePolicy();
			sizePolicy.setVerticalStretch(1);
			boxLayout->parentWidget()->setSizePolicy(sizePolicy);
		}

		//Add the property view item, either to the group box or directly to the view, if not grouped.
		boxLayout->addRow(propertyViewItem->getLabel(), propertyViewItem->getEdit());

		//Store it internally.
		_propertyViews[property] = propertyViewItem;
	}
	else
		qDebug() << "Missing edit field for	property of type:" << property->getName();
}

void PipelineItemPropertyView::deletePropertyItemView(IProperty* property)
{
	//Ensure the property belongs to the correct pipeline item.
	if(property->getOwner() != _pipelineItem) return;

	//Delete the property view.
	if(_propertyViews.contains(property))
	{
		delete _propertyViews[property];
		_propertyViews.remove(property);
	}
}

void PipelineItemPropertyView::recreatePropertyViewItems()
{
	//Remove the layout.
	delete _layout;

	//Recreate the layout, and add to root child widget.
	_layout = new QFormLayout(widget());
	_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

	//Iterate over all properties and create the views.
	foreach(IProperty* property, _pipelineItem->getProperties())
		createPropertyItemView(property);

	//Add default layout, if not empty.
	if(!_defaultLayout->isEmpty())
		_layout->addWidget(_defaultLayout->parentWidget());

	//Iterate over all layouts, and add their parents.
	foreach(QLayout* layout, _boxLayouts.values())
		if(!layout->isEmpty())
			_layout->addWidget(layout->parentWidget());
}

void PipelineItemPropertyView::setPropertyGroup(IProperty* property, QString groupName)
{
	//Simply store the existance of the group.
	_propertySettings[property].groupName = groupName;

	//If the group has not been registered, yet, create the group box.
	if(!groupName.isEmpty() && !_boxLayouts.contains(groupName))
	{
		QFormLayout* formLayout = new QFormLayout(new QGroupBox(groupName));
		formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
		formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
		_boxLayouts[groupName] = formLayout;
	}
}

void PipelineItemPropertyView::setPropertyHidden(IProperty* property, bool hidden)
{
	//Simply store the visibility.
	_propertySettings[property].isHidden = hidden;
}

IPipelineItem* PipelineItemPropertyView::getPipelineItem() const { return _pipelineItem; }
