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

#ifndef PIPELINEVIEW_H
#define PIPELINEVIEW_H

#include "../view.h"
#include "data/iserializable.h"

#include <QWidget>
#include <QTabWidget>
#include <QMap>

namespace ysm
{
	class PipelineTab;
	class PipelineSceneLayouter;
	class IBlock;

	//! \brief Pipeline view displays the different available documents in tabs.
	class PipelineView : public QWidget, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view.
		 */
		PipelineView(QWidget* parentWidget, IView* parentView);

		/*!
		 * \brief Tries to retrieve the tab that displays the given document.
		 * \param document The document.
		 * \return The tab or NULL.
		 */
		PipelineTab* findTab(Document* document);

		/*!
		 * \brief requestEditor Emits the editor requested signal for the given block.
		 * \param shaderBlock The block.
		 */
		void requestEditor(IBlock* shaderBlock);

	signals:

		//! \brief Signal fired, whenever a document should be closed.
		void closeDocument(Document*);

		//! \brief Editor was requested for the given block.
		void editorRequested(Document*, IBlock*);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

		/*!
		 * \brief Tries to retrieve the tab index that displays the given document.
		 * \param document The document.
		 * \return The tab index or -1.
		 */
		int findIndex(Document* document);

	protected:

		/*!
		 * \brief A document was created, create the tab.
		 * \param document The document that was created.
		 */
		void createTab(Document* document);

		/*!
		 * \brief A document was updated, update the tab.
		 * \param document The document that was updated.
		 */
		void updateTab(Document* document);

		/*!
		 * \brief A document was removed, remove the tab.
		 * \param document The document that was removed.
		 */
		void removeTab(Document* document);

		/*!
		 * \brief Return the tab's title.
		 * \param document The document.
		 * \return The title.
		 */
		QString getTabTitle(Document* document) const;

	protected slots:

		/*!
		 * \brief The active tab changed, update the document.
		 * \param tabIndex The new tab index.
		 */
		void tabChanged(int tabIndex);

		/*!
		 * \brief Close tab was clicked, close the document.
		 * \param closedTab The tab index.
		 */
		void closeTab(int closedTab);

	private:

		//! \brief The tab widget.
		QTabWidget* _tabWidget;
	};

}
#endif // PIPELINEVIEW_H
