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

#ifndef GLSLPIPELINECHECK_H
#define GLSLPIPELINECHECK_H

#include "iglslpipelinecheck.h"
#include "../glslquickfix.h"

namespace ysm
{
	class GLSLDeclaration;

	//! \brief Base class for declaration checks, provides helper routines.
	template<typename T, typename S> class GLSLDeclarationCheck : public IGLSLPipelineCheck
	{
	public:

		//! \brief Initialize new instance.
		GLSLDeclarationCheck();

		/*!
		 * \brief Prepares the pipeline check and returns the fixed statements.
		 * \param pipelineBlock The pipeline block.
		 * \param codeBlock The code block.
		 */
		QList<FixedStatement> preCheck(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock) Q_DECL_OVERRIDE;

		/*!
		 * \brief Executes the pipeline check, which sets declaration errors.
		 * \param pipelineBlock The pipeline block.
		 * \param codeBlock The code block.
		 */
		void postCheck(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock) Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Maps all statements in the given block that are relevant to this check to their GLSL code name.
		 * \param codeBlock The code block that contains the declarations.
		 * \returns The map containing the declarations by name.
		 */
		QMap<QString, GLSLDeclaration*> mapStatements(GLSLCodeBlock* codeBlock);

		/*!
		 * \brief Checks, wether the declaration is handled by the declaration check.
		 * \param declaration The declaration that is checked.
		 * \return True, if handled.
		 */
		virtual bool isHandled(GLSLDeclaration* declaration) const = 0;

		/*!
		 * \brief Retrieves a list of pipeline elements that are compared against the available statements.
		 * Can furthermore be used to store pipeline data in subclasses. Called once before the pre-checks are run,
		 * and once before the post-checks are run.
		 * \param pipelineBlock The pipeline block that is checked against.
		 * \param codeBlock The code block that contains the declarations.
		 * \return The list of pipeline elements that must be represented in code.
		 */
		virtual QList<T> beginPipelineParse(IBlock* pipelineBlock, GLSLCodeBlock* codeBlock) = 0;

		/*!
		 * \brief Can be used to store element data in subclasses, before the integrity check is executed.
		 * This is called once before the integrity checks for the given element are executed, or before it's
		 * statement is created.
		 * \param pipelineElement The pipeline element that is represented in code.
		 * \param codeBlock The code block that contains the declarations.
		 */
		virtual void extractElementData(T pipelineElement, GLSLCodeBlock* codeBlock) = 0;

		/*!
		 * \brief Integrity check was executed successfully. Store data to document, if needed.
		 * \param pipelineElement The pipeline element that is represented in code.
		 * \param codeBlock The code block that contains the declarations.
		 * \param declaration The declaration that represents this element.
		 */
		virtual void storeDocumentData(T pipelineElement, GLSLCodeBlock* codeBlock, GLSLDeclaration* declaration);

		/*!
		 * \brief Gets the element's identifier (as declared in GLSL).
		 * \param pipelineElement The pipeline element that is represented in code.
		 * \return The GLSL name of that pipeline element.
		 */
		virtual QString getElementName(T pipelineElement) = 0;

		/*!
		 * \brief Gets the elements GLSL statement.
		 * \param pipelineElement The pipeline element that is represented in code.
		 * \return The pipeline element's GLSL representation.
		 */
		virtual FixedStatement getElementStatement(T pipelineElement) const = 0;

		/*!
		 * \brief True if missing errors can be fixed quickly.
		 * \return True, if possible.
		 */
		virtual bool canFixMissingElement() const;

		/*!
		 * \brief Fixes a missing pipeline element.
		 * \param declaration The declaration, that holds the data for the missing element.
		 */
		virtual void fixMissingElement(Document* document, GLSLDeclaration* declaration);

		/*!
		 * \brief Fixes a missing pipeline element.
		 * \param declaration The declaration, that holds the data for the missing element.
		 */
		virtual void fixMissingElement(GLSLDeclaration* declaration);

		/*!
		 * \brief Registers a new integrity check, that ensures GLSL matches the pipeline data.
		 * \param integrityCheck The integrity check.
		 * \param onPreCheck If true, the integrity check is executed on pre-check, otherwise on post-check.
		 */
		void registerIntegrityCheck(bool(S::*integrityCheck)(T, GLSLDeclaration*), bool onPreCheck);

		/*!
		 * \brief Runs the given list of integrity checks on the given set of pipeline elements.
		 * \param pipelineElements The list of pipeline elements that must be represented in code.
		 * \param codeBlock The code block that contains the declarations.
		 * \param pipelineBlock The pipeline block that is checked against.
		 * \param integrityChecks The integrity checks to execute.
		 */
		void runIntegrityChecks(QList<T> pipelineElements, GLSLCodeBlock* codeBlock, IBlock* pipelineBlock,
								QList<bool(S::*)(T, GLSLDeclaration*)> integrityChecks);

		/*!
		 * \brief Integrity check for empty body.
		 * \param connection The connection.
		 * \param declaration The declaration.
		 * \return True if declaration matches connection.
		 */
		bool ensureEmptyBody(T connection, GLSLDeclaration* declaration);

		/*!
		 * \brief Integrity check for non-empty body.
		 * \param connection The connection.
		 * \param declaration The declaration.
		 * \return True if declaration matches connection.
		 */
		bool ensureHasBody(T connection, GLSLDeclaration* declaration);

		/*!
		 * \brief Integrity check for non-array type.
		 * \param connection The connection.
		 * \param declaration The declaration.
		 * \return True if declaration matches connection.
		 */
		bool ensureNoArray(T connection, GLSLDeclaration* declaration);

		/*!
		 * \brief Generate the declaration body for the given declaration.
		 * \param declaration The declaration.
		 * \param level The indentation level.
		 * \return The body text.
		 */
		QStringList generateDeclarationBody(GLSLDeclaration* declaration, int level = 1) const;

	private:

		//! \brief The integrity checks.
		QList<bool(S::*)(T, GLSLDeclaration*)> _integrityPreChecks;
		QList<bool(S::*)(T, GLSLDeclaration*)> _integrityPostChecks;

		//! \brief Quick fixes.
		GLSLQuickFix<GLSLDeclaration, GLSLDeclarationCheck<T, S>>* _fixMissingElementByCreation;
		GLSLQuickFix<GLSLStatement, IGLSLPipelineCheck>* _fixMissingElementByDeletion;
	};
}

#include "glsldeclarationcheck.h"
#include "../glslstatements/glslstatement.h"
#include "../glslstatements/glsldeclaration.h"
#include "../glslcodeblock.h"

namespace ysm
{
	template<typename T, typename S> GLSLDeclarationCheck<T, S>::GLSLDeclarationCheck()
	{
		//Create create element fix.
		_fixMissingElementByCreation = new GLSLQuickFix<GLSLDeclaration, GLSLDeclarationCheck<T, S>>(
					&GLSLDeclarationCheck<T, S>::fixMissingElement,
					&GLSLDeclarationCheck<T, S>::fixMissingElement,
					"Fix: Create Pipeline Element",
					this);

		//Create remove statement fix.
		_fixMissingElementByDeletion = new GLSLQuickFix<GLSLStatement, IGLSLPipelineCheck>(
					&IGLSLPipelineCheck::removeStatementCode,
					"Fix: Remove Statement",
					this);
	}

	template<typename T, typename S> QMap<QString, GLSLDeclaration*> GLSLDeclarationCheck<T, S>::mapStatements(
		GLSLCodeBlock* codeBlock)
	{
		//Iterate over all declarations and find and structurize the needed ones.
		QMap<QString, GLSLDeclaration*> declarations;
		foreach(GLSLDeclaration* declaration, codeBlock->getStatements<GLSLDeclaration>(GLSLStatementType::Declaration))
			if(isHandled(declaration) && !declarations.contains(declaration->getName()))
				declarations[declaration->getName()] = declaration;

		return declarations;
	}

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::runIntegrityChecks(
		QList<T> pipelineElements, GLSLCodeBlock* codeBlock, IBlock* pipelineBlock,
		QList<bool(S::*)(T, GLSLDeclaration*)> integrityChecks)
	{
		Q_UNUSED(pipelineBlock);
		Q_UNUSED(integrityChecks);

		//Map the statements to be able to access them by name/identifier and get the list of pipeline elements.
		QMap<QString, GLSLDeclaration*> availableDeclarations = mapStatements(codeBlock);

		//Iterate over all pipeline elements, and execute the integrity checks.
		foreach(T pipelineElement, pipelineElements)
		{
			//Get the pipeline element's data that is relevant for the checks.
			extractElementData(pipelineElement, codeBlock);
			QString pipelineName = getElementName(pipelineElement);

			//Try to find a declaration, that matches the pipeline element. If one is found, run the checks.
			if(availableDeclarations.contains(pipelineName))
			{
			   //Ensure that declaration and pipeline element match.
			   GLSLDeclaration* declaration = availableDeclarations[pipelineName];
			   S* specificDeclarationCheck = dynamic_cast<S*>(this);
			   foreach(bool(S::*integrityCheck)(T, GLSLDeclaration*), _integrityPreChecks)
				   (specificDeclarationCheck->*integrityCheck)(pipelineElement, declaration);
			}
		}
	}

	template<typename T, typename S> QList<IGLSLPipelineCheck::FixedStatement> GLSLDeclarationCheck<T, S>::preCheck(
		IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
	{
		//Map the statements to be able to access them by name/identifier and get the list of pipeline elements.
		QMap<QString, GLSLDeclaration*> availableDeclarations = mapStatements(codeBlock);
		QList<T> pipelineElements = beginPipelineParse(pipelineBlock, codeBlock);

		//Run the integrity pre-checks.
		runIntegrityChecks(pipelineElements, codeBlock, pipelineBlock, _integrityPreChecks);

		//Iterate over all pipeline elements and generate their fixed section code.
		QList<FixedStatement> fixedDeclarations;
		foreach(T pipelineElement, pipelineElements)
		{
			//Get the pipeline element's name back.
			QString pipelineName = getElementName(pipelineElement);

			//Again, try to find a declaration, that matches the pipeline element. Remove it, if it's correct.
			if(availableDeclarations.contains(pipelineName))
			{
				//If the element was declared correctly, remove it. It's then added to the fixed section.
				if(availableDeclarations[pipelineName]->getErrors().isEmpty())
				{
					storeDocumentData(pipelineElement, codeBlock, availableDeclarations[pipelineName]);
					removeStatementCode(availableDeclarations[pipelineName]);
					availableDeclarations.remove(pipelineName);
				}
			}

			//If the declaration is not part of the available declarations (anymore), it can be added to the fixed
			//section.
			if(!availableDeclarations.contains(pipelineName) && !pipelineName.isEmpty())
			{
				//Again, extract the element data, which is not cached anymore.
				extractElementData(pipelineElement, codeBlock);
				fixedDeclarations.append(getElementStatement(pipelineElement));
			}
		}

		//Return the missing declarations.
		return fixedDeclarations;
	}

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::postCheck(
		IBlock* pipelineBlock, GLSLCodeBlock* codeBlock)
	{
		//Map the statements to be able to access them by name/identifier and get the list of pipeline elements.
		QMap<QString, GLSLDeclaration*> availableDeclarations = mapStatements(codeBlock);
		QList<T> pipelineElements = beginPipelineParse(pipelineBlock, codeBlock);

		//Create a list of unhandled declarations, that do not have a matching element in the pipeline.
		QList<QString> unhandledDeclarations = availableDeclarations.keys();
		foreach(T pipelineElement, pipelineElements)
			unhandledDeclarations.removeAll(getElementName(pipelineElement));

		//Handle all obsolete declarations.
		foreach(QString declarationName, unhandledDeclarations)
		{
			//Get the declaration.
			GLSLDeclaration* declaration = availableDeclarations[declarationName];

			//Create quick fix list.
			QList<IGLSLQuickFix*> quickFixes;
			if(canFixMissingElement())
				quickFixes.append(_fixMissingElementByCreation);
			quickFixes.append(_fixMissingElementByDeletion);

			//Create the error.
			declaration->addPipelineError(
			{
				QString("'%1' does not have a matching element in pipeline.").arg(declarationName),
				declaration,
				quickFixes,
				canFixMissingElement() ? _fixMissingElementByCreation : NULL
			});
		}

		//Run the integrity post-checks on the available declarations.
		runIntegrityChecks(pipelineElements, codeBlock, pipelineBlock, _integrityPostChecks);
	}

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::registerIntegrityCheck(
		bool(S::*integrityCheck)(T, GLSLDeclaration*), bool onPreCheck)
	{
		//Just add the integrity check.
		(onPreCheck ? _integrityPreChecks : _integrityPostChecks).append(integrityCheck);
	}

	template<typename T, typename S> bool GLSLDeclarationCheck<T, S>::canFixMissingElement() const { return false; }

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::fixMissingElement(
		Document* document, GLSLDeclaration* declaration)
	{
		Q_UNUSED(document);
		Q_UNUSED(declaration);
	}

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::fixMissingElement(
		GLSLDeclaration* declaration)
	{
		Q_UNUSED(declaration);
	}

	template<typename T, typename S> bool GLSLDeclarationCheck<T, S>::ensureEmptyBody(
		T connection, GLSLDeclaration* declaration)
	{
		Q_UNUSED(connection);

		//Ensure empty body.
		QString codeType = declaration->getDataType();
		if(declaration->getBody())
		{
			declaration->addPipelineError(
			{
				QString("'%1' does not support complex data structure body.").arg(codeType),
				declaration,
				QList<IGLSLQuickFix*>(),
				NULL
			});

			return false;
		}

		//Successful check.
		return true;
	}

	template<typename T, typename S> bool GLSLDeclarationCheck<T, S>::ensureHasBody(
		T connection, GLSLDeclaration* declaration)
	{
		Q_UNUSED(connection);

		//Ensure empty body.
		QString codeName = declaration->getName();
		if(!declaration->getBody())
		{
			declaration->addPipelineError(
			{
				QString("'%1' must declare complex data structure body.").arg(codeName),
				declaration,
				QList<IGLSLQuickFix*>(),
				NULL
			});

			return false;
		}

		//Successful check.
		return true;
	}

	template<typename T, typename S> bool GLSLDeclarationCheck<T, S>::ensureNoArray(
		T connection, GLSLDeclaration* declaration)
	{
		Q_UNUSED(connection);

		//Ensure no array.
		QString codeName = declaration->getName();
		if(!declaration->getArray().isEmpty())
		{
			declaration->addPipelineError(
			{
				QString("'%1' must not be declared as array.").arg(codeName),
				declaration,
				QList<IGLSLQuickFix*>(),
				NULL
			});

			return false;
		}

		//Successful check.
		return true;
	}

	template<typename T, typename S> void GLSLDeclarationCheck<T, S>::storeDocumentData(
		T pipelineElement, GLSLCodeBlock* codeBlock, GLSLDeclaration* declaration)
	{
		Q_UNUSED(pipelineElement);
		Q_UNUSED(codeBlock);
		Q_UNUSED(declaration);
	}

	template<typename T, typename S> QStringList GLSLDeclarationCheck<T, S>::generateDeclarationBody(
		GLSLDeclaration* declaration, int level) const
	{
		//Set indentation level.
		QStringList declarationBody;
		QString indentation(level, '\t');

		//Iterate over all body statements.
		GLSLCodeBlock* bodyBlock = declaration->getBody();
		if(bodyBlock)
			foreach(GLSLDeclaration* statement, bodyBlock->getStatements<GLSLDeclaration>(GLSLStatementType::Declaration))
			{
				//If the statement does not have a body, add it with semicolon.
				if(!statement->getBody() && !statement->getDataType().isEmpty())
					declarationBody << indentation + statement->getText() + ";";

				//Otherwise, add it with body text and optional semicolon.
				if(statement->getBody() && statement->getDataType().isEmpty())
				{
					declarationBody << QString("%1%2").arg(indentation).arg(statement->getText());
					declarationBody << QString("%1{").arg(indentation);
					declarationBody.append(generateDeclarationBody(statement, level + 1));
					declarationBody << QString("%1}").arg(indentation);
					declarationBody << QString("%1%2")
						.arg(indentation)
						.arg(statement->getInstanceDeclaration() ? QString() : ";");
				}
			}

		//Ensure body is not empty, to add code.
		if(declarationBody.isEmpty())
			declarationBody << indentation;

		//Return all declaration body lines.
		return declarationBody;
	}
}

#endif // GLSLPIPELINECHECK_H
