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

#ifndef GLSLILINEBARDELEGATE
#define GLSLILINEBARDELEGATE

#include <QObject>
#include <QVector>

namespace ysm
{
	class GLSLLineBar;
	class GLSLLineBarBlock;

	//! \brief Delegate interface, must be implemented by classes that want to provide data to a GLSLLineBar.
	class IGLSLLineBarDelegate
	{
	public:

		//! \brief Destruct instance.
		virtual ~IGLSLLineBarDelegate() { }

		/*!
		 * \brief Returns the line bar's block count.
		 * \param lineBar The caller.
		 * \return The block count.
		 */
		virtual int getLineBarBlockCount(const GLSLLineBar* lineBar) const = 0;

		/*!
		 * \brief Returns the line bar's GLSLLineBarBlock items.
		 * \param lineBar The caller.
		 * \return The line bar's blocks.
		 */
		virtual QVector<GLSLLineBarBlock> getLineBarBlocks(const GLSLLineBar* lineBar) const = 0;

	signals:

		//! \brief Emits, when the line bar blocks changed.
		virtual void lineBarBlocksChanged() = 0;

	protected:

		//! \brief Initialize new instance.
		IGLSLLineBarDelegate() { }
	};
}

#define LINEBARDELEGATE_IID "de.wwu.ysm.linebardelegate"
Q_DECLARE_INTERFACE(ysm::IGLSLLineBarDelegate, LINEBARDELEGATE_IID)

#endif // GLSLILINEBARDELEGATE

