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

#include "documentmanager.h"
#include "mainwindow.h"
#include "rendermanager.h"
#include "data/ipipelinemanager.h"
#include "commands/iuicommandqueue.h"
#include "../document.h"
#include "../logview/logview.h"

#include <QMessageBox>

using namespace ysm;

DocumentManager::DocumentManager(MainWindow* parentWindow) :
	QObject(parentWindow),
	_parentWindow(parentWindow)
{ }

Document* DocumentManager::createDocument()
{
	//Add a new document.
	Document* document = new Document(this);
	addDocument(document);

	//Show the document.
	_parentWindow->setActiveDocument(document);

	//Return the newly active document.
	return document;
}

Document* DocumentManager::loadDocument(QString filename)
{
	//Add a new document.
	Document* document = new Document(filename, this);
	addDocument(document);

	//Load the document after adding it, to ensure all objects are attached to deserialization by now.
	QStringList messages;
	bool success = document->load(messages);
	LogView::log(messages);

	//Check if the document was loaded successfully.
	if(!success)
	{
		//Notify the user about the failure.
		QMessageBox::critical(_parentWindow, tr("Could not load document"),
							  tr("The document could not be loaded. See the console for more information."),
							  QMessageBox::Ok);

		//Close the document.
		closeDocument(document);
		return NULL;
	}

	//Send reset notification, then show the document.
	_parentWindow->resetView(document);
	_parentWindow->setActiveDocument(document);

	//Return the newly active document.
	return document;
}

void DocumentManager::saveDocument(Document* document)
{
	//Save the document.
	QStringList messages;
	bool success = document->save(messages, false, false);
	LogView::log(messages);

	//Check if the document was stored successfully.
	if(!success)
	{
		//Notify the user about the failure.
		QMessageBox::critical(_parentWindow, tr("Could not save document"),
							  tr("The document could not be saved. See the console for more information."),
							  QMessageBox::Ok);
	}
}

void DocumentManager::importDocument(Document* document, QString filename)
{
	//Import the document.
	QStringList messages;
	bool success = document->import(filename, messages);
	LogView::log(messages);

	//Check if the document was imported successfully.
	if(!success)
	{
		//Notify the user about the failure.
		QMessageBox::critical(_parentWindow, tr("Could not import document"),
							  tr("The document could not be imported. See the console for more information."),
							  QMessageBox::Ok);
	}

	//Reset the document.
	_parentWindow->resetView(document);
}

void DocumentManager::exportDocument(Document* document, QString filename, bool assets, bool compress)
{
	//Export the document.
	QStringList messages;
	bool success = document->save(filename, messages, assets, compress);
	LogView::log(messages);

	//Check if the document was exported successfully.
	if(!success)
	{
		//Notify the user about the failure.
		QMessageBox::critical(_parentWindow, tr("Could not export document"),
							  tr("The document could not be exported. See the console for more information."),
							  QMessageBox::Ok);
	}
}

void DocumentManager::addDocument(Document* document)
{
	//Store document.
	_documents.append(document);

	//Create list containing the document as added objects.
	QList<IChangeable*> changedObjects = QList<IChangeable*>();
	changedObjects.append(document);

	//Notify about change.
	_parentWindow->updateView(document, changedObjects, IChangeable::Add);

	//Watch the document.
	IUICommandQueue* commandQueue = document->getCommandQueue();
	connect(commandQueue, &IUICommandQueue::willRemoveData, this, &DocumentManager::willRemoveData);
	connect(commandQueue, &IUICommandQueue::didChangeData, this, &DocumentManager::didChangeData);
	connect(commandQueue, &IUICommandQueue::didAddData, this, &DocumentManager::didAddData);

	//Watch the rendering.
	RenderManager* renderManager = _parentWindow->getRenderManager();
	connect(document, &Document::renderingChanged, renderManager, &RenderManager::evaluatePipeline);
	connect(commandQueue, &IUICommandQueue::renderingChanged, renderManager, &RenderManager::evaluateDocument);

	//Watch the state.
	connect(document, &Document::unsavedChangesChanged, this, &DocumentManager::notifyDocumentChanged);
}

void DocumentManager::notifyDocumentChanged(Document* document)
{
	//Create list containing the document as added objects.
	QList<IChangeable*> changedObjects = QList<IChangeable*>();
	changedObjects.append(document);

	//Notify about change.
	_parentWindow->updateView(document, changedObjects, IChangeable::Change);
}

bool DocumentManager::closeDocument(Document* document)
{
	//Ask for confirmation, if document has unsaved changes.
	if(document->hasUnsavedChanges())
	{
		//Configure the message box.
		QMessageBox::StandardButton replyButton = QMessageBox::warning(_parentWindow, document->getName(),
			tr("The document has been modified, do you want to discard your changes?"),
			QMessageBox::Discard |  QMessageBox::Cancel);

		//Confirm the user wants to close.
		if(replyButton == QMessageBox::Cancel)
			return false;
	}

	//Release the document.
	IUICommandQueue* commandQueue = document->getCommandQueue();
	disconnect(commandQueue, &IUICommandQueue::willRemoveData, this, &DocumentManager::willRemoveData);
	disconnect(commandQueue, &IUICommandQueue::didChangeData, this, &DocumentManager::didChangeData);
	disconnect(commandQueue, &IUICommandQueue::didAddData, this, &DocumentManager::didAddData);

	//Release the rendering.
	RenderManager* renderManager = _parentWindow->getRenderManager();
	disconnect(document, &Document::renderingChanged, renderManager, &RenderManager::evaluatePipeline);
	disconnect(commandQueue, &IUICommandQueue::renderingChanged, renderManager, &RenderManager::evaluateDocument);

	//Release the state.
	disconnect(document, &Document::unsavedChangesChanged, this, &DocumentManager::notifyDocumentChanged);

	//Create list containing the document as removed objects.
	QList<IChangeable*> changedObjects = QList<IChangeable*>();
	changedObjects.append(document);

	//Notify about change.
	_parentWindow->updateView(document, changedObjects, IChangeable::Remove);

	//Remove document.
	_documents.removeAll(document);
	delete document;

	//Document closed.
	return true;
}

bool DocumentManager::closeAll()
{
	//Iterate over all documents and close them.
	while(!_documents.isEmpty())
		if(!closeDocument(_documents.first()))
			return false;

	//All documents closed.
	return true;
}

QList<Document*> DocumentManager::getDocuments() const { return _documents; }

void DocumentManager::willRemoveData(Document* document, const QList<IChangeable*>& changes)
{ _parentWindow->updateView(document, changes, IChangeable::Remove); }

void DocumentManager::didChangeData(Document* document, const QList<IChangeable*>& changes)
{ _parentWindow->updateView(document, changes, IChangeable::Change); }

void DocumentManager::didAddData(Document* document, const QList<IChangeable*>& changes)
{ _parentWindow->updateView(document, changes, IChangeable::Add); }
