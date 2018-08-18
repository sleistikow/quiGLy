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

#include "visualnameditem.h"

#include "data/iproperty.h"
#include "data/properties/property.h"
#include "commands/pipeline/change/renamepipelineitemcommand.h"

using namespace ysm;

VisualNamedItem::VisualNamedItem(IView* parentView) :
	VisualLayoutedItem(parentView),
	_isEditingName(false)
{
	//Add invisible rename text field.
	_nameEdit = new QLineEdit();
	_nameEdit->setStyleSheet("QLineEdit { background: transparent; border: none; }");
	_nameEdit->setAlignment(Qt::AlignCenter);
	_nameEdit->setContextMenuPolicy(Qt::NoContextMenu);
	_nameEdit->setVisible(false);

	//Create proxy widget, which is required to display the text edit correctly.
	_textEditWidget = new QGraphicsProxyWidget(this);
	_textEditWidget->setWidget(_nameEdit);

	//Create context menu actions.
	_renameAction = new QAction(QIcon(":/tango/16x16/stock/form/stock_form-text-box.png"), tr("Rename"), this);
	connect(_renameAction, SIGNAL(triggered(bool)), this, SLOT(beginRenameItem()));
}

float VisualNamedItem::getHeaderHeight() const
{
	//Calculate the rectangle required for the name and take it's height. Add 10px as text border.
	QFontMetrics defaultMetrics = QFontMetrics(QFont());
	return defaultMetrics
			.boundingRect(0, 0, getInnerWidth(), 40, Qt::AlignCenter | Qt::TextWordWrap, getPipelineItem()->getName())
			.height() + 10;
}

float VisualNamedItem::getMessageHeight() const
{
	//Ensure block has a status message.
	QString statusMessage = getPipelineItem()->getProperty<StringProperty>(PropertyID::MessageLog)->getValue();
	if(statusMessage.isEmpty())
		return 0;

	//Use smaller font.
	QFont font;
	font.setPixelSize(10);

	//Calculate the rectangle required for the name and take it's height. Add 10px as text border.
	QFontMetrics fontMetrics = QFontMetrics(font);
	return fontMetrics
			.boundingRect(0, 0, getInnerWidth(), 40, Qt::AlignCenter | Qt::TextWordWrap, statusMessage)
			.height() + 10;
}

QRectF VisualNamedItem::calculateBoundingRect() const
{
	//Calculate total block height.
	int blockHeight = getContentHeight() + getHeaderHeight() + getMessageHeight();
	int blockOffset = -getContentHeight() / 2 - getHeaderHeight();

	//Add 2.5px margins, because outline draws overlapping.
	return QRectF(QPointF(-getFullWidth() / 2, blockOffset), QSizeF(getFullWidth(), blockHeight))
			.marginsAdded(QMarginsF(2.5, 2.5, 2.5, 2.5));
}

QList<QAction*> VisualNamedItem::getContextActions() const
{
	//Get parent actions.
	QList<QAction*> baseList = VisualPipelineItem::getContextActions();

	//Add rename action.
	baseList.prepend(_renameAction);
	return baseList;
}

void VisualNamedItem::beginRenameItem()
{
	//Connect to finished signal.
	connect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(endRenameItem()));

	//Calculate the header's location.
	QRectF headerRect = boundingRect().marginsRemoved(QMarginsF(2.5, 2.5, 2.5, 2.5));
	headerRect.setHeight(getHeaderHeight());

	//Adjust the text edit's location, which might change if the header height changes.
	_nameEdit->setMaximumWidth(getInnerWidth());
	_textEditWidget->setPos(-_textEditWidget->size().width() / 2,
							headerRect.center().y() -_textEditWidget->size().height() / 2);

	//Show the text edit. When it looses it's focus, it will automatically trigger the rename action.
	_textEditWidget->setVisible(_isEditingName = true);
	_nameEdit->setText(getPipelineItem()->getName());
	_nameEdit->selectAll();
	_nameEdit->setFocus();

	//Redraw to hide the name.
	update();
}

void VisualNamedItem::endRenameItem()
{
	//Disconnect from finished signal.
	disconnect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(endRenameItem()));

	//Release the text edit prior to recreation of the scene, this seems to crash.
	QString name = _nameEdit->text();
	_textEditWidget->setVisible(_isEditingName = false);

	//Execute the rename command, then update the item to force redraw.
	executeCommand(new RenamePipelineItemCommand(getPipelineItem(), name));
	update();
}

void VisualNamedItem::cancelRenameItem()
{
	//Disconnect from finished signal.
	disconnect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(endRenameItem()));

	//Execute the rename command, then update the item to force redraw.
	_textEditWidget->setVisible(_isEditingName = false);
	update();
}

void VisualNamedItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	//Calculate the header's location.
	QRectF headerRect = boundingRect().marginsRemoved(QMarginsF(2.5, 2.5, 2.5, 2.5));
	headerRect.setHeight(getHeaderHeight());

	//Check if double clicked in header, start rename.
	if(headerRect.contains(mapFromScene(event->scenePos())))
		beginRenameItem();
}

void VisualNamedItem::keyReleaseEvent(QKeyEvent* event)
{
	//ESC cancels renaming.
	if(event->key() == Qt::Key_Escape)
	{
		cancelRenameItem();
		event->accept();
	}

	//Forward key release.
	VisualLayoutedItem::keyReleaseEvent(event);
}

bool VisualNamedItem::isVisualProperty(PropertyID propertyID)
{
	//Message log property change requires redrawing.
	return VisualLayoutedItem::isVisualProperty(propertyID) || propertyID == PropertyID::MessageLog;
}

bool VisualNamedItem::isEditingName() const { return _isEditingName; }
