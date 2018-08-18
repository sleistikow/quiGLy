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

#ifndef IGLSLCOMPLETERDELEGATE
#define IGLSLCOMPLETERDELEGATE

#include <QString>
#include <QKeyEvent>

namespace ysm
{
	class GLSLCompleter;
	class GLSLCodeBlock;

	//! \brief Delegate interface, must be implemented by classes that want to provide data to a GLSLCompleter.
	class IGLSLCompleterDelegate
	{
	public:

		//! \brief Initialize new instance.
		virtual ~IGLSLCompleterDelegate() { }

		/*!
		 * \brief Gets the prefix for the given completer.
		 * \param completer The completer.
		 * \return The completer's prefix.
		 */
		virtual QString getCompleterPrefix(const GLSLCompleter* completer) const = 0;

		/*!
		 * \brief Gets the bounds for the given completer.
		 * \param completer The completer.
		 * \return The completer's bounds. If the width is 0, it will be automatically calculated.
		 */
		virtual QRect getCompleterRect(const GLSLCompleter* completer) const = 0;

		/*!
		 * \brief Gets code block, where to complete
		 * \param completer The completer.
		 * \return The code block, that provides local scope declarations.
		 */
		virtual GLSLCodeBlock* getCompleterBlock(const GLSLCompleter* completer) const = 0;

	public slots:

		/*!
		 * \brief Handle a completer's completion.
		 * \param completion The completion data.
		 */
		virtual void completerCompleted(const GLSLCompleter* completer, const QString& completion) = 0;

	protected:

		//! \brief Initialize new instance.
		IGLSLCompleterDelegate() { }
	};
}

#define COMPLETERDELEGATE_IID "de.wwu.ysm.linebardelegate"
Q_DECLARE_INTERFACE(ysm::IGLSLCompleterDelegate, COMPLETERDELEGATE_IID)

#endif // IGLSLCOMPLETERDELEGATE

