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

#ifndef UICOMMAND_H
#define UICOMMAND_H

#include "iuicommand.h"

namespace ysm
{

	//! \brief Abstract base implementation of the IUICommand interface.
	class UICommand : public IUICommand
	{

	public:

		//! \brief Destruct instance.
		virtual ~UICommand() { }

		/*!
		 * \brief Gets the command block identifier, which is used to group commands together.
		 * \return The command block identifier.
		 */
		int getCommandBlock() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Sets the command block identifier, which is used to group commands together.
		 * \param commandBlock The command block identifier.
		 */
		void setCommandBlock(int commandBlock) Q_DECL_OVERRIDE;

		/*!
		 * \brief Execute implicit commands, that are grouped to this command but executed right before this command.
		 * \return True, if execution was successful.
		 */
		bool executeImplicit(IUICommandQueue* commandQueue) Q_DECL_OVERRIDE;


		/*!
		 * \brief List of all objects, that are changed during execution.
		 * \param operation The type of operation.
		 * \return List of changed objects.
		 */
		QList<IChangeable*> getChangedObjects(IChangeable::Operation operation) Q_DECL_OVERRIDE;

	protected:

		//! \brief Initialize new instance.
		UICommand();

	private:

		//! \brief Wether the list was already created or not.
		bool _canChange;

		//! \brief The command's block.
		int _commandBlock;

		//! \brief List of all internal implicit commands.
		QList<IUICommand*> _internalCommands;

		//! \brief List of all external implicit commands.
		QList<IUICommand*> _externalCommands;
	};

}

#endif // UICOMMAND_H
