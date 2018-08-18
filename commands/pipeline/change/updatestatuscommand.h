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

#ifndef UPDATESTATUSCOMMAND_H
#define UPDATESTATUSCOMMAND_H

#include "../../uicommand.h"
#include "data/ipipelineitem.h"

namespace ysm
{

	//! \brief Command that updates a pipeline item's status.
	//! Command cannot be undone, because the status is never set by the user himself, but generated.
	class UpdateStatusCommand : public UICommand
	{
	public:

		/*!
		 * \brief Initialize new instance.
		 * \param pipelineItem The pipeline item to update.
		 * \param status The new status.
		 * \param message The status message.
		 */
		UpdateStatusCommand(IPipelineItem* pipelineItem, PipelineItemStatus status, QString message);

		/*!
		 * \brief Execute the command.
		 * \return True, if the command was executed successful.
		 */
		bool execute() Q_DECL_OVERRIDE;

		/*!
		 * \brief Execute the command.
		 * \return True, if the command was undone successful.
		 */
		bool undo() Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns wether the command is undoable.
		 * \return True, if undoable.
		 */
		bool isUndoable() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Check if the rendering needs to be re-evaluted after execution.
		 * \return True, if rendering could have changed.
		 */
		bool didChangeRendering() const Q_DECL_OVERRIDE;

		/*!
		 * \brief List of all objects, that are changed during execution.
		 * \param operation The type of operation.
		 * \return List of changed objects.
		 */
		QList<IChangeable*> getChangedObjects(IChangeable::Operation operation) Q_DECL_OVERRIDE;

	private:

		//! \brief The pipeline item.
		IPipelineItem* _pipelineItem;

		//! \brief The status.
		PipelineItemStatus _status;

		//! \brief The message.
		QString _message;
	};

}

#endif // UPDATESTATUSCOMMAND_H
