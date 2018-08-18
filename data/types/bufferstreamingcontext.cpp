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

#include "bufferstreamingcontext.h"

#include <stdexcept>

namespace ysm
{
	BufferStreamingContext::BufferStreamingContext(QByteArray* bufferData) : _bufferData{bufferData}
	{
		if (!bufferData)
			throw std::invalid_argument{"bufferData may not be null"};
	}

	QByteArray* BufferStreamingContext::getBufferData()
	{
		return _bufferData;
	}

	unsigned int BufferStreamingContext::getCurrentDataIndex(const MixerBlock* block)
	{
		if (!_dataIndices.contains(block))
			_dataIndices[block] = 0;

		return _dataIndices[block];
	}

	void BufferStreamingContext::advanceDataIndex(const MixerBlock* block)
	{
		if (!_dataIndices.contains(block))
			_dataIndices[block] = 0;

		_dataIndices[block] += 1;
	}
}
