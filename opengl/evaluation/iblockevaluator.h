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

#ifndef IBLOCKEVALUATOR
#define IBLOCKEVALUATOR

#include <QList>
#include "data/blocks/blocktype.h"

namespace ysm
{

	class IBlock;
	class GLRenderPass;

	class IBlockEvaluator
	{
	public:

		/// @brief Destructs this instance
		virtual ~IBlockEvaluator() {}

		/// @brief Evaluates the given block as part of the given pass
		virtual void evaluate(IBlock* block, GLRenderPass* pass) = 0;

		/// @brief Returns the dependencies, which have to be evaluated before this
		virtual QList<BlockType> getDependencies() const = 0;

		/// @brief Returns the type which can be evaluated by this evaluator
		virtual QList<BlockType> getEvaluatedTypes() const = 0;

		/// @brief Determines, whether this evaluator needs to be evaluated at rendertime
		virtual bool isRendertimeEvaluated() const = 0;

	protected:

		/// @brief Initialize new instance.
		IBlockEvaluator() {}

	};

}

#endif // IBLOCKEVALUATOR

