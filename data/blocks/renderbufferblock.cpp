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

#include "renderbufferblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "data/properties/propertylist.h"
#include "data/blocks/texturebaseblock.h"

namespace ysm
{
	QMap<int, QString> RenderBufferBlock::getInternalFormatNames()
	{
		QMap<int, QString> enumNames;

		enumNames[TextureBaseBlock::Format_Red] = "Red";
		enumNames[TextureBaseBlock::Format_RG] = "RG";
		enumNames[TextureBaseBlock::Format_RGB] = "RGB";
		enumNames[TextureBaseBlock::Format_RGBA] = "RGBA";
		enumNames[TextureBaseBlock::Format_RGBA32I] = "RGBA32I";
		enumNames[TextureBaseBlock::Format_RGBA32UI] = "RGBA32UI";
		enumNames[TextureBaseBlock::Format_Depth24Stencil8] = "Depth24 Stencil8";
		enumNames[TextureBaseBlock::Format_Depth32FStencil8] = "Depth32F Stencil8";
		enumNames[TextureBaseBlock::Format_DepthComponent16] = "Depth Component16";
		enumNames[TextureBaseBlock::Format_DepthComponent24] = "Depth Component24";
		enumNames[TextureBaseBlock::Format_DepthComponent32F] = "Depth Component32F";
		enumNames[TextureBaseBlock::Format_StencilIndex1] = "Stencil Index1";
		enumNames[TextureBaseBlock::Format_StencilIndex4] = "Stencil Index4";
		enumNames[TextureBaseBlock::Format_StencilIndex8] = "Stencil Index8";
		enumNames[TextureBaseBlock::Format_StencilIndex16] = "Stencil Index16";

		return enumNames;
	}

	RenderBufferBlock::RenderBufferBlock(Pipeline* parent) : Block(parent, block_type, "Render Buffer")
	{

	}

	EnumProperty* RenderBufferBlock::getInternalFormat()
	{
		return _internalFormat;
	}

	UIntProperty* RenderBufferBlock::getWidth()
	{
		return _width;
	}

	UIntProperty* RenderBufferBlock::getHeight()
	{
		return _height;
	}

	BoolProperty* RenderBufferBlock::getAutoSize()
	{
		return _autoSize;
	}

	UIntProperty* RenderBufferBlock::getSamples()
	{
		return _samples;
	}

	Port* RenderBufferBlock::getDataInPort()
	{
		return _dataInPort;
	}

	void RenderBufferBlock::createProperties()
	{
		Block::createProperties();

		_internalFormat = _properties->newProperty<EnumProperty>(PropertyID::RenderBuffer_InternalFormat, "Internal Format");
		*_internalFormat = TextureBaseBlock::Format_RGBA;

		_width = _properties->newProperty<UIntProperty>(PropertyID::RenderBuffer_Width, "Width");
		*_width = 250;

		_height = _properties->newProperty<UIntProperty>(PropertyID::RenderBuffer_Height, "Height");
		*_height = 250;

		_samples = _properties->newProperty<UIntProperty>(PropertyID::RenderBuffer_Samples, "Samples");
		*_samples = 0;

		_autoSize = _properties->newProperty<BoolProperty>(PropertyID::RenderBuffer_ViewPortAutoSize, "Auto Size");
		*_autoSize = false;

		//TODO: implement in future version
		_samples->setReadOnly(true);
	}

	void RenderBufferBlock::createPorts()
	{
		Block::createPorts();

		_dataInPort = _ports->newPort(PortType::Data_In, PortDirection::In, "Data In");
	}
}
