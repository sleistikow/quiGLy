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

#include "glsldeclaration.h"
#include "../keywordreader.h"
#include "../glslcodeblock.h"

#include <QDebug>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

using namespace ysm;

//A declaration has the following format: [:qualifier:]* [:datatype:] [:name:] (= [:value:])?
QRegularExpression* GLSLDeclaration::_declarationExpression = NULL;
QRegularExpression* GLSLDeclaration::_structureExpression = NULL;
QRegularExpression* GLSLDeclaration::_layoutExpression = NULL;

QMap<QString, TextureBlock::SamplerType> GLSLDeclaration::TEXTURE_DATA_TYPES;
QMap<QString, QList<BlockType>> GLSLDeclaration::UNIFORM_DATA_TYPES;

GLSLDeclaration* GLSLDeclaration::createStatement(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent)
{
	//Initialize data types if neccessary.
	if(UNIFORM_DATA_TYPES.isEmpty())
	{
		UNIFORM_DATA_TYPES["double"] << BlockType::Uniform_Double;
		UNIFORM_DATA_TYPES["float"] << BlockType::Uniform_Float;
		UNIFORM_DATA_TYPES["int"] << BlockType::Uniform_Int;
		UNIFORM_DATA_TYPES["int"] << BlockType::Uniform_ElapsedTime;
		UNIFORM_DATA_TYPES["uint"] << BlockType::Uniform_UInt;

		UNIFORM_DATA_TYPES["vec2"] << BlockType::Uniform_Vec2;
		UNIFORM_DATA_TYPES["vec3"] << BlockType::Uniform_Vec3;
		UNIFORM_DATA_TYPES["vec4"] << BlockType::Uniform_Vec4;

		UNIFORM_DATA_TYPES["mat3"] << BlockType::Uniform_Mat3x3;
		UNIFORM_DATA_TYPES["mat3"] << BlockType::ModelViewProjection;
		UNIFORM_DATA_TYPES["mat3x3"] << BlockType::Uniform_Mat3x3;
		UNIFORM_DATA_TYPES["mat3x3"] << BlockType::ModelViewProjection;
		UNIFORM_DATA_TYPES["mat4"] << BlockType::Uniform_Mat4x4;
		UNIFORM_DATA_TYPES["mat4"] << BlockType::ModelViewProjection;
		UNIFORM_DATA_TYPES["mat4x4"] << BlockType::Uniform_Mat4x4;
		UNIFORM_DATA_TYPES["mat4x4"] << BlockType::ModelViewProjection;
	}

	//Initialize data types if neccessary.
	if(TEXTURE_DATA_TYPES.isEmpty())
	{
		TEXTURE_DATA_TYPES["sampler1D"] = TextureBlock::Sampler_Sampler1D;
		TEXTURE_DATA_TYPES["sampler1DArray"] = TextureBlock::Sampler_Sampler1DArray;
		TEXTURE_DATA_TYPES["sampler1DArrayShadow"] = TextureBlock::Sampler_Sampler1DArrayShadow;
		TEXTURE_DATA_TYPES["sampler1DShadow"] = TextureBlock::Sampler_Sampler1DShadow;
		TEXTURE_DATA_TYPES["sampler2D"] = TextureBlock::Sampler_Sampler2D;
		TEXTURE_DATA_TYPES["sampler2DArray"] = TextureBlock::Sampler_Sampler2DArray;
		TEXTURE_DATA_TYPES["sampler2DArrayShadow"] = TextureBlock::Sampler_Sampler2DArrayShadow;
		TEXTURE_DATA_TYPES["sampler2DMS"] = TextureBlock::Sampler_Sampler2DMS;
		TEXTURE_DATA_TYPES["sampler2DMSArray"] = TextureBlock::Sampler_Sampler2DMSArray;
		TEXTURE_DATA_TYPES["sampler2DRect"] = TextureBlock::Sampler_Sampler2DRect;
		TEXTURE_DATA_TYPES["sampler2DRectShadow"] = TextureBlock::Sampler_Sampler2DRectShadow;
		TEXTURE_DATA_TYPES["sampler2DShadow"] = TextureBlock::Sampler_Sampler2DShadow;
		TEXTURE_DATA_TYPES["sampler3D"] = TextureBlock::Sampler_Sampler3D;
		TEXTURE_DATA_TYPES["samplerBuffer"] = TextureBlock::Sampler_SamplerBuffer;
		TEXTURE_DATA_TYPES["samplerCube"] = TextureBlock::Sampler_SamplerCube;
		TEXTURE_DATA_TYPES["samplerCubeArray"] = TextureBlock::Sampler_SamplerCubeArray;
		TEXTURE_DATA_TYPES["samplerCubeArrayShadow"] = TextureBlock::Sampler_SamplerCubeArrayShadow;
		TEXTURE_DATA_TYPES["samplerCubeShadow"] = TextureBlock::Sampler_SamplerCubeShadow;
	}

	//Check, if the expression has been created.
	if(!_declarationExpression)
	{
		//Get GLSL datatypes and qualifiers.
		KeywordReader::KeywordList datatypes = KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_DATATYPES);
		KeywordReader::KeywordList qualifiers = KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_QUALIFIERS);

		//Start expression with layout filter (layout is parsed manually), and open qualifiers group.
		QString declarationPattern =
			"^\\s*(?:layout\\s*\\("
			"(?<layout>.*)\\)\\s*)?"
			"(?<qualifiers>(?:";

		//Append qualifiers by any count.
		foreach(KeywordReader::Keyword qualifier, qualifiers)
			declarationPattern += "(?:" + qualifier.text + "\\s+)|";

		//Cut last pipe and append multiplier.
		declarationPattern = declarationPattern.left(declarationPattern.length() - 1) + ")*)(?<datatype>";

		//Append single data type.
		foreach(KeywordReader::Keyword datatype, datatypes)
			declarationPattern += "(?:" + datatype.text + ")|";

		//Cut last pipe and append GLSL naming expression, as well as array expression.
		declarationPattern =
			declarationPattern.left(declarationPattern.length() - 1) + ")\\s*"
			"(?<array1>(\\[[0-9]*\\])*)\\s+"
			"(?<name>[A-Za-z_][A-Za-z0-9_]*)\\s*"
			"(?<array2>(\\[[0-9]*\\])*)\\s*"
			"(?:(?<parameters>\\(.*\\))|=.*)?\\s*$";

		//Create the expression.
		_declarationExpression = new QRegularExpression(declarationPattern);
	}

	//Check, if the expression has been created.
	if(!_structureExpression)
	{
		//Get GLSL qualifiers only.
		KeywordReader::KeywordList qualifiers = KeywordReader().getKeywords().ofType(GLSL_KEYWORDS_QUALIFIERS);

		//Start expression with layout filter (layout is parsed manually), and open qualifiers group.
		QString declarationPattern =
			"^\\s*(?:layout\\s*\\("
			"(?<layout>.*)\\)\\s*)?"
			"(?<qualifiers>(?:";

		//Append qualifiers by any count.
		foreach(KeywordReader::Keyword qualifier, qualifiers)
			declarationPattern += "(?:" + qualifier.text + "\\s+)|";

		//Cut last pipe and append multiplier and GLSL naming expression, as well as array expression.
		declarationPattern =
			declarationPattern.left(declarationPattern.length() - 1) + ")*)\\s*"
			"(?<name>[A-Za-z_][A-Za-z0-9_]*)\\s*"
			"(?<array>(\\[[0-9]*\\])*)\\s*$";

		//Create the expression.
		_structureExpression = new QRegularExpression(declarationPattern);
	}

	//Check, if the expression has been created.
	if(!_layoutExpression)
		_layoutExpression = new QRegularExpression("([A-Za-z0-9_]+)(?:\\s*=\\s*([^,^\\s]))?");

	//Ensure datatype is specified.
	QRegularExpressionMatch match = _declarationExpression->match(statement);
	if(match.hasMatch())
	{
		//Create declaration and store the data.
		GLSLDeclaration* declaration = new GLSLDeclaration(statement, codeDocument, parent);

		//Store layout qualifiers.
		QRegularExpressionMatchIterator layoutMatches = _layoutExpression->globalMatch(match.captured("layout"));
		while(layoutMatches.hasNext())
		{
			//Get the next match, check if has value.
			QRegularExpressionMatch layoutMatch = layoutMatches.next();
			declaration->_layout[layoutMatch.captured(1)] = layoutMatch.captured(2);
		}

		//Store other data.
		declaration->_qualifiers = match.captured("qualifiers").split(QRegularExpression("\\s"),
																	  QString::SkipEmptyParts);
		declaration->_datatype = match.captured("datatype");
		declaration->_name = match.captured("name");
		declaration->_method = !match.captured("parameters").isEmpty();
		declaration->_array = match.captured("array1");
		if(declaration->_array.isEmpty())
			declaration->_array = match.captured("array2");


		return declaration;
	}

	//No datatype specified.
	match = _structureExpression->match(statement);
	if(match.hasMatch())
	{
		//Create declaration and store the data.
		GLSLDeclaration* declaration = new GLSLDeclaration(statement, codeDocument, parent);

		//Store layout qualifiers.
		QRegularExpressionMatchIterator layoutMatches = _layoutExpression->globalMatch(match.captured("layout"));
		while(layoutMatches.hasNext())
		{
			//Get the next match, check if has value.
			QRegularExpressionMatch layoutMatch = layoutMatches.next();
			declaration->_layout[layoutMatch.captured(1)] = layoutMatch.captured(2);
		}

		//Store other data.
		declaration->_datatype = QString();
		declaration->_method = false;
		declaration->_name = match.captured("name");
		declaration->_array = match.captured("array");
		declaration->_qualifiers = match.captured("qualifiers").split(QRegularExpression("\\s"),
																	  QString::SkipEmptyParts);

		return declaration;
	}

	//Not matched.
	return NULL;
}

GLSLDeclaration::GLSLDeclaration(QString statement, GLSLDocument* codeDocument, GLSLCodeBlock* parent) :
	GLSLStatement(statement, codeDocument, parent)
{ }

bool GLSLDeclaration::hasLayoutParameter(QString layoutParameter) const { return _layout.contains(layoutParameter); }
QString GLSLDeclaration::getLayoutParameter(QString layoutParameter) const
{
	//Ensure parameter exists, then return.
	if(hasLayoutParameter(layoutParameter))
		return _layout[layoutParameter];
	return QString();
}

bool GLSLDeclaration::isGlobalScope() const
{
	//Check if block has root block.
	GLSLCodeBlock* parentBlock = qobject_cast<GLSLCodeBlock*>(parent());
	if(!parentBlock || !parentBlock->getParentBlock())
		return true;

	//Check if parent block is structural body without instance declaration.
	if(parentBlock->getParentStatement() && parentBlock->getParentStatement()->getType() == GLSLStatementType::Declaration)
	{
		GLSLDeclaration* parentDeclaration = qobject_cast<GLSLDeclaration*>(parentBlock->getParentStatement());
		if(parentDeclaration && !parentDeclaration->isMethod() && !parentDeclaration->getInstanceDeclaration())
			return true;
	}

	//Not at global scope.
	return false;
}

GLSLDeclaration* GLSLDeclaration::getInstanceDeclaration() const
{
	//Check wether this declaration has a body and is followed by another declaration. If so, the following declaration
	//specifies the instance name.
	if(getBody() && getNextStatement() && getNextStatement()->getType() == GLSLStatementType::Declaration)
		return qobject_cast<GLSLDeclaration*>(getNextStatement());

	//No instance declaration available.
	return NULL;
}

GLSLDeclaration* GLSLDeclaration::getStructureDeclaration() const
{
	//Check wether this declaration is preceeded by another declaration with body. If so, the preceeding declaration
	//specifies pipeline name and structure.
	if(getPreviousStatement() && getPreviousStatement()->getType() == GLSLStatementType::Declaration && getPreviousStatement()->getBody())
		return qobject_cast<GLSLDeclaration*>(getPreviousStatement());

	//No instance declaration available.
	return NULL;
}

QList<GLSLStatementError> GLSLDeclaration::getGLSLErrors()
{
	//Create the error list.
	QList<GLSLStatementError> internalErrors;

	//Run variable checks.
	if(!_method)
	{
		//Require either body or datatype or structure declaration.
		if((getBody() ? 1 : 0) + !_datatype.isEmpty() + (getStructureDeclaration() ? 1 : 0) != 1)
			internalErrors.append(
			{
				QString("%1 must declare either data type or complex data structure body.").arg(getName()),
				this,
				QList<IGLSLQuickFix*>(),
				NULL,
			});

		//Require one or zero storage qualifiers.
		if(hasQualifier("in") + hasQualifier("out") + hasQualifier("uniform") + hasQualifier("buffer") + hasQualifier("shared") > 1)
			internalErrors.append(
			{
				QString("%1 must not specify more than one storage qualifier.").arg(getName()),
				this,
				QList<IGLSLQuickFix*>(),
				NULL,
			});
	}

	//If inside another block, only variable declarations allowed.
	GLSLCodeBlock* parentBlock = qobject_cast<GLSLCodeBlock*>(parent());
	if((parentBlock && parentBlock->getParentBlock()) && (getBody() || _method || !_qualifiers.isEmpty()))
		internalErrors.append(
		{
			QString("Declaration of %1 not allowed inside block.").arg(getName()),
			this,
			QList<IGLSLQuickFix*>(),
			NULL,
		});

	//Return the error list.
	return internalErrors;
}

bool GLSLDeclaration::hasQualifier(QString qualifier) const { return _qualifiers.contains(qualifier); }
bool GLSLDeclaration::hasQualifiers() const { return !_qualifiers.isEmpty(); }
bool GLSLDeclaration::hasLayout() const { return !_layout.isEmpty(); }
QString GLSLDeclaration::getName() const { return _name; }
QString GLSLDeclaration::getDataType() const { return _datatype; }
QString GLSLDeclaration::getArray() const { return _array; }
bool GLSLDeclaration::isMethod() const { return _method; }
bool GLSLDeclaration::isStructural() const { return _datatype.isEmpty() && getBody(); }
bool GLSLDeclaration::hasTextureType() const { return TEXTURE_DATA_TYPES.contains(_datatype); }
bool GLSLDeclaration::hasUniformType() const { return UNIFORM_DATA_TYPES.contains(_datatype); }
GLSLStatementType GLSLDeclaration::getType() const { return GLSLStatementType::Declaration; }

