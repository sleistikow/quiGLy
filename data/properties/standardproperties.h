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

#ifndef STANDARDPROPERTIES_H
#define STANDARDPROPERTIES_H

#include <QStringList>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QGenericMatrix>
#include <QMatrix4x4>

#include "property.h"
#include "data/types/types.h"

namespace ysm
{
	// Standard property types

	using BoolProperty = Property<bool, PropertyType::Bool>;
	using IntProperty = Property<int, PropertyType::Int>;
	using UIntProperty = Property<unsigned int, PropertyType::UInt>;
	using FloatProperty = Property<float, PropertyType::Float>;
	using DoubleProperty = Property<double, PropertyType::Double>;

	using CharProperty = Property<QChar, PropertyType::Char>;
	using StringProperty = Property<QString, PropertyType::String>;
	using StringListProperty = Property<QStringList, PropertyType::StringList>;

	using EnumProperty = Property<int, PropertyType::Enumeration>;
	using SizeProperty = Property<QSize, PropertyType::Size>;
	using ColorProperty = Property<QVector4D, PropertyType::Color>;

	using Vec2Property = Property<QVector2D, PropertyType::Vec2>;
	using Vec3Property = Property<QVector3D, PropertyType::Vec3>;
	using Vec4Property = Property<QVector4D, PropertyType::Vec4>;
	using Mat3x3Property = Property<QMatrix3x3, PropertyType::Mat3x3>;
	using Mat4x4Property = Property<QMatrix4x4, PropertyType::Mat4x4>;	

	// Data property types

	using ByteArrayProperty = Property<QByteArray, PropertyType::DataByteArray>;

	using Vec2DataProperty = Property<Vec2Data, PropertyType::DataVec2>;
	using Vec3DataProperty = Property<Vec3Data, PropertyType::DataVec3>;
	using Vec4DataProperty = Property<Vec4Data, PropertyType::DataVec4>;
	using IntDataProperty = Property<IntData, PropertyType::DataInt>;
	using UIntDataProperty = Property<UIntData, PropertyType::DataUInt>;
	using FloatDataProperty = Property<FloatData, PropertyType::DataFloat>;

	// String representation member specializations
	// Standard property types

	template<> bool Property<bool, PropertyType::Bool>::fromString(const QString& string);
	template<> bool Property<int, PropertyType::Int>::fromString(const QString& string);
	template<> bool Property<unsigned int, PropertyType::UInt>::fromString(const QString& string);
	template<> bool Property<float, PropertyType::Float>::fromString(const QString& string);
	template<> bool Property<double, PropertyType::Double>::fromString(const QString& string);

	template<> bool Property<QChar, PropertyType::Char>::fromString(const QString& string);
	template<> QString Property<QStringList, PropertyType::StringList>::toString() const;
	template<> bool Property<QStringList, PropertyType::StringList>::fromString(const QString& string);

	template<> bool Property<int, PropertyType::Enumeration>::fromString(const QString& string);
	template<> QString Property<QSize, PropertyType::Size>::toString() const;
	template<> bool Property<QSize, PropertyType::Size>::fromString(const QString& string);
	template<> QString Property<QVector4D, PropertyType::Color>::toString() const;
	template<> bool Property<QVector4D, PropertyType::Color>::fromString(const QString& string);

	template<> QString Property<QVector2D, PropertyType::Vec2>::toString() const;
	template<> bool Property<QVector2D, PropertyType::Vec2>::fromString(const QString& string);
	template<> QString Property<QVector3D, PropertyType::Vec3>::toString() const;
	template<> bool Property<QVector3D, PropertyType::Vec3>::fromString(const QString& string);
	template<> QString Property<QVector4D, PropertyType::Vec4>::toString() const;
	template<> bool Property<QVector4D, PropertyType::Vec4>::fromString(const QString& string);
	template<> QString Property<QMatrix3x3, PropertyType::Mat3x3>::toString() const;
	template<> bool Property<QMatrix3x3, PropertyType::Mat3x3>::fromString(const QString& string);
	template<> QString Property<QMatrix4x4, PropertyType::Mat4x4>::toString() const;
	template<> bool Property<QMatrix4x4, PropertyType::Mat4x4>::fromString(const QString& string);	

	// Data property types

	template<> QString Property<QByteArray, PropertyType::DataByteArray>::toString() const;
	template<> bool Property<QByteArray, PropertyType::DataByteArray>::fromString(const QString& string);

	template<> QString Property<Vec2Data, PropertyType::DataVec2>::toString() const;
	template<> bool Property<Vec2Data, PropertyType::DataVec2>::fromString(const QString& string);
	template<> QString Property<Vec3Data, PropertyType::DataVec3>::toString() const;	
	template<> bool Property<Vec3Data, PropertyType::DataVec3>::fromString(const QString& string);
	template<> QString Property<Vec4Data, PropertyType::DataVec4>::toString() const;
	template<> bool Property<Vec4Data, PropertyType::DataVec4>::fromString(const QString& string);
	template<> QString Property<IntData, PropertyType::DataInt>::toString() const;
	template<> bool Property<IntData, PropertyType::DataInt>::fromString(const QString& string);
	template<> QString Property<UIntData, PropertyType::DataUInt>::toString() const;
	template<> bool Property<UIntData, PropertyType::DataUInt>::fromString(const QString& string);
	template<> QString Property<FloatData, PropertyType::DataFloat>::toString() const;
	template<> bool Property<FloatData, PropertyType::DataFloat>::fromString(const QString& string);
}

#endif
