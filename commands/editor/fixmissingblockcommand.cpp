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

#include "fixmissingblockcommand.h"
#include "data/ipipeline.h"
#include "data/iconnection.h"
#include "data/iproperty.h"
#include "data/properties/property.h"
#include "data/common/dataexceptions.h"

#include <QDebug>

using namespace ysm;

FixMissingBlockCommand::FixMissingBlockCommand(IBlock* shaderBlock, BlockType missingBlockType, PortType targetPort,
											   bool (*setProperties)(IBlock*, IConnection*)) :
	_blockType(missingBlockType),
	_targetPort(targetPort),
	_shaderBlock(shaderBlock),
	_setProperties(setProperties)
{ }

bool FixMissingBlockCommand::execute()
{
	//Try to create and store the block.
	try
	{
		//Create the block, then connect to shader block.
		_createdBlock = _shaderBlock->getPipeline()->addBlock(_blockType);

		//Add the connection.
		IPort* sourcePort = _createdBlock->getPort(PortType::GenericOut);
		IPort* destinationPort = _shaderBlock->getPort(_targetPort);
		IConnection* connection = _shaderBlock->getPipeline()->addConnection(sourcePort, destinationPort);

		//Initialize the block.
		_setProperties(_createdBlock, connection);

		//Add block to added objects.
		addChangedObject(_createdBlock, IChangeable::Add);
	}
	catch (PipelineException& exception) { qDebug() << exception.what(); return false; }

	//No exceptions, success.
	return true;
}

bool FixMissingBlockCommand::undo()
{
	//Remove the block.
	_shaderBlock->getPipeline()->deleteBlock(_createdBlock);

	//Reset the changes.
	resetChangedObjects(IChangeable::Add);

	//Always succeed.
	return true;
}
