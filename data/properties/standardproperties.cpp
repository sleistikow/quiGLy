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

#include "standardproperties.h"

namespace ysm
{
	// Helpers

	template<typename T, typename F>
	bool deserializeStandardProperty(T& prop, F fnc)
	{
		bool ok = false;
		typename T::value_type v = fnc(ok);

		if (ok)
			prop.setValue(v);

		return ok;
	}

	template<typename V, typename P, typename T>
	QString convertDataArrayToString(T& t)
	{
		QStringList vals;

		for (V v : t.getValue())
		{
			P prop{v};
			vals << prop.toString();
		}

		return vals.join('|');
	}

	template<typename V, typename P, typename T>
	bool convertStringToDataArray(const QString& s, T& t)
	{
		QStringList vals = s.split('|');
		V newVals;

		for (QString v : vals)
		{
			P prop;

			if (prop.fromString(v))
				newVals << prop.getValue();
			else
				return false;
		}

		t.setValue(newVals);
		return true;
	}

	// Serialization member specializations
	// Standard property types

	template<>
	bool Property<bool, PropertyType::Bool>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { int x = string.toInt(&ok); return (x != 0); });
	}

	template<>
	bool Property<int, PropertyType::Int>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { return string.toInt(&ok); });
	}

	template<>
	bool Property<unsigned int, PropertyType::UInt>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { return string.toUInt(&ok); });
	}

	template<>
	bool Property<float, PropertyType::Float>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { return string.toFloat(&ok); });
	}

	template<>
	bool Property<double, PropertyType::Double>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { return string.toDouble(&ok); });
	}

	template<>
	bool Property<QChar, PropertyType::Char>::fromString(const QString& string)
	{
		if (string.length() == 1)
		{
			setValue(string[0]);
			return true;
		}

		return false;
	}

	template<>
	QString Property<QStringList, PropertyType::StringList>::toString() const
	{
		QStringList vals = getValue();

		for (int i = 0; i < vals.length(); ++i)
			vals[i] = Utils::encodeEscapeCharacters(vals[i]);

		return vals.join('\n');
	}

	template<>
	bool Property<QStringList, PropertyType::StringList>::fromString(const QString& string)
	{
		QStringList vals = string.split('\n');
		QStringList valList;

		for (QString str : vals)
			valList << Utils::decodeEscapeCharacters(str);

		setValue(valList);
		return true;
	}

	template<>
	bool Property<int, PropertyType::Enumeration>::fromString(const QString& string)
	{
		return deserializeStandardProperty(*this, [&](bool &ok) { return string.toInt(&ok); });
	}

	template<>
	QString Property<QVector2D, PropertyType::Vec2>::toString() const
	{
		QVector2D v = getValue();
		return QString("%1;%2").arg(v.x()).arg(v.y());
	}

	template<>
	bool Property<QVector2D, PropertyType::Vec2>::fromString(const QString& string)
	{
		QVector<float> vals;

		if (Utils::convertStringToFloatArray(string, vals) && vals.length() == 2)
		{
			setValue(QVector2D{vals[0], vals[1]});
			return true;
		}

		return false;
	}

	template<>
	QString Property<QVector3D, PropertyType::Vec3>::toString() const
	{
		QVector3D v = getValue();
		return QString("%1;%2;%3").arg(v.x()).arg(v.y()).arg(v.z());
	}

	template<>
	bool Property<QVector3D, PropertyType::Vec3>::fromString(const QString& string)
	{
		QVector<float> vals;

		if (Utils::convertStringToFloatArray(string, vals) && vals.length() == 3)
		{
			setValue(QVector3D{vals[0], vals[1], vals[2]});
			return true;
		}

		return false;
	}

	template<>
	QString Property<QVector4D, PropertyType::Vec4>::toString() const
	{
		QVector4D v = getValue();
		return QString("%1;%2;%3;%4").arg(v.x()).arg(v.y()).arg(v.z()).arg(v.w());
	}

	template<>
	bool Property<QVector4D, PropertyType::Vec4>::fromString(const QString& string)
	{
		QVector<float> vals;

		if (Utils::convertStringToFloatArray(string, vals) && vals.length() == 4)
		{
			setValue(QVector4D{vals[0], vals[1], vals[2], vals[3]});
			return true;
		}

		return false;
	}

	template<>
	QString Property<QMatrix3x3, PropertyType::Mat3x3>::toString() const
	{
		QMatrix3x3 m = getValue();
		Vec3Property r0{QVector3D{m(0, 0), m(0, 1), m(0, 2)}};
		Vec3Property r1{QVector3D{m(1, 0), m(1, 1), m(1, 2)}};
		Vec3Property r2{QVector3D{m(2, 0), m(2, 1), m(2, 2)}};
		QString s0, s1, s2;

		s0 = r0.toString();
		s1 = r1.toString();
		s2 = r2.toString();

		return QString("%1|%2|%3").arg(s0).arg(s1).arg(s2);
	}

	template<>
	bool Property<QMatrix3x3, PropertyType::Mat3x3>::fromString(const QString& string)
	{
		QStringList rowData = string.split('|');

		if (rowData.length() == 3)
		{
			Vec3Property row;
			QMatrix3x3 m = getValue();

			for (int i = 0; i < 3; ++i)
			{
				if (row.fromString(rowData[i]))
				{
					m(i, 0) = row->x();
					m(i, 1) = row->y();
					m(i, 2) = row->z();
				}
				else
					return false;
			}

			setValue(m);
		}

		return true;
	}

	template<>
	QString Property<QMatrix4x4, PropertyType::Mat4x4>::toString() const
	{
		QMatrix4x4 m = getValue();
		Vec4Property r0{m.row(0)};
		Vec4Property r1{m.row(1)};
		Vec4Property r2{m.row(2)};
		Vec4Property r3{m.row(3)};
		QString s0, s1, s2, s3;

		s0 = r0.toString();
		s1 = r1.toString();
		s2 = r2.toString();
		s3 = r3.toString();

		return QString("%1|%2|%3|%4").arg(s0).arg(s1).arg(s2).arg(s3);
	}

	template<>
	bool Property<QMatrix4x4, PropertyType::Mat4x4>::fromString(const QString& string)
	{
		QStringList rowData = string.split('|');

		if (rowData.length() == 4)
		{
			Vec4Property row;
			QMatrix4x4 m = getValue();

			for (int i = 0; i < 4; ++i)
			{
				if (row.fromString(rowData[i]))
					m.setRow(i, row);
				else
					return false;
			}

			setValue(m);
		}

		return true;
	}	

	// Data property types

	template<>
	QString Property<QByteArray, PropertyType::DataByteArray>::toString() const
	{
		return _value.toBase64();
	}

	template<>
	bool Property<QByteArray, PropertyType::DataByteArray>::fromString(const QString& string)
	{
		_value = QByteArray::fromBase64(string.toLatin1());
		return true;
	}

	template<>
	QString Property<Vec2Data, PropertyType::DataVec2>::toString() const
	{
		return convertDataArrayToString<QVector2D, Vec2Property>(*this);
	}

	template<>
	bool Property<Vec2Data, PropertyType::DataVec2>::fromString(const QString& string)
	{
		return convertStringToDataArray<Vec2Data, Vec2Property>(string, *this);
	}

	template<>
	QString Property<Vec3Data, PropertyType::DataVec3>::toString() const
	{
		return convertDataArrayToString<QVector3D, Vec3Property>(*this);
	}

	template<>
	bool Property<Vec3Data, PropertyType::DataVec3>::fromString(const QString& string)
	{
		return convertStringToDataArray<Vec3Data, Vec3Property>(string, *this);
	}

	template<>
	QString Property<Vec4Data, PropertyType::DataVec4>::toString() const
	{
		return convertDataArrayToString<QVector4D, Vec4Property>(*this);
	}

	template<>
	bool Property<Vec4Data, PropertyType::DataVec4>::fromString(const QString& string)
	{
		return convertStringToDataArray<Vec4Data, Vec4Property>(string, *this);
	}

	template<>
	QString Property<IntData, PropertyType::DataInt>::toString() const
	{
		return convertDataArrayToString<int, IntProperty>(*this);
	}

	template<>
	bool Property<IntData, PropertyType::DataInt>::fromString(const QString& string)
	{
		return convertStringToDataArray<IntData, IntProperty>(string, *this);
	}

	template<>
	QString Property<UIntData, PropertyType::DataUInt>::toString() const
	{
		return convertDataArrayToString<unsigned int, UIntProperty>(*this);
	}

	template<>
	bool Property<UIntData, PropertyType::DataUInt>::fromString(const QString& string)
	{
		return convertStringToDataArray<UIntData, UIntProperty>(string, *this);
	}

	template<>
	QString Property<FloatData, PropertyType::DataFloat>::toString() const
	{
		return convertDataArrayToString<float, FloatProperty>(*this);
	}

	template<>
	bool Property<FloatData, PropertyType::DataFloat>::fromString(const QString& string)
	{
		return convertStringToDataArray<FloatData, FloatProperty>(string, *this);
	}

	template<>
	QString Property<QSize, PropertyType::Size>::toString() const
	{
		QSize s = getValue();
		return QString("%1x%2").arg(s.width()).arg(s.height());
	}

	template<>
	bool Property<QSize, PropertyType::Size>::fromString(const QString& string)
	{
		QStringList vals = string.split('x');

		if (vals.count() == 2)
		{
			setValue(QSize{vals[0].toInt(), vals[1].toInt()});
			return true;
		}

		return false;
	}

	template<>
	QString Property<QVector4D, PropertyType::Color>::toString() const
	{
		QVector4D c = getValue();
		return QString("%1;%2;%3;%4").arg(c.x()).arg(c.y()).arg(c.z()).arg(c.w());
	}

	template<>
	bool Property<QVector4D, PropertyType::Color>::fromString(const QString& string)
	{
		QStringList vals = string.split(';');

		if (vals.count() == 4)
		{
			setValue(QVector4D{vals[0].toFloat(), vals[1].toFloat(), vals[2].toFloat(), vals[3].toFloat()});
			return true;
		}

		return false;
	}
}
