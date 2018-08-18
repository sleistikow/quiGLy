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

#ifndef GLRENDERPASSSET_H
#define GLRENDERPASSSET_H

#include "data/blocks/blocktype.h"

#include <QVector>
#include <QMap>
#include <QSet>

namespace ysm
{
	class IBlock;
	class IPipeline;
	class IRenderCommand;
	class GLRenderPass;

	class GLRenderPassSet
	{
	private:

		static bool isPassSwitchIndicator(IBlock* block);

	public:

		/**
		 * @brief GLRenderPassSet Constructs a new GLPipelineSet instance.
		 * @param outputBlock block to create the set for
		 */
		GLRenderPassSet(IBlock* outputBlock);

		/// @brief Destructs this instance.
		virtual ~GLRenderPassSet();

		/// @brief Determines, whether all passes are valid.
		bool isValid() const;

		/// @brief Returns the pipeline, set set was created in.
		IPipeline* getPipeline() const;

		/// @brief Returns the block, the set was created for.
		IBlock* getOutputBlock() const;

		/// @bief Returns all RenderPasses logically and actually connected to the end Block.
		const QList<GLRenderPass*>& getRenderPasses() const;

	private:

		/// @brief Returns a list containing all render commands assigned to the specified block
		QList<IRenderCommand*> findAssignedRenderCommands(IBlock* block);

	private:

		/// @brief The pipeline the passes are part of
		IPipeline* _pipeline;

		/// @brief The block the passes will be created for
		IBlock* _outputBlock;

		/// @brief The actually determined passes
		QList<GLRenderPass*> _passes;

		/// @brief validity flag
		bool _valid;

		/**
		 * @brief Interally used iterator to collect all components of a Renderpass.
		 */
		class PipelineIterator
		{
		public:
			// Construction
			/// @brief Constructs a new iterator
			PipelineIterator(IBlock* begin, GLRenderPassSet* parent);

		public:
			// Iteration
			/// @brief Iterates one step.
			QVector<PipelineIterator*> iterate();

		public:
			// Access
			/// @brief Determines, whether this iterator is valid.
			bool isValid() const;

			/// @brief Gets the renderpass build by this iterator.
			GLRenderPass* getRenderPass() const;

		private:

			/// @brief Parent Set, which created this iterator.
			GLRenderPassSet* _parent;

			/// @brief The renderpass, the iterator is going to fill.
			GLRenderPass* _pass;

			/// @brief The Blocks, the a new pass is created on
			QMap<IBlock*, QSet<GLRenderPass*> > _passSwitches;

			/// @brief The block front is a set of most left blocks, which the iterator reached so far.
			QSet<IBlock*> _blockFront;
		};
	};

}

#endif // GLRENDERPASSSET_H
