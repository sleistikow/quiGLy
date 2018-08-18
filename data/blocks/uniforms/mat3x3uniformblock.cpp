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

#include "mat3x3uniformblock.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	Mat3x3UniformBlock::Mat3x3UniformBlock(Pipeline* parent) : UniformBlock(parent, block_type, "3x3 Matrix Uniform")
	{

	}

	void Mat3x3UniformBlock::createProperties()
	{
		UniformBlock::createProperties();

		_value = _properties->newProperty<Mat3x3Property>(PropertyID::Uniform_Value, "Value");
	}

	Mat3x3Property* Mat3x3UniformBlock::getValue()
	{
		return _value;
	}

	unsigned int Mat3x3UniformBlock::getOutputSize(IPort* port) const
	{
		Q_UNUSED(port);

		return 3 * 3 * sizeof(float); // Hardcoded so we definitely get the correct size
	}

	QByteArray Mat3x3UniformBlock::retrieveUniformData(IPort* port) const
	{
		Q_UNUSED(port);

		QByteArray data;
		uniform_value_type v = *_value;

		data.append(reinterpret_cast<char*>(v.data()), getOutputSize(port));
		return data;
	}
}