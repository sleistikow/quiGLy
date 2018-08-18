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

#include "glslstatementfactory.h"
#include "glslstatements/glslstatement.h"
#include "glslcodeblock.h"

#include <QDebug>

using namespace ysm;

GLSLStatementFactory::GLSLStatementFactory(QObject *parent) :
	QObject(parent)
{ }

GLSLStatement* GLSLStatementFactory::createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent)
{
	//Iterate over all known statement types.
	for(int i = 0; i < _statements.count(); i++)
	{
		//Try to create an instance of the statement type.
		GLSLStatement* instance = _statements[i](statement, codeDocument, parent);
		if(instance)
			return instance;
	}

	// qDebug() <<"Unparsed:" << statement;

	//No known statement type matches the statement.
	return new GLSLStatement(statement, codeDocument, parent);
}
