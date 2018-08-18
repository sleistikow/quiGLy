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

#include "textureblock.h"
#include "portlist.h"
#include "port.h"
#include "connection.h"
#include "data/properties/propertylist.h"
#include "data/properties/textureproperty.h"
#include "opengl/gli.h"


namespace ysm
{
	TextureBlock::TextureBlock(Pipeline* parent) : TextureBaseBlock(parent, block_type, "Texture")
	{

		connect(this, SIGNAL(blockDisconnected(Connection*)), this, SLOT(onConnectionRemoved(Connection*)));
		connect(this, SIGNAL(blockConnected(Connection*)), this, SLOT(onConnectionEstablished(Connection*)));
	}


	QMap<int, QString> TextureBlock::getTextureModeTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[TextureMode_DepthComponent] = "Depth Component";
		enumNames[TextureMode_StencilIndex] = "Stencil Index";

		return enumNames;
	}

	QMap<int, QString> TextureBlock::getSwizzleTypeNames()
	{
		QMap<int, QString> enumNames;

		enumNames[Swizzle_Red] = "Red";
		enumNames[Swizzle_Green] = "Green";
		enumNames[Swizzle_Blue] = "Blue";
		enumNames[Swizzle_Alpha] = "Alpha";
		enumNames[Swizzle_One] = "One";
		enumNames[Swizzle_Zero] = "Zero";

		return enumNames;
	}


	Port* TextureBlock::getDataInPort()
	{
		return _dataInPort;
	}

	Port* TextureBlock::getSamplerPort()
	{
		return _samplerPort;
	}

	void TextureBlock::createProperties()
	{

		TextureBaseBlock::createProperties();

		_target = _properties->newProperty<EnumProperty>(PropertyID::Texture_TargetType, "Texture Target");
		*_target = Target_2D;

		_mipmaps = _properties->newProperty<BoolProperty>(PropertyID::Texture_Mipmaps, "Mipmaps");
		*_mipmaps = false;

		_width = _properties->newProperty<UIntProperty>(PropertyID::Texture_Width, "Width");
		*_width = 250;
		_width->setReadOnly(true);

		_height = _properties->newProperty<UIntProperty>(PropertyID::Texture_Height, "Height");
		*_height = 250;
		_height->setReadOnly(true);

		_autoSize = _properties->newProperty<BoolProperty>(PropertyID::Texture_RenderBufferAutoSize, "Match Render Buffer");
		*_autoSize = false;

		_depthStencilTextureMode = _properties->newProperty<EnumProperty>(PropertyID::Texture_DepthStencilMode, "Texture Mode");
		*_depthStencilTextureMode = TextureMode_DepthComponent;

		_baseLevel = _properties->newProperty<IntProperty>(PropertyID::Texture_BaseLevel, "Base Level");
		*_baseLevel = 0;

		_maxLevel = _properties->newProperty<IntProperty>(PropertyID::Texture_MaxLevel, "Max Level");
		*_maxLevel = 1000;

		_swizzleR = _properties->newProperty<EnumProperty>(PropertyID::Texture_SwizzleRed, "Swizzle Red");
		*_swizzleR = Swizzle_Red;

		_swizzleG = _properties->newProperty<EnumProperty>(PropertyID::Texture_SwizzleGreen, "Swizzle Green");
		*_swizzleG = Swizzle_Green;

		_swizzleB = _properties->newProperty<EnumProperty>(PropertyID::Texture_SwizzleBlue, "Swizzle Blue");
		*_swizzleB = Swizzle_Blue;

		_swizzleA = _properties->newProperty<EnumProperty>(PropertyID::Texture_SwizzleAlpha, "Swizzle Alpha");
		*_swizzleA = Swizzle_Alpha;

	}

	void TextureBlock::createPorts()
	{
		TextureBaseBlock::createPorts();

		_dataInPort = _ports->newPort(PortType::Data_In, PortDirection::In, "Data In");
		_samplerPort = _ports->newPort(PortType::Texture_Sampler, PortDirection::In, "Sampler In");
	}

	bool TextureBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
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
				denialReason = "Texture output must be connected to a Shader or Texture View block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	void TextureBlock::prepareConnection(Connection* con)
	{
		// If we're connecting to a Shader, add a needed properties to the connection
		if (con->getSourcePort()->getType() == PortType::GenericOut && con->getDestPort()->getType() == PortType::Shader_Texture)
		{
			UIntProperty* unit = con->getPropertyList()->newProperty<UIntProperty>(PropertyID::Texture_Unit, "Texture Unit");
			*unit = 0;

			StringProperty* name = con->getPropertyList()->newProperty<StringProperty>(PropertyID::Texture_Name, "Texture Name");
			*name = getName().replace(" ", "");

			BoolProperty* binding = con->getPropertyList()->newProperty<BoolProperty>(PropertyID::Texture_Binding, "Explicit Texture Binding");
			binding->setMinimumVersion(420);
			*binding = false;

			EnumProperty* sampler = con->getPropertyList()->newProperty<EnumProperty>(PropertyID::Sampler_Type, "Sampler type");
			*sampler = Sampler_Sampler2D;
		}
	}

	void TextureBlock::onConnectionEstablished(Connection *con)
	{
		//set some properties to read-only if textureblock is connected to Sampler
		if(con->getSource()->getType() == BlockType::TextureSampler)
		{
			beginPropertyUpdate();
			_minFilterType->setReadOnly();
			_magFilterType->setReadOnly();
			_minLOD->setReadOnly();
			_maxLOD->setReadOnly();
			_wrapModeR->setReadOnly();
			_wrapModeS->setReadOnly();
			_wrapModeT->setReadOnly();
			_borderColor->setReadOnly();
			_compareMode->setReadOnly();
			_compareFunc->setReadOnly();
			_anisotropy->setReadOnly();
			endPropertyUpdate();
		}
		//TODO: if no path added to textureloader nothing is changed. should be changed as soon as filename added
		//if connected to textureloader -> retrieve properties from loaded texture
		else if(con->getSource()->getType() == BlockType::TextureLoader)
		{
			//connected textureloader
			IBlock* textureLoader = con->getSource();

			const gli::texture* texture = textureLoader->getProperty<TextureProperty>(PropertyID::Texture_Texture)->getValue();
			if(!texture || texture->empty())
				return;

			gli::gl GL(gli::gl::PROFILE_GL33);
			gli::gl::format const format = GL.translate(texture->format(), texture->swizzles());

			//set min/max LOD
			_baseLevel->setValue(0);
			_maxLevel->setValue(texture->levels() - 1);
			//set swizzle
			_swizzleR->setValue(format.Swizzles[0]);
			_swizzleG->setValue(format.Swizzles[1]);
			_swizzleB->setValue(format.Swizzles[2]);
			_swizzleA->setValue(format.Swizzles[3]);
			//mipmaps
			_mipmaps->setValue(true);
		}
		else if(con->getSource()->getType() == BlockType::FrameBufferObject)
		{
			_width->setReadOnly(false);
			_height->setReadOnly(false);
		}
	}

	void TextureBlock::onConnectionRemoved(Connection *con)
	{
		//set properties back when connection is removed
		if(con->getSource()->getType() == BlockType::TextureSampler)
		{
			beginPropertyUpdate();
			_minFilterType->setReadOnly(false);
			_magFilterType->setReadOnly(false);
			_minLOD->setReadOnly(false);
			_maxLOD->setReadOnly(false);
			_wrapModeR->setReadOnly(false);
			_wrapModeS->setReadOnly(false);
			_wrapModeT->setReadOnly(false);
			_borderColor->setReadOnly(false);
			_compareMode->setReadOnly(false);
			_compareFunc->setReadOnly(false);
			_anisotropy->setReadOnly(false);
			endPropertyUpdate();
		}
		else if(con->getSource()->getType() == BlockType::FrameBufferObject)
		{
			_width->setReadOnly(true);
			_height->setReadOnly(true);
		}

	}

	BoolProperty* TextureBlock::getMipmaps()
	{
		return _mipmaps;
	}

	IntProperty* TextureBlock::getBaseLevel()
	{
		return _baseLevel;
	}


	IntProperty* TextureBlock::getMaxLevel()
	{
		return _maxLevel;
	}

	BoolProperty* TextureBlock::getAutoSize()
	{
		return _autoSize;
	}
}
