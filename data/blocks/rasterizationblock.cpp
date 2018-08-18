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

#include "rasterizationblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

namespace ysm
{	
	RasterizationBlock::RasterizationBlock(Pipeline* parent) : Block(parent, block_type, "Rasterization")
	{

	}

	QMap<int, QString> RasterizationBlock::getCullFaceModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[CullFace_Front] = "Front";
		enumNames[CullFace_Back] = "Back";
		enumNames[CullFace_Both] = "Both";

		return enumNames;
	}

	QMap<int, QString> RasterizationBlock::getPolygonModeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[PolyMode_Point] = "Point";
		enumNames[PolyMode_Line] = "Line";
		enumNames[PolyMode_Fill] = "Fill";

		return enumNames;
	}

	QMap<int, QString> RasterizationBlock::getFrontFaceNames()
	{
		QMap<int, QString> enumNames;

		enumNames[FrontFace_Clockwise] = "Clockwise";
		enumNames[FrontFace_Counterclockwise] = "Counter-Clockwise";

		return enumNames;
	}

	BoolProperty* RasterizationBlock::getCullingEnabled()
	{
		return _cullingEnabled;
	}

	BoolProperty* RasterizationBlock::getLineAntialiasingEnabled()
	{
		return _lineAntialiasingEnabled;
	}

	BoolProperty* RasterizationBlock::getPointAntialiasingEnabled()
	{
		return _pointAntialiasingEnabled;
	}

	EnumProperty* RasterizationBlock::getCullFaceMode()
	{
		return _cullFaceMode;
	}

	EnumProperty* RasterizationBlock::getPolygonMode()
	{
		return _polygonMode;
	}

	EnumProperty* RasterizationBlock::getFrontFace()
	{
		return _frontFace;
	}

	FloatProperty* RasterizationBlock::getLineWidth()
	{
		return _lineWidth;
	}

	FloatProperty* RasterizationBlock::getPointSize()
	{
		return _pointSize;
	}

	Port* RasterizationBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* RasterizationBlock::getGenericOutPort()
	{
		return _outPort;
	}

	void RasterizationBlock::createProperties()
	{
		Block::createProperties();

		_cullingEnabled = _properties->newProperty<BoolProperty>(PropertyID::Rasterization_EnableCulling, "Culling");
		*_cullingEnabled = false;

		_cullFaceMode = _properties->newProperty<EnumProperty>(PropertyID::Rasterization_CullFaceMode, "Cull-Face Mode");
		*_cullFaceMode = CullFace_Back;

		_polygonMode = _properties->newProperty<EnumProperty>(PropertyID::Rasterization_PolygonMode, "Polygon Mode");
		*_polygonMode = PolyMode_Fill;

		_frontFace = _properties->newProperty<EnumProperty>(PropertyID::Rasterization_FrontFace, "Front Faces");
		*_frontFace = FrontFace_Counterclockwise;

		_pointSize = _properties->newProperty<FloatProperty>(PropertyID::Rasterization_PointSize, "Pointsize");
		*_pointSize = 1.0;

		_lineWidth = _properties->newProperty<FloatProperty>(PropertyID::Rasterization_LineWidth, "Linewidth");
		*_lineWidth = 1.0;

		_lineAntialiasingEnabled = _properties->newProperty<BoolProperty>(PropertyID::Rasterization_EnableLineAntialiasing, "Line-Antialiasing");
		*_lineAntialiasingEnabled = false;

		_pointAntialiasingEnabled = _properties->newProperty<BoolProperty>(PropertyID::Rasterization_EnablePointAntialiasing, "Point-Antialiasing");
		*_pointAntialiasingEnabled = false;
	}

	void RasterizationBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

    bool RasterizationBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
    {
        if (!Block::canAcceptConnection(src, dest, denialReason))
            return false;

        if (src == _outPort)
        {
            ConnectionPoints conPoints;

            conPoints << qMakePair(BlockType::Shader_Fragment, PortType::GenericIn);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Rasterization output must be connected to a Fragment Shader block";
				return false;
			}

			return true;
        }

        // Nope, we don't like this connection
        return false;
    }
}
