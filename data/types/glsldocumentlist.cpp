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

#include "glsldocumentlist.h"
#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"
#include "data/common/serializationcontext.h"

using namespace ysm;

GLSLDocumentList::GLSLDocumentList() : _generateInput(false) { }
GLSLDocumentList::GLSLDocumentList(const GLSLDocumentList& src) :
	_generateInput(src._generateInput)
{
	//Copy the documents and retain them.
	_documents = src._documents;
	foreach(GLSLDocument* document, _documents)
		document->retain();
}

GLSLDocumentList::~GLSLDocumentList()
{
	//Release all existing documents.
	foreach(GLSLDocument* document, _documents)
		document->release();
}

GLSLDocumentList& GLSLDocumentList::operator =(const GLSLDocumentList& src)
{
	//Copy the documents and retain them.
	_documents = src._documents;
	foreach(GLSLDocument* document, _documents)
		document->retain();

	//Return copied reference.
	return *this;
}

bool GLSLDocumentList::operator !=(const GLSLDocumentList& src) const { return !(*this == src); }
bool GLSLDocumentList::operator ==(const GLSLDocumentList& src) const
{
	//Number of documents must match.
	if (_documents.size() != src._documents.size())
		return false;

	//Documents must match.
	for (int i = 0; i < _documents.size(); ++i)
		if(_documents[i] != src._documents[i])
			return false;

	//All documents match.
	return true;
}

void GLSLDocumentList::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
{
	//Iterate over all documents and store their plain text.
	foreach(const GLSLDocument* document, _documents)
	{
		QDomElement elem = ctx->createElement("GLSLDocument");
		elem.appendChild(ctx->createTextElement(document->getSavedContent()));
		xmlElement->appendChild(elem);
	}
}

void GLSLDocumentList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
{
	//Release all existing documents.
	foreach(GLSLDocument* document, _documents)
		document->release();

	//Clear the current document list.
	_documents.clear();

	//Iterate over all documents and restore their plain text.
	foreach(const QDomElement& elem, ctx->getElements(xmlElement, "GLSLDocument"))
	{
		//Add the document using default add method.
		GLSLDocument* document = addDocument();

		//Check wether input should be generated.
		_generateInput = elem.attribute("generateInput") == "true";

		//LEGACY: Check wether the element has it's content specified as text or attribute.
		QString documentContent = elem.text();
		if(documentContent.isEmpty())
			documentContent = elem.attribute("content");

		//Set the saved content and load it by reverting to saved state.
		//Further deserialization is done in the property's deserialization.
		document->setSavedContent(documentContent);
		document->revertDocument();
	}
}

bool GLSLDocumentList::hasGenerateInput() const { return _generateInput; }
void GLSLDocumentList::generateInputBlocks(GLSLCodeBlock* codeBlock)
{
	//Iterate over all statements and let them generate their needs.
	foreach(GLSLStatement* statement, codeBlock->getStatements())
		foreach(GLSLStatementError error, statement->getErrors())
			if(error.generateInputs)
				error.generateInputs->execute(statement);

	//Recursively operate on the child blocks.
	foreach(GLSLCodeBlock* childBlock, codeBlock->getChildBlocks())
		generateInputBlocks(childBlock);
}

QVector<GLSLDocument*> GLSLDocumentList::getDocuments() const { return _documents; }

void GLSLDocumentList::removeDocument(int documentIndex)
{
	//Remove the document and release it.
	GLSLDocument* document = _documents[documentIndex];
	_documents.remove(documentIndex);
	document->release();
}

void GLSLDocumentList::removeDocument(GLSLDocument* document)
{
	//Remove the document and release it.
	_documents.removeOne(document);
	document->release();
}

GLSLDocument* GLSLDocumentList::addDocument(GLSLDocument* document, int index)
{
	//Check if document needs to be created.
	if(!document)
		document = new GLSLDocument();

	//Use insert or append depending on index.
	if(index < 0)
		_documents.append(document);
	else
		_documents.insert(index, document);

	//Retain the document, so it's not deleted.
	document->retain();

	//Return the new document.
	return document;
}

GLSLDocument* GLSLDocumentList::addDocument(QString plainText, IBlock* targetBlock, int index)
{
	//Create the new document.
	GLSLDocument* document = addDocument(NULL, index);

	//Set the provided content.
	document->setSavedContent(plainText);
	document->revertDocument();

	//Connect to block and synchronize.
	document->getPipelineAdapter()->setBlock(targetBlock);
	document->synchronizeDocument();
	document->saveDocument();

	//Return the new document.
	return document;
}


