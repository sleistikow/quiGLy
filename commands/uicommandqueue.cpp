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

#include "uicommandqueue.h"
#include "iuicommand.h"

#include <iostream>

using namespace ysm;


UICommandQueue::UICommandQueue(Document* parent) :
	IUICommandQueue(parent),
	_document(parent),
	_undoCounter(0),
	_currentBlock(0),
	_nextBlock(0),
	_currentBlockDepth(0)
{ }

UICommandQueue::~UICommandQueue()
{
	//Clear the stacks.
	qDeleteAll(_redoStack);
	qDeleteAll(_undoStack);
}

void UICommandQueue::emitPreExecute(IUICommand* command, bool isUndo)
{
	//Check for objects that will be removed.
	QList<IChangeable*> removedList = command->getChangedObjects(isUndo ? IChangeable::Add : IChangeable::Remove);
	if(!removedList.isEmpty())
		emit willRemoveData(_document, removedList);
}

void UICommandQueue::emitPostExecute(IUICommand* command, bool isUndo)
{
	//Check for objects that were changed.
	QList<IChangeable*> changedList = command->getChangedObjects(IChangeable::Change);
	if(!changedList.isEmpty())
		emit didChangeData(_document, changedList);

	//Check for objects that were added.
	QList<IChangeable*> addedList = command->getChangedObjects(isUndo ? IChangeable::Remove : IChangeable::Add);
	if(!addedList.isEmpty())
		emit didAddData(_document, addedList);

	//Check if rendering was changed.
	if(command->didChangeRendering())
		emit renderingChanged(_document);

	//Emit state change in any case.
	emit stateChanged();
}

void UICommandQueue::execute(IUICommand* command)
{
	//Ensure the command exists.
	if(!command) return;

	//Clear the redo stack.
	_redoStack.clear();

	//Begin command block to group implicit commands.
	beginCommandBlock();

	//Execute possible internal commands.
	command->setCommandBlock(_currentBlock);
	command->executeImplicit(this);

	//Emit pre execution signals.
	emitPreExecute(command, false);

	//Execute, then add to undo list if successful.
	bool success = command->execute();
	if(success && command->isUndoable())
	{
		_undoStack.push(command);
		_undoCounter++;
	}

	//Emit post execution signals.
	emitPostExecute(command, false);

	//End inner command block.
	endCommandBlock();

	//Delete the command, if not used anymore.
	if(!success || !command->isUndoable())
		delete command;
}


bool UICommandQueue::undo()
{
	//Ensure commands are available.
	if(_undoStack.empty()) return false;

	//Access the latest command from stack.
	IUICommand* undoCommand = _undoStack.pop();

	//Emit pre execution signals (reverse execution).
	emitPreExecute(undoCommand, true);

	//Execute, then add to redo list if successful.
	bool success = undoCommand->undo();
	if(success && undoCommand->isUndoable())
	{
		_redoStack.push(undoCommand);
		_undoCounter--;
	}

	//Emit post execution signals (reverse execution).
	emitPostExecute(undoCommand, true);

	//Ensure the command was executed correctly.
	if(!success) return false;

	//Retrieve the current command's block, and check for further commands.
	int currentBlock = undoCommand->getCommandBlock();
	IUICommand* nextCommand = _undoStack.empty() ? NULL : _undoStack.last();
	int nextBlock = nextCommand ? nextCommand->getCommandBlock() : 0;

	//Iteratively execute a possible block of commands.
	if(currentBlock && nextBlock == currentBlock)
		return undo();

	//Successfully executed.
	return true;
}

bool UICommandQueue::redo()
{
	//Ensure commands are available.
	if(_redoStack.empty()) return false;

	//Access the latest command from stack.
	IUICommand* redoCommand = _redoStack.pop();

	//Emit pre execution signals.
	emitPreExecute(redoCommand, false);

	//Execute, then add to undo list if successful.
	bool success = redoCommand->execute();
	if(success && redoCommand->isUndoable())
	{
		_undoStack.push(redoCommand);
		_undoCounter++;
	}

	//Emit post execution signals.
	emitPostExecute(redoCommand, false);

	//Retrieve the current command's block, and check for further commands.
	int currentBlock = redoCommand->getCommandBlock();
	IUICommand* nextCommand = _redoStack.empty() ? NULL : _redoStack.last();
	int nextBlock = nextCommand ? nextCommand->getCommandBlock() : 0;

	//Iteratively execute a possible block of commands.
	if(currentBlock && nextBlock == currentBlock)
		return redo();

	//Successfully executed.
	return success;
}

void UICommandQueue::clear()
{
	//Clear stacks.
	qDeleteAll(_redoStack);
	qDeleteAll(_undoStack);

	_redoStack.clear();
	_undoStack.clear();

	//Mark as saved.
	save();
}

void UICommandQueue::save()
{
	//Mark as saved.
	_undoCounter = 0;

	//Emit state change.
	emit stateChanged();
}

bool UICommandQueue::isSaved() const { return _undoCounter == 0; }

void UICommandQueue::endCommandBlock()
{
	//Decrease the command depth.
	_currentBlockDepth--;

	//Clear the current command block, if neccessary.
	if(!_currentBlockDepth)
		_currentBlock = 0;
}

void UICommandQueue::beginCommandBlock()
{
	//Create new command block, if neccessary.
	if(!_currentBlockDepth)
		_currentBlock = ++_nextBlock;

	//Increase the command depth.
	_currentBlockDepth++;
}
