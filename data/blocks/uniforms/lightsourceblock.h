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

#ifndef LIGHTSOURCEBLOCK_H
#define LIGHTSOURCEBLOCK_H

#include "uniformbaseblock.h"
#include "data/blocks/port.h"
#include "data/blocks/porttype.h"
#include "data/blocks/portlist.h"
#include "data/blocks/connection.h"
#include "data/blocks/connectionlist.h"
#include "data/properties/property.h"

namespace ysm
{
	class LightSourceBlock : public UniformBaseBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::LightSource};

		explicit LightSourceBlock(Pipeline* pipeline);

	public:
		// Raw data functions
		unsigned int getOutputSize(IPort* port) const override;
		QByteArray retrieveUniformData(IPort* port) const override;

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		Vec4Property* _lightPosition{nullptr};
		Vec3Property* _lightDirection{nullptr};

		FloatProperty* _spotExponent{nullptr};
		FloatProperty* _spotAngle{nullptr};

		ColorProperty* _colorAmbient{nullptr};
		ColorProperty* _colorDiffuse{nullptr};
		ColorProperty* _colorSpecular{nullptr};

		FloatProperty* _attenuationConstant{nullptr};
		FloatProperty* _attenuationLinear{nullptr};
		FloatProperty* _attenuationQuadratic{nullptr};

		Port* _posPort{nullptr};
		Port* _dirPort{nullptr};
		Port* _expPort{nullptr};
		Port* _angPort{nullptr};
		Port* _ambiPort{nullptr};
		Port* _diffPort{nullptr};
		Port* _specPort{nullptr};
		Port* _attConstPort{nullptr};
		Port* _attLinPort{nullptr};
		Port* _attQuadPort{nullptr};
	};
}

#endif // LIGHTSOURCEBLOCK_H
