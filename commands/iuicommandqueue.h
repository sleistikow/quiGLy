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

#ifndef IUICOMMANDQUEUE_H
#define IUICOMMANDQUEUE_H

#include "ichangeable.h"

#include <QObject>

namespace ysm
{
	class Document;
	class IUICommand;

	/// @brief Interface for a UICommandQueue. Derives QObject, because signal/slot mechanism is required.
	class IUICommandQueue : public QObject
	{
		Q_OBJECT

	public:

		/// @brief Destruct instance.
		virtual ~IUICommandQueue() { }

		/**
		 * @brief Determines, whether any change has been applied since the last save.
		 * @return True, if state does not match saved state.
		 */
		virtual bool isSaved() const = 0;

		/// @brief Clears the undo and redo stack.
		virtual void clear() = 0;

		/// @brief Mark the current state as saved, so that hasChanged returns false for exactly this state.
		virtual void save() = 0;

		/**
		 * @brief Execute given command.
		 * If the command can not be undone, it will be deleted afterwards, otherwise the queue maintains ownership.
		 * @param command Command to be executed.
		 */
		virtual void execute(IUICommand* command) = 0;

		/**
		 * @brief Undo the latest executed command.
		 * @return False, if undo stack was empty.
		 */
		virtual bool undo() = 0;

		/**
		 * @brief Redo the latest undone command.
		 * @return False, if redo stack was empty.
		 */
		virtual bool redo() = 0;

		/// @brief Begins a command block, that is used to group multiple commands together.
		/// If begin is called more than once, the calls are ignored and the current group is used.
		virtual void beginCommandBlock() = 0;

		/// @brief Ends the command block.
		virtual void endCommandBlock() = 0;

	signals:

		/// @brief Emitted, whenever a command will be executed, that removes data.
		void willRemoveData(Document*, const QList<IChangeable*>&);

		/// @brief Emitted, whenever a command was executed, that changes data.
		void didChangeData(Document*, const QList<IChangeable*>&);

		/// @brief Emitted, whenever a command was executed, that added data.
		void didAddData(Document*, const QList<IChangeable*>&);

		/// @brief Emmited, whenever a command was executed, that changes rendering output.
		void renderingChanged(Document*);

		/// @brief Emitted, whenever the internal state was changed.
		void stateChanged();

	protected:

		/**
		 * @brief Initialize new instance.
		 * @param parent The parent.
		 */
		IUICommandQueue(QObject* parent) : QObject(parent) { }
	};
}

#endif // IUICOMMANDQUEUE_H
