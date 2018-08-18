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

#include "uidatachangingcommand.h"

#include "data/ipipeline.h"
#include "data/iproperty.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"
#include "data/properties/property.h"

using namespace ysm;


UIDataChangingCommand::PropertyBackup::PropertyBackup(IProperty* property) :
	_target(property),
	_backup(NULL)
{
	//For read-only properties, there is no backup value available.
	if(!_target->isReadOnly())
	{
		//Ensure the backup value is available.
		ITraceableValue* unchangedValue = _target->getUnchangedValue();
		if(unchangedValue)
			_backup = unchangedValue->cloneValue();
	}
}

UIDataChangingCommand::PropertyBackup::~PropertyBackup() { delete _backup; }

void UIDataChangingCommand::PropertyBackup::restoreProperty() const
{
	//Ensure backup is available.
	if(_backup)
	{
		//Restore the backup, then clear the change that was caused by restoring. If a new change occurs, it's based on
		//the value that was available AFTER the undo, not before the undo.
		_target->fromTraceableValue(_backup);
		_target->clearChanged();
	}
}

UIDataChangingCommand::UIDataChangingCommand() :
	_backupsDone(false)
{ }

UIDataChangingCommand::~UIDataChangingCommand() { qDeleteAll(_propertyBackups); }

bool UIDataChangingCommand::isUndoable() const { return true; }
bool UIDataChangingCommand::didChangeRendering() const { return true; }

QList<IChangeable*> UIDataChangingCommand::getChangedObjects(IChangeable::Operation operation)
{ return _changedObjects[operation]; }

void UIDataChangingCommand::resetChangedObjects(IChangeable::Operation operation)
{ _changedObjects[operation].clear(); }

void UIDataChangingCommand::addChangedObject(IChangeable *object, IChangeable::Operation operation)
{ _changedObjects[operation].append(object); }

void UIDataChangingCommand::backupProperty(IProperty* property)
{
	//Store the new backup and add to changed objects.
	if(!_backupsDone)
	{
		_propertyBackups.append(new PropertyBackup(property));
		addChangedObject(property, IChangeable::Change);
	}
}

void UIDataChangingCommand::backupProperties(IPipelineItem* pipelineItem)
{
	//Iterate over all properties and backup all changes.
	foreach(IProperty* property, pipelineItem->getProperties())
		if(property->hasChanged())
			backupProperty(property);
}

void UIDataChangingCommand::backupProperties(IPipeline* pipeline)
{
	//Find block and connection changes.
	foreach(IBlock* block, pipeline->getBlocks())
	{
		//Backup block changes.
		backupProperties(block);

		//Backup connection changes.
		foreach(IConnection* connection, block->getOutConnections())
			backupProperties(connection);
	}

	//Find command changes.
	foreach(IRenderCommand* command, pipeline->getRenderCommands())
		backupProperties(command);
}

void UIDataChangingCommand::clearProperties(IPipelineItem *pipelineItem)
{
	//Iterate over all properties and clear the change flag.
	foreach(IProperty* property, pipelineItem->getProperties())
		if(property->hasChanged())
			property->clearChanged();
}

void UIDataChangingCommand::clearProperties(IPipeline* pipeline)
{
	//Clear block and connection changes.
	foreach(IBlock* block, pipeline->getBlocks())
	{
		//Backup block changes.
		clearProperties(block);

		//Clear connection changes.
		foreach(IConnection* connection, block->getOutConnections())
			clearProperties(connection);
	}

	//Clear command changes.
	foreach(IRenderCommand* command, pipeline->getRenderCommands())
		clearProperties(command);
}

void UIDataChangingCommand::propertyBackupsDone() { _backupsDone = true; }

void UIDataChangingCommand::restoreProperties()
{
	//Iterate over all properties and restore their value.
	foreach(PropertyBackup* propertyBackup, _propertyBackups)
		propertyBackup->restoreProperty();
}

