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

#include "glslssbodeclarationcheck.h"
#include "../glslstatements/glsldeclaration.h"
#include "../glsldocument.h"
#include "../glsldocumentinfo.h"
#include "data/properties/property.h"

using namespace ysm;

QList<QString> GLSLSSBODeclarationCheck::PACKAGE_QUALIFIERS;

GLSLSSBODeclarationCheck::GLSLSSBODeclarationCheck()
{
	//Initialize package qualifiers if neccessary.
	if(PACKAGE_QUALIFIERS.isEmpty())
	{
		PACKAGE_QUALIFIERS.append("packed");
		PACKAGE_QUALIFIERS.append("shared");
		PACKAGE_QUALIFIERS.append("std140");
		PACKAGE_QUALIFIERS.append("std430");
	}

	//Register integrity checks.
	registerIntegrityCheck(&GLSLSSBODeclarationCheck::ensureHasBody, true);
	registerIntegrityCheck(&GLSLSSBODeclarationCheck::checkLocationIntegrity, false);
	registerIntegrityCheck(&GLSLSSBODeclarationCheck::checkVersionIntegrity, false);
}

bool GLSLSSBODeclarationCheck::isHandled(GLSLDeclaration* declaration) const
{
	//Must be buffer and no data type specified.
	return declaration->hasQualifier("buffer") && declaration->getDataType().isEmpty();
}

QList<IConnection*> GLSLSSBODeclarationCheck::beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Filter all valid connections.
	QList<IConnection*> availableConnections;

	//Use SSBO ins and SSBO outs.
	foreach(IConnection* connection, pipelineBlock->getPort(PortType::Shader_SSBOIn)->getInConnections())
		if(connection->getSource())
			availableConnections.append(connection);
	foreach(IConnection* connection, pipelineBlock->getPort(PortType::Shader_SSBOOut)->getOutConnections())
		if(connection->getDest())
			availableConnections.append(connection);

	return availableConnections;
}

void GLSLSSBODeclarationCheck::extractElementData(IConnection* pipelineElement, GLSLCodeBlock* codeBlock)
{
	//Get the pipeline data.
	GLSLDocumentInfo* documentInfo = codeBlock->getDocument()->getDocumentInfo();
	_pipelineLocation = pipelineElement->getProperty<UIntProperty>(PropertyID::Buffer_Binding)->getValue();
	_pipelineVersion = documentInfo->getVersion().first;
	_pipelineName = getElementName(pipelineElement);

	//Get structure body.
	QMap<int, GLSLDeclaration*> declarations = documentInfo->getDeclarations();
	_pipelineDeclaration = declarations.contains(pipelineElement->getID()) ?
		declarations[pipelineElement->getID()] :
		NULL;
}

void GLSLSSBODeclarationCheck::storeDocumentData(IConnection* pipelineElement, GLSLCodeBlock* codeBlock,
												 GLSLDeclaration* declaration)
{
	Q_UNUSED(pipelineElement)

	//Get the document data and inject the declaration.
	GLSLDocumentInfo* documentInfo = codeBlock->getDocument()->getDocumentInfo();
	QMap<int, GLSLDeclaration*> declarations = documentInfo->getDeclarations();
	declarations[pipelineElement->getID()] = declaration;
	documentInfo->setDeclarations(declarations);
}

QString GLSLSSBODeclarationCheck::getElementName(IConnection* pipelineElement)
{
	//Get the connection's name property.
	return pipelineElement->getProperty<StringProperty>(PropertyID::Buffer_Name)->getValue();
}

IGLSLPipelineCheck::FixedStatement GLSLSSBODeclarationCheck::getElementStatement(IConnection* pipelineElement) const
{
	Q_UNUSED(pipelineElement);

	//Create the content string and layout specifier.
	QString instanceName;
	QString packageQualifier = "std430";
	QStringList structureDeclarations;
	if(_pipelineDeclaration)
	{
		//Generate the body.
		structureDeclarations = generateDeclarationBody(_pipelineDeclaration);

		//Create the layout specifier.
		foreach(QString qualifier, PACKAGE_QUALIFIERS)
			if(_pipelineDeclaration->hasLayoutParameter(qualifier))
				packageQualifier = qualifier;

		//Check if instance name is available.
		GLSLDeclaration* instanceDeclaration = _pipelineDeclaration->getInstanceDeclaration();
		if(instanceDeclaration)
			instanceName = instanceDeclaration->getName() + instanceDeclaration->getArray();
	}

	//Ensure at least a line to add content is available.
	if(structureDeclarations.isEmpty())
		structureDeclarations << "\t";

	//Create the code string.
	QStringList elementCode = QStringList()
		<< QString("layout(%1%5%2%3) buffer %4")
			.arg(GLSLDocument::editableStart)
			.arg(GLSLDocument::editableEnd)
			.arg(_pipelineLocation > -1 ? QString(", binding = %1").arg(_pipelineLocation) : QString())
			.arg(_pipelineName)
			.arg(packageQualifier)
		<< QString("{%1").arg(GLSLDocument::editableBlockStart);

	//Append the body lines.
	elementCode.append(structureDeclarations);

	//Append the closing bracket and optional instance name.
	elementCode
		<< QString("}%2").arg(GLSLDocument::editableBlockEnd)
		<< QString("%1%3%2;")
			.arg(GLSLDocument::editableStart)
			.arg(GLSLDocument::editableEnd)
			.arg(instanceName);

	//Return the fixed statement.
	return { elementCode, NULL, "Buffers" };
}

bool GLSLSSBODeclarationCheck::checkLocationIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	//Get the code location.
	int codeLocation = declaration->hasLayoutParameter("binding") ?
		declaration->getLayoutParameter("binding").toInt() :
		-1;

	//Ensure it's correct.
	if(codeLocation != _pipelineLocation)
	{
		declaration->addPipelineError(
		{
			QString("Binding of '%1' (%2) does not match binding in connection from buffer block #%3 (%4).")
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

bool GLSLSSBODeclarationCheck::checkVersionIntegrity(IConnection* connection, GLSLDeclaration* declaration)
{
	Q_UNUSED(connection)

	//Get the code location and ensure it's correct.
	//TODO: Minimum GLSL version, not OpenGL version.
	int minimumVersion = 430; //connection->getDestPort()->getMinimumVersion();
	if(_pipelineVersion < minimumVersion)
	{
		declaration->addPipelineError(
		{
			QString("Shader storage buffer objects not supported by GLSL version %1 (minimum required version: %2).")
						.arg(_pipelineVersion)
						.arg(minimumVersion),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}
