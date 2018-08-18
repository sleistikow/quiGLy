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

#ifndef UTILS_H
#define UTILS_H

#include <QVector>
#include <type_traits>

#include "data/types/types.h"

namespace ysm
{
	/**
	 * @brief Basic utility functions
	 */
	class Utils
	{
	public:		
		// String utilities
		/**
		 * @brief Encodes escape characters
		 */
		static QString encodeEscapeCharacters(QString str, bool includeQuotes = false);

		/**
		 * @brief Decodes escape characters
		 */
		static QString decodeEscapeCharacters(QString str, bool includeQuotes = false);

		/**
		 * @brief Converts string to array of float
		 */
		static bool convertStringToFloatArray(const QString& s, QVector<float> &result);

	private:
		explicit Utils();	
	};
}

#endif
