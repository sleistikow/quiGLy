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

#include "glslversiondirectivecheck.h"
#include "../glslcodeblock.h"
#include "../glsldocument.h"
#include "../glsldocumentinfo.h"
#include "../glslstatements/glslversiondirective.h"
#include "data/iblock.h"
#include "data/ipipeline.h"

#include <QTextBlock>

using namespace ysm;

QList<IGLSLPipelineCheck::FixedStatement> GLSLVersionDirectiveCheck::preCheck(IBlock* pipelineBlock,
																			  GLSLCodeBlock* codeBlock)
{
	//Validate the version directive, must only be declared once.
	GLSLDocumentInfo* documentInfo = codeBlock->getDocument()->getDocumentInfo();
	QList<GLSLVersionDirective*> versionDirectives =
			codeBlock->getStatements<GLSLVersionDirective>(GLSLStatementType::VersionDirective);

	//Retrieve the current version data.
	int glslVersion = 0, openGLVersion = pipelineBlock->getPipeline()->getOpenGLVersion();
	QString glslAddon;

	//No version directive, use the document version.
	if(versionDirectives.empty())
	{
		glslAddon = documentInfo->getVersion().second;
		glslVersion = documentInfo->getVersion().first;

		//No document version set, use project version.
		if(!glslVersion)
			glslVersion = openGLVersion;
	}

	//Use first version directive, mark others as obsolete.
	else
	{
		//Get version and addon and store to document.
		glslVersion = versionDirectives.first()->getVersion();
		glslAddon = versionDirectives.first()->getAddon();
		documentInfo->setVersion(QPair<int, QString>(glslVersion, glslAddon));

		//Remove the first version directive, it's added to the fixed section.
		removeStatementCode(versionDirectives.first());
	}

	//Adjust the document addon. Add single whitespace in front, if addon is specified.
	if(!glslAddon.isEmpty())
		glslAddon.insert(0, " ");

	//Create the single fixed directive.
	QString fixedDirective = QString("#version %1%3%4%2")
		.arg(GLSLDocument::editableStart)
		.arg(GLSLDocument::editableEnd)
		.arg(glslVersion)
		.arg(glslAddon);

	//Return the single directive.
	FixedStatement fixedStatement = { QStringList() << fixedDirective, NULL, QString() };
	return QList<FixedStatement>() << fixedStatement;
}

void GLSLVersionDirectiveCheck::postCheck(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	//Get list of all version directives.
	int openGLVersion = pipelineBlock->getPipeline()->getOpenGLVersion();
	QList<GLSLVersionDirective*> versionDirectives =
			codeBlock->getStatements<GLSLVersionDirective>(GLSLStatementType::VersionDirective);

	//Mark all version directives as obsolete, if there's more than one.
	if(versionDirectives.count() != 1)
		foreach(GLSLVersionDirective* versionDirective, versionDirectives)
			versionDirective->addPipelineError(
			{
				QString("Version directive must be declared exactly once."),
				versionDirective,
				QList<IGLSLQuickFix*>(),
				NULL
			});

	//Mark all version directives, that are higher than supported version.
	foreach(GLSLVersionDirective* versionDirective, versionDirectives)
		if(openGLVersion < versionDirective->getVersion())
			versionDirective->addPipelineError(
			{
				QString("Project version (%1) does not support GLSL version (%2).")
							.arg(openGLVersion)
							.arg(versionDirective->getVersion()),
				versionDirective,
				QList<IGLSLQuickFix*>(),
				NULL
			});
}

