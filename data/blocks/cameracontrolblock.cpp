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

#include "cameracontrolblock.h"
#include "data/properties/propertylist.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"

namespace ysm
{
	CameraControlBlock::CameraControlBlock(Pipeline* parent) : Block(parent, block_type, "Camera Control")
	{

	}

	FloatProperty* CameraControlBlock::getTranslationSpeed()
	{
		return _translationSpeed;
	}

	FloatProperty* CameraControlBlock::getRotationSpeed()
	{
		return _rotationSpeed;
	}

	Port* CameraControlBlock::getGenericOutPort()
	{
		return _outPort;
	}

	void CameraControlBlock::createPorts()
	{
		Block::createPorts();

		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

	void CameraControlBlock::createProperties()
	{
		Block::createProperties();

		_translationSpeed = _properties->newProperty<FloatProperty>(PropertyID::CameraControl_TranslationSpeed, "Translation speed");
		*_translationSpeed = 0.5f;

		_rotationSpeed = _properties->newProperty<FloatProperty>(PropertyID::CameraControl_RotationSpeed, "Rotation speed");
		*_rotationSpeed = 0.5f;
	}

	bool CameraControlBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _outPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::ModelViewProjection, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Camera Control output must be connected to a Model View Projection Block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}
}
