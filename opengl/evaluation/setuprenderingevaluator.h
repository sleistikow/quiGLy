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

#ifndef SETUPRENDERINGEVALUATOR_H
#define SETUPRENDERINGEVALUATOR_H

#include "iglrenderpassevaluator.h"

#include <QLinkedList>
#include <QMap>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

namespace ysm
{
	class IBlock;
	class IConnection;
	class IBlockEvaluator;
	class GLWrapper;
	class GLRenderPass;
	class GLRenderPassSet;

	class SetupRenderingEvaluator : public IGLRenderPassEvaluator
	{
	public:
		// Types
		struct Warning {
			QString message;
			IBlock* block;
		};

	public:
		SetupRenderingEvaluator();
		virtual ~SetupRenderingEvaluator();

		/// @brief Adds a warning to the internal list
		void addWarning(const Warning& warning);

		/// @brief Returns all warning gathered during the last evaluation
		const QList<Warning>& getWarnings() const;

	public:
		// IGLRenderPassEvaluator
		void clear(bool destruct = false) Q_DECL_OVERRIDE;
		void evaluate(GLRenderPassSet* renderPassSet) Q_DECL_OVERRIDE;
		BlockType getEvaluatedBlockType() const Q_DECL_OVERRIDE;

	public:
		// Acces for GLRenderView
		/// @brief Return the evaluated data to the given block, if any.
		template<typename T = GLWrapper>
		T* getEvaluatedData(IBlock* block) const;

		/// @brief Returns the Shaderprogram used in the given pass.
		QOpenGLShaderProgram* getShaderProgram(GLRenderPass* pass) const;

		/// @brief Maps evaluated data to the given block.
		void setEvaluatedData(IBlock* block, GLWrapper* data, bool replace = true);

		/// @brief Applies the settings provides by the specified block to the currently active OpenGL context
		void initializeContext(IBlock* block, GLRenderPass* pass);

	private:

		/// @brief Registers a particular evaluator and updates the evaluation order
		template<typename T>
		void registerBlockEvaluator();

		/// @brief Builds the order, blocks are going to be evaluated in.
		void buildEvaluationOrder();

	private:

		QLinkedList<BlockType> _evaluationOrder;
		QMap<IBlock*, GLWrapper*> _evaluationData;
		QMap<BlockType, IBlockEvaluator*> _evaluators;

		QMap<BlockType, IBlockEvaluator*> _initializers;
		QMap<GLRenderPass*, QOpenGLShaderProgram*> _shaderPrograms;

		QList<Warning> _warnings;
	};

	template<typename T>
	T* SetupRenderingEvaluator::getEvaluatedData(IBlock* block) const
	{
		return dynamic_cast<T*>(_evaluationData.value(block, nullptr));
	}

	template<typename T>
	void SetupRenderingEvaluator::registerBlockEvaluator()
	{
		T* evaluator = new T(this);

		if(evaluator->isRendertimeEvaluated())
			for(BlockType type : evaluator->getEvaluatedTypes())
				_initializers[type] = evaluator;
		else
			for(BlockType type : evaluator->getEvaluatedTypes())
				_evaluators[type] = evaluator;
	}

}

#endif // SETUPRENDERINGEVALUATOR_H
