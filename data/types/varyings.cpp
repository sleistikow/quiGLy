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

#include "varyings.h"
#include "data/blocks/port.h"
#include "data/types/gltypes.h"

#include <QOpenGLFunctions>

namespace ysm
{
	Varyings::Varyings()
	{

	}

	Varyings::Varyings(const Varyings &src)
	{
		copyTfbVarying(src);
	}

	Varyings& Varyings::operator =(const Varyings& src)
	{
		copyTfbVarying(src);
		return *this;
	}

	bool Varyings::operator ==(const Varyings& src) const
	{
		if (_entries.size() != src._entries.size())
			return false;

		for (int i = 0; i < _entries.size(); ++i)
		{
			const VaryingEntry& entry = _entries[i];
			const VaryingEntry& entrySrc = src._entries[i];

			if (!compareEntries(entry, entrySrc))
				return false;
		}

		return true;
	}

	bool Varyings::operator !=(const Varyings& src) const
	{
		return !(*this == src);
	}

	void Varyings::addEntry(QString name, GLSLDataType type, int entryIndex)
	{
		VaryingEntry entry;
		entry.name = name;
		entry.type = type;

		if(entryIndex == -1)
			_entries << entry;
		else
			_entries.insert(entryIndex, entry);
	}


	QVector<Varyings::VaryingEntry>* Varyings::getEntries()
	{
		return &_entries;
	}

	unsigned int Varyings::getSize() const
	{
		unsigned int size = 0;

		for(const VaryingEntry& entry : _entries)
		{
			switch(entry.type)
			{
			case GLSLDataType::Bool:	size += sizeof(GLboolean);			break;
			case GLSLDataType::BVec2:	size += sizeof(GLboolean) * 2;		break;
			case GLSLDataType::BVec3:	size += sizeof(GLboolean) * 3;		break;
			case GLSLDataType::BVec4:	size += sizeof(GLboolean) * 4;		break;
			case GLSLDataType::Double:	size += sizeof(GLdouble);			break;
			case GLSLDataType::DVec2:	size += sizeof(GLdouble) * 2;		break;
			case GLSLDataType::DVec3:	size += sizeof(GLdouble) * 3;		break;
			case GLSLDataType::DVec4:	size += sizeof(GLdouble) * 4;		break;
			case GLSLDataType::Float:	size += sizeof(GLfloat);			break;
			case GLSLDataType::Vec2:	size += sizeof(GLfloat) * 2;		break;
			case GLSLDataType::Vec3:	size += sizeof(GLfloat) * 3;		break;
			case GLSLDataType::Vec4:	size += sizeof(GLfloat) * 4;		break;
			case GLSLDataType::Int:		size += sizeof(GLint);				break;
			case GLSLDataType::IVec2:	size += sizeof(GLint) * 2;			break;
			case GLSLDataType::IVec3:	size += sizeof(GLint) * 3;			break;
			case GLSLDataType::IVec4:	size += sizeof(GLint) * 4;			break;
			case GLSLDataType::UInt:	size += sizeof(GLuint);				break;
			case GLSLDataType::UVec2:	size += sizeof(GLuint) * 2;			break;
			case GLSLDataType::UVec3:	size += sizeof(GLuint) * 3;			break;
			case GLSLDataType::UVec4:	size += sizeof(GLuint) * 4;			break;
			case GLSLDataType::Mat2:	size += sizeof(GLfloat) * 2 * 2;	break;
			case GLSLDataType::Mat2x3:	size += sizeof(GLfloat) * 2 * 3;	break;
			case GLSLDataType::Mat2x4:	size += sizeof(GLfloat) * 2 * 4;	break;
			case GLSLDataType::Mat3:	size += sizeof(GLfloat) * 3 * 3;	break;
			case GLSLDataType::Mat3x2:	size += sizeof(GLfloat) * 3 * 2;	break;
			case GLSLDataType::Mat3x4:	size += sizeof(GLfloat) * 3 * 4;	break;
			case GLSLDataType::Mat4:	size += sizeof(GLfloat) * 4 * 4;	break;
			case GLSLDataType::Mat4x2:	size += sizeof(GLfloat) * 4 * 2;	break;
			case GLSLDataType::Mat4x3:	size += sizeof(GLfloat) * 4 * 3;	break;
			default:														break;
			}
		}

		return size;
	}

	void Varyings::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		for (const VaryingEntry& entry : _entries)
		{
			QDomElement elem = ctx->createElement("TfbVaryingEntry");

			elem.setAttribute("name", entry.name);
			elem.setAttribute("type", static_cast<int>(entry.type));

			xmlElement->appendChild(elem);
		}
	}

	void Varyings::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		_entries.clear();

		for(QDomElement& elem : ctx->getElements(xmlElement, "TfbVaryingEntry"))
		{
			VaryingEntry entry;
			entry.name = elem.attribute("name");
			entry.type = static_cast<GLSLDataType>(elem.attribute("type").toInt());

			_entries << entry;
		}
	}

	bool Varyings::compareEntries(const VaryingEntry& entry1, const VaryingEntry& entry2) const
	{
		if(entry1.name != entry2.name)
			return false;
		if(entry1.type != entry2.type)
			return false;

		return true;
	}

	void Varyings::copyTfbVarying(const Varyings& src)
	{
		_entries = src._entries;
	}


}

