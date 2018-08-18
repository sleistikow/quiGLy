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

#include "glrenderpassset.h"
#include "glrenderpass.h"

#include "data/blocks/port.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"
#include "data/ipipeline.h"

namespace ysm
{

bool GLRenderPassSet::isPassSwitchIndicator(IBlock* block)
{
	switch(block->getType())
	{
	case BlockType::FrameBufferObject:
	case BlockType::TransformFeedback:
		return true;
	case BlockType::Buffer:
	{
		QVector<IConnection*> inConnections  = block->getPort(PortType::Data_In)->getInConnections();
		//TODO: is it possible to connect an SSBO from a vertex to a fragment shader for example?
		if(inConnections.isEmpty())
			return false;
		if(inConnections[0]->getSourcePort()->getType() == PortType::Shader_SSBOOut)
			return true;

		// else, pass through
	}
	default:
		return false;
	}
}

GLRenderPassSet::GLRenderPassSet(IBlock* outputBlock)
	: _outputBlock(outputBlock),
	  _valid(true)
{
	if(!_outputBlock)
		throw std::runtime_error("outputBlock may not be null");

	// Receive pipeline
	_pipeline = _outputBlock->getPipeline();

	// A vector from all iterators not having finished their iteration
	QVector<PipelineIterator*> iterators;

	// Add the first iterators
	for(IConnection* connection : _outputBlock->getInConnections())
	{
		// Create a new iterator for every incoming connection
		PipelineIterator* iter = new PipelineIterator(connection->getSource(), this);

		// Add the output block manually
		iter->getRenderPass()->addInvolvedBlock(_outputBlock);

		// Look for a render command connected to the output block
		for(IRenderCommand* command : findAssignedRenderCommands(_outputBlock))
			iter->getRenderPass()->addInvolvedRenderCommand(command);

		// Append the iterator
		iterators.append(iter);
	}

	while(!iterators.isEmpty())
	{
		for(int i=iterators.size() - 1; i >=0 ; i--)
		{
			PipelineIterator* iterator = iterators.at(i);

			// Expand block front, if not all blocks are already visited
			while(iterator->isValid())
			{
				iterators.append(iterator->iterate());

				// Check if set is still valid
				if(!_valid)
				{
					// In case something went wrong, we have to clean up manually
					for(int j=0; j < iterators.size(); j++)
					{
						delete iterators[j]->getRenderPass();
						delete iterators[j];
					}

					// We don't need to continue parsing
					return;
				}
			}

			// In case the current iterator has nothing more to do, its pass is complete
			_passes.append(iterator->getRenderPass());

			// Finally, we can remove the iterator
			iterators.remove(i);
			delete iterator;
		}
	}
}

GLRenderPassSet::~GLRenderPassSet()
{
	for(GLRenderPass* pass : _passes)
		delete pass;
}

bool GLRenderPassSet::isValid() const
{
	return _valid;
}

IPipeline* GLRenderPassSet::getPipeline() const
{
	return _pipeline;
}

IBlock* GLRenderPassSet::getOutputBlock() const
{
	return _outputBlock;
}

const QList<GLRenderPass*>& GLRenderPassSet::getRenderPasses() const
{
	return _passes;
}

GLRenderPassSet::PipelineIterator::PipelineIterator(IBlock* block, GLRenderPassSet* parent)
	: _parent(parent)
{
	if(!block)
		throw std::runtime_error("block may not be null");

	// Check for circular dependencies
	for(GLRenderPass* pass : parent->getRenderPasses())
	{
		if(pass->getInvolvedBlocks().contains(block))
		{
			parent->_valid = false;
			return;
		}
	}

	// Add the block as first and only front block
	_blockFront.insert(block);

	// Create the renderpass
	_pass = new GLRenderPass();
}

GLRenderPass* GLRenderPassSet::PipelineIterator::getRenderPass() const
{
	return _pass;
}

bool GLRenderPassSet::PipelineIterator::isValid() const
{
	return !_blockFront.isEmpty();
}

QList<IRenderCommand*> GLRenderPassSet::findAssignedRenderCommands(IBlock* block)
{
	QList<IRenderCommand*> commands;
	for(IRenderCommand* command : _pipeline->getRenderCommands())
	{
		if(command->getAssignedBlock() == block)
			commands.append(command);
	}

	return commands;
}

QVector<GLRenderPassSet::PipelineIterator*> GLRenderPassSet::PipelineIterator::iterate()
{
	// This vector stores the iterators being produced due to a switch to another pass
	QVector<GLRenderPassSet::PipelineIterator*> iterators;

	QSet<IBlock*> nextBlockFront;
	for(IBlock* block : _blockFront)
	{
		// Add rendercommands, if any
		for(IRenderCommand* command : _parent->findAssignedRenderCommands(block))
			_pass->addInvolvedRenderCommand(command);

		// Add all front blocks to the pass
		_pass->addInvolvedBlock(block);

		// Add connections to the pass
		QVector<IConnection*> connections = block->getInConnections();

		// Actual update of the block front happens here
		for(IConnection* connection : connections)
		{			
			IBlock* source = connection->getSource();
			if(isPassSwitchIndicator(source))
			{
				if(!_passSwitches.contains(source))
				{	
					// We found a new pass :-)
					for(IConnection* furtherConnection : source->getInConnections())
					{
						PipelineIterator* iter = new PipelineIterator(furtherConnection->getSource(), _parent);
						if(iter->isValid())
						{
							// Get the pass behind the iterator
							GLRenderPass* pass = iter->getRenderPass();

							// Add the switch indicator to the pass
							pass->addInvolvedBlock(source);

							// FrameBufferObject out port workaround
							if(source->getType() == BlockType::FrameBufferObject)
							{
								for(IRenderCommand* command : _parent->findAssignedRenderCommands(source))
									pass->addInvolvedRenderCommand(command);

								for(IConnection* connection : source->getOutConnections())
									_pass->addInvolvedBlock(connection->getDest());
							}

							// Note the switch as visited
							_passSwitches[source].insert(pass);

							// Add the iterator for further iteration
							iterators.append(iter);
						}
						else
							delete iter;
					}
				}
				else
				{
					for(GLRenderPass* pass : _passSwitches[source])
						pass->addInvolvedBlock(block);
				}
			}
			else
			{
				// Since we only catch cycles between passes, Texture-View cyclces inside a pass are not caught	.
				// To prevent an infinite loop here, we have to check for this case manually.
				if(	source->getType() == BlockType::TextureView &&
					block->getType() == BlockType::TextureView &&
					_pass->getInvolvedBlocks().contains(source))
				{
					// Signal the parent something went wrong
					_parent->_valid = false;

					// Return the gathered iterators to prevent memory leaks
					return iterators;
				}
				else
					// If everything went fine, we can add the block to the blockfront
					nextBlockFront.insert(source);
			}
		}
	}

	// Assign the next front of blocks
	_blockFront = nextBlockFront;

	// Finally, return the new iterators, if any
	return iterators;
}

}

