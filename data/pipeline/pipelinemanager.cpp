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

#include "pipelinemanager.h"
#include "pipelinelist.h"
#include "visitors/searchpipelinevisitor.h"
#include "data/blocks/block.h"
#include "data/blocks/blocklist.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"
#include "data/rendercommands/rendercommandlist.h"

using namespace ysm;

//Set initial pipeline item ID to zero.
PipelineItemID PipelineManager::_highestItemID = 0;

//Initialize the manager registry.
QList<PipelineManager*> PipelineManager::_registry;

PipelineManager::PipelineManager(QObject* parent) :
	QObject(parent)
{
	//Register the manager.
	_pipelines = new PipelineList(this);
	_registry.append(this);
}

PipelineManager::~PipelineManager()
{
	//Unregister from manager, then delete the data.
	_registry.removeAll(this);
	delete _pipelines;
}

IPipelineItem* PipelineManager::findGlobalPipelineItem(const PipelineItemID itemId)
{
	//Execute a search visitor, that stores a possible match.
	SearchPipelineVisitor visitor(itemId);
	takeGlobalVisitor(&visitor);
	return visitor.getFoundItem();
}

Block* PipelineManager::findGlobalBlock(const PipelineItemID itemId)
{ return dynamic_cast<Block*>(findGlobalPipelineItem(itemId)); }

Port* PipelineManager::findGlobalPort(const PipelineItemID itemId)
{ return dynamic_cast<Port*>(findGlobalPipelineItem(itemId)); }

Connection* PipelineManager::findGlobalConnection(const PipelineItemID itemId)
{ return dynamic_cast<Connection*>(findGlobalPipelineItem(itemId)); }

RenderCommand* PipelineManager::findGlobalRenderCommand(const PipelineItemID itemId)
{ return dynamic_cast<RenderCommand*>(findGlobalPipelineItem(itemId)); }

bool PipelineManager::takeGlobalVisitor(PipelineVisitor* visitor)
{
	//Ensure the visitor exists.
	if(!visitor) throw std::invalid_argument("visitor may not be null");

	//Iterate over all pipelines managers and execute the visitor.
	foreach(PipelineManager* pipelineManager, _registry)
		if(!pipelineManager->takeVisitor(visitor))
			return false;

	return true;
}

bool PipelineManager::takeVisitor(PipelineVisitor* visitor)
{
	//Ensure the visitor exists.
	if(!visitor) throw std::invalid_argument("visitor may not be null");

	//Iterate over all pipelines and execute the visitor.
	for(Pipeline* pipeline : *_pipelines)
		if(!pipeline->takeVisitor(visitor))
			return false;

	return true;
}

PipelineList* PipelineManager::getPipelineList() { return _pipelines; }
QVector<IPipeline*> PipelineManager::getPipelines() const { return _pipelines->objects<IPipeline>(); }

PipelineObjectFactory* PipelineManager::getObjectFactory() { return &_objectFactory; }
CachePool* PipelineManager::getCachePool() { return &_cachePool; }

PipelineItemID PipelineManager::getHighestItemID() { return _highestItemID; }

PipelineItemID PipelineManager::requestNextItemID() { return ++_highestItemID; }
void PipelineManager::setHighestItemID(const PipelineItemID identifier, bool higherOnly)
{
	//Adjust the highest item identifier.
	_highestItemID = higherOnly ? qMax(identifier, _highestItemID) : identifier;
}

Pipeline* PipelineManager::addPipeline() { return _pipelines->newPipeline(); }
void PipelineManager::deletePipeline(const IPipeline* pipeline)
{
	//Try to cast to pipeline instance.
	const Pipeline* castedPipeline = dynamic_cast<const Pipeline*>(pipeline);
	if(!castedPipeline)
		throw std::invalid_argument("pipeline is not an instance of Pipeline");

	//Remove the pipeline.
	_pipelines->remove(castedPipeline);
}

void PipelineManager::reset()
{
	//Cleanup cache.
	_cachePool.clearCache();

	//Remove all pipelines.
	_pipelines->clear();

	//Reset highest item identifier.
	setHighestItemID(0);
}

void PipelineManager::serialize(QDomElement* root, SerializationContext* context) const
{
	//Create the pipelines node.
	QDomElement element = context->createElement("Pipelines");
	root->appendChild(element);

	//Serialize the content.
	_pipelines->serialize(&element, context);
}

void PipelineManager::deserialize(const QDomElement* root, SerializationContext* context)
{
	//Try to retrieve the pipelines node.
	QDomElement element = root->firstChildElement("Pipelines");
	if(element.isNull())
		throw SerializationException("The serialization data doesn't contain any pipelines");

	//Deserialize the content.
	_pipelines->deserialize(&element, context);
}
