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

#include "utils.h"

namespace ysm
{	
	QString Utils::encodeEscapeCharacters(QString str, bool includeQuotes)
	{
		str.replace("\\", "\\\\");
		str.replace("\n", "\\n");
		str.replace("\r", "\\r");
		str.replace("\t", "\\t");

		if (includeQuotes)
			str.replace("\"", "\\\"");

		return str;
	}

	QString Utils::decodeEscapeCharacters(QString str, bool includeQuotes)
	{
		QString	strOut;
		bool isEscape = false;

		for (int i = 0; i < str.length(); ++i)
		{
			QChar c = str[i];

			if (isEscape)
			{
				if (c == 'n')
					c = '\n';
				else if (c == 'r')
					c = '\r';
				else if (c == 't')
					c = '\t';
				else if (c == '\\')
					c = '\\';
				else if (c == '\"' && includeQuotes)
					c = '\"';

				strOut += c;
				isEscape = false;
			}
			else
			{
				if (c == '\\')
					isEscape = true;
				else
					strOut += c;
			}
		}

		return strOut;
	}

	bool Utils::convertStringToFloatArray(const QString& s, QVector<float> &result)
	{
		QStringList vals = s.split(';');

		for (QString fString : vals)
		{
			bool ok = false;
			float f = fString.toFloat(&ok);

			if (!ok)
				return false;

			result << f;
		}

		return true;
	}

	Utils::Utils()
	{

	}
}
