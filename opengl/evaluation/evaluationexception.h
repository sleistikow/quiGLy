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

#ifndef EVALUATIONEXCEPTION_H
#define EVALUATIONEXCEPTION_H

#include <exception>
#include <QString>

namespace ysm
{

	class IBlock;

	class EvaluationException : public std::exception
	{
	public:

		// Construction
		explicit EvaluationException(const QString& reason, IBlock* block = nullptr, const QString& log = "");

	public:

		/**
		 * @brief Gets a description of this exception
		 */
		const char* what() const Q_DECL_NOEXCEPT override;

		/**
		 * @brief Returns the block, the error occurd on
		 */
		IBlock* getBlock() const;

		/**
		 * @brief Returns the log, if any
		 */
		const QString& getLog() const;

	protected:

		std::string _reason;
		IBlock* _block;
		QString _log;

	};

} // end namespace ysm

#endif // EVALUATIONEXCEPTION_H
