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

#include "materialblock.h"


namespace ysm
{
	MaterialBlock::MaterialBlock(Pipeline* pipeline) : UniformBaseBlock(pipeline, block_type, "Material")
	{

	}

	void MaterialBlock::createProperties()
	{
		Block::createProperties();

		_ambient = _properties->newProperty<ColorProperty>(PropertyID::Material_Ambient, "Ambient Color");
		*_ambient = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_diffuse = _properties->newProperty<ColorProperty>(PropertyID::Material_Diffuse, "Diffuse Color");
		*_ambient = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_specular = _properties->newProperty<ColorProperty>(PropertyID::Material_Specular, "Specular Color");
		*_specular = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_shininess = _properties->newProperty<FloatProperty>(PropertyID::Material_Shininess, "Shininess");
		*_shininess = 0.0f;
	}

	void MaterialBlock::createPorts()
	{
		Block::createPorts();

		_ambiPort = _ports->newPort(PortType::Material_Ambient, PortDirection::Out, "Ambient Color");
		_diffPort = _ports->newPort(PortType::Material_Diffuse, PortDirection::Out, "Diffuse Color");
		_specPort = _ports->newPort(PortType::Material_Specular, PortDirection::Out, "Specular Color");
		_shinyPort = _ports->newPort(PortType::Material_Shininess, PortDirection::Out, "Shininess");
	}

	unsigned int MaterialBlock::getOutputSize(IPort* port) const
	{
		switch (port->getType())
		{
		case PortType::Material_Ambient:
		case PortType::Material_Diffuse:
		case PortType::Material_Specular:
			return sizeof(QVector4D);

		case PortType::Material_Shininess:
			return sizeof(float);

		default:
			return 0;
		}
	}

	QByteArray MaterialBlock::retrieveUniformData(IPort* port) const
	{
		switch (port->getType())
		{
		case PortType::Material_Ambient:
			return retrieveData<QVector4D>(_ambient);

		case PortType::Material_Diffuse:
			return retrieveData<QVector4D>(_diffuse);

		case PortType::Material_Specular:
			return retrieveData<QVector4D>(_specular);

		case PortType::Material_Shininess:
			return retrieveData<float>(_shininess);

		default:
			return QByteArray();
		}
	}
}
