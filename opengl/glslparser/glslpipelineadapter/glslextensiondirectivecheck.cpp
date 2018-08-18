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

#include "glslextensiondirectivecheck.h"
#include "../glslstatements/glslextensiondirective.h"
#include "../glslcodeblock.h"
#include "../glsldocument.h"
#include "../glsldocumentinfo.h"

#include <QTextBlock>

using namespace ysm;

QList<IGLSLPipelineCheck::FixedStatement> GLSLExtensionDirectiveCheck::preCheck(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	Q_UNUSED(pipelineBlock);

	//Get all extension directives.
	QList<GLSLExtensionDirective*> extensionDirectives = codeBlock->getStatements<GLSLExtensionDirective>(GLSLStatementType::ExtensionDirective);

	//Remove all extension directives from code check and add them to the extension directive map.
	QMap<QString, QString> documentExtensions;
	foreach(GLSLExtensionDirective* extensionDirective, extensionDirectives)
	{
		//Remove the directive to readd it to fixed section.
		removeStatementCode(extensionDirective);
		documentExtensions[extensionDirective->getKey()] = extensionDirective->getValue();
	}

	//Store the document extensions.
	GLSLDocumentInfo* documentInfo = codeBlock->getDocument()->getDocumentInfo();
	documentInfo->setExtensions(documentExtensions);

	//Add all document extensions to the fixed section.
	QList<FixedStatement> fixedDirectives;
	foreach(QString key, documentExtensions.keys())
	{
		//Create new extension directive.
		QString fixedDirective = QString("#extension %1%3: %4%2")
			.arg(GLSLDocument::editableStart)
			.arg(GLSLDocument::editableEnd)
			.arg(key)
			.arg(documentExtensions[key]);

		//Append directive as single line.
		fixedDirectives.append({ QStringList() << fixedDirective, NULL, QString() });
	}

	return fixedDirectives;
}

void GLSLExtensionDirectiveCheck::postCheck(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
{
	//Pure data storage, no errors checked.
	Q_UNUSED(pipelineBlock)
	Q_UNUSED(codeBlock)
}
