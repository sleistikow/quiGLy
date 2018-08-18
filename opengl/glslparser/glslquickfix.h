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

#ifndef GLSLQUICKFIX_H
#define GLSLQUICKFIX_H

#include <QObject>
#include <QAction>

namespace ysm
{
	class Document;
	class IPipeline;
	class GLSLStatement;

	//! \brief Non-templated base interface for quick fixes.
	class IGLSLQuickFix
	{
	public:

		/*!
		 * \brief Retrieve the user text.
		 * \return The text.
		 */
		virtual QString getText() const = 0;

		/*!
		 * \brief Executes the quick fix for the given statement.
		 * The fix will use the document and operate on the UI queue using commands.
		 * \param document The document.
		 * \param statement The statement.
		 */
		virtual void execute(Document* document, GLSLStatement* statement) = 0;

		/*!
		 * \brief Executes the quick fix for the given statement.
		 * The fix directly operate on the pipeline, so no notifications are sent.
		 * \param statement The statement.
		 */
		virtual void execute(GLSLStatement* statement) = 0;
	};

	//! \brief Wraps quick fix information for a given pipeline check.
	template<typename T, typename S> class GLSLQuickFix : public IGLSLQuickFix
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The delegate.
		 * \param quickFix Method called to fix the error.
		 * \param userText User text.
		 */
		GLSLQuickFix(void (S::*directFix)(T*), QString userText, S* delegate) :
			_delegate(delegate),
			_commandFix(NULL),
			_directFix(directFix),
			_userText(userText)
		{ }

		/*!
		 * \brief Initialize new instance.
		 * \param parent The delegate.
		 * \param quickFix Method called to fix the error.
		 * \param userText User text.
		 */
		GLSLQuickFix(void (S::*commandFix)(Document*, T*), void(S::*directFix)(T*), QString userText, S* delegate) :
			_delegate(delegate),
			_commandFix(commandFix),
			_directFix(directFix),
			_userText(userText)
		{ }

		/*!
		 * \brief Retrieve the user text.
		 * \return The text.
		 */
		QString getText() const Q_DECL_OVERRIDE { return _userText; }

		/*!
		 * \brief Executes the quick fix for the given statement.
		 * The fix will use the document and operate on the UI queue using commands.
		 * \param document The document.
		 * \param statement The statement.
		 */
		void execute(Document* document, GLSLStatement* statement) Q_DECL_OVERRIDE
		{
			//Call specialized fix, if available.
			if(_commandFix)
				(_delegate->*_commandFix)(document, dynamic_cast<T*>(statement));

			//Otherwise execute default fix.
			else
				(_delegate->*_directFix)(dynamic_cast<T*>(statement));
		}

		/*!
		 * \brief Executes the quick fix for the given statement.
		 * The fix directly operate on the pipeline, so no notifications are sent.
		 * \param statement The statement.
		 */
		void execute(GLSLStatement* statement) Q_DECL_OVERRIDE
		{
			//Execute default fix.
			(_delegate->*_directFix)(dynamic_cast<T*>(statement));
		}

	private:

		//! \brief The parent.
		S* _delegate;

		//! \brief The quick fix methods.
		void (S::*_commandFix)(Document*, T*);
		void (S::*_directFix)(T*);

		//! \brief The user text.
		QString _userText;
	};
}

#endif // GLSLQUICKFIX_H
