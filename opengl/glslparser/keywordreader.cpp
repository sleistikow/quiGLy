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

#include "keywordreader.h"
#include "views/logview/logview.h"

#include <QXmlReader>
#include <QXmlStreamReader>
#include <QDebug>

using namespace ysm;

KeywordReader::KeywordList KeywordReader::_keywords;

KeywordReader::KeywordList KeywordReader::KeywordList::ofShader(QString shader) const
{
	//Iterate over all keywords and create a list of the matches.
	KeywordList keywordList;
	foreach(Keyword keyword, *this)
		if(keyword.shader.isEmpty() || keyword.shader.contains(shader))
			keywordList.append(keyword);

	//Return the created list.
	return keywordList;
}

KeywordReader::KeywordList KeywordReader::KeywordList::ofType(QString type) const
{
	//Iterate over all keywords and create a list of the matches.
	KeywordList keywordList;
	foreach(Keyword keyword, *this)
		if(keyword.type == type)
			keywordList.append(keyword);

	//Return the created list.
	return keywordList;
}

KeywordReader::KeywordList KeywordReader::KeywordList::ofVersion(int version) const
{
	//Iterate over all keywords and create a list of the matches.
	KeywordList keywordList;
	foreach(Keyword keyword, *this)
		if((keyword.version.first == 0 || keyword.version.first <= version) &&
			(keyword.version.second == 0 || keyword.version.second >= version))
			keywordList.append(keyword);

	//Return the created list.
	return keywordList;
}

KeywordReader::KeywordList KeywordReader::KeywordList::ofShader(BlockType shader) const
{
	//Just use the overloaded method.
	return ofShader(getShader(shader));
}

KeywordReader::KeywordList KeywordReader::getKeywords() const
{
	//Check if keyword list has been loaded.
	if(_keywords.isEmpty())
	{
		//Try to load the XML file.
		QFile xmlFile(":/glsl/keywords.xml");
		if(xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			//Parse the XML.
			QXmlStreamReader xmlReader(&xmlFile);
			QString type;
			while(!xmlReader.error() && !xmlReader.isEndDocument())
			{
				//Read next tag, ensure starting tag.
				if(xmlReader.readNext() != QXmlStreamReader::StartElement)
					continue;

				//"dictionary" is simply ignored.
				if(xmlReader.name() == "dictionary")
					continue;

				//"type" stores the current type.
				if(xmlReader.name() == "type")
					type = xmlReader.attributes().value("name").toString();

				//"keyword" or "expression" adds a new keyword.
				if(xmlReader.name() == "keyword" || xmlReader.name() == "expression")
				{
					//Initialize keyword of corrent type.
					QXmlStreamAttributes attributes = xmlReader.attributes();
					Keyword keyword =
					{
						xmlReader.readElementText().trimmed(),
						type,
						QList<QString>(),
						QPair<int, int>(),
						xmlReader.name() == "expression"
					};

					//Fill remaining data.
					if(attributes.hasAttribute("shader"))
						keyword.shader = attributes.value("shader").toString().split('|', QString::SkipEmptyParts);
					if(attributes.hasAttribute("from"))
						keyword.version.first = attributes.value("from").toInt();
					if(attributes.hasAttribute("to"))
						keyword.version.second = attributes.value("to").toInt();

					//Store the keyword.
					if(!keyword.text.isEmpty())
						_keywords.append(keyword);
				}
			}

			//Check for XML error.
			if(xmlReader.hasError())
				LogView::log(QString("keywords.xml could not be parsed. No GLSL data available.\nLine %1: %2.")
							 .arg(xmlReader.lineNumber())
							 .arg(xmlReader.errorString()));

			//Cleanup.
			xmlReader.clear();
			xmlFile.close();
		}

		//XML file not available.
		else
			LogView::log("keywords.xml could not be opened. No GLSL data available.");
	}

	//Return the keyword list.
	return _keywords;
}


QString KeywordReader::getShader(BlockType blockType)
{
	//Find the shader name for the given block type.
	switch(blockType)
	{
	case BlockType::Shader_Vertex: return GLSL_SHADER_VERTEX;
	case BlockType::Shader_Fragment: return GLSL_SHADER_FRAGMENT;
	case BlockType::Shader_Geometry: return GLSL_SHADER_GEOMETRY;
	case BlockType::Shader_TessellationControl: return GLSL_SHADER_TESSELLATION_CONTROL;
	case BlockType::Shader_TessellationEvaluation: return GLSL_SHADER_TESSELLATION_EVALUATION;
	default: return QString();
	}
}
