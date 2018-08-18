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

#include "modelviewprojectionblock.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	ModelViewProjectionBlock::ModelViewProjectionBlock(Pipeline* parent) : UniformBaseBlock(parent, block_type, "Model View Projection")
	{

	}

	FloatProperty* ModelViewProjectionBlock::getProjFar()
	{
		return _projFar;
	}

	FloatProperty* ModelViewProjectionBlock::getProjNear()
	{
		return _projNear;
	}

	FloatProperty* ModelViewProjectionBlock::getProjAspectRatio()
	{
		return _projAspectRatio;
	}

	FloatProperty* ModelViewProjectionBlock::getProjVerticalAngle()
	{
		return _projVerticalAngle;
	}

	Vec3Property* ModelViewProjectionBlock::getCameraUpVector()
	{
		return _cameraUpVector;
	}

	Vec3Property* ModelViewProjectionBlock::getCameraCenter()
	{
		return _cameraTarget;
	}

	Vec3Property* ModelViewProjectionBlock::getCameraTarget()
	{
		return _cameraPosition;
	}

	Vec3Property* ModelViewProjectionBlock::getModelScaling()
	{
		return _modelScaling;
	}

	FloatProperty* ModelViewProjectionBlock::getModelRotationAngle()
	{
		return _modelRotationAngle;
	}

	Vec3Property* ModelViewProjectionBlock::getModelRotationAxis()
	{
		return _modelRotationAxis;
	}

	Vec3Property* ModelViewProjectionBlock::getModelPosition()
	{
		return _modelPosition;
	}

	Mat3x3Property* ModelViewProjectionBlock::getMatNormal()
	{
		return _matNormal;
	}

	Mat4x4Property* ModelViewProjectionBlock::getMatMVP()
	{
		return _matMVP;
	}

	Mat4x4Property* ModelViewProjectionBlock::getMatMV()
	{
		return _matMV;
	}

	Mat4x4Property* ModelViewProjectionBlock::getMatP()
	{
		return _matP;
	}

	Mat4x4Property* ModelViewProjectionBlock::getMatV()
	{
		return _matV;
	}

	Mat4x4Property* ModelViewProjectionBlock::getMatM()
	{
		return _matM;
	}

	Port* ModelViewProjectionBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* ModelViewProjectionBlock::getMatNormalPort()
	{
		return _matNormalPort;
	}

	unsigned int ModelViewProjectionBlock::getOutputSize(IPort* port) const
	{
		switch (port->getType())
		{
		case PortType::MVP_MatM:
		case PortType::MVP_MatV:
		case PortType::MVP_MatP:
		case PortType::MVP_MatMV:
		case PortType::MVP_MatMVP:
			return 4 * 4 * sizeof(float); // Hardcoded so we definitely get the correct size

		case PortType::MVP_MatNormal:
			return 3 * 3 * sizeof(float); // Hardcoded so we definitely get the correct size

		default:
			return 0;
		}
	}

	QByteArray ModelViewProjectionBlock::retrieveUniformData(IPort* port) const
	{
		QByteArray data;
		QMatrix4x4 mat4;
		QMatrix3x3 mat3;

		switch (port->getType())
		{
		case PortType::MVP_MatM:
			mat4 = *_matM;
			data.append(reinterpret_cast<char*>(mat4.data()), 4 * 4 * sizeof(float));
			break;

		case PortType::MVP_MatV:
			mat4 = *_matV;
			data.append(reinterpret_cast<char*>(mat4.data()), 4 * 4 * sizeof(float));
			break;

		case PortType::MVP_MatP:
			mat4 = *_matP;
			data.append(reinterpret_cast<char*>(mat4.data()), 4 * 4 * sizeof(float));
			break;

		case PortType::MVP_MatMV:
			mat4 = *_matMV;
			data.append(reinterpret_cast<char*>(mat4.data()), 4 * 4 * sizeof(float));
			break;

		case PortType::MVP_MatMVP:
			mat4 = *_matMVP;
			data.append(reinterpret_cast<char*>(mat4.data()), 4 * 4 * sizeof(float));
			break;

		case PortType::MVP_MatNormal:
			mat3 = *_matNormal;
			data.append(reinterpret_cast<char*>(mat3.data()), 3 * 3 * sizeof(float));
			break;

		default:
			break;
		}

		return data;
	}

	Port* ModelViewProjectionBlock::getMatMVPPort()
	{
		return _matMVPPort;
	}

	Port* ModelViewProjectionBlock::getMatMVPort()
	{
		return _matMVPort;
	}

	Port* ModelViewProjectionBlock::getMatPPort()
	{
		return _matPPort;
	}

	Port* ModelViewProjectionBlock::getMatVPort()
	{
		return _matVPort;
	}

	Port* ModelViewProjectionBlock::getMatMPort()
	{
		return _matMPort;
	}

	void ModelViewProjectionBlock::createProperties()
	{
		Block::createProperties();

		// Model properties
		_modelPosition = _properties->newProperty<Vec3Property>(PropertyID::MVP_ModelPosition, "Position");
		_modelRotationAxis = _properties->newProperty<Vec3Property>(PropertyID::MVP_ModelRotationAxis, "Rotation Axis");

		_modelRotationAngle = _properties->newProperty<FloatProperty>(PropertyID::MVP_ModelRotationAngle, "Rotation Angle");
		*_modelRotationAngle = 0.0f;

		_modelScaling = _properties->newProperty<Vec3Property>(PropertyID::MVP_ModelScaling, "Scaling");
		*_modelScaling = QVector3D{1.0f, 1.0f, 1.0f};

		// Camera properties
		_cameraPosition = _properties->newProperty<Vec3Property>(PropertyID::MVP_CameraPosition, "Position");
		*_cameraPosition = QVector3D(0, 0, 1);

		_cameraTarget = _properties->newProperty<Vec3Property>(PropertyID::MVP_CameraTarget, "Target");
		*_cameraTarget = QVector3D(0, 0, 0);

		_cameraUpVector = _properties->newProperty<Vec3Property>(PropertyID::MVP_CameraUpVector, "Up Vector");
		*_cameraUpVector = QVector3D(0, 1, 0);

		// Projection properties
		_projVerticalAngle = _properties->newProperty<FloatProperty>(PropertyID::MVP_ProjVerticalAngle, "Vertical Angle");
		*_projVerticalAngle = 60.0f;

		_projAspectRatio = _properties->newProperty<FloatProperty>(PropertyID::MVP_ProjAspectRation, "Aspect Ratio");
		*_projAspectRatio = 1.0f;

		_projNear = _properties->newProperty<FloatProperty>(PropertyID::MVP_ProjNear, "Near Plane");
		*_projNear = 0.1f;

		_projFar = _properties->newProperty<FloatProperty>(PropertyID::MVP_ProjFar, "Far Plane");
		*_projFar = 100.0f;

		// Matrix properties
		_matM = _properties->newProperty<Mat4x4Property>(PropertyID::MVP_MatM, "Model Matrix", true);
		_matM->setSerializable(false);

		_matV = _properties->newProperty<Mat4x4Property>(PropertyID::MVP_MatV, "View Matrix", true);
		_matV->setSerializable(false);

		_matP = _properties->newProperty<Mat4x4Property>(PropertyID::MVP_MatP, "Projection Matrix", true);
		_matP->setSerializable(false);

		_matMV = _properties->newProperty<Mat4x4Property>(PropertyID::MVP_MatMV, "Model-View Matrix", true);
		_matMV->setSerializable(false);

		_matMVP = _properties->newProperty<Mat4x4Property>(PropertyID::MVP_MatMVP, "Model-View-Projection Matrix", true);
		_matMVP->setSerializable(false);

		_matNormal = _properties->newProperty<Mat3x3Property>(PropertyID::MVP_MatNormal, "Normal Matrix", true);
		_matNormal->setSerializable(false);

		updateMatrices();
	}

	void ModelViewProjectionBlock::createPorts()
	{
		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "Camera Control");
		_matMPort = _ports->newPort(PortType::MVP_MatM, PortDirection::Out, "Model Matrix");
		_matVPort = _ports->newPort(PortType::MVP_MatV, PortDirection::Out, "View Matrix");
		_matPPort = _ports->newPort(PortType::MVP_MatP, PortDirection::Out, "Projection Matrix");
		_matMVPort = _ports->newPort(PortType::MVP_MatMV, PortDirection::Out, "MV Matrix");
		_matMVPPort = _ports->newPort(PortType::MVP_MatMVP, PortDirection::Out, "MVP Matrix");
		_matNormalPort = _ports->newPort(PortType::MVP_MatNormal, PortDirection::Out, "Normal Matrix");
	}

	void ModelViewProjectionBlock::applyPropertyChanges(IProperty* prop)
	{
		Block::applyPropertyChanges(prop);

		QVector<IProperty*> props;

		// List all properties which make updating the matrix values necessary
		props << _modelPosition << _modelRotationAxis << _modelRotationAngle << _modelScaling << _cameraPosition << _cameraTarget << _cameraUpVector << _projVerticalAngle << _projAspectRatio << _projNear << _projFar;

		if (props.contains(prop) || !prop)
			updateMatrices();
	}

	void ModelViewProjectionBlock::updateMatrices()
	{
		QMatrix4x4 mat;

		auto updateMatrix = [&mat](Mat4x4Property* prop) {
			prop->setReadOnly(false);
			*prop = mat;
			prop->setReadOnly(true);
		};

		// Update the model matrix
		mat.translate(*_modelPosition);
		mat.rotate(*_modelRotationAngle, *_modelRotationAxis);
		mat.scale(*_modelScaling);

		updateMatrix(_matM);

		// Update the view matrix, if no camera control connected
		mat.setToIdentity();
		mat.lookAt(*_cameraPosition, *_cameraTarget, *_cameraUpVector);

		updateMatrix(_matV);

		// Update the projection matrix
		mat.setToIdentity();
		mat.perspective(*_projVerticalAngle, *_projAspectRatio, *_projNear, *_projFar);

		updateMatrix(_matP);

		// Update the model-view matrix
		mat = _matV->getValue() * _matM->getValue();
		updateMatrix(_matMV);

		// Update the model-view-projection matrix
		mat = _matP->getValue() * _matV->getValue() * _matM->getValue();
		updateMatrix(_matMVP);

		// Update the normal matrix
		QMatrix3x3 matN = _matM->getValue().normalMatrix();

		_matNormal->setReadOnly(false);
		*_matNormal = matN;
		_matNormal->setReadOnly(true);
	}
}
