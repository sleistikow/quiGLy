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

#ifndef ADDBLOCKCOMMAND_H
#define ADDBLOCKCOMMAND_H

#include "../../uidatachangingcommand.h"
#include "data/ipipeline.h"
#include "data/iblock.h"

#include <QObject>

namespace ysm
{
	//! \brief Command that creates a new block.
	class AddBlockCommand : public UIDataChangingCommand
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param pipeline The pipeline to operate on.
		 * \param blockType The block type to add.
		 */
		AddBlockCommand(IPipeline* pipeline, const BlockType blockType);

		/*!
		 * \brief Execute the command.
		 * \return True, on success.
		 */
		bool execute() Q_DECL_OVERRIDE;

		/*!
		 * \brief Undo the action.
		 * \return True, on success.
		 */
		bool undo() Q_DECL_OVERRIDE;

		/*!
		 * \brief Return the created block.
		 * \return The block.
		 */
		IBlock* getBlock() const;

	private:

		//! \brief The block type to add.
		const BlockType _blockType;

		//! \brief The created block.
		IBlock* _createdBlock;

		//! \brief The pipeline to operate on.
		IPipeline* _pipeline;
	};

}

#endif // ADDBLOCKCOMMAND_H
