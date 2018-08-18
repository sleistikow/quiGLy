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

#ifndef GLTYPES_H
#define GLTYPES_H

#include "types.h"

#include <QMap>

namespace ysm
{
	// OpenGL data types

	enum class GLDataType
	{
		NoType = 0,
		Byte,
		UByte,
		Short,
		UShort,
		Int,
		UInt,
		HalfFloat,
		Float,
		Double,
		Fixed,
	};

	enum class GLSLDataType
	{
		NoType = 0,
		Bool,
		Int,
		UInt,
		Float,
		Double,
		BVec2,
		BVec3,
		BVec4,
		IVec2,
		IVec3,
		IVec4,
		UVec2,
		UVec3,
		UVec4,
		Vec2,
		Vec3,
		Vec4,
		DVec2,
		DVec3,
		DVec4,
		Mat2,
		Mat2x3,
		Mat2x4,
		Mat3x2,
		Mat3,
		Mat3x4,
		Mat4x2,
		Mat4x3,
		Mat4,
	};

	/**
	 * @brief Helper class for various OpenGL types
	 */
	class GLTypes
	{
	public:
		/**
		 * @brief Gets a list of all type names
		 */
		static const QMap<GLDataType, QString>& getDataTypeNames();

		/**
		 * @brief Gets a list of all GL type names
		 */
		static const QMap<GLSLDataType, QString>& getGLSLDataTypeNames();

		/**
		 * @brief Converts the given datat type to its corresponding GL types
		 */
		static void convertDataTypeToGLTypes(DataType dataType, GLDataType& glType, int& count, GLSLDataType& glslType);

	private:
		explicit GLTypes();

	private:
		static void fillDataTypeNames();
		static void fillGLSLDataTypeNames();

		static QMap<GLDataType, QString> _dataTypeNames;
		static QMap<GLSLDataType, QString> _glslDataTypeNames;
	};
}

#endif
