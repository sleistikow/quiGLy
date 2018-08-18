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

#include "glsldocumentlistproperty.h"
#include "opengl/glslparser/glsldocument.h"
#include "opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h"

using namespace ysm;

GLSLDocumentListProperty::GLSLDocumentListProperty(const PropertyID id, const QString& name, bool isReadOnly) :
	Property<GLSLDocumentList, PropertyType::GLSLDocumentList>(id, name, isReadOnly)
{ }

void GLSLDocumentListProperty::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
{
	//Simply serialize base and value.
	base_type::serialize(xmlElement, ctx);
	_value.serialize(xmlElement, ctx);
}

void GLSLDocumentListProperty::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
{
	//Simply deserialize base and value.
	if (ctx->isProcessingDeferredElements())
	{
		//Deserialize the document's contents.
		_value.deserialize(xmlElement, ctx);

		//Iterate over all document's to update them.
		foreach(GLSLDocument* document, _value.getDocuments())
		{
			//Set the document's block and synchronize.
			document->getPipelineAdapter()->setBlock(dynamic_cast<IBlock*>(getOwner()));
			document->synchronizeDocument();

			//Check if the document should generate it's input blocks.
			if(_value.hasGenerateInput())
			{
				_value.generateInputBlocks(document->getFullCodeBlock());
				document->synchronizeDocument();
			}

			//Save the new document.
			document->saveDocument();
		}
	}
	else
		//Should be deserialized at the very ending, to allow correct parsing.
		ctx->deferElement(this, *xmlElement, SerializationContext::DeferringPriority::Lowest);
}

template<> QString Property<GLSLDocumentList, PropertyType::GLSLDocumentList>::toString() const { return QString(); }
template<> bool Property<GLSLDocumentList, PropertyType::GLSLDocumentList>::fromString(const QString& string) { Q_UNUSED(string); return true; }
