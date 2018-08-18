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

#include "displayblock.h"
#include "portlist.h"
#include "port.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	DisplayBlock::DisplayBlock(Pipeline* parent) : Block(parent, block_type, "Display")
	{

	}

    BoolProperty* DisplayBlock::getVisibility()
    {
        return _visible;
    }

	BoolProperty* DisplayBlock::getMultiSample()
	{
		return _multiSample;
	}

	UIntProperty* DisplayBlock::getSamples()
	{
		return _samples;
	}

	Port* DisplayBlock::getGenericInPort()
	{
		return _inPort;
	}

	void DisplayBlock::createPorts()
	{
		Block::createPorts();

		_inPort = _ports->newPort(PortType::GenericIn, PortDirection::In, "In");
	}

    void DisplayBlock::createProperties()
    {
        Block::createProperties();

		_visible = _properties->newProperty<BoolProperty>(PropertyID::Display_Visible, "Visible");
		*_visible = true;

		_multiSample = _properties->newProperty<BoolProperty>(PropertyID::Display_MultiSample, "Multisampling");
		*_multiSample = true;

		_samples = _properties->newProperty<UIntProperty>(PropertyID::Display_Samples, "Number of samples");
		*_samples = 4;

		_gammaCorrection = _properties->newProperty<BoolProperty>(PropertyID::Display_GammaCorrection, "Gamma correction (sRGB)");
		*_gammaCorrection = true;
    }	
}
