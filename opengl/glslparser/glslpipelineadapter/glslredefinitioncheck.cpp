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

#include "glslredefinitioncheck.h"
#include "../glslstatements/glsldeclaration.h"
#include "../glslcodeblock.h"

using namespace ysm;

QList<IGLSLPipelineCheck::FixedStatement> GLSLRedefinitionCheck::preCheck(IBlock *pipelineBlock, GLSLCodeBlock *codeBlock)
{
	//Pure error check, no data stored.
	Q_UNUSED(pipelineBlock)
	Q_UNUSED(codeBlock)

	return QList<FixedStatement>();
}

void GLSLRedefinitionCheck::postCheck(IBlock *pipelineBlock, GLSLCodeBlock *codeBlock)
{
	Q_UNUSED(pipelineBlock)

	//Recursively check all blocks.
	checkCodeBlock(codeBlock, QMap<QString, GLSLDeclaration*>());
}

void GLSLRedefinitionCheck::checkCodeBlock(GLSLCodeBlock* codeBlock, QMap<QString, GLSLDeclaration*> parentDeclarations)
{
	//Iterate over all block declarations, and check for redefinitions.
	foreach(GLSLDeclaration* declaration, codeBlock->getStatements<GLSLDeclaration>(GLSLStatementType::Declaration))
	{
		//Check if the declaration is a redeclaration, otherwise just note it.
		if(parentDeclarations.contains(declaration->getName()))
			declaration->addPipelineError(
			{
				QString("'%1' already declared as '%2'.")
						 .arg(declaration->getName())
						 .arg(parentDeclarations[declaration->getName()]->getText()),
				declaration,
				QList<IGLSLQuickFix*>(),
				NULL
			});
		else
			parentDeclarations[declaration->getName()] = declaration;
	}

	//Run all child block checks. Declaration blocks are independent of parent declarations, because they define members
	//instead of actual variables. Same for struct statements, which cannot be parsed, yet, though.
	foreach(GLSLCodeBlock* childBlock, codeBlock->getChildBlocks())
	{
		if(childBlock->getParentStatement() && childBlock->getParentStatement()->getType() == GLSLStatementType::Declaration)
			checkCodeBlock(childBlock, QMap<QString, GLSLDeclaration*>());

		//TODO: Run actual child block checks, which would require to parse structural elements (structs).
//		else
//			checkCodeBlock(childBlock, parentDeclarations);
	}
}
