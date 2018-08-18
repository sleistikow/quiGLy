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

#include "commandview.h"
#include "../document.h"

#include "data/ipipeline.h"
#include "data/irendercommand.h"

#include "commands/pipeline/change/changerendercommandindexcommand.h"
#include "commands/iuicommandqueue.h"

using namespace ysm;

CommandView::CommandView(QWidget* parentWidget, IView* parentView) :
	QDockWidget(parentWidget),
	View(parentView)
{
	//Initialize the list widget.
	_list = new QListWidget();
	_list->setSelectionBehavior(QAbstractItemView::SelectRows);
	_list->setSelectionMode(QAbstractItemView::SingleSelection);
	_list->setDragDropMode(QAbstractItemView::InternalMove);

	//Use the list widget.
	setWidget(_list);

	//Connect to the change signals.
	connect(_list, &QListWidget::currentItemChanged, this, &CommandView::selectionChanged);
	connect(_list->model(), &QAbstractItemModel::rowsMoved, this, &CommandView::rowMoved);

	//Watch for data changes to update the items.
	notifyDynamic(this, &CommandView::updateDocument);
	notifyDynamic(IChangeable::Add, this, &CommandView::createRenderCommand);
	notifyDynamic(IChangeable::Remove, this, &CommandView::deleteRenderCommand);
}

void CommandView::createRenderCommand(IRenderCommand* renderCommand)
{
	//Create the new list widget item.
	QListWidgetItem* item = new QListWidgetItem(_list);

	//Set it up.
	item->setText(QString("%1 (%2)").arg(renderCommand->getName()).arg(renderCommand->getID()));
	item->setData(Qt::UserRole, renderCommand->getID());
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}

void CommandView::deleteRenderCommand(IRenderCommand* renderCommand)
{
	//Iterate over all items to find the one that has to be deleted.
	for(int i = _list->count() - 1; i >= 0; i--)
		if(_list->item(i)->data(Qt::UserRole).toUInt() == renderCommand->getID())
			delete _list->item(i);
}

void CommandView::updateDocument()
{
	//Clear the current list.
	_list->clear();

	//Ensure a document is available.
	if(!getActiveDocument()) return;

	//Recreate all render commands.
	QVector<IRenderCommand*> renderCommands = getActiveDocument()->getPipeline()->getRenderCommands();
	for(int i = 0; i < renderCommands.count(); i++)
		createRenderCommand(renderCommands[i]);
}

void CommandView::updateItem()
{
	//Clear the current selection.
	_list->clearSelection();

	//Ensure an item is selected, then iterate over all list items to find it.
	if(getActiveItem())
		for(int i = _list->count() - 1; i >= 0; i--)
			if(_list->item(i)->data(Qt::UserRole).toUInt() == getActiveItem()->getID())
				_list->item(i)->setSelected(true);
}

void CommandView::rowMoved(QModelIndex oldIndex, int start, int end, QModelIndex newIndex, int row)
{
	Q_UNUSED(oldIndex);
	Q_UNUSED(newIndex);
	Q_UNUSED(end);

	//Access the pipeline.
	IPipeline* pipeline = getActiveDocument()->getPipeline();

	//Row needs to be adjusted, if moved downwards, because the moved element is missing there.
	executeCommand(new ChangeRenderCommandIndexCommand(pipeline, start, start < row ? row - 1 : row));
}

void CommandView::selectionChanged()
{
	//Get the current item.
	QListWidgetItem* item = _list->currentItem();
	if(item)
	{
		//Iterate over all render commands, find the matching one.
		foreach(IRenderCommand* renderCommand, getActiveDocument()->getPipeline()->getRenderCommands())
			if(renderCommand->getID() == item->data(Qt::UserRole).toUInt())
			{
				setActiveItem(renderCommand);
				return;
			}
	}

	//If not available, clear selection.
	setActiveItem(NULL);
}
