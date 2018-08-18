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

#include "gltypes.h"
#include "typeutils.h"

namespace ysm
{
	QMap<GLDataType, QString> GLTypes::_dataTypeNames;
	QMap<GLSLDataType, QString> GLTypes::_glslDataTypeNames;

	const QMap<GLDataType, QString>&GLTypes::getDataTypeNames()
	{
		if (_dataTypeNames.isEmpty())
			fillDataTypeNames();

		return _dataTypeNames;
	}

	const QMap<GLSLDataType, QString>&GLTypes::getGLSLDataTypeNames()
	{
		if (_glslDataTypeNames.isEmpty())
			fillGLSLDataTypeNames();

		return _glslDataTypeNames;
	}

	void GLTypes::convertDataTypeToGLTypes(DataType dataType, GLDataType& glType, int& count, GLSLDataType& glslType)
	{
		count = DataTypeUtils::getTypeComponentCount(dataType);

		switch (dataType)
		{
		case DataType::Vec2:
			glType = GLDataType::Float;
			glslType = GLSLDataType::Vec2;
			break;

		case DataType::Vec3:
			glType = GLDataType::Float;
			glslType = GLSLDataType::Vec3;
			break;

		case DataType::Vec4:
			glType = GLDataType::Float;
			glslType = GLSLDataType::Vec4;
			break;

		case DataType::Int:
			glType = GLDataType::Int;
			glslType = GLSLDataType::Int;
			break;

		case DataType::UInt:
			glType = GLDataType::UInt;
			glslType = GLSLDataType::UInt;
			break;

		case DataType::Float:
			glType = GLDataType::Float;
			glslType = GLSLDataType::Float;
			break;

		default:
			glType = GLDataType::NoType;
			glslType = GLSLDataType::NoType;
		}
	}

	GLTypes::GLTypes()
	{

	}

	void GLTypes::fillGLSLDataTypeNames()
	{
		// Fill out all GLSL data type names
		_glslDataTypeNames[GLSLDataType::Bool] = "bool";
		_glslDataTypeNames[GLSLDataType::Int] = "int";
		_glslDataTypeNames[GLSLDataType::UInt] = "uint";
		_glslDataTypeNames[GLSLDataType::Float] = "float";
		_glslDataTypeNames[GLSLDataType::Double] = "double";
		_glslDataTypeNames[GLSLDataType::BVec2] = "bvec2";
		_glslDataTypeNames[GLSLDataType::BVec3] = "bvec3";
		_glslDataTypeNames[GLSLDataType::BVec4] = "bvec4";
		_glslDataTypeNames[GLSLDataType::IVec2] = "ivec2";
		_glslDataTypeNames[GLSLDataType::IVec3] = "ivec3";
		_glslDataTypeNames[GLSLDataType::IVec4] = "ivec4";
		_glslDataTypeNames[GLSLDataType::UVec2] = "uvec2";
		_glslDataTypeNames[GLSLDataType::UVec3] = "uvec3";
		_glslDataTypeNames[GLSLDataType::UVec4] = "uvec4";
		_glslDataTypeNames[GLSLDataType::Vec2] = "vec2";
		_glslDataTypeNames[GLSLDataType::Vec3] = "vec3";
		_glslDataTypeNames[GLSLDataType::Vec4] = "vec4";
		_glslDataTypeNames[GLSLDataType::DVec2] = "dvec2";
		_glslDataTypeNames[GLSLDataType::DVec3] = "dvec3";
		_glslDataTypeNames[GLSLDataType::DVec4] = "dvec4";
		_glslDataTypeNames[GLSLDataType::Mat2] = "mat2";
		_glslDataTypeNames[GLSLDataType::Mat2x3] = "mat2x3";
		_glslDataTypeNames[GLSLDataType::Mat2x4] = "mat2x4";
		_glslDataTypeNames[GLSLDataType::Mat3x2] = "mat3x2";
		_glslDataTypeNames[GLSLDataType::Mat3] = "mat3";
		_glslDataTypeNames[GLSLDataType::Mat3x4] = "mat3x4";
		_glslDataTypeNames[GLSLDataType::Mat4x2] = "mat4x2";
		_glslDataTypeNames[GLSLDataType::Mat4x3] = "mat4x3";
		_glslDataTypeNames[GLSLDataType::Mat4] = "mat4";
	}

	void GLTypes::fillDataTypeNames()
	{
		// Fill out all data type names
		_dataTypeNames[GLDataType::Byte] = "Byte";
		_dataTypeNames[GLDataType::UByte] = "Unsigned Byte";
		_dataTypeNames[GLDataType::Short] = "Short";
		_dataTypeNames[GLDataType::UShort] = "Unsigned Short";
		_dataTypeNames[GLDataType::Int] = "Integer";
		_dataTypeNames[GLDataType::UInt] = "Unsigned Integer";
		_dataTypeNames[GLDataType::Float] = "Float";
		_dataTypeNames[GLDataType::HalfFloat] = "Half Float";
		_dataTypeNames[GLDataType::Double] = "Double";
		_dataTypeNames[GLDataType::Fixed] = "Fixed";
	}
}
