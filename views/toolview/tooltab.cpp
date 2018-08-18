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

#include "tooltab.h"
#include "../document.h"
#include "../registry.h"
#include "../common/ysmpalette.h"

#include "commands/pipeline/add/addblockcommand.h"

//HACK: Required to retrieve block versions.
#include "data/pipeline/pipelineobjectfactory.h"
#include "data/pipeline/pipelinemanager.h"
#include "data/pipeline/pipeline.h"
#include "data/blocks/block.h"

#include <QFormLayout>
#include <QLabel>

using namespace ysm;

ToolTab::ToolTab(QWidget* parentWidget, IView* parentView) :
	QToolBox(parentWidget),
	ViewItem(parentView)
{
	//Create the signal mapper.
	_blockSignalMapper = new QSignalMapper(this);
	connect(_blockSignalMapper, SIGNAL(mapped(int)), this, SLOT(createBlock(int)));

	//Access the document's registry and register all available items.
	foreach(BlockType blockType, getActiveDocument()->getRegistry()->getBlocks())
		registerBlockType(blockType, getActiveDocument()->getRegistry()->getTool(blockType));

	//Listen to pipeline version changes.
	notifyStatic(IChangeable::Change, this, &ToolTab::updateVersion);

	//Manually trigger one version update.
	updateVersion(NULL);
}

QLayout* ToolTab::updateGroup(const QString& groupName)
{
	//Check if the group already exists.
	if(!_groupLayouts.contains(groupName))
	{
		QWidget* groupWidget = new QWidget(this);
		QLayout* groupLayout = new QFormLayout(groupWidget);

		addItem(groupWidget, groupName);
		_groupLayouts[groupName] = groupLayout;
	}

	//Return existing group.
	return _groupLayouts[groupName];
}

void ToolTab::registerBlockType(BlockType blockType, const ToolView::Tool& tool)
{
	//Get the group's layout.
	QLayout* groupLayout = updateGroup(tool.group);

	//Create a tool button with icon and a wrapping title beneath.
	QToolButton* toolButton = new QToolButton(this);
	toolButton->setFixedSize(80, 60);

	//Get the color.
	QColor blockColor = YSMPalette::getBlockColor(blockType);
	QColor disabledColor = YSMPalette::getInactiveColor();

	//Set the button's style.
	toolButton->setStyleSheet((QString("QToolButton { background: %1; border: 1px solid black; }") +
							   QString("QToolButton:hover { background: %2; border: 1px solid black; }") +
							   QString("QToolButton:disabled { background: %3; border: 1px solid black;}") +
							   QString("QToolButton:pressed { background: %4; border: 1px solid black; }"))
							  .arg(blockColor.name())
							  .arg(blockColor.light(135).name())
							  .arg(disabledColor.name())
							  .arg(blockColor.dark(135).name()));

	//Create the title label.
	QLabel* buttonLabel = new QLabel(tool.name, toolButton);
	buttonLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
	buttonLabel->setTextInteractionFlags(Qt::NoTextInteraction);
	buttonLabel->setMouseTracking(false);
	buttonLabel->setWordWrap(true);

	//Set the label's style.
	buttonLabel->setStyleSheet((QString("QLabel { padding: 5px; color: %1; }") +
								QString("QLabel:disabled { padding: 5px; color: black; }"))
								.arg(blockColor.lightness() > 128 ? "black" : "white"));

	QVBoxLayout* buttonLayout = new QVBoxLayout(toolButton);
	buttonLayout->setMargin(0);
	buttonLayout->addWidget(buttonLabel);

	//Add to the group layout.
	groupLayout->addWidget(toolButton);
	groupLayout->setAlignment(toolButton, Qt::AlignCenter | Qt::AlignTop);

	//HACK: Create the dummy block to be able to retrieve versioning info.
	//TODO: Data method to retrieve versioning info from BlockType instead of IBlock instance.
	Pipeline* activePipeline = dynamic_cast<Pipeline*>(getActiveDocument()->getPipeline());
	PipelineManager* activeManager = activePipeline->getManager();

	//Store the created button with dummy button for version checks.
	IBlock* dummyInstance = activeManager->getObjectFactory()->createBlock(blockType, activePipeline);
	_dummyData[toolButton] = dummyInstance;

	//Register the action.
	_blockSignalMapper->setMapping(toolButton, static_cast<int>(blockType));
	connect(toolButton, SIGNAL(clicked(bool)), _blockSignalMapper, SLOT(map()));
}

void ToolTab::createBlock(const int blockType)
{
	//Just execute the command to add a block.
	executeCommand(new AddBlockCommand(getActiveDocument()->getPipeline(), static_cast<BlockType>(blockType)));
}

void ToolTab::updateVersion(IPipeline* pipeline)
{
	Q_UNUSED(pipeline);

	//Iterate over all blocks and update their state.
	foreach(QToolButton* toolButton, _dummyData.keys())
		toolButton->setEnabled(_dummyData[toolButton]->isSupported());
}

void ToolTab::updateDocument() { }
void ToolTab::updateItem() { }
