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

#include "framebufferobjectblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "textureblock.h"
#include "data/properties/propertylist.h"
#include "data/blocks/imageloaderblock.h"

namespace ysm
{
	FrameBufferObjectBlock::FrameBufferObjectBlock(Pipeline* parent) : Block(parent, block_type, "Frame Buffer Object")
	{

	}

	QMap<int, QString> FrameBufferObjectBlock::getAttachmentTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Attachment_Color0] = "Color";
		enumNames[Attachment_Depth] = "Depth";
		enumNames[Attachment_Stencil] = "Stencil";
		enumNames[Attachment_DepthStencil] = "Depth and Stencil";

		return enumNames;
	}

	Port* FrameBufferObjectBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* FrameBufferObjectBlock::getRenderBufferPort()
	{
		return _renderBufferPort;
	}

	Port* FrameBufferObjectBlock::getTexturePort()
	{
		return _texturePort;
	}

	void FrameBufferObjectBlock::createProperties()
	{
		Block::createProperties();

		//TODO: map out location to attachment
	}

	void FrameBufferObjectBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
		_renderBufferPort = _ports->newPort(PortType::FrameBufferObject_RenderBuffer, PortDirection::Out, "Render Buffer Out");
		_texturePort = _ports->newPort(PortType::FrameBufferObject_Texture, PortDirection::Out, "Texture Out");
	}

	bool FrameBufferObjectBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _renderBufferPort)
		{
			ConnectionPoints conPoints;
			conPoints << qMakePair(BlockType::RenderBuffer, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "FBO RenderBuffer Port must be connected to a Render Buffer block";
				return false;
			}

			return true;
		}

		if (src == _texturePort)
		{
			ConnectionPoints conPoints;
			conPoints << qMakePair(BlockType::Texture, PortType::Data_In);
			conPoints << qMakePair(BlockType::TextureView, PortType::Texture_FrameBuffer);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "FBO Texture Port must be connected to a Texture or Texture View block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	void FrameBufferObjectBlock::prepareConnection(Connection* con)
	{
		bool toTexture = (con->getSourcePort()->getType() == PortType::FrameBufferObject_Texture);

		if ((con->getDestPort()->getType() == PortType::Data_In || con->getDestPort()->getType() == PortType::Texture_FrameBuffer) &&
			(con->getSourcePort()->getType() == PortType::FrameBufferObject_RenderBuffer || toTexture))
		{
			EnumProperty* attachment = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::FrameBufferObject_Attachment, "Attachment");
			*attachment = Attachment_Color0;

			UIntProperty* colorIndex = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::FrameBufferObject_ColorIndex, "Color Buffer Index (Color Attachment only)");
			*colorIndex = 0;

			if(toTexture)
			{
				EnumProperty* cubeMapSide = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::FrameBufferObject_CubeMapSide, "Cubemap Side (Cubemap only)");
				*cubeMapSide = TextureBlock::CubeMap_PositiveX;

				UIntProperty* textureLevel = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::FrameBufferObject_TextureLevel, "Texture Level");
				*textureLevel = 0;

				UIntProperty* textureLayer = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::FrameBufferObject_TextureLayer, "Texture Layer");
				*textureLayer = 0;

				BoolProperty* allLayers = con->getPropertyList()->newProperty<BoolProperty>(PropertyID::FrameBufferObject_AllLayers, "Render into all layers");
				*allLayers = true;

				EnumProperty* pixelDataFormat = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::Img_PixelDataFormat, "Pixeldata Format");
				*pixelDataFormat = ImageLoaderBlock::PixelFormat_RGBA;

				EnumProperty* pixelDataType = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::Img_PixelDataType, "Pixeldata Type");
				*pixelDataType = ImageLoaderBlock::PixelType_Float;

			}
		}
	}

}
