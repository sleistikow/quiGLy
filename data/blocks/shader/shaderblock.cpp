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

#include "shaderblock.h"
#include "data/blocks/portlist.h"
#include "data/blocks/port.h"
#include "data/properties/propertylist.h"
#include "data/properties/glsldocumentlistproperty.h"

namespace ysm
{
	ShaderBlock::ShaderBlock(Pipeline* parent, BlockType block_type, QString name) : Block(parent, block_type, name)
	{

	}

    GLSLDocumentListProperty* ShaderBlock::getShaderCode()
	{
		return _shaderCode;
	}


	Port* ShaderBlock::getGenericInPort()
	{
		return _inPort;
	}

	Port* ShaderBlock::getGenericOutPort()
	{
		return _outPort;
	}

	Port* ShaderBlock::getTextureInPort()
	{
		return _textureInPort;
	}

	Port* ShaderBlock::getUniformInPort()
	{
		return _uniformInPort;
	}

	Port* ShaderBlock::getUboInPort()
	{
		return _uboInPort;
	}

	Port* ShaderBlock::getSsboInPort()
	{
		return _ssboInPort;
	}

	Port* ShaderBlock::getSsboOutPort()
	{
		return _ssboOutPort;
	}

	Port* ShaderBlock::getAtomicCountersInPort()
	{
		return _atomicCounterInPort;
	}

	void ShaderBlock::createProperties()
	{
		Block::createProperties();

        _shaderCode = _properties->newProperty<GLSLDocumentListProperty>(PropertyID::Shader_Code, "Shader Code");
	}

	void ShaderBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");

		_textureInPort = _ports->newPort(PortType::Shader_Texture, PortDirection::In, "Texture In");
		_uniformInPort = _ports->newPort(PortType::Shader_Uniform, PortDirection::In, "Uniform In");
		_uboInPort = _ports->newPort(PortType::Shader_UBO, PortDirection::In, "UBO In");
		_ssboInPort = _ports->newPort(PortType::Shader_SSBOIn, PortDirection::In, "SSBO In");
		_ssboInPort->setMinimumVersion(430);
		_ssboOutPort = _ports->newPort(PortType::Shader_SSBOOut, PortDirection::Out, "SSBO Out");
		_ssboOutPort->setMinimumVersion(430);
		_atomicCounterInPort = _ports->newPort(PortType::Shader_AtomicCounterIn, PortDirection::In, "Atomic Counter");
		_atomicCounterInPort->setMinimumVersion(420);
	}

}
