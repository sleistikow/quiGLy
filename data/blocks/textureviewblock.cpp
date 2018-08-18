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

#include "textureviewblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "data/properties/propertylist.h"
#include "data/blocks/texturebaseblock.h"
#include "data/blocks/textureblock.h"

namespace ysm
{
	TextureViewBlock::TextureViewBlock(Pipeline* parent) : TextureBaseBlock(parent, block_type, "Texture View")
	{

	}

	unsigned int TextureViewBlock::getMinimumVersion() const
	{
		return 430;
	}

	UIntProperty* TextureViewBlock::getMinimumLevel()
	{
		return _minLevel;
	}

	UIntProperty* TextureViewBlock::getLevelCount()
	{
		return _numLevels;
	}

	UIntProperty* TextureViewBlock::getMinimumLayer()
	{
		return _minLayer;
	}

	UIntProperty* TextureViewBlock::getLayerCount()
	{
		return _numLayers;
	}

	EnumProperty* TextureViewBlock::getInternalFormat()
	{
		return _internalFormat;
	}

	EnumProperty* TextureViewBlock::getTextureTarget()
	{
		return _textureTarget;
	}

	Port* TextureViewBlock::getTexturePort()
	{
		return _texturePort;
	}

	Port* TextureViewBlock::getFrameBufferObjectPort()
	{
		return _frameBufferObjectInPort;
	}

	Port* TextureViewBlock::getSamplerPort()
	{
		return _samplerPort;
	}

	Port* TextureViewBlock::getGenericOutPort()
	{
		return _outPort;
	}

	void TextureViewBlock::createProperties()
	{
		TextureBaseBlock::createProperties();

		_minLevel = _properties->newProperty<UIntProperty>(PropertyID::TextureView_MinimumLevel, "Minimum Level");
		*_minLevel = 0;

		_numLevels = _properties->newProperty<UIntProperty>(PropertyID::TextureView_LevelCount, "Level Count");
		*_numLevels = 1;

		_minLayer = _properties->newProperty<UIntProperty>(PropertyID::TextureView_MinimumLayer, "Minimum Layer");
		*_minLayer = 0;

		_numLayers = _properties->newProperty<UIntProperty>(PropertyID::TextureView_LayerCount, "Layer Count");
		*_numLayers = 1;

		_internalFormat = _properties->newProperty<EnumProperty>(PropertyID::TextureBase_InternalFormat, "Internal Format");
        *_internalFormat = TextureBaseBlock::Format_RGBA32F;

		_textureTarget = _properties->newProperty<EnumProperty>(PropertyID::Texture_TargetType, "Texture Target");
		*_textureTarget = TextureBlock::Target_2D;
	}

	void TextureViewBlock::createPorts()
	{
		Block::createPorts();

		_texturePort				= _ports->newPort(PortType::TextureView_Texture, PortDirection::In, "Texture");
		_frameBufferObjectInPort	= _ports->newPort(PortType::Texture_FrameBuffer, PortDirection::In, "FrameBufferObject");
		_samplerPort				= _ports->newPort(PortType::Texture_Sampler, PortDirection::In, "Sampler");
		_outPort					= _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

	bool TextureViewBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _outPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Shader_Vertex, PortType::Shader_Texture);
			conPoints << qMakePair(BlockType::Shader_TessellationControl, PortType::Shader_Texture);
			conPoints << qMakePair(BlockType::Shader_TessellationEvaluation, PortType::Shader_Texture);
			conPoints << qMakePair(BlockType::Shader_Geometry, PortType::Shader_Texture);
			conPoints << qMakePair(BlockType::Shader_Fragment, PortType::Shader_Texture);
			conPoints << qMakePair(BlockType::TextureView, PortType::TextureView_Texture);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "TextureView output must be connected to another Texture View or Shader block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	void TextureViewBlock::prepareConnection(Connection* con)
	{
		// If we're connecting to a Shader, add a needed properties to the connection
		if (con->getSourcePort()->getType() == PortType::GenericOut && con->getDestPort()->getType() == PortType::Shader_Texture)
		{
			UIntProperty* unit = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::Texture_Unit, "Texture Unit");
			*unit = 0;

			StringProperty* name = con->getPropertyList()->newProperty<StringProperty>(PropertyID::Texture_Name, "Texture Name");
			*name = getName().replace(" ", "");

			BoolProperty* binding = con->getPropertyList()->newProperty<BoolProperty>(PropertyID::Texture_Binding, "Explicit Texture Binding");
			binding->setMinimumVersion(420); // actually not needed, because the Block's version is higher
			*binding = false;

			EnumProperty* sampler = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::Sampler_Type, "Sampler type");
			*sampler = Sampler_Sampler2D;
		}
	}
}
