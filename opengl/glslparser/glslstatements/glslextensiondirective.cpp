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

#include "glslextensiondirective.h"
#include "../glsldocument.h"

#include <QDebug>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

using namespace ysm;

//A version directive has the following format: #version [:version:] [:whatever:]
QRegularExpression* GLSLExtensionDirective::_directiveExpression = NULL;

GLSLExtensionDirective* GLSLExtensionDirective::createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent)
{
	//Check, if the expression has been created.
	if(!_directiveExpression)
	{
		//Create the version expression.
		QString directivePattern = QString(
			"^\\s*#extension\\s*"
			"(?<key>[A-Za-z0-9_]+)"
			"\\s*:\\s*"
			"(?<value>[A-Za-z0-9_]+)\\s*$");

		//Create the expression.
		_directiveExpression = new QRegularExpression(directivePattern);
	}

	//Check if expression matches.
	QRegularExpressionMatch match = _directiveExpression->match(statement);
	if(match.hasMatch())
	{
		//Create declaration and store the data.
		GLSLExtensionDirective* directive = new GLSLExtensionDirective(statement, codeDocument, parent);
		directive->_key = match.captured("key");
		directive->_value = match.captured("value");

		//Return the directive.
		return directive;
	}

	//Not matched.
	return NULL;
}

GLSLExtensionDirective::GLSLExtensionDirective(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent) :
	GLSLStatement(statement, codeDocument, parent)
{ }

QString GLSLExtensionDirective::getKey() const { return _key; }
QString GLSLExtensionDirective::getValue() const { return _value; }

GLSLStatementType GLSLExtensionDirective::getType() const { return GLSLStatementType::ExtensionDirective; }
