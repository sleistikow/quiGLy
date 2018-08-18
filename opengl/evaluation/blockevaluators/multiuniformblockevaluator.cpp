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

#include "multiuniformblockevaluator.h"
#include "opengl/glconfiguration.h"
#include "opengl/glrenderpass.h"
#include "opengl/evaluation/setuprenderingevaluator.h"

#include "data/iblock.h"
#include "data/iport.h"
#include "data/iconnection.h"
#include "data/properties/property.h"
#include "data/blocks/porttype.h"

#include <QOpenGLShaderProgram>

namespace ysm
{

	MultiUniformBlockEvaluator::MultiUniformBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> MultiUniformBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::ModelViewProjection
								  << BlockType::LightSource
								  << BlockType::Material;
	}

	bool MultiUniformBlockEvaluator::isRendertimeEvaluated() const
	{
		return true;
	}

	void MultiUniformBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get shaderprogram
		QOpenGLShaderProgram* program = getEvaluator()->getShaderProgram(pass);

		for(IConnection* connection : pass->getOutConnections(block))
		{
			// Get uniform location
			GLint location = -1;
			if(connection->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)->getValue())
				location = connection->getProperty<UIntProperty>(PropertyID::Uniform_Location)->getValue();
			else
				location = program->uniformLocation(*connection->getProperty<StringProperty>(PropertyID::Uniform_Name));

			switch(connection->getSourcePort()->getType())
			{
			// Handle Model-View-Projection Ports
			case PortType::MVP_MatM:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::MVP_MatM));
				break;
			case PortType::MVP_MatMV:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::MVP_MatMV));
				break;
			case PortType::MVP_MatMVP:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::MVP_MatMVP));
				break;
			case PortType::MVP_MatNormal:
				program->setUniformValue(location, *block->getProperty<Mat3x3Property>(PropertyID::MVP_MatNormal));
				break;
			case PortType::MVP_MatP:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::MVP_MatP));
				break;
			case PortType::MVP_MatV:
				program->setUniformValue(location, *block->getProperty<Mat4x4Property>(PropertyID::MVP_MatV));
				break;

			// Handle Light-Source Ports
			case PortType::Light_Position:
				program->setUniformValue(location, *block->getProperty<Vec4Property>(PropertyID::Light_Position));
				break;
			case PortType::Light_Direction:
				program->setUniformValue(location, *block->getProperty<Vec3Property>(PropertyID::Light_Direction));
				break;
			case PortType::Light_ColorSpecular:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Light_ColorSpecular));
				break;
			case PortType::Light_ColorAmbient:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Light_ColorAmbient));
				break;
			case PortType::Light_ColorDiffuse:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Light_ColorDiffuse));
				break;
			case PortType::Light_SpotAngle:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Light_SpotAngle));
				break;
			case PortType::Light_SpotExponent:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Light_SpotExponent));
				break;
			case PortType::Light_AttenuationConstant:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Light_AttenuationConstant));
				break;
			case PortType::Light_AttenuationLinear:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Light_AttenuationLinear));
				break;
			case PortType::Light_AttenuationQuadratic:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Light_AttenuationQuadratic));
				break;

			// Handle Material Ports
			case PortType::Material_Ambient:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Material_Ambient));
				break;
			case PortType::Material_Diffuse:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Material_Diffuse));
				break;
			case PortType::Material_Shininess:
				program->setUniformValue(location, *block->getProperty<FloatProperty>(PropertyID::Material_Shininess));
				break;
			case PortType::Material_Specular:
				program->setUniformValue(location, *block->getProperty<ColorProperty>(PropertyID::Material_Specular));
				break;

			default:
				qDebug() << "Unhandled Uniform port found.";
			}
		}
	}

}
