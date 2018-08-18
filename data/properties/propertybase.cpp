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

#include "propertybase.h"
#include "data/ipipelineitem.h"
#include "data/ipipeline.h"

namespace ysm
{
	PropertyBase::PropertyBase()
	{

	}

	IPipelineItem* PropertyBase::getOwner() const
	{
		return _owner;
	}

	unsigned int PropertyBase::getVersion() const
	{
		return _owner->getPipeline()->getOpenGLVersion();
	}

	unsigned int PropertyBase::getMinimumVersion() const
	{
		return _minimumVersion;
	}

	unsigned int PropertyBase::getDeprecatedVersion() const
	{
		return _deprecatedVersion;
	}

	unsigned int PropertyBase::getMaximumVersion() const
	{
		return _maximumVersion;
	}

	void PropertyBase::setSerializable(const bool isSerializable)
	{
		_isSerializable = isSerializable;
	}

	bool PropertyBase::isSerializable() const
	{
		return _isSerializable;
	}

	void PropertyBase::setMinimumVersion(const unsigned int minVersion)
	{
		_minimumVersion = minVersion;
	}

	void PropertyBase::setDeprecatedVersion(const unsigned int depVersion)
	{
		_deprecatedVersion = depVersion;
	}

	void PropertyBase::setMaximumVersion(const unsigned int maxVersion)
	{
		_maximumVersion = maxVersion;
	}
}
