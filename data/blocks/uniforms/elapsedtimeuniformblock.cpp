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

#include "elapsedtimeuniformblock.h"

#include <QElapsedTimer>

namespace ysm
{

	QElapsedTimer* ElapsedTimeUniformBlock::TIMER = nullptr;

	QElapsedTimer* ElapsedTimeUniformBlock::getElapsedTimer()
	{
		if(!TIMER)
			TIMER = new QElapsedTimer;

		return TIMER;
	}

	void ElapsedTimeUniformBlock::resetElapsedTime()
	{
		getElapsedTimer()->restart();
	}

	ElapsedTimeUniformBlock::ElapsedTimeUniformBlock(Pipeline* parent) : UniformBlock(parent, block_type, "Elapsed Time Uniform")
	{

	}

	void ElapsedTimeUniformBlock::createProperties()
	{
		UniformBlock::createProperties();

		_value = _properties->newProperty<IntProperty>(PropertyID::Uniform_Value, "Value", true);
		_value->setSerializable(false);
		_value->delegateValue(
					[this]()->int& { static int __ret = 0; __ret = (int) (getElapsedTimer()->elapsed() % INT16_MAX); return __ret; },
					nullptr,
					[this](bool clear)->bool { return !clear; });
	}

	IntProperty* ElapsedTimeUniformBlock::getValue()
	{
		return _value;
	}

	unsigned int ElapsedTimeUniformBlock::getOutputSize(IPort* port) const
	{
		Q_UNUSED(port);

		return sizeof(uniform_value_type);
	}

	QByteArray ElapsedTimeUniformBlock::retrieveUniformData(IPort* port) const
	{
		Q_UNUSED(port);

		return retrieveData<uniform_value_type>(_value);
	}
}

