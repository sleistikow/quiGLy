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

#include "glsldocumentinfo.h"
#include "glsldocument.h"

using namespace ysm;

GLSLDocumentInfo::GLSLDocumentInfo(GLSLDocument* parent) :
	_document(parent),
	_version(0, QString())
{ }

QPair<int, QString> GLSLDocumentInfo::getVersion() const { return _version; }
void GLSLDocumentInfo::setVersion(QPair<int, QString> version) { _version = version; }

QMap<QString, QString> GLSLDocumentInfo::getExtensions() const { return _extensions; }
void GLSLDocumentInfo::setExtensions(QMap<QString, QString> extensions) { _extensions = extensions; }

QMap<int, GLSLDeclaration*> GLSLDocumentInfo::getDeclarations() const { return _declarations; }
void GLSLDocumentInfo::setDeclarations(QMap<int, GLSLDeclaration *> declarations) { _declarations = declarations; }
