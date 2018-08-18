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

#include "glsltexturedeclarationcheck.h"
#include "glslpipelineadapter.h"
#include "../glslcodeblock.h"
#include "../glslstatements/glslstatement.h"
#include "../glslstatements/glsldeclaration.h"
#include "../glsldocument.h"
#include "../glsldocumentinfo.h"
#include "data/types/gltypes.h"

#include "data/iconnection.h"
#include "data/iproperty.h"
#include "data/iport.h"

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/add/addblockcommand.h"
#include "commands/pipeline/add/addconnectioncommand.h"
#include "commands/pipeline/change/updatepropertiescommand.h"
#include "commands/pipeline/change/renamepipelineitemcommand.h"

#include "views/document.h"

#include <QDebug>
#include <QTextBlock>

using namespace ysm;

GLSLTextureDeclarationCheck::GLSLTextureDeclarationCheck()
{
	//Register integrity checks.
	registerIntegrityCheck(&GLSLTextureDeclarationCheck::ensureEmptyBody, true);
	registerIntegrityCheck(&GLSLTextureDeclarationCheck::checkLocationIntegrity, false);
	registerIntegrityCheck(&GLSLTextureDeclarationCheck::checkTypeIntegrity, false);
}

bool GLSLTextureDeclarationCheck::isHandled(GLSLDeclaration* declaration) const
{
	//Must be texture and known data type.
	return declaration->hasQualifier("uniform") && declaration->hasTextureType();
}

QList<IConnection*> GLSLTextureDeclarationCheck::beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Filter all valid connections.
	QList<IConnection*> availableConnections;
	foreach(IConnection* connection, pipelineBlock->getPort(PortType::Shader_Texture)->getInConnections())
		if(connection->getSource())
			availableConnections.append(connection);

	return availableConnections;
}

void GLSLTextureDeclarationCheck::extractElementData(IConnection* pipelineElement, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Get the texture data type.
	_pipelineType = static_cast<TextureBlock::SamplerType>(pipelineElement->getProperty<EnumProperty>(PropertyID::Sampler_Type)->getValue());
	_pipelineTypeName = GLSLDeclaration::TEXTURE_DATA_TYPES.key(_pipelineType);

	//Get the texture location.
	_pipelineLocation = pipelineElement->getProperty<BoolProperty>(PropertyID::Texture_Binding)->getValue() ?
				pipelineElement->getProperty<UIntProperty>(PropertyID::Texture_Unit)->getValue() : -1;

	//Get the connection's name.
	_pipelineName = getElementName(pipelineElement);
}

QString GLSLTextureDeclarationCheck::getElementName(IConnection* pipelineElement)
{
	//Get the connection's name property.
	return pipelineElement->getProperty<StringProperty>(PropertyID::Texture_Name)->getValue();
}

IGLSLPipelineCheck::FixedStatement GLSLTextureDeclarationCheck::getElementStatement(IConnection* pipelineElement) const
{
	Q_UNUSED(pipelineElement);

	//Create the code string.
	QString elementCode = QString("%1uniform %2 %3;")
			.arg(_pipelineLocation > -1 ? QString("layout(binding = %1) ").arg(_pipelineLocation) : QString())
			.arg(_pipelineTypeName)
			.arg(_pipelineName);

	//Return the single line code.
	return { QStringList() << elementCode, NULL, "Uniforms" };
}

bool GLSLTextureDeclarationCheck::checkLocationIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	//Get the code location and ensure it's correct.
	int codeLocation = declaration->hasLayoutParameter("binding") ? declaration->getLayoutParameter("binding").toInt() : -1;
	if(codeLocation != _pipelineLocation)
	{
		declaration->addPipelineError(
		{
			QString("Binding of '%1' (%2) does not match binding in connection from texture block #%3 (%4).")
						.arg(_pipelineName)
						.arg(codeLocation >= 0 ? QString::number(codeLocation) : "not specified")
						.arg(connection->getSource()->getID())
						.arg(_pipelineLocation >= 0 ? QString::number(_pipelineLocation) : "not specified"),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}

bool GLSLTextureDeclarationCheck::checkTypeIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	//Get the code type and ensure it's correct.
	QString codeType = declaration->getDataType();
	if(codeType != _pipelineTypeName)
	{
		declaration->addPipelineError(
		{
			QString("Type of '%1' (%2) does not match type in connection from texture block #%3 (%4).")
						.arg(_pipelineName)
						.arg(codeType)
						.arg(connection->getSource()->getID())
						.arg(_pipelineTypeName),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}

bool GLSLTextureDeclarationCheck::canFixMissingElement() const { return true; }

void GLSLTextureDeclarationCheck::fixMissingElement(Document* document, GLSLDeclaration* declaration)
{
	//Get the document block and command queue.
	IBlock* documentBlock = declaration->getDocument()->getPipelineAdapter()->getBlock();
	IUICommandQueue* commandQueue = document->getCommandQueue();

	//Validate the data.
	if(documentBlock && declaration->hasTextureType())
	{
		//Begin command block.
		commandQueue->beginCommandBlock();

		//Add the uniform block.
		AddBlockCommand* addBlockCommand = new AddBlockCommand(documentBlock->getPipeline(), BlockType::Texture);
		commandQueue->execute(addBlockCommand);

		//Retrieve the ports to connect both blocks.
		IBlock* addedBlock = addBlockCommand->getBlock();
		IPort* sourcePort = addedBlock->getPort(PortType::GenericOut);
		IPort* targetPort = documentBlock->getPort(PortType::Shader_Texture);

		//Add connection.
		AddConnectionCommand* addConnectionCommand = new AddConnectionCommand(sourcePort, targetPort);
		commandQueue->execute(addConnectionCommand);

		//Create the command to update the connection's properties.
		IConnection* addedConnection = addConnectionCommand->getConnection();
		UpdatePropertiesCommand* updateConnectionCommand = new UpdatePropertiesCommand(addedConnection);

		//Set the uniform's type according to GLSL code.
		updateConnectionCommand->addOperation(
			addedConnection->getProperty<EnumProperty>(PropertyID::Sampler_Type),
			GLSLDeclaration::TEXTURE_DATA_TYPES[declaration->getDataType()]);

		//Set the uniform's name according to GLSL code.
		updateConnectionCommand->addOperation(
			addedConnection->getProperty<StringProperty>(PropertyID::Texture_Name),
			declaration->getName());

		//Check if the uniform has explicit location set in GLSL code.
		bool explicitLocation = declaration->hasLayoutParameter("location");
		updateConnectionCommand->addOperation(
			addedConnection->getProperty<BoolProperty>(PropertyID::Texture_Binding),
			explicitLocation);

		//Set the uniform's location according to GLSL code.
		if(declaration->hasLayoutParameter("location"))
			updateConnectionCommand->addOperation(
				addedConnection->getProperty<UIntProperty>(PropertyID::Texture_Unit),
				declaration->getLayoutParameter("location").toInt());

		//Execute the property update.
		commandQueue->execute(updateConnectionCommand);

		//Update the block's name.
		commandQueue->execute(new RenamePipelineItemCommand(addedBlock, declaration->getName()));

		//End command block.
		commandQueue->endCommandBlock();
	}
}

void GLSLTextureDeclarationCheck::fixMissingElement(GLSLDeclaration* declaration)
{
	//Get the document block and pipeline.
	IBlock* documentBlock = declaration->getDocument()->getPipelineAdapter()->getBlock();
	IPipeline* documentPipeline = documentBlock->getPipeline();

	//Validate the data.
	if(documentBlock && declaration->hasTextureType())
	{
		//Add the uniform block.
		IBlock* addedBlock = documentPipeline->addBlock(BlockType::Texture);

		//Retrieve the ports to connect both blocks.
		IPort* sourcePort = addedBlock->getPort(PortType::GenericOut);
		IPort* targetPort = documentBlock->getPort(PortType::Shader_Texture);

		//Add connection.
		IConnection* addedConnection = documentPipeline->addConnection(sourcePort, targetPort);

		//Begin to update the connection's properties.
		addedConnection->beginPropertyUpdate();

		//Set the uniform's type according to GLSL code.
		addedConnection
			->getProperty<EnumProperty>(PropertyID::Sampler_Type)
			->setValue(GLSLDeclaration::TEXTURE_DATA_TYPES[declaration->getDataType()]);

		//Set the uniform's name according to GLSL code.
		addedConnection
			->getProperty<StringProperty>(PropertyID::Texture_Name)
			->setValue(declaration->getName());

		//Check if the uniform has explicit location set in GLSL code.
		bool explicitLocation = declaration->hasLayoutParameter("location");
		addedConnection
			->getProperty<BoolProperty>(PropertyID::Texture_Binding)
			->setValue(explicitLocation);

		//Set the uniform's location according to GLSL code.
		if(declaration->hasLayoutParameter("location"))
			addedConnection
				->getProperty<UIntProperty>(PropertyID::Texture_Unit)
				->setValue(declaration->getLayoutParameter("location").toInt());

		//Execute the property update.
		addedConnection->endPropertyUpdate();

		//Update the block's name.
		addedBlock->setName(declaration->getName());
	}
}
