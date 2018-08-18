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

#include "typeutils.h"

#include <QOpenGLFunctions>

namespace ysm
{
	DataTypeUtils::DataTypeUtils()
	{

	}

	unsigned int DataTypeUtils::getTypeSize(DataType type, bool useGLTypes)
	{
		switch (type)
		{
		case DataType::Vec2:
			if (useGLTypes)
				return sizeof(GLfloat) * 2;
			else
				return sizeof(float) * 2;

		case DataType::Vec3:
			if (useGLTypes)
				return sizeof(GLfloat) * 3;
			else
				return sizeof(float) * 3;

		case DataType::Vec4:
			if (useGLTypes)
				return sizeof(GLfloat) * 4;
			else
				return sizeof(float) * 4;

		case DataType::Int:
			if (useGLTypes)
				return sizeof(GLint);
			else
				return sizeof(int);

		case DataType::UInt:
			if (useGLTypes)
				return sizeof(GLuint);
			else
				return sizeof(unsigned int);

		case DataType::Float:
			if (useGLTypes)
				return sizeof(GLfloat);
			else
				return sizeof(float);

		default:
			break;
		}

		return 0;
	}

	unsigned int DataTypeUtils::getTypeComponentCount(DataType type)
	{
		switch (type)
		{
		case DataType::NoType:
			return 0;

		case DataType::Vec2:
			return 2;

		case DataType::Vec3:
			return 3;

		case DataType::Vec4:
			return 4;

		default:
			return 1;
		}
	}
}
