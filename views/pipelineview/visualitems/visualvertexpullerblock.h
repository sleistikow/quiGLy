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

#ifndef VISUALVERTEXPULLERBLOCK_H
#define VISUALVERTEXPULLERBLOCK_H

#include "visualblock.h"
#include "commands/pipeline/add/addrendercommandcommand.h"
#include "data/ipipelinemanager.h"
#include "data/rendercommands/rendercommandtype.h"
#include "commands/iuicommandqueue.h"

namespace ysm {

	//! \brief Represents a VertexpullerBlock to the user.
	class VisualVertexPullerBlock : public VisualBlock
	{
		Q_OBJECT


	public:
		//! \brief Creates a instance.
		//! \param block The underlying block.
		//! \param parentView The parent view.
		explicit VisualVertexPullerBlock(IBlock* block, IView* parentView);

		//! \brief Get the block's context actions.
		QList<QAction *> getContextActions() const;

	public slots:

		//! \brief Slot for the creation.
		void createDrawCommand();

	private:

		//! \brief the Action
		QAction* _createCommandAction;

	};
}

#endif // VISUALVERTEXPULLERBLOCK_H
