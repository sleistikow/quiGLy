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

#include "propertyviewfactory.h"
#include "pipelineitempropertyview.h"

using namespace ysm;

PropertyViewFactory::PropertyViewFactory(QObject *parent) :
	QObject(parent)
{

}

PipelineItemPropertyView* PropertyViewFactory::createBlockPropertyView(IBlock* pipelineItem,
																	   IView* parentView) const
{
	//Make sure the block is specified.
	PipelineItemPropertyView* propertyView = NULL;
	if(pipelineItem)
	{
		//Check if there is a constructor available, and create the property view.
		BlockType blockType = pipelineItem->getType();
		if(_blockConstructors.contains(blockType))
			propertyView = _blockConstructors[blockType](pipelineItem, parentView);

		//Use undefined block constructor as fallback.
		else if(_blockConstructors.contains(BlockType::Undefined))
			propertyView = _blockConstructors[BlockType::Undefined](pipelineItem, parentView);
	}

	//Initialize the property view, if available.
	if(propertyView)
		propertyView->recreatePropertyViewItems();

	return propertyView;
}

PipelineItemPropertyView* PropertyViewFactory::createRenderCommandPropertyView(IRenderCommand* pipelineItem,
																			   IView* parentView) const
{
	//Make sure the command is specified.
	PipelineItemPropertyView* propertyView = NULL;
	if(pipelineItem)
	{
		//Check if there is a constructor available, and create the property view.
		RenderCommandType renderCommandType = pipelineItem->getCommand();
		if(_renderCommandConstructors.contains(renderCommandType))
			propertyView = _renderCommandConstructors[renderCommandType](pipelineItem, parentView);
	}

	//Initialize the property view, if available.
	if(propertyView)
		propertyView->recreatePropertyViewItems();

	return propertyView;
}

PipelineItemPropertyView* PropertyViewFactory::createConnectionPropertyView(IConnection* pipelineItem,
																			IView* parentView) const
{
	//Make sure the connection is specified.
	PipelineItemPropertyView* propertyView = NULL;
	if(pipelineItem)
	{
		//Check if there is a constructor available, and create the property view.
		if(_connectionConstructor)
			propertyView = _connectionConstructor(pipelineItem, parentView);
	}

	//Initialize the property view, if available.
	if(propertyView)
		propertyView->recreatePropertyViewItems();

	return propertyView;
}

IPropertyViewItem* PropertyViewFactory::createPropertyViewItem(IProperty* property,
															   IView* parentView) const
{
	//Make sure the property is specified.
	if(property)
	{
		//Get the property type, and try to cast the owner.
		PropertyType propertyType = property->getType();
		PropertyID propertyID = property->getID();

		//Check for a specialized block constructor.
		IBlock* blockOwner = dynamic_cast<IBlock*>(property->getOwner());
		if(blockOwner)
		{
			BlockType blockType = blockOwner->getType();

			//Block type and property ID.
			if(_specialBlockItemConstructors.contains(blockType) &&
					_specialBlockItemConstructors[blockType].contains(propertyID))
				return _specialBlockItemConstructors[blockType][propertyID](property, parentView);

			//Block type and property type.
			if(_blockItemConstructors.contains(blockType) &&
					_blockItemConstructors[blockType].contains(propertyType))
				return _blockItemConstructors[blockType][propertyType](property, parentView);
		}

		//Check for a specialized render command constructor.
		IRenderCommand* renderCommandOwner = dynamic_cast<IRenderCommand*>(property->getOwner());
		if(renderCommandOwner)
		{
			RenderCommandType renderCommandType = renderCommandOwner->getCommand();

			//Command type and property ID.
			if(_specialRenderCommandItemConstructors.contains(renderCommandType) &&
					_specialRenderCommandItemConstructors[renderCommandType].contains(propertyID))
				return _specialRenderCommandItemConstructors[renderCommandType][propertyID](property, parentView);

			//Command type and property type.
			if(_renderCommandItemConstructors.contains(renderCommandType) &&
					_renderCommandItemConstructors[renderCommandType].contains(propertyType))
				return _renderCommandItemConstructors[renderCommandType][propertyType](property, parentView);
		}

		//Check for default constructor by ID.
		if(_specialDefaultItemConstructors.contains(propertyID))
			return _specialDefaultItemConstructors[propertyID](property, parentView);

		//Check for default constructor by type.
		if(_defaultItemConstructors.contains(propertyType))
			return _defaultItemConstructors[propertyType](property, parentView);
	}

	return NULL;
}

void PropertyViewFactory::setBlockConstructor(const BlockType itemType,
											  PipelineItemPropertyView* (* constructor)(IBlock*, IView*),
											  void (*registrator)(PropertyViewFactory*))
{
	//Simply store the constructor.
	if(constructor)
		_blockConstructors[itemType] = constructor;
	else if(_blockConstructors.contains(itemType))
		_blockConstructors.remove(itemType);

	//Execute registrator, if set.
	if(registrator)
		registrator(this);
}

void PropertyViewFactory::setRenderCommandConstructor(const RenderCommandType itemType,
													  PipelineItemPropertyView* (*constructor)(IRenderCommand*, IView*),
													  void (*registrator)(PropertyViewFactory*))
{
	//Simply store the constructor.
	if(constructor)
		_renderCommandConstructors[itemType] = constructor;
	else if(_renderCommandConstructors.contains(itemType))
		_renderCommandConstructors.remove(itemType);

	//Execute registrator, if set.
	if(registrator)
		registrator(this);
}

void PropertyViewFactory::setConnectionConstructor(PipelineItemPropertyView* (*constructor)(IConnection*, IView*))
{
	//Simply store the constructor.
	_connectionConstructor = constructor;
}

void PropertyViewFactory::setItemConstructor(const PropertyType propertyType,
											 IPropertyViewItem* (*constructor)(IProperty*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_defaultItemConstructors[propertyType] = constructor;
	else if(_defaultItemConstructors.contains(propertyType))
		_defaultItemConstructors.remove(propertyType);
}

void PropertyViewFactory::setItemConstructor(const PropertyID propertyId,
											 IPropertyViewItem* (*constructor)(IProperty*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_specialDefaultItemConstructors[propertyId] = constructor;
	else if(_specialDefaultItemConstructors.contains(propertyId))
		_specialDefaultItemConstructors.remove(propertyId);
}

void PropertyViewFactory::setBlockItemConstructor(const BlockType itemType,
												  const PropertyType propertyType,
												  IPropertyViewItem* (*constructor)(IProperty *, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_blockItemConstructors[itemType][propertyType] = constructor;
	else if(_blockItemConstructors.contains(itemType) &&
			_blockItemConstructors[itemType].contains(propertyType))
		_blockItemConstructors[itemType].remove(propertyType);
}

void PropertyViewFactory::setRenderCommandItemConstructor(const RenderCommandType itemType,
														  const PropertyType propertyType,
														 IPropertyViewItem* (*constructor)(IProperty*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_renderCommandItemConstructors[itemType][propertyType] = constructor;
	else if(_renderCommandItemConstructors.contains(itemType) &&
			_renderCommandItemConstructors[itemType].contains(propertyType))
		_renderCommandItemConstructors[itemType].remove(propertyType);
}

void PropertyViewFactory::setBlockItemConstructor(const BlockType itemType,
												  const PropertyID propertyID,
												  IPropertyViewItem* (*constructor)(IProperty *, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_specialBlockItemConstructors[itemType][propertyID] = constructor;
	else if(_specialBlockItemConstructors.contains(itemType) &&
			_specialBlockItemConstructors[itemType].contains(propertyID))
		_specialBlockItemConstructors[itemType].remove(propertyID);
}

void PropertyViewFactory::setRenderCommandItemConstructor(const RenderCommandType itemType,
														  const PropertyID propertyID,
														  IPropertyViewItem* (*constructor)(IProperty*, IView*))
{
	//Simply store the constructor.
	if(constructor)
		_specialRenderCommandItemConstructors[itemType][propertyID] = constructor;
	else if(_specialRenderCommandItemConstructors.contains(itemType) &&
			_specialRenderCommandItemConstructors[itemType].contains(propertyID))
		_specialRenderCommandItemConstructors[itemType].remove(propertyID);
}

