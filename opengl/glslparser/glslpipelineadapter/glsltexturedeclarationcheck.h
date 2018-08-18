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

#ifndef GLSLTEXTUREDECLARATIONADAPTER_H
#define GLSLTEXTUREDECLARATIONADAPTER_H

#include "glsldeclarationcheck.h"
#include "data/blocks/textureblock.h"

#include <QMap>

namespace ysm
{
	class GLSLDeclaration;

	//! \brief Checks the texture uniforms integrity between a shader block and it's GLSL code.
    class GLSLTextureDeclarationCheck : public GLSLDeclarationCheck<IConnection*, GLSLTextureDeclarationCheck>
	{

	public:

		//! \brief Initialize new instance.
		GLSLTextureDeclarationCheck();

    protected:

        /*!
         * \brief Checks, wether the declaration is handled by the declaration check.
         * \param declaration The declaration.
         * \return True, if handled.
         */
        bool isHandled(GLSLDeclaration* declaration) const Q_DECL_OVERRIDE;

        /*!
         * \brief Retrieves a list of pipeline elements that are compared against the available statements.
         * Can furthermore be used to store pipeline data in subclasses.
         * \param pipelineBlock The pipeline block.
         * \return The list of statements.
         */
		QList<IConnection*> beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)  Q_DECL_OVERRIDE;

        /*!
         * \brief Gets the elements GLSL name, as matched against the code
         * Can furthermore be used to store element data in subclasses.
         * \param pipelineElement The pipeline element.
         * \return The GLSL name.
         */
		void extractElementData(IConnection* pipelineElement, GLSLCodeBlock* codeBlock)  Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the element's identifier (as declared in GLSL).
		 * \param pipelineElement The pipeline element.
		 * \return The GLSL name.
		 */
		QString getElementName(IConnection* pipelineElement) Q_DECL_OVERRIDE;

		/*!
		 * \brief Gets the elements GLSL statement.
		 * \param pipelineElement The pipeline element.
		 * \return The statement data.
		 */
		FixedStatement getElementStatement(IConnection* pipelineElement) const Q_DECL_OVERRIDE;

		/*!
		 * \brief True if missing errors can be fixed quickly.
		 * \return True, if possible.
		 */
		bool canFixMissingElement() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Fixes a missing pipeline element.
		 * \param document The document.
		 * \param declaration The declaration.
		 */
		void fixMissingElement(Document* document, GLSLDeclaration* declaration) Q_DECL_OVERRIDE;

		/*!
		 * \brief Fixes a missing pipeline element.
		 * \param declaration The declaration, that holds the data for the missing element.
		 */
		void fixMissingElement(GLSLDeclaration* declaration) Q_DECL_OVERRIDE;

    private:

        /*!
         * \brief Checks the location.
         * \param connection The connection.
         * \param declaration The declaration.
         */
        bool checkLocationIntegrity(IConnection* connection, GLSLDeclaration* declaration);

        /*!
         * \brief Checks the location.
         * \param connection The connection.
         * \param declaration The declaration.
         */
        bool checkTypeIntegrity(IConnection* connection, GLSLDeclaration* declaration);

    private:

        //! \brief The type of the current element.
		TextureBlock::SamplerType _pipelineType;

        //! \brief The type name of the current element.
        QString _pipelineTypeName;

        //! \brief The location of the current element.
        int _pipelineLocation;

        //! \brief The name of the current element.
        QString _pipelineName;

	};

}

#endif // GLSLTEXTUREDECLARATIONADAPTER_H
