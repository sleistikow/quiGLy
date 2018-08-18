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

#ifndef TYPES_H
#define TYPES_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <functional>

namespace ysm
{
	enum class PortType;

	// Data types (used by buffers etc.)

	using Vec2Data = QVector<QVector2D>;
	using Vec3Data = QVector<QVector3D>;
	using Vec4Data = QVector<QVector4D>;
	using IntData = QVector<int>;
	using UIntData = QVector<unsigned int>;
	using FloatData = QVector<float>;

	// Type conversion stuff

	enum class DataType
	{
		NoType = 0,
		Vec2,
		Vec3,
		Vec4,
		Int,
		UInt,
		Float,
	};

	enum class VectorComponent
	{
		X,
		Y,
		Z,
		W,
	};

	/**
	 * @brief Helper class for type conversions
	 */
	class TypeConversion
	{
	public:
		struct ConversionOptions
		{
			DataType targetType{DataType::NoType};
			VectorComponent swizzlingX{VectorComponent::X};
			VectorComponent swizzlingY{VectorComponent::Y};
			VectorComponent swizzlingZ{VectorComponent::Z};
			VectorComponent swizzlingW{VectorComponent::W};
		};

	public:
		// Conversion helpers
		/**
		 * @brief Checks whether type @p fromType can be converted to type @p toType
		 */
		static bool canConvertBetweenTypes(DataType fromType, DataType toType);

		// Conversion functions (yes, we need all of those; everything else would lead to ugly code)
		static Vec2Data convertToVec2(const Vec2Data& data);
		static Vec2Data convertToVec2(const Vec3Data& data);
		static Vec2Data convertToVec2(const Vec4Data& data);
		static Vec2Data convertToVec2(const IntData& data);
		static Vec2Data convertToVec2(const UIntData& data);
		static Vec2Data convertToVec2(const FloatData& data);

		static Vec3Data convertToVec3(const Vec2Data& data);
		static Vec3Data convertToVec3(const Vec3Data& data);
		static Vec3Data convertToVec3(const Vec4Data& data);
		static Vec3Data convertToVec3(const IntData& data);
		static Vec3Data convertToVec3(const UIntData& data);
		static Vec3Data convertToVec3(const FloatData& data);

		static Vec4Data convertToVec4(const Vec2Data& data);
		static Vec4Data convertToVec4(const Vec3Data& data);
		static Vec4Data convertToVec4(const Vec4Data& data);
		static Vec4Data convertToVec4(const IntData& data);
		static Vec4Data convertToVec4(const UIntData& data);
		static Vec4Data convertToVec4(const FloatData& data);

		static IntData convertToInt(const Vec2Data& data);
		static IntData convertToInt(const Vec3Data& data);
		static IntData convertToInt(const Vec4Data& data);
		static IntData convertToInt(const IntData& data);
		static IntData convertToInt(const UIntData& data);
		static IntData convertToInt(const FloatData& data);

		static UIntData convertToUInt(const Vec2Data& data);
		static UIntData convertToUInt(const Vec3Data& data);
		static UIntData convertToUInt(const Vec4Data& data);
		static UIntData convertToUInt(const IntData& data);
		static UIntData convertToUInt(const UIntData& data);
		static UIntData convertToUInt(const FloatData& data);

		static FloatData convertToFloat(const Vec2Data& data);
		static FloatData convertToFloat(const Vec3Data& data);
		static FloatData convertToFloat(const Vec4Data& data);
		static FloatData convertToFloat(const IntData& data);
		static FloatData convertToFloat(const UIntData& data);
		static FloatData convertToFloat(const FloatData& data);

		// Vector conversions
		static QByteArray convertVectorToByteArray(const Vec2Data& data, int index, const ConversionOptions& convOptions);
		static QByteArray convertVectorToByteArray(const Vec3Data& data, int index, const ConversionOptions& convOptions);
		static QByteArray convertVectorToByteArray(const Vec4Data& data, int index, const ConversionOptions& convOptions);
		static QByteArray convertVectorToByteArray(const IntData& data, int index, const ConversionOptions& convOptions);
		static QByteArray convertVectorToByteArray(const UIntData& data, int index, const ConversionOptions& convOptions);
		static QByteArray convertVectorToByteArray(const FloatData& data, int index, const ConversionOptions& convOptions);

		/**
		 * @brief Converts objects in a given vector to type @p T; @p F must be convertible to @p T
		 * @arg T The object return type
		 * @arg F The object input type (automatically deduced)
		 */
		template<typename T, typename F>
		static QVector<T> convertVectorToVector(const QVector<F>& vec);

	private:
		/**
		 * @brief Converts the given vector @p vec to a vector of element type @p T
		 * This function simply creates a new object of type @p T by passing the objects to the type's constructor
		 */
		template<typename T, typename F>
		static QVector<T> convertVector(const QVector<F>& vec);

		/**
		 * @brief Converts the given vector @p vec to a vector of element type @p T
		 * This function creates a new object of type @p T using a callback function
		 */
		template<typename T, typename F>
		static QVector<T> convertVector(const QVector<F>& vec, std::function<T(const F&)> objGenerator);

		/**
		 * @brief Converts a QVector to a raw byte array
		 * @arg T The vector element type
		 * @arg VElement The type of split values in an element
		 * @arg VBuffer The type to store in the buffer
		 * @arg F The value retriever function type (automatically deduced)
		 */
		template<typename T, typename VElement, typename VBuffer, typename F>
		static QByteArray convertVectorToByteArray(const QVector<T>& data, F valRetriever, int index);

	private:
		// Swizzling helpers
		/**
		 * @brief Retrieves the given component of the vector @p vec
		 * If the vector has no such component, an exception is thrown.
		 */
		static float getVectorComponent(const QVector2D& vec, VectorComponent comp);
		static float getVectorComponent(const QVector3D& vec, VectorComponent comp);
		static float getVectorComponent(const QVector4D& vec, VectorComponent comp);

	private:
		// Construction
		explicit TypeConversion();
	};

	// Template member functions

	template<typename T, typename F>
	QVector<T> TypeConversion::convertVector(const QVector<F>& vec)
	{
		QVector<T> vecNew;

		vecNew.reserve(vec.size());

		for (const F& f : vec)
			vecNew << T(f);

		return vecNew;
	}

	template<typename T, typename F>
	QVector<T> TypeConversion::convertVector(const QVector<F>& vec, std::function<T(const F&)> objGenerator)
	{
		QVector<T> vecNew;

		vecNew.reserve(vec.size());

		for (const F& f : vec)
			vecNew << objGenerator(f);

		return vecNew;
	}


	template<typename T, typename VElement, typename VBuffer, typename F>
	QByteArray TypeConversion::convertVectorToByteArray(const QVector<T>& data, F valRetriever, int index)
	{
		int startIndex = (index < 0 ? 0 : index);
		int endIndex = (index < 0 ? data.size() - 1 : std::min(index, data.size() - 1));
		int elemCount = endIndex - startIndex + 1;
		QByteArray bytes;

		if (elemCount > 0)
		{
			bytes.reserve(sizeof(VBuffer) * elemCount);

			for (int i = startIndex; i <= endIndex; ++i)
			{
				T dataVal = data[i];

				QVector<VElement> convVals = valRetriever(dataVal);

				for (int j = 0; j < convVals.size(); ++j)
				{
					VBuffer convVal = static_cast<VBuffer>(convVals[j]);
					bytes.append(reinterpret_cast<const char*>(&convVal), sizeof(convVal));
				}
			}
		}

		return bytes;
	}

	template<typename T, typename F>
	QVector<T> TypeConversion::convertVectorToVector(const QVector<F>& vec)
	{
		static_assert(std::is_convertible<F,T>::value, "F must be convertible to T");

		QVector<T> vecNew;

		vecNew.reserve(vec.size());

		for (const F& f : vec)
			vecNew << static_cast<T>(f);

		return vecNew;
	}
}

#endif
