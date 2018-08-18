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

#ifndef UICOMMANDQUEUE_H
#define UICOMMANDQUEUE_H

#include "iuicommand.h"
#include "iuicommandqueue.h"
#include "views/document.h"

#include <QObject>
#include <QStack>

namespace ysm
{

	/**
	 * @brief Implementation of the command queue, responsible for data manipulation.
	 * There is one command queue for every main window document.
	 * NOTE THAT THE COMMAND QUEUE BECOMES THE COMMANDS OWNER WHEN EXECUTED.
	 */
	class UICommandQueue: public IUICommandQueue
	{
		Q_OBJECT

	public:

		/**
		 * @brief Initialize new instance.
		 * @param parent The parent
		 */
		explicit UICommandQueue(Document* parent);

		/// @brief Destruct instance.
		virtual ~UICommandQueue();

		/**
		 * @brief Execute given command.
		 * If the command can not be undone, it will be deleted afterwards, otherwise the queue maintains ownership.
		 * @param command Command to be executed.
		 */
		void execute(IUICommand* command) Q_DECL_OVERRIDE;

		/**
		 * @brief Determines, whether any change has been applied since the last save.
		 * @return True, if state does not match saved state.
		 */
		bool isSaved() const Q_DECL_OVERRIDE;

		/// @brief Clears the undo and redo stack.
		void clear() Q_DECL_OVERRIDE;

		/// @brief Mark the current state as saved, so that hasChanged returns false for exactly this state.
		void save() Q_DECL_OVERRIDE;

		/// @brief Begins a command block, that is used to group multiple commands together.
		/// If begin is called more than once it's ignored, but endCommandBlock() must be called accordingly.
		void beginCommandBlock() Q_DECL_OVERRIDE;

		/// @brief Ends the command block.
		void endCommandBlock() Q_DECL_OVERRIDE;

	public slots:

		/**
		 * @brief Undo the latest executed command.
		 * @return False, if undo stack was empty.
		 */
		bool undo() Q_DECL_OVERRIDE;

		/**
		 * @brief Redo the latest undone command.
		 * @return False, if redo stack was empty.
		 */
		bool redo() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Emit the pre execution signals.
		 * \param command The command to use.
		 * \param isUndo If true, an undo operation is assumed.
		 */
		void emitPreExecute(IUICommand* command, bool isUndo);

		/*!
		 * \brief Emit the post execution signals.
		 * \param command The command to use.
		 * \param isUndo If true, an undo operation is assumed.
		 */
		void emitPostExecute(IUICommand* command, bool isUndo);

	private:

		/// @brief The queue's parent.
		Document* _document;

		/// @brief The undo stack.
		QStack<IUICommand*> _undoStack;

		/// @brief  The redo stack.
		QStack<IUICommand*> _redoStack;

		/// @brief Counts operations to identify the saved state.
		int _undoCounter;

		/// @brief The current command block.
		int _currentBlock;

		/// @brief The next command block.
		int _nextBlock;

		/// @brief The current block's depth.
		int _currentBlockDepth;
	};
}

#endif // UICOMMANDQUEUE_H
