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

#include "serializationcontext.h"
#include "data/irendercommand.h"
#include "data/pipeline/pipelinemanager.h"
#include "data/pipeline/pipelinelist.h"
#include "data/blocks/blocklist.h"
#include "data/blocks/block.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"
#include "data/blocks/connectionlist.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"
#include "data/rendercommands/rendercommandlist.h"
#include "dataexceptions.h"

#include <stdexcept>

using namespace ysm;

SerializationContext::SerializationContext(QDomDocument* document, QDomElement* root) :
	_xmlDocument{document},
	_xmlRoot{root}
{
	// Ensure XML document is set.
	if (!document)
		throw std::invalid_argument{"document may not be null"};

	// Ensure XML root element is set.
	if (!root)
		throw std::invalid_argument{"root may not be null"};
}


void SerializationContext::initContext(PathAdjustMode adjustMode, QDir baseDirectory, QDir assetsDirectory,
									   unsigned int offsetID, bool importMode)
{
	// Just store the given data.
	_adjustMode = adjustMode;
	_baseDirectory = baseDirectory;
	_assetsDirectory = assetsDirectory;
	_offsetID = offsetID;
	_importMode = importMode;
}

unsigned int SerializationContext::getObjectID(ISerializable* serializableObject)
{
	// Ensure object is specified.
	if (!serializableObject)
		throw std::invalid_argument{"serializableObject may not be null"};

	// If the object has not been pooled, add it with the next available ID.
	if (!_objectIDs.contains(serializableObject))
		setObjectID(serializableObject, _currentID + 1);

	// Return the object's ID.
	return _objectIDs[serializableObject];
}

unsigned int SerializationContext::setObjectID(ISerializable* serializableObject, const unsigned int objectID)
{
	// Ensure object is specified.
	if (!serializableObject)
		throw std::invalid_argument{"serializableObject may not be null"};

	// Adjust the current ID, if neccessary.
	_currentID = qMax(_currentID, objectID);

	// Store the object's ID.
	_objectIDs[serializableObject] = objectID + _offsetID;
	return _objectIDs[serializableObject];
}

unsigned int SerializationContext::setObjectID(ISerializable* serializableObject, const QDomElement* element)
{
	// Ensure object is specified.
	if (!serializableObject)
		throw std::invalid_argument{"serializableObject may not be null"};

	// Ensure the ID tag is available.
	if (element->hasAttribute("id"))
		setObjectID(serializableObject, element->attribute("id").toInt());

	// Element does not have an ID.
	return 0;
}

ISerializable* SerializationContext::findObject(unsigned int objectID) const
{
	// Adjust the object's ID.
	objectID += _offsetID;

	// Iterate over all available objects, until a match is found.
	for (auto it = _objectIDs.cbegin(); it != _objectIDs.cend(); ++it)
		if (it.value() == objectID)
			return it.key();

	// No object was found.
	return nullptr;
}

void SerializationContext::poolPipelineItems(IPipelineManager* pipelineManager)
{
	// Ensure pipeline manager is specified.
	if (!pipelineManager)
		throw std::invalid_argument{"pipelineManager may not be null"};

	// Iterate over all pipelines in the manager.
	foreach(IPipeline* pipeline, pipelineManager->getPipelines())
	{
		// Iterate over all available blocks.
		foreach(IBlock* block, pipeline->getBlocks())
		{
			// Retrieve ID for the block.
			getObjectID(block);

			// Retrieve ID for all incoming ports.
			foreach(IPort* port, block->getInPorts())
				getObjectID(port);

			// Retrieve ID for all outgoing ports.
			foreach(IPort* port, block->getOutPorts())
				getObjectID(port);

			// Retrieve ID for all incoming connections.
			foreach(IConnection* connection, block->getInConnections())
				getObjectID(connection);

			// Retrieve ID for all outgoing connections.
			foreach(IConnection* connection, block->getOutConnections())
				getObjectID(connection);
		}

		// Retrieve ID for all render commands.
		foreach(IRenderCommand* renderCommand, pipeline->getRenderCommands())
			getObjectID(renderCommand);
	}
}

void SerializationContext::deferElement(ISerializable* serializableObject, const QDomElement& element,
										DeferringPriority priority)
{
	// Ensure object is specified.
	if (!serializableObject)
		throw std::invalid_argument{"serializableObject may not be null"};

	// Add object to deferred list.
	_deferredElements.insert(priority, qMakePair(serializableObject, element));
}

void SerializationContext::processDeferredElements()
{
	// Set processing deferred flag.
	_processingDeferredElements = true;

	// Process all deferred elements by priority.
	processDeferredElements(DeferringPriority::Highest);
	processDeferredElements(DeferringPriority::High);
	processDeferredElements(DeferringPriority::Normal);
	processDeferredElements(DeferringPriority::Low);
	processDeferredElements(DeferringPriority::Lowest);

	// Clear processing deferred list and flag.
	_deferredElements.clear();
	_processingDeferredElements = false;
}

void SerializationContext::processDeferredElements(DeferringPriority priority)
{
	// Iterate over all elements of that priority and deserialize them.
	QList<DeferredElement> elements = _deferredElements.values(priority);
	for (const DeferredElement& element : elements)
		element.first->deserialize(&element.second, this);
}

// TODO!!!
QString SerializationContext::resolveFilename(QString sourceFile) const
{
	// Check the current file adjustment mode.
	QFileInfo sourceInfo(sourceFile);
	switch(_adjustMode)
	{

	// Filename should not be changed.
	case PathAdjustMode::Keep:
		return sourceFile;

	// Path information should be erased.
	case PathAdjustMode::Move:
		return "assets:/" + sourceInfo.fileName();

	// Path should be contained in asset.
	case PathAdjustMode::Load:
		sourceFile = sourceFile.replace("assets:", _assetsDirectory.absolutePath());
		return QDir::cleanPath(sourceFile);

	}

	// Unknown operation.
	return sourceFile;
}





QVector<QDomElement> SerializationContext::getElements(const QDomElement* parent, const QString& name) const
{
	// Ensure parent is specified.
	if (!parent)
		throw std::invalid_argument{"parent may not be null"};

	// Create list of all elements with that name by iterating over the childs.
	QVector<QDomElement> elements;
	for(QDomElement element = parent->firstChildElement(name);
		!element.isNull();
		element = element.nextSiblingElement(name))
	{
		elements << element;
	}

	// Return the generated list.
	return elements;
}

bool SerializationContext::assertAttributes(const QDomElement* element, const QStringList& attributes)
{
	// Ensure element is specified.
	if (!element)
		throw std::invalid_argument{"element may not be null"};

	// Iterate over all attributes and ensure each is set.
	for (QString attribute : attributes)
		if (!element->hasAttribute(attribute))
		{
			// Element not set, generate message.
			QString message = QString("XML element '%1' is missing the '%2' attribute")
					.arg(element->nodeName())
					.arg(attribute);

			// Store the message and return failure.
			_messages << message;
			return false;
		}

	// All attributes available.
	return true;
}

QDomElement SerializationContext::createElement(const QString& name, ISerializable* serializableObject)
{
	// Create the new element.
	QDomElement element = _xmlDocument->createElement(name);

	// If an object is specified, set the ID.
	if (serializableObject)
		element.setAttribute("id", getObjectID(serializableObject));

	// Return the new object.
	return element;
}

QDomText SerializationContext::createTextElement(const QString& text) { return _xmlDocument->createTextNode(text); }

QDir SerializationContext::getBaseDirectory() const { return _baseDirectory; }
QDir SerializationContext::getAssetsDirectory() const { return _assetsDirectory; }

QDomDocument* SerializationContext::getDocument() { return _xmlDocument; }
QDomElement* SerializationContext::getRoot() { return _xmlRoot; }

unsigned int SerializationContext::getOffsetID() const { return _offsetID; }

bool SerializationContext::isImportMode() const { return _importMode; }
bool SerializationContext::isProcessingDeferredElements() const { return _processingDeferredElements; }
SerializationContext::PathAdjustMode SerializationContext::getPathAdjustMode() const { return _adjustMode; }

QStringList SerializationContext::getMessages() const { return _messages; }
void SerializationContext::addMessage(const QString& message) { _messages << message; }
