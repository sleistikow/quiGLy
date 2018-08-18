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

#include "lightsourceblock.h"

namespace ysm
{
	LightSourceBlock::LightSourceBlock(Pipeline* pipeline) : UniformBaseBlock(pipeline, block_type, "Light Source")
	{

	}

	void LightSourceBlock::createProperties()
	{
		Block::createProperties();

		_lightPosition = _properties->newProperty<Vec4Property>(PropertyID::Light_Position, "Position");
		*_lightPosition = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_lightDirection = _properties->newProperty<Vec3Property>(PropertyID::Light_Direction, "Direction");
		*_lightDirection = QVector3D(0.0f, 0.0f, 0.0f);

		_spotExponent = _properties->newProperty<FloatProperty>(PropertyID::Light_SpotExponent, "Spotlight Exponent");
		*_spotExponent = 1.0f;

		_spotAngle = _properties->newProperty<FloatProperty>(PropertyID::Light_SpotAngle, "Spotlight Angle");
		*_spotAngle = 180.0f;

		_colorAmbient = _properties->newProperty<ColorProperty>(PropertyID::Light_ColorAmbient, "Ambient Color");
		*_colorAmbient = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_colorDiffuse = _properties->newProperty<ColorProperty>(PropertyID::Light_ColorDiffuse, "Diffuse Color");
		*_colorDiffuse = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_colorSpecular = _properties->newProperty<ColorProperty>(PropertyID::Light_ColorSpecular, "Specular Color");
		*_colorSpecular = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		_attenuationConstant = _properties->newProperty<FloatProperty>(PropertyID::Light_AttenuationConstant, "Constant Attenuation");
		*_attenuationConstant = 1.0f;

		_attenuationLinear = _properties->newProperty<FloatProperty>(PropertyID::Light_AttenuationLinear, "Linear Attenuation");
		*_attenuationLinear = 1.0f;

		_attenuationQuadratic = _properties->newProperty<FloatProperty>(PropertyID::Light_AttenuationLinear, "Quadratic Attenuation");
		*_attenuationQuadratic = 1.0f;
	}

	void LightSourceBlock::createPorts()
	{
		Block::createPorts();

		_posPort = _ports->newPort(PortType::Light_Position, PortDirection::Out, "Position");
		_dirPort = _ports->newPort(PortType::Light_Direction, PortDirection::Out, "Direction");
		_expPort = _ports->newPort(PortType::Light_SpotExponent, PortDirection::Out, "Spotlight Exponent");
		_angPort = _ports->newPort(PortType::Light_SpotAngle, PortDirection::Out, "Spotlight Cutoff Angle");
		_ambiPort = _ports->newPort(PortType::Light_ColorAmbient, PortDirection::Out, "Ambient Color");
		_diffPort = _ports->newPort(PortType::Light_ColorDiffuse, PortDirection::Out, "Diffuse Color");
		_specPort = _ports->newPort(PortType::Light_ColorSpecular, PortDirection::Out, "Specular Color");
		_attConstPort = _ports->newPort(PortType::Light_AttenuationConstant, PortDirection::Out, "Constant Attenuation");
		_attLinPort = _ports->newPort(PortType::Light_AttenuationLinear, PortDirection::Out, "Linear Attenuation");
		_attQuadPort = _ports->newPort(PortType::Light_AttenuationQuadratic, PortDirection::Out, "Quadratic Attenuation");
	}

	unsigned int LightSourceBlock::getOutputSize(IPort* port) const
	{
		switch (port->getType())
		{
		case PortType::Light_Position:
		case PortType::Light_ColorAmbient:
		case PortType::Light_ColorDiffuse:
		case PortType::Light_ColorSpecular:
			return sizeof(QVector4D);

		case PortType::Light_Direction:
			return sizeof(QVector3D);

		case PortType::Light_SpotExponent:
		case PortType::Light_SpotAngle:
		case PortType::Light_AttenuationConstant:
		case PortType::Light_AttenuationLinear:
		case PortType::Light_AttenuationQuadratic:
			return sizeof(float);

		default:
			return 0;
		}
	}

	QByteArray LightSourceBlock::retrieveUniformData(IPort* port) const
	{
		switch (port->getType())
		{
		case PortType::Light_Position:
			return retrieveData<QVector4D>(_lightPosition);

		case PortType::Light_ColorAmbient:
			return retrieveData<QVector4D>(_colorAmbient);

		case PortType::Light_ColorDiffuse:
			return retrieveData<QVector4D>(_colorDiffuse);

		case PortType::Light_ColorSpecular:
			return retrieveData<QVector4D>(_colorSpecular);

		case PortType::Light_Direction:
			return retrieveData<QVector3D>(_lightDirection);

		case PortType::Light_SpotExponent:
			return retrieveData<float>(_spotExponent);

		case PortType::Light_SpotAngle:
			return retrieveData<float>(_spotAngle);

		case PortType::Light_AttenuationConstant:
			return retrieveData<float>(_attenuationConstant);

		case PortType::Light_AttenuationLinear:
			return retrieveData<float>(_attenuationLinear);

		case PortType::Light_AttenuationQuadratic:
			return retrieveData<float>(_attenuationQuadratic);

		default:
			return QByteArray();
		}
	}
}
