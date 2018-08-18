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

#ifndef IUICOMMAND_H
#define IUICOMMAND_H

#include "ichangeable.h"

#include <QObject>
#include <QList>

namespace ysm
{
	class IUICommandQueue;

	//! \brief Basic interface for a UI command, that is executed by the interface to interact with data.
	//! NOTE: Between creation and execution of a command must no data changes must happen.
	//! It's best practice to simply create and execute a command immediately.
	class IUICommand
	{

	public:

		//! \brief Destruct instance.
		virtual ~IUICommand() { }

		/*!
		 * \brief Gets the command block identifier, which is used to group commands together.
		 * \return The command block identifier.
		 */
		virtual int getCommandBlock() const = 0;

		/*!
		 * \brief Sets the command block identifier, which is used to group commands together.
		 * \param commandBlock The command block identifier.
		 */
		virtual void setCommandBlock(int commandBlock) = 0;

		/*!
		 * \brief Execute the command.
		 * \return True, if the command was executed successful.
		 */
		virtual bool execute() = 0;

		/*!
		 * \brief Execute the command.
		 * \return True, if the command was undone successful.
		 */
		virtual bool undo() = 0;

		/*!
		 * \brief Execute implicit commands, that are grouped to this command but executed right before this command.
		 * \param commandQueue The command queue to operate on.
		 * \return True, if execution was successful.
		 */
		virtual bool executeImplicit(IUICommandQueue* commandQueue) = 0;

		/*!
		 * \brief Check if the command can be undone, and should be queued for undo.
		 * \return True, if the command can be undone.
		 */
		virtual bool isUndoable() const = 0;

		/*!
		 * \brief Check if the rendering needs to be re-evaluted after execution.
		 * \return True, if rendering could have changed.
		 */
		virtual bool didChangeRendering() const = 0;

		/*!
		 * \brief List of all objects, that are changed during execution.
		 * Note:
		 * - Changed objects are available before and after execution.
		 * - Removed objects are available before execution.
		 * - Added objects are available after execution.
		 * \param operation The type of operation.
		 * \return List of changed objects.
		 */
		virtual QList<IChangeable*> getChangedObjects(IChangeable::Operation operation) = 0;

	protected:

		//! \brief Initialize new instance.
		IUICommand() { }
	};
}

#endif // IUICOMMAND_H
