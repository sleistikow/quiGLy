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

#include "tessellationprimitivegeneratorblockevaluator.h"
#include "opengl/evaluation/evaluationexception.h"

#include "data/iblock.h"
#include "data/properties/property.h"

#include <QOpenGLFunctions_4_0_Core>

namespace ysm
{

	TessellationPrimitiveGeneratorBlockEvaluator::TessellationPrimitiveGeneratorBlockEvaluator(SetupRenderingEvaluator* evaluator)
		: BlockEvaluator(evaluator)
	{
	}

	QList<BlockType> TessellationPrimitiveGeneratorBlockEvaluator::getEvaluatedTypes() const
	{
		return QList<BlockType>() << BlockType::TessellationPrimitiveGenerator;
	}

	bool TessellationPrimitiveGeneratorBlockEvaluator::isRendertimeEvaluated() const
	{
		return true;
	}

	void TessellationPrimitiveGeneratorBlockEvaluator::evaluate(IBlock* block, GLRenderPass* pass)
	{
		// Catch most of the trivial errors
		BlockEvaluator::evaluate(block, pass);

		// Get a functions object on the currently active context
		QOpenGLFunctions_4_0_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();
		if(!f)
			throw EvaluationException("Tessellation Primitive Generator needs OpenGL 4.0, which is not supported by your system", block);

		f->glPatchParameteri(GL_PATCH_VERTICES, *block->getProperty<UIntProperty>(PropertyID::TessellationPrimitiveGenerator_PatchVertices));
		f->glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, (*block->getProperty<FloatDataProperty>(PropertyID::TessellationPrimitiveGenerator_PatchDefaultInnerLevel))->data());
		f->glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, (*block->getProperty<FloatDataProperty>(PropertyID::TessellationPrimitiveGenerator_PatchDefaultOuterLevel))->data());
	}

}
