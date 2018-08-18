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

#include "glsluniformdeclarationcheck.h"
#include "glslpipelineadapter.h"
#include "../glslcodeblock.h"
#include "../glsldocument.h"
#include "../glsldocumentinfo.h"

#include "data/properties/property.h"
#include "data/iconnection.h"
#include "data/iproperty.h"
#include "data/iport.h"

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/add/addblockcommand.h"
#include "commands/pipeline/add/addconnectioncommand.h"
#include "commands/pipeline/change/updatepropertiescommand.h"
#include "commands/pipeline/change/renamepipelineitemcommand.h"

#include "views/document.h"

#include <QTextBlock>

using namespace ysm;

GLSLUniformDeclarationCheck::GLSLUniformDeclarationCheck()
{
	//Register integrity checks.
	registerIntegrityCheck(&GLSLUniformDeclarationCheck::ensureEmptyBody, true);
	registerIntegrityCheck(&GLSLUniformDeclarationCheck::checkLocationIntegrity, false);
	registerIntegrityCheck(&GLSLUniformDeclarationCheck::checkTypeIntegrity, false);
}

bool GLSLUniformDeclarationCheck::isHandled(GLSLDeclaration* declaration) const
{
	//Must be uniform and known data type.
	return declaration->hasQualifier("uniform") && declaration->hasUniformType();
}

QList<IConnection*> GLSLUniformDeclarationCheck::beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Filter all valid connections.
	QList<IConnection*> availableConnections;
	foreach(IConnection* connection, pipelineBlock->getPort(PortType::Shader_Uniform)->getInConnections())
		if(connection->getSource())
			availableConnections.append(connection);

	return availableConnections;
}

void GLSLUniformDeclarationCheck::extractElementData(IConnection* pipelineElement, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Ensure connection exists.
	_uniformBlock = pipelineElement->getSource();

	//Get the pipeline uniform data from connection.
	_pipelineType = _uniformBlock->getType();
	_pipelineLocation = pipelineElement->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)->getValue() ?
						pipelineElement->getProperty<UIntProperty>(PropertyID::Uniform_Location)->getValue() : -1;
	_pipelineTypeNames = getElementTypes(pipelineElement);

	//Just return the connection's name.
	_pipelineName = getElementName(pipelineElement);
}

QString GLSLUniformDeclarationCheck::getElementName(IConnection* pipelineElement)
{
	//Get the connection's name property.
	return pipelineElement->getProperty<StringProperty>(PropertyID::Uniform_Name)->getValue();
}

IGLSLPipelineCheck::FixedStatement GLSLUniformDeclarationCheck::getElementStatement(IConnection* pipelineElement) const
{
	Q_UNUSED(pipelineElement)

	//Create the code string using first possible type.
	QString elementCode;
	if(!_pipelineTypeNames.empty())
		elementCode = QString("%1uniform %2 %3;")
				.arg(_pipelineLocation >= 0 ? QString("layout(location = %1) ").arg(_pipelineLocation) : "")
				.arg(_pipelineTypeNames.first())
				.arg(_pipelineName);

	//Return the single line code.
	return { QStringList() << elementCode,  NULL, "Uniforms" };
}

QList<QString> GLSLUniformDeclarationCheck::getElementTypes(IConnection* pipelineElement) const
{
	//Get the uniform block.
	IBlock* uniformBlock = pipelineElement->getSource();

	//Get the pipeline uniform data from connection.
	BlockType pipelineType = uniformBlock->getType();
	QList<QString> pipelineTypeNames;

	//Check if model view projection uniform, manually adjust type name.
	if(pipelineType == BlockType::ModelViewProjection)
		switch(pipelineElement->getSourcePort()->getType())
		{
		case PortType::MVP_MatM:
		case PortType::MVP_MatMV:
		case PortType::MVP_MatMVP:
		case PortType::MVP_MatP:
		case PortType::MVP_MatV:
			pipelineTypeNames << "mat4" << "mat4x4";
			break;

		case PortType::MVP_MatNormal:
			pipelineTypeNames << "mat3" << "mat3x3";
			break;

		default: ;
		}

	//Check if light source uniform, manually adjust type name.
	else if(pipelineType == BlockType::LightSource)
		switch(pipelineElement->getSourcePort()->getType())
		{
		case PortType::Light_ColorAmbient:
		case PortType::Light_ColorDiffuse:
		case PortType::Light_ColorSpecular:
		case PortType::Light_Position:
			pipelineTypeNames << "vec4";
			break;

		case PortType::Light_SpotAngle:
		case PortType::Light_SpotExponent:
		case PortType::Light_AttenuationConstant:
		case PortType::Light_AttenuationLinear:
		case PortType::Light_AttenuationQuadratic:
			pipelineTypeNames << "float";
			break;

		case PortType::Light_Direction:
			pipelineTypeNames << "vec3";

		default: ;
		}

	//Check if material uniform, manually adjust type name.
	else if(pipelineType == BlockType::Material)
		switch(pipelineElement->getSourcePort()->getType())
		{
		case PortType::Material_Ambient:
		case PortType::Material_Diffuse:
		case PortType::Material_Specular:
			pipelineTypeNames << "vec4";
			break;

		case PortType::Material_Shininess:
			pipelineTypeNames << "float";
			break;

		default: ;
		}

	//Otherwise find all possible type names.
	else
		foreach(QString typeName, GLSLDeclaration::UNIFORM_DATA_TYPES.keys())
			if(GLSLDeclaration::UNIFORM_DATA_TYPES[typeName].contains(pipelineType))
				pipelineTypeNames.append(typeName);

	//Use the first possible declaration, if multiple available.
	return pipelineTypeNames;
}

bool GLSLUniformDeclarationCheck::checkLocationIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	Q_UNUSED(connection)

	//Get the code location and ensure it's correct.
	int codeLocation = declaration->hasLayoutParameter("location") ? declaration->getLayoutParameter("location").toInt() : -1;
	if(codeLocation != _pipelineLocation)
	{
		declaration->addPipelineError(
		{
			QString("Location of '%1' (%2) does not match location in connection from uniform block #%3 (%4).")
						.arg(_pipelineName)
						.arg(codeLocation >= 0 ? QString::number(codeLocation) : "not specified")
						.arg(_uniformBlock->getID())
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

bool GLSLUniformDeclarationCheck::checkTypeIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	Q_UNUSED(connection)

	//Get the code type and ensure it's correct.
	QString codeType = declaration->getDataType();
	if(!_pipelineTypeNames.contains(codeType))
	{
		declaration->addPipelineError(
		{
			QString("Type of '%1' (%2) does not match type in uniform block #%3 (%4).")
						.arg(_pipelineName)
						.arg(codeType)
						.arg(_uniformBlock->getID())
						.arg(_pipelineTypeNames.first()),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}

bool GLSLUniformDeclarationCheck::canFixMissingElement() const { return true; }

void GLSLUniformDeclarationCheck::fixMissingElement(Document* document, GLSLDeclaration* declaration)
{
	//Get the document block and command queue.
	IBlock* documentBlock = declaration->getDocument()->getPipelineAdapter()->getBlock();
	IUICommandQueue* commandQueue = document->getCommandQueue();

	//Validate the data.
	if(documentBlock && declaration->hasUniformType())
	{
		//Begin command block.
		commandQueue->beginCommandBlock();

		//Add the uniform block.
		BlockType blockType = GLSLDeclaration::UNIFORM_DATA_TYPES[declaration->getDataType()].first();
		AddBlockCommand* addBlockCommand = new AddBlockCommand(documentBlock->getPipeline(), blockType);
		commandQueue->execute(addBlockCommand);

		//Retrieve the ports to connect both blocks.
		IBlock* addedBlock = addBlockCommand->getBlock();
		IPort* sourcePort = addedBlock->getPort(PortType::GenericOut);
		IPort* targetPort = documentBlock->getPort(PortType::Shader_Uniform);

		//Add connection.
		AddConnectionCommand* addConnectionCommand = new AddConnectionCommand(sourcePort, targetPort);
		commandQueue->execute(addConnectionCommand);

		//Create the command to update the connection's properties.
		IConnection* addedConnection = addConnectionCommand->getConnection();
		UpdatePropertiesCommand* updateConnectionCommand = new UpdatePropertiesCommand(addedConnection);

		//Set the uniform's name according to GLSL code.
		updateConnectionCommand->addOperation(
			addedConnection->getProperty<StringProperty>(PropertyID::Uniform_Name),
			declaration->getName());

		//Check if the uniform has explicit location set in GLSL code.
		bool explicitLocation = declaration->hasLayoutParameter("location");
		updateConnectionCommand->addOperation(
			addedConnection->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation),
			explicitLocation);

		//Set the uniform's location according to GLSL code.
		if(declaration->hasLayoutParameter("location"))
			updateConnectionCommand->addOperation(
				addedConnection->getProperty<UIntProperty>(PropertyID::Uniform_Location),
				declaration->getLayoutParameter("location").toInt());

		//Execute the property update.
		commandQueue->execute(updateConnectionCommand);

		//Update the block's name.
		commandQueue->execute(new RenamePipelineItemCommand(addedBlock, declaration->getName()));

		//End command block.
		commandQueue->endCommandBlock();
	}
}

void GLSLUniformDeclarationCheck::fixMissingElement(GLSLDeclaration* declaration)
{
	//Get the document block and pipeline.
	IBlock* documentBlock = declaration->getDocument()->getPipelineAdapter()->getBlock();
	IPipeline* documentPipeline = documentBlock->getPipeline();

	//Validate the data.
	if(documentBlock && declaration->hasUniformType())
	{
		//Add the uniform block.
		BlockType blockType = GLSLDeclaration::UNIFORM_DATA_TYPES[declaration->getDataType()].first();
		IBlock* addedBlock = documentPipeline->addBlock(blockType);

		//Retrieve the ports to connect both blocks.
		IPort* sourcePort = addedBlock->getPort(PortType::GenericOut);
		IPort* targetPort = documentBlock->getPort(PortType::Shader_Uniform);

		//Add connection.
		IConnection* addedConnection = documentPipeline->addConnection(sourcePort, targetPort);

		//Begin to update the connection's properties.
		addedConnection->beginPropertyUpdate();

		//Set the uniform's name according to GLSL code.
		addedConnection
			->getProperty<StringProperty>(PropertyID::Uniform_Name)
			->setValue(declaration->getName());

		//Check if the uniform has explicit location set in GLSL code.
		bool explicitLocation = declaration->hasLayoutParameter("location");
		addedConnection
			->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)
			->setValue(explicitLocation);

		//Set the uniform's location according to GLSL code.
		if(declaration->hasLayoutParameter("location"))
			addedConnection
				->getProperty<UIntProperty>(PropertyID::Uniform_Location)
				->setValue(declaration->getLayoutParameter("location").toInt());

		//Execute the property update.
		addedConnection->endPropertyUpdate();

		//Update the block's name.
		addedBlock->setName(declaration->getName());
	}
}
