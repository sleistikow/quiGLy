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

#include "types.h"
#include "data/blocks/porttype.h"

#include <QOpenGLFunctions>

namespace ysm
{
	bool TypeConversion::canConvertBetweenTypes(DataType fromType, DataType toType)
	{
		bool ret = false;

		if (fromType == toType)
			return true;

		if (fromType == DataType::NoType || toType == DataType::NoType)
			return false;

		// Effectively, we can convert everything to everything so far, but that might change one day
		switch (fromType)
		{
		case DataType::Vec2:
			ret = (toType == DataType::Vec3 || toType == DataType::Vec4 || toType == DataType::Float || toType == DataType::Int || toType == DataType::UInt);
			break;

		case DataType::Vec3:
			ret = (toType == DataType::Vec2 || toType == DataType::Vec4 || toType == DataType::Float || toType == DataType::Int || toType == DataType::UInt);
			break;

		case DataType::Vec4:
			ret = (toType == DataType::Vec2 || toType == DataType::Vec3 || toType == DataType::Float || toType == DataType::Int || toType == DataType::UInt);
			break;

		case DataType::Int:
			ret = (toType == DataType::Vec2 || toType == DataType::Vec3 || toType == DataType::Vec4 || toType == DataType::Float || toType == DataType::UInt);
			break;

		case DataType::UInt:
			ret = (toType == DataType::Vec2 || toType == DataType::Vec3 || toType == DataType::Vec4 || toType == DataType::Float || toType == DataType::Int);
			break;


		case DataType::Float:
			ret = (toType == DataType::Vec2 || toType == DataType::Vec3 || toType == DataType::Vec4 || toType == DataType::Int || toType == DataType::UInt);
			break;

		default:
			ret = false;
		}

		return ret;
	}

	Vec2Data TypeConversion::convertToVec2(const Vec2Data& data)
	{
		return data;
	}

	Vec2Data TypeConversion::convertToVec2(const Vec3Data& data)
	{
		return convertVector<QVector2D, QVector3D>(data);
	}

	Vec2Data TypeConversion::convertToVec2(const Vec4Data& data)
	{
		return convertVector<QVector2D, QVector4D>(data);
	}

	Vec2Data TypeConversion::convertToVec2(const IntData& data)
	{
		return convertVector<QVector2D, int>(data, [](const int& v) { return QVector2D{static_cast<float>(v), 0.0f}; });
	}

	Vec2Data TypeConversion::convertToVec2(const UIntData& data)
	{
		return convertVector<QVector2D, unsigned int>(data, [](const unsigned int& v) { return QVector2D{static_cast<float>(v), 0.0f}; });
	}

	Vec2Data TypeConversion::convertToVec2(const FloatData& data)
	{
		return convertVector<QVector2D, float>(data, [](const float& v) { return QVector2D{v, 0.0f}; });
	}

	Vec3Data TypeConversion::convertToVec3(const Vec2Data& data)
	{
		return convertVector<QVector3D, QVector2D>(data);
	}

	Vec3Data TypeConversion::convertToVec3(const Vec3Data& data)
	{
		return data;
	}

	Vec3Data TypeConversion::convertToVec3(const Vec4Data& data)
	{
		return convertVector<QVector3D, QVector4D>(data);
	}

	Vec3Data TypeConversion::convertToVec3(const IntData& data)
	{
		return convertVector<QVector3D, int>(data, [](const int& v) { return QVector3D{static_cast<float>(v), 0.0f, 0.0f}; });
	}

	Vec3Data TypeConversion::convertToVec3(const UIntData& data)
	{
		return convertVector<QVector3D, unsigned int>(data, [](const unsigned int& v) { return QVector3D{static_cast<float>(v), 0.0f, 0.0f}; });
	}

	Vec3Data TypeConversion::convertToVec3(const FloatData& data)
	{
		return convertVector<QVector3D, float>(data, [](const float& v) { return QVector3D{v, 0.0f, 0.0f}; });
	}

	Vec4Data TypeConversion::convertToVec4(const Vec2Data& data)
	{
		return convertVector<QVector4D, QVector2D>(data);
	}

	Vec4Data TypeConversion::convertToVec4(const Vec3Data& data)
	{
		return convertVector<QVector4D, QVector3D>(data);
	}

	Vec4Data TypeConversion::convertToVec4(const Vec4Data& data)
	{
		return data;
	}

	Vec4Data TypeConversion::convertToVec4(const IntData& data)
	{
		return convertVector<QVector4D, int>(data, [](const int& v) { return QVector4D{static_cast<float>(v), 0.0f, 0.0f, 0.0f}; });
	}

	Vec4Data TypeConversion::convertToVec4(const UIntData& data)
	{
		return convertVector<QVector4D, unsigned int>(data, [](const unsigned int& v) { return QVector4D{static_cast<float>(v), 0.0f, 0.0f, 0.0f}; });
	}

	Vec4Data TypeConversion::convertToVec4(const FloatData& data)
	{
		return convertVector<QVector4D, float>(data, [](const float& v) { return QVector4D{v, 0.0f, 0.0f, 0.0f}; });
	}

	IntData TypeConversion::convertToInt(const Vec2Data& data)
	{
		return convertVector<int, QVector2D>(data, [](const QVector2D& v) { return static_cast<int>(v.x()); });
	}

	IntData TypeConversion::convertToInt(const Vec3Data& data)
	{
		return convertVector<int, QVector3D>(data, [](const QVector3D& v) { return static_cast<int>(v.x()); });
	}

	IntData TypeConversion::convertToInt(const Vec4Data& data)
	{
		return convertVector<int, QVector4D>(data, [](const QVector4D& v) { return static_cast<int>(v.x()); });
	}

	IntData TypeConversion::convertToInt(const IntData& data)
	{
		return data;
	}

	IntData TypeConversion::convertToInt(const UIntData& data)
	{
		return convertVector<int, unsigned int>(data, [](const unsigned int& v) { return static_cast<int>(v); });
	}

	IntData TypeConversion::convertToInt(const FloatData& data)
	{
		return convertVector<int, float>(data, [](const float& v) { return static_cast<int>(v); });
	}

	UIntData TypeConversion::convertToUInt(const Vec2Data& data)
	{
		return convertVector<unsigned int, QVector2D>(data, [](const QVector2D& v) { return static_cast<unsigned int>(v.x()); });
	}

	UIntData TypeConversion::convertToUInt(const Vec3Data& data)
	{
		return convertVector<unsigned int, QVector3D>(data, [](const QVector3D& v) { return static_cast<unsigned int>(v.x()); });
	}

	UIntData TypeConversion::convertToUInt(const Vec4Data& data)
	{
		return convertVector<unsigned int, QVector4D>(data, [](const QVector4D& v) { return static_cast<unsigned int>(v.x()); });
	}

	UIntData TypeConversion::convertToUInt(const IntData& data)
	{
		return convertVector<unsigned int, int>(data, [](const int& v) { return static_cast<unsigned int>(v); });
	}

	UIntData TypeConversion::convertToUInt(const UIntData& data)
	{
		return data;
	}

	UIntData TypeConversion::convertToUInt(const FloatData& data)
	{
		return convertVector<unsigned int, float>(data, [](const float& v) { return static_cast<unsigned int>(v); });
	}

	FloatData TypeConversion::convertToFloat(const Vec2Data& data)
	{
		return convertVector<float, QVector2D>(data, [](const QVector2D& v) { return v.x(); });
	}

	FloatData TypeConversion::convertToFloat(const Vec3Data& data)
	{
		return convertVector<float, QVector3D>(data, [](const QVector3D& v) { return v.x(); });
	}

	FloatData TypeConversion::convertToFloat(const Vec4Data& data)
	{
		return convertVector<float, QVector4D>(data, [](const QVector4D& v) { return v.x(); });
	}

	FloatData TypeConversion::convertToFloat(const IntData& data)
	{
		return convertVector<float, int>(data, [](const int& v) { return static_cast<float>(v); });
	}

	FloatData TypeConversion::convertToFloat(const UIntData& data)
	{
		return convertVector<float, unsigned int>(data, [](const unsigned int& v) { return static_cast<float>(v); });
	}

	FloatData TypeConversion::convertToFloat(const FloatData& data)
	{
		return data;
	}

	QByteArray TypeConversion::convertVectorToByteArray(const Vec2Data& data, int index, const ConversionOptions& convOptions)
	{
		auto valRetriever = [&convOptions](const QVector2D& v)
		{
			QVector<float> vals;

			vals << getVectorComponent(v, convOptions.swizzlingX) << getVectorComponent(v, convOptions.swizzlingY);
			return vals;
		};

		return convertVectorToByteArray<QVector2D, float, GLfloat>(data, valRetriever, index);
	}

	QByteArray TypeConversion::convertVectorToByteArray(const Vec3Data& data, int index, const ConversionOptions& convOptions)
	{
		auto valRetriever = [&convOptions](const QVector3D& v)
		{
			QVector<float> vals;

			vals << getVectorComponent(v, convOptions.swizzlingX) << getVectorComponent(v, convOptions.swizzlingY) << getVectorComponent(v, convOptions.swizzlingZ);
			return vals;
		};

		return convertVectorToByteArray<QVector3D, float, GLfloat>(data, valRetriever, index);
	}

	QByteArray TypeConversion::convertVectorToByteArray(const Vec4Data& data, int index, const ConversionOptions& convOptions)
	{
		auto valRetriever = [&convOptions](const QVector4D& v)
		{
			QVector<float> vals;

			vals << getVectorComponent(v, convOptions.swizzlingX) << getVectorComponent(v, convOptions.swizzlingY) << getVectorComponent(v, convOptions.swizzlingZ) << getVectorComponent(v, convOptions.swizzlingW);
			return vals;
		};

		return convertVectorToByteArray<QVector4D, float, GLfloat>(data, valRetriever, index);
	}

	QByteArray TypeConversion::convertVectorToByteArray(const IntData& data, int index, const ConversionOptions& convOptions)
	{
		auto valRetriever = [&convOptions](const int& v)
		{
			QVector<int> vals;

			vals << v;
			return vals;
		};

		return convertVectorToByteArray<int, int, GLint>(data, valRetriever, index);
	}

	QByteArray TypeConversion::convertVectorToByteArray(const UIntData& data, int index, const ConversionOptions& convOptions)
	{
		Q_UNUSED(convOptions);

		auto valRetriever = [](const unsigned int& v)
		{
			QVector<unsigned int> vals;

			vals << v;
			return vals;
		};

		return convertVectorToByteArray<unsigned int, unsigned int, GLuint>(data, valRetriever, index);
	}

	QByteArray TypeConversion::convertVectorToByteArray(const FloatData& data, int index, const ConversionOptions& convOptions)
	{
		Q_UNUSED(convOptions);

		auto valRetriever = [](const float& v)
		{
			QVector<float> vals;

			vals << v;
			return vals;
		};

		return convertVectorToByteArray<float, float, GLfloat>(data, valRetriever, index);
	}

	float TypeConversion::getVectorComponent(const QVector2D& vec, VectorComponent comp)
	{
		switch (comp)
		{
		case VectorComponent::X:
			return vec.x();

		case VectorComponent::Y:
			return vec.y();

		default:
			throw std::invalid_argument{qPrintable(QString("The vector has no component %1").arg(static_cast<int>(comp)))};
		}
	}

	float TypeConversion::getVectorComponent(const QVector3D& vec, VectorComponent comp)
	{
		switch (comp)
		{
		case VectorComponent::X:
			return vec.x();

		case VectorComponent::Y:
			return vec.y();

		case VectorComponent::Z:
			return vec.z();

		default:
			throw std::invalid_argument{qPrintable(QString("The vector has no component %1").arg(static_cast<int>(comp)))};
		}
	}

	float TypeConversion::getVectorComponent(const QVector4D& vec, VectorComponent comp)
	{
		switch (comp)
		{
		case VectorComponent::X:
			return vec.x();

		case VectorComponent::Y:
			return vec.y();

		case VectorComponent::Z:
			return vec.z();

		case VectorComponent::W:
			return vec.w();

		default:
			throw std::invalid_argument{qPrintable(QString("The vector has no component %1").arg(static_cast<int>(comp)))};
		}
	}

	TypeConversion::TypeConversion()
	{

	}
}
