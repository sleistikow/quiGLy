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

#include "glslversiondirective.h"

#include <QDebug>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

using namespace ysm;

//A version directive has the following format: #version [:version:]
QRegularExpression* GLSLVersionDirective::_directiveExpression = NULL;

GLSLVersionDirective* GLSLVersionDirective::createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent)
{
	//Check, if the expression has been created.
	if(!_directiveExpression)
	{
		//Create the version expression, that catches GLSL version and possible addon.
		QString directivePattern =
			"^\\s*#version\\s*"
			"(?<version>[0-9]+)"
			"(?:\\s+(?<addon>.*))?\\s*$";

		//Create the expression.
		_directiveExpression = new QRegularExpression(directivePattern);
	}

	//Check if expression matches.
	QRegularExpressionMatch match = _directiveExpression->match(statement);
	if(match.hasMatch())
	{
		//Create declaration and store the data.
		GLSLVersionDirective* directive = new GLSLVersionDirective(statement, codeDocument, parent);
		directive->_version = match.captured("version").toInt();
		directive->_addon = match.captured("addon").trimmed();

		//Return the directive.
		return directive;
	}

	//Not matched.
	return NULL;
}

GLSLVersionDirective::GLSLVersionDirective(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent) :
	GLSLStatement(statement, codeDocument, parent)
{ }

GLSLStatementType GLSLVersionDirective::getType() const { return GLSLStatementType::VersionDirective; }
int GLSLVersionDirective::getVersion() const { return _version; }
QString GLSLVersionDirective::getAddon() const { return _addon; }
