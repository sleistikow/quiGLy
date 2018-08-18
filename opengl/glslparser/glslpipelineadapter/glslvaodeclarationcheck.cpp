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

#include "glslvaodeclarationcheck.h"
#include "../glslcodeblock.h"
#include "../glslstatements/glslstatement.h"
#include "../glslstatements/glsldeclaration.h"

#include "data/properties/property.h"
#include "data/iconnection.h"
#include "data/iproperty.h"
#include "data/iport.h"

#include <QTextBlock>

using namespace ysm;

GLSLVaoDeclarationCheck::GLSLVaoDeclarationCheck()
{
	//Get the GLSL types.
	_glslTypes = GLTypes::getGLSLDataTypeNames();

	//Register integrity checks.
	registerIntegrityCheck(&GLSLVaoDeclarationCheck::ensureEmptyBody, true);
	registerIntegrityCheck(&GLSLVaoDeclarationCheck::checkLocationIntegrity, false);
	registerIntegrityCheck(&GLSLVaoDeclarationCheck::checkTypeIntegrity, false);
}

IBlock* GLSLVaoDeclarationCheck::findFirstInBlockByType(IBlock* startBlock, BlockType blockType, IBlock* loopBreaker) const
{
	//Check if this block exists.
	if(!startBlock) return NULL;
	if(startBlock == loopBreaker) return NULL;
	if(startBlock->getType() == blockType) return startBlock;

	//Iterate over inputs, until block is found.
	foreach(IConnection* connection, startBlock->getInConnections())
	{
		IBlock* firstBlock = findFirstInBlockByType(connection->getSource(), blockType, loopBreaker ? loopBreaker : startBlock);
		if(firstBlock)
			return firstBlock;
	}

	//No block found.
	return NULL;
}

bool GLSLVaoDeclarationCheck::isHandled(GLSLDeclaration* declaration) const
{
	//Must be in and with layout specification.
	return declaration->hasQualifier("in");
}

QList<VaoLayout::VaoLayoutEntry> GLSLVaoDeclarationCheck::beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(codeBlock)

	//Get the connected VAO block.
	_vaoBlock = findFirstInBlockByType(pipelineBlock, BlockType::VertexArrayObject);

	//Block must be vertex shader, VAO must exist.
	if(_vaoBlock && pipelineBlock->getType() == BlockType::Shader_Vertex)
	{
		//Convert layout from vector to list and return it.
		VaoLayout vaoLayout = _vaoBlock->getProperty<VaoLayoutProperty>(PropertyID::Vao_Layout)->getValue();
		return QList<VaoLayout::VaoLayoutEntry>::fromVector(*vaoLayout.getEntries());
	}

	//Otherwise return empty list.
	return QList<VaoLayout::VaoLayoutEntry>();
}

void GLSLVaoDeclarationCheck::extractElementData(VaoLayout::VaoLayoutEntry pipelineElement, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(pipelineElement)
	Q_UNUSED(codeBlock)
}

QString GLSLVaoDeclarationCheck::getElementName(VaoLayout::VaoLayoutEntry pipelineElement)
{
	//Return the VAO's name entry.
	return pipelineElement.name;
}

IGLSLPipelineCheck::FixedStatement GLSLVaoDeclarationCheck::getElementStatement(VaoLayout::VaoLayoutEntry pipelineElement) const
{
	//Create the code string.
	QString elementCode = QString("%1in %2 %3;")
			.arg(pipelineElement.index >= 0 ? QString("layout(location = %1) ").arg(pipelineElement.index) : "")
			.arg(GLTypes::getGLSLDataTypeNames()[pipelineElement.glslType])
			.arg(pipelineElement.name);

	//Return the single line code.
	return { QStringList() << elementCode, NULL, "Inputs" };
}

bool GLSLVaoDeclarationCheck::checkLocationIntegrity(VaoLayout::VaoLayoutEntry layoutEntry, GLSLDeclaration* declaration)
{
	//Get the code location and ensure it's correct.
	long codeLocation = declaration->hasLayoutParameter("location") ? declaration->getLayoutParameter("location").toInt() : -1l;
	if(codeLocation != layoutEntry.index)
	{
		declaration->addPipelineError(
		{
			QString("Location of '%1' (%2) does not match location in VAO block #%3 (%4).")
						.arg(layoutEntry.name)
						.arg(codeLocation >= 0 ? QString::number(codeLocation) : "not specified")
						.arg(_vaoBlock->getID())
						.arg(layoutEntry.index >= 0 ? QString::number(layoutEntry.index) : "not specified"),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}

bool GLSLVaoDeclarationCheck::checkTypeIntegrity(VaoLayout::VaoLayoutEntry layoutEntry, GLSLDeclaration* declaration)
{
	//Get the code type and ensure it's correct.
	QString codeType = declaration->getDataType();
	if(codeType != _glslTypes[layoutEntry.glslType])
	{
		declaration->addPipelineError(
		{
			QString("Type of '%1' (%2) does not match type in VAO block #%3 (%4).")
						.arg(layoutEntry.name)
						.arg(codeType)
						.arg(_vaoBlock->getID())
						.arg(_glslTypes[layoutEntry.glslType]),
			declaration,
			QList<IGLSLQuickFix*>(),
			NULL
		});

		return false;
	}

	//Successful check.
	return true;
}

