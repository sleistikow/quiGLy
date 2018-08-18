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

#include "rasterizationblockevaluator.h"
#include "opengl/glconfiguration.h"

#include "data/blocks/rasterizationblock.h"

namespace ysm
{

	RasterizationBlockEvaluator::RasterizationBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> RasterizationBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::Rasterization;
	}

	bool RasterizationBlockEvaluator::isRendertimeEvaluated() const
	{
		return true;
	}

	void RasterizationBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);


		// Get a functions object on the currently active context
		GLConfiguration::Functions* f = QOpenGLContext::currentContext()->versionFunctions<GLConfiguration::Functions>();

		//FronFace settings
		int frontFace = *block->getProperty<EnumProperty>(PropertyID::Rasterization_FrontFace);
		switch (frontFace) {
		case RasterizationBlock::FrontFace_Clockwise: f->glFrontFace(GL_CW);
			break;
		default: f->glFrontFace(GL_CCW);
			break;
		}

		//Cullmode settings
		if(*block->getProperty<BoolProperty>(PropertyID::Rasterization_EnableCulling))
		{
			f->glEnable(GL_CULL_FACE);

			int cullMode = *block->getProperty<EnumProperty>(PropertyID::Rasterization_CullFaceMode);
			switch (cullMode) {
			case RasterizationBlock::CullFace_Front: f->glCullFace(GL_FRONT);
				break;
			case RasterizationBlock::CullFace_Both: f->glCullFace(GL_FRONT_AND_BACK);
				break;
			default: f->glCullFace(GL_BACK);
				break;
			}
		}
		else
		{
			f->glDisable(GL_CULL_FACE);
		}

		//PolygonMode settings
		int polygonMode = *block->getProperty<EnumProperty>(PropertyID::Rasterization_PolygonMode);
		switch (polygonMode) {
		case RasterizationBlock::PolyMode_Point: f->glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case RasterizationBlock::PolyMode_Line: f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default: f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}

		//linewidth and pointsize
		bool antialiasingLineEnabled = block->getProperty<BoolProperty>(PropertyID::Rasterization_EnableLineAntialiasing)->getValue();
		if(antialiasingLineEnabled)
			f->glEnable(GL_LINE_SMOOTH);
		else
			f->glDisable(GL_LINE_SMOOTH);
		f->glLineWidth(block->getProperty<FloatProperty>(PropertyID::Rasterization_LineWidth)->getValue());
		bool antialiasingPointEnabled = block->getProperty<BoolProperty>(PropertyID::Rasterization_EnablePointAntialiasing)->getValue();
		if(antialiasingPointEnabled)
			f->glEnable(GL_POINT_SMOOTH);
		else
			f->glDisable(GL_POINT_SMOOTH);
		f->glPointSize(block->getProperty<FloatProperty>(PropertyID::Rasterization_PointSize)->getValue());
	}
}
