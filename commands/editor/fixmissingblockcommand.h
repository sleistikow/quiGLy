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

#ifndef FIXMISSINGBLOCK_H
#define FIXMISSINGBLOCK_H

#include "data/iblock.h"
#include "../uidatachangingcommand.h"

namespace ysm
{

	//! \brief Fixes a missing block for the GLSL editor. Uses a callback to initialize the block.
	class FixMissingBlockCommand : public UIDataChangingCommand
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param shaderBlock The shader block.
		 * \param missingBlockType The missing block type.
		 * \param targetPort The target port.
		 * \param setProperties Callback that sets the block's properties.
		 */
		FixMissingBlockCommand(IBlock* shaderBlock, BlockType missingBlockType, PortType targetPort,
							   bool (*setProperties)(IBlock*, IConnection*));

		/*!
		 * \brief Execute the command.
		 * \return True, on success.
		 */
		bool execute() Q_DECL_OVERRIDE;

		/*!
		 * \brief Undo the command.
		 * \return True, on success.
		 */
		bool undo() Q_DECL_OVERRIDE;

	private:

		//! \brief The block type to add.
		const BlockType _blockType;

		//! \brief The target port.
		const PortType _targetPort;

		//! \brief The created block.
		IBlock* _createdBlock;

		//! \brief The shader block.
		IBlock* _shaderBlock;

		//! \brief Callback that sets the blocks properties.
		bool (*_setProperties)(IBlock*, IConnection*);
	};

}

#endif // FIXMISSINGBLOCK_H
