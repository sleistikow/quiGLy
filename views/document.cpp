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

#include "document.h"
#include "registry.h"

#include "opengl/glcontroller.h"
#include "commands/uicommandqueue.h"

#include "data/ipipeline.h"
#include "data/pipeline/pipelinemanager.h"
#include "data/pipeline/pipelineprojectstream.h"

#include "views/logview/logview.h"

using namespace ysm;

Document::Document(QObject* parent):
	Document(QString(), parent)
{ }

Document::Document(const QString& filename, QObject* parent) :
	QObject(parent),
	_file(filename),
	_isRendering(false)
{
	//Create the pipeline manager.
	_manager = new PipelineManager();

	//Create the pipeline and use default registry.
	_pipeline = _manager->addPipeline();
	_commandQueue = new UICommandQueue(this);
	_registry = new Registry(this);

	//Try to create the OpenGL controller.
	try { _openGL = new GLController(this); }
	catch(std::exception) { _openGL = NULL; }

	//Connect to the command queue for changes.
	connect(_commandQueue, &IUICommandQueue::stateChanged, this, &Document::emitChanges);
}

Document::~Document()
{
	//Ensure rendering is halted.
	//Do not emit a signal here, the document is already invalid.
	_isRendering = false;
}

bool Document::save(QStringList& messages, bool assets, bool compress) { return save(_file, messages, assets, compress); }
bool Document::save(const QString &filename, QStringList& messages, bool assets, bool compress)
{
	//Save project.
	bool success = compress ?
		PipelineProjectStream::compressProject(_manager, filename, messages, &_attachments) :
		PipelineProjectStream::storeProject(_manager, filename, messages, assets, &_attachments);

	//Set command queue to saved state.
	if(success)
		_commandQueue->save();

	//Return success.
	return success;
}

bool Document::load(QStringList& messages) { return import(_file, messages); }
bool Document::import(const QString &filename, QStringList& messages)
{
	//Ensure rendering is halted.
	setRendering(false);

	//Import project, return success.
	return PipelineProjectStream::loadProject(filename, messages, _manager, &_attachments);
}

QString Document::getName() const
{
	//Return document title or <Untitled>.
	return _file.isEmpty() ? tr("Untitled") : QFileInfo(_file).fileName();
}

bool Document::isRenderingSupported() const
{
	//Convert the OpenGL version.
	int openGLVersion = _pipeline->getOpenGLVersion();
	int majorVersion = openGLVersion / 100;
	int minorVersion = (openGLVersion - majorVersion * 100) / 10;

	//Check if the version is supported.
	return _pipeline && _openGL->isOpenGLVersionSupported(majorVersion, minorVersion);
}

QString Document::getFile() const { return _file; }
void Document::setFile(const QString& filename) { _file = filename; }

void Document::emitChanges() { emit unsavedChangesChanged(this); }
bool Document::hasUnsavedChanges() { return !_commandQueue->isSaved(); }

IUICommandQueue* Document::getCommandQueue() const { return _commandQueue; }
IPipeline* Document::getPipeline() const { return _pipeline; }
GLController* Document::getGLController() const { return _openGL; }
Registry* Document::getRegistry() const { return _registry; }

bool Document::isRendering() const { return _isRendering; }
void Document::setRendering(bool isRendering) { _isRendering = isRendering; emit renderingChanged(); }

void Document::attach(ISerializable* attachment) { _attachments.append(attachment); }
void Document::detach(ISerializable* attachment) { _attachments.removeOne(attachment); }
