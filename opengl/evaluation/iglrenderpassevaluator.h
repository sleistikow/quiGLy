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

#ifndef IGLRENDERPASSEVALUATOR_H
#define IGLRENDERPASSEVALUATOR_H

#include "data/blocks/blocktype.h"

namespace ysm
{
	class IBlock;
	class GLRenderPassSet;

	class IGLRenderPassEvaluator
	{
	public:

		/// @brief Destructs this instance
		virtual ~IGLRenderPassEvaluator() {}

		/// @brief Clears all the data from the last evaluation.
		virtual void clear(bool destruct = false) = 0;

		/// @brief Evaluates the given pass.
		virtual void evaluate(GLRenderPassSet* renderPassSet) = 0;

		/// @brief Returns the Blocktype which can be evaluated by this instance.
		virtual BlockType getEvaluatedBlockType() const = 0;

	protected:

		/// @brief Destruct instance.
		IGLRenderPassEvaluator() {}

	};

}

#endif // IGLRENDERPASSEVALUATOR_H
