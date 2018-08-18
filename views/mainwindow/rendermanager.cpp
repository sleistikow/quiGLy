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

#include "rendermanager.h"
#include "mainwindow.h"

#include "commands/iuicommandqueue.h"

#include "../document.h"
#include "commands/pipeline/change/validatepipelinecommand.h"
#include "commands/document/changeopenglversioncommand.h"

#include "data/properties/property.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

using namespace ysm;

RenderManager::RenderManager(MainWindow* parentWindow) :
	QObject(parentWindow),
	_parentWindow(parentWindow)
{
	// All supported OpenGL versions are listed here. (Minimum version OGL 3.3).
	_supportedVersions.append({ 3, 3 });
	_supportedVersions.append({ 4, 0 });
	_supportedVersions.append({ 4, 1 });
	_supportedVersions.append({ 4, 2 });
	_supportedVersions.append({ 4, 3 });
	_supportedVersions.append({ 4, 4 });
	_supportedVersions.append({ 4, 5 });
}

QList<RenderManager::OpenGLVersion> RenderManager::getSupportedVersions() { return _supportedVersions; }

void RenderManager::toggleRendering() { setRendering(!_parentWindow->getActiveDocument()->isRendering()); }
void RenderManager::setRendering(bool isRendering)
{
	//Retrieve the active document data.
	Document* activeDocument = _parentWindow->getActiveDocument();

	//Set the new rendering state.
	//Render manager will then react to rendering changed event, to be able to respond if rendering is changed from
	//somewhere else, where only the document (but not the render manager) might be accessible.
	if(activeDocument->isRendering() != isRendering)
		activeDocument->setRendering(isRendering);
}

QList<IBlock*> RenderManager::getDisplayBlocks(Document* activeDocument) const
{
	//Ensure the pipeline exists.
	QList<IBlock*> renderBlocks;
	IPipeline* pipeline = activeDocument->getPipeline();
	if(pipeline)
	{
		//Reset the pipeline status.
		activeDocument->getCommandQueue()->execute(new ValidatePipelineCommand(pipeline, true));

		//Iterate over all blocks in the pipeline.
		foreach(IBlock* block, pipeline->getBlocks())
			if(block->getType() == BlockType::Display)
				if(block->getProperty<BoolProperty>(PropertyID::Display_Visible)->getValue())
					renderBlocks.append(block);
	}

	//Return the blocks.
	return renderBlocks;
}

RenderManager::RenderDockWidget RenderManager::recreateRenderWidget(IBlock* renderBlock, QDockWidget* tabifyWidget)
{
	//Check if an active widget is available.
	QDockWidget* renderDockWidget = NULL;
	if(_renderDockWidgets.contains(renderBlock))
		renderDockWidget = _renderDockWidgets[renderBlock]._dockWidget;

	//Create a new dock widget, if none existed before.
	if(!renderDockWidget)
	{
		//Create the widget.
		renderDockWidget = new QDockWidget(_parentWindow);
		renderDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
		renderDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		//Place the widget.
		_parentWindow->addDockWidget(Qt::RightDockWidgetArea, renderDockWidget);
		if(tabifyWidget)
			_parentWindow->tabifyDockWidget(tabifyWidget, renderDockWidget);
	}

	//Recreate the GL render view.
	Document* activeDocument = _parentWindow->getActiveDocument();
	GLRenderView* renderView = new GLRenderView(activeDocument->getGLController(), renderBlock);

	//Initialize the render view.
	renderView->setActiveDocument(activeDocument);
	renderView->setMinimumSize(250, 250);

	//Restore the backup data.
	QVector3D cameraBackup = _renderDockWidgets[renderBlock]._cameraBackup;
	renderView->setCamera(cameraBackup);

	//Update the render view as content.
	renderDockWidget->setWidget(renderView);

	//Store the new data and return it.
	_renderDockWidgets[renderBlock] = { renderDockWidget, renderView, cameraBackup };
	return _renderDockWidgets[renderBlock];
}

void RenderManager::recreateRenderWidgets()
{
	//Recreate the document's render views.
	Document* activeDocument = _parentWindow->getActiveDocument();
	if(activeDocument && activeDocument->isRendering())
	{
		//Create or update the dock widget for every block.
		QDockWidget* tabifyWidget = NULL;
		QList<IBlock*> renderBlocks = getDisplayBlocks(activeDocument);
		foreach(IBlock* renderBlock, renderBlocks)
		{
			//Retrieve the render widget.
			RenderDockWidget renderWidget = recreateRenderWidget(renderBlock, tabifyWidget);
			tabifyWidget = renderWidget._dockWidget;

			//Update the properties of the dock widget.
			QString widgetName = QString("%1 #%2").arg(renderBlock->getName()).arg(renderBlock->getID());
			renderWidget._dockWidget->setWindowTitle(widgetName);
			renderWidget._dockWidget->show();
		}
	}
}

QList<GLRenderView*> RenderManager::releaseRenderViews()
{
	//Build a list of all released render views.
	QList<GLRenderView*> releasedRenderViews;

	//Iterate over all render widgets and invalidate the active ones.
	Document* activeDocument = _parentWindow->getActiveDocument();
	foreach(IBlock* renderBlock, _renderDockWidgets.keys())
	{
		//Check if the widget belongs to the active document.
		RenderDockWidget renderWidget = _renderDockWidgets[renderBlock];
		if(renderWidget._renderView->getActiveDocument() == activeDocument)
		{
			//Update the widget entry.
			QVector3D cameraBackup = renderWidget._renderView->getCamera();
			_renderDockWidgets[renderBlock] = { renderWidget._dockWidget, NULL, cameraBackup };

			//Disconnect the render view.
			renderWidget._renderView->onRenderingAborted();
			renderWidget._dockWidget->setWidget(NULL);

			//Store the render view for deletion.
			releasedRenderViews.append(renderWidget._renderView);
		}
	}

	//Return the list of all released views.
	return releasedRenderViews;
}

void RenderManager::releaseRenderDocks()
{
	//Iterate over all render widgets delete the obsolete ones.
	QList<IBlock*> renderBlocks = _renderDockWidgets.keys();
	foreach(IBlock* renderBlock, renderBlocks)
	{
		//Check if the widget has been invalidated.
		RenderDockWidget renderWidget = _renderDockWidgets[renderBlock];
		if(!renderWidget._renderView)
		{
			//Remove the dock widget.
			_renderDockWidgets.remove(renderBlock);
			delete renderWidget._dockWidget;
		}
	}
}

void RenderManager::updateRenderViews()
{
	//Iterate over all render widgets and hide the inactive ones.
	Document* activeDocument = _parentWindow->getActiveDocument();
	foreach(RenderDockWidget renderWidget, _renderDockWidgets.values())
		if(renderWidget._renderView && renderWidget._renderView->getActiveDocument() != activeDocument)
			renderWidget._dockWidget->setVisible(false);

	//Iterate over all render widgets and show the active ones.
	//This needs to be done afterwards, otherwise the window might be stretched by too many visible elements.
	foreach(RenderDockWidget renderWidget, _renderDockWidgets.values())
		if(renderWidget._renderView && renderWidget._renderView->getActiveDocument() == activeDocument)
			renderWidget._dockWidget->setVisible(activeDocument->isRendering());
}

void RenderManager::evaluatePipeline()
{
	//Deactivate rendering on the active views.
	QList<GLRenderView*> releasedRenderViews = releaseRenderViews();
	if(!releasedRenderViews.isEmpty())
	{
		//Clear the active GL controller.
		Document* activeDocument = _parentWindow->getActiveDocument();
		activeDocument->getGLController()->getEvaluator()->clear();

		//Delete the released render views after clear.
		qDeleteAll(releasedRenderViews);
	}

	//Update the widget's.
	recreateRenderWidgets();
	releaseRenderDocks();
	updateRenderViews();
}

void RenderManager::evaluateDocument(Document* document)
{
	//If document is active, re-evalute the pipeline.
	if(_parentWindow->getActiveDocument() == document)
		evaluatePipeline();
}

void RenderManager::setVersion(RenderManager::OpenGLVersion version)
{
	//Calculate numeric version compatible with pipeline manager.
	int numericVersion = version._major * 100 + version._minor * 10;

	//Ensure version actually changes.
	IPipeline* activePipeline = _parentWindow->getActiveDocument()->getPipeline();
	if(activePipeline->getOpenGLVersion() == numericVersion)
		return;

	//Execute command to set the new version.
	_parentWindow->executeCommand(new ChangeOpenGLVersionCommand(numericVersion, activePipeline));
}
