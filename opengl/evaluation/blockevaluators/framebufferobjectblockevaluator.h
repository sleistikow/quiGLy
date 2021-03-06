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

#ifndef FRAMEBUFFEROBJECTBLOCKEVALUATOR_H
#define FRAMEBUFFEROBJECTBLOCKEVALUATOR_H

#include "blockevaluator.h"

namespace ysm
{
	class FrameBufferObjectBlockEvaluator : public BlockEvaluator
	{
	public:
		FrameBufferObjectBlockEvaluator(SetupRenderingEvaluator* evaluator);

	public:
		// BlockEvaluator
		void evaluate(IBlock* block, GLRenderPass* pass) Q_DECL_OVERRIDE;
		QList<BlockType> getEvaluatedTypes() const Q_DECL_OVERRIDE;

		// a FBO has no dependency, because all the attachments are evaluated in the pass before
		//QList<BlockType> getDependencies() const Q_DECL_OVERRIDE;
	};
}

#endif // FRAMEBUFFEROBJECTBLOCKEVALUATOR_H
