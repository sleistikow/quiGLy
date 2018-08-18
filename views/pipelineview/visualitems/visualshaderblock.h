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

#ifndef VISUALSHADERBLOCK_H
#define VISUALSHADERBLOCK_H

#include "visualblock.h"

#include <QObject>

namespace ysm
{

	//! \brief Displays a shader block to the user.
	class VisualShaderBlock : public VisualBlock
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param block Represented block, which must implement QObject and ShaderBlock.
		 * \param parentView The parent view item.
		 */
		explicit VisualShaderBlock(IBlock* block, IView* parentView);

	protected:

		/*!
		 * \brief Returns all context menu actions.
		 * \return The actions. Add NULL for a seperator.
		 */
		QList<QAction*> getContextActions() const Q_DECL_OVERRIDE;

	protected slots:

		//! \brief Shows the code editor.
		void showCodeEditor();

		//! \brief Adds code and opens editor.
		void addCode();

	private:

		//! \brief Context menu actions.
		QAction* _editorAction;
		QAction* _addCodeAction;
	};

}

#endif // VISUALSHADERBLOCK_H
