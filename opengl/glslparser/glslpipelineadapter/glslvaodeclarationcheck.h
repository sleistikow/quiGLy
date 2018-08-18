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

#ifndef GLSLVAODECLARATIONADAPTER_H
#define GLSLVAODECLARATIONADAPTER_H

#include "glsldeclarationcheck.h"
#include "data/properties/vaolayoutproperty.h"

namespace ysm
{
	class GLSLDeclaration;

	//! \brief Checks the uniforms integrity between a shader block and it's GLSL code.
	class GLSLVaoDeclarationCheck : public GLSLDeclarationCheck<VaoLayout::VaoLayoutEntry, GLSLVaoDeclarationCheck>
	{
	public:

		//! \brief Initialize new instance.
		GLSLVaoDeclarationCheck();

		/*!
		 * \brief Helper function: Iterates through the pipeline, and finds the first in block of the given type.
		 * \param startBlock The start block.
		 * \param blockType The block type to find.
		 * \param loopBreaker Block that is used to break loops (should be the initial block).
		 * \return The first found block or NULL.
		 */
		IBlock* findFirstInBlockByType(IBlock* startBlock, BlockType blockType, IBlock* loopBreaker = 0) const;

	protected:

		/*!
		 * \brief Checks, wether the declaration is handled by the declaration check.
		 * \param declaration The declaration.
		 * \return True, if handled.
		 */
		bool isHandled(GLSLDeclaration* declaration) const Q_DECL_OVERRIDE;

		/*!
		 * \brief Retrieves a list of pipeline elements that are compared against the available statements.
		 * \param pipelineBlock The pipeline block.
		 * \return The list of statements.
		 */
		QList<VaoLayout::VaoLayoutEntry> beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the elements GLSL name, as matched against the code
		 * Can furthermore be used to store element data in subclasses.
		 * \param pipelineElement The pipeline element.
		 * \return The GLSL name.
		 */
		void extractElementData(VaoLayout::VaoLayoutEntry pipelineElement, GLSLCodeBlock* codeBlock) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the element's identifier (as declared in GLSL).
		 * \param pipelineElement The pipeline element.
		 * \return The GLSL name.
		 */
		QString getElementName(VaoLayout::VaoLayoutEntry pipelineElement) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the elements GLSL statement.
		 * \param pipelineElement The pipeline element.
		 * \return The statement data.
		 */
		FixedStatement getElementStatement(VaoLayout::VaoLayoutEntry pipelineElement) const Q_DECL_OVERRIDE;

	private:

		/*!
		 * \brief Checks the location.
		 * \param connection The connection.
		 * \param layoutEntry The layout entry.
		 */
		bool checkLocationIntegrity(VaoLayout::VaoLayoutEntry layoutEntry, GLSLDeclaration* declaration);

		/*!
		 * \brief Checks the location.
		 * \param connection The connection.
		 * \param layoutEntry The layout entry.
		 */
		bool checkTypeIntegrity(VaoLayout::VaoLayoutEntry layoutEntry, GLSLDeclaration* declaration);

	private:

		//! \brief The vao block.
		IBlock* _vaoBlock;

		//! \brief Contains all GLSL types.
		QMap<GLSLDataType, QString> _glslTypes;
	};

}

#endif // GLSLVAODECLARATIONADAPTER_H
