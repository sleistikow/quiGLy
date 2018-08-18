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

#include "uniformbaseblock.h"
#include "data/blocks/connection.h"
#include "data/blocks/portlist.h"

namespace ysm
{
	UniformBaseBlock::UniformBaseBlock(Pipeline* parent, BlockType type, const QString& name) : Block(parent, type, name)
	{
	}

	UniformBaseBlock::~UniformBaseBlock()
	{
	}

	bool UniformBaseBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		ConnectionPoints conPoints;

		conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_Uniform)
				  << qMakePair(BlockType::Shader_Geometry, PortType::Shader_Uniform)
				  << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_Uniform)
				  << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_Uniform)
				  << qMakePair(BlockType::Shader_Fragment, PortType::Shader_Uniform)
				  << qMakePair(BlockType::Mixer, PortType::Data_In)
				  << qMakePair(BlockType::Buffer, PortType::Data_In);

		if (!checkConnectionPoints(dest, conPoints))
		{
			denialReason = "Uniform output must be connected to a Shader, Mixer or Buffer block";
			return false;
		}

		return true;
	}

	void UniformBaseBlock::prepareConnection(Connection* con)
	{
		// If we're connecting to a Shader, add a location property to the connection
		if (con->getDestPort()->getType() == PortType::Shader_Uniform)
		{
			StringProperty* name = con->getPropertyList()->newProperty<StringProperty>(PropertyID::Uniform_Name, "Uniform Name");
			*name = getName().section(' ', 0, 0).replace("-", "");

			UIntProperty* location = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::Uniform_Location, "Uniform Location");
			*location = 0;

			BoolProperty* explicitLocation = con->getPropertyList()->newProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation, "Explicit Location");
			*explicitLocation = false;
		}
	}

	CacheObject::Key UniformBaseBlock::getCacheKey(bool retrieveForeignKey)
	{
		Q_UNUSED(retrieveForeignKey);

		CacheObject::Key key;
		QString data;

		for (Port* port : *_ports)
		{
			data += retrieveUniformData(port).toBase64();
			data += ";";
		}

		// Key consists of class qualifier + value (as base64)
		key = QString("UniformBlock/%1").arg(data);
		return key;
	}

	CacheObject::CacheObjectData* UniformBaseBlock::createCacheData()
	{
		// Never actually cache this object
		return nullptr;
	}
}
