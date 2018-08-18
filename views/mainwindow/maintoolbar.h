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

#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include "data/ipipeline.h"
#include "../view.h"

#include <QToolBar>
#include <QAction>

namespace ysm
{
	class MainWindow;
	class MainDelegate;

	//! \brief The main window's tool bar.
	class MainToolBar : public QToolBar, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param delegate The action delegate.
		 * \param parentWindow The parent window.
		 */
		MainToolBar(MainDelegate* delegate, MainWindow* parentWindow);

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief Update the tool bar icons from document change.
		 * \param document The document.
		 */
		void documentChanged(IPipeline* pipeline);

	protected slots:

		//! \brief Update the tool bar icons.
		void updateToolBar();

	private:

		//! \brief The delegate.
		MainDelegate* _delegate;

		//! \brief The current document.
		Document* _document;

		//! \brief The member actions.
		QAction* _saveAction;
		QAction* _saveAsAction;
		QAction* _autoLayoutAction;
		QAction* _validateAction;
		QAction* _renderAction;
	};

}

#endif // MAINTOOLBAR_H
