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

#ifndef KEYWORDREADER_H
#define KEYWORDREADER_H

#include "data/blocks/blocktype.h"

#include <QString>
#include <QMap>
#include <QXmlReader>
#include <QXmlStreamReader>

namespace ysm
{
	//! Define the GLSL keyword types.
	const QString GLSL_KEYWORDS_BUILTIN = "builtin";
	const QString GLSL_KEYWORDS_CONSTANT = "constant";
	const QString GLSL_KEYWORDS_DATATYPES = "datatype";
	const QString GLSL_KEYWORDS_FUNCTIONS = "function";
	const QString GLSL_KEYWORDS_QUALIFIERS = "qualifier";
	const QString GLSL_KEYWORDS_STRUCTURAL = "structural";

	//! Define the GLSL shader types.
	const QString GLSL_SHADER_VERTEX = "vertex";
	const QString GLSL_SHADER_FRAGMENT = "fragment";
	const QString GLSL_SHADER_GEOMETRY = "geometry";
	const QString GLSL_SHADER_TESSELLATION_CONTROL = "tcontrol";
	const QString GLSL_SHADER_TESSELLATION_EVALUATION = "tevaluation";

	//! \brief Holds information about all keywords.
	class KeywordReader
	{
	public:

		//! \brief Contains brief data about a keyword.
		struct Keyword
		{
			//! \brief The text.
			QString text;

			//! \brief The keyword type.
			QString type;

			//! \brief Shaders that support this keyword.
			QStringList shader;

			//! \brief The minimum and maximum version.
			QPair<int, int> version;

			//! \brief True, if keyword is regex.
			bool isRegex;
		};

		//! \brief
		class KeywordList : public QList<Keyword>
		{
		public:
			/*!
			 * \brief Gets all keywords of the given type.
			 * \param type The type (see above, GLSL_KEYWORDS_*).
			 * \return The keywords.
			 */
			KeywordList ofType(QString type) const;

			/*!
			 * \brief Gets all keywords of the given shader.
			 * \param shader The shader as block type.
			 * \return The keywords.
			 */
			KeywordList ofShader(BlockType shader) const;

			/*!
			 * \brief Gets all keywords of the given shader.
			 * \param shader The shader (see above, GLSL_SHADER_*).
			 * \return The keywords.
			 */
			KeywordList ofShader(QString shader) const;

			/*!
			 * \brief Gets all keywords of the given version.
			 * \param version The version.
			 * \return The keywords.
			 */
			KeywordList ofVersion(int version) const;
		};

	public:

		/*!
		 * \brief Gets the shader type, as required for the keyword reader.
		 * \return The shader type as string.
		 */
		static QString getShader(BlockType blockType);

		/*!
		 * \brief Gets all keywords.
		 * \return The keywords.
		 */
		KeywordList getKeywords() const;

	private:

		//! \brief Includes all keywords.
		static KeywordList _keywords;

	};

}

#endif // KEYWORDREADER_H
