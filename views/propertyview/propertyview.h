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

#ifndef PROPERTYVIEW_H
#define PROPERTYVIEW_H

#include "../view.h"
#include "data/ipipelineitem.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"

#include <QWidget>
#include <QStackedWidget>
#include <QDockWidget>
#include <QMap>

namespace ysm
{
	class PropertyViewFactory;
	class PipelineItemPropertyView;

	//! \brief Stacks all property views together and acts as the dock widget.
	class PropertyView : public QDockWidget, public View
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent view.
		 */
		PropertyView(QWidget* parent, IView* parentView);

	protected:

		//! \brief Called whenever the selected document might have changed.
		void updateDocument() Q_DECL_OVERRIDE;

		//! \brief Called whenever the selected item might have changed.
		void updateItem() Q_DECL_OVERRIDE;

	protected:

		/*!
		 * \brief A property view is required, create it.
		 * \param pipelineItem The pipeline item.
		 */
		void createPropertyView(IPipelineItem* pipelineItem);

		/*!
		 * \brief A pipeline item was deleted, delete the corresponding property view.
		 * \param pipelineItem The pipeline item.
		 */
		void deletePropertyView(IPipelineItem* pipelineItem);

		/*!
		 * \brief A document was closed, delete all corresponding property views.
		 * \param document The document.
		 */
		void deletePropertyViews(Document* document);

	private:

		//! \brief Stacked widget, used to preserve the property view selection.
		QStackedWidget* _viewStack;

		//! \brief The active property view.
		PipelineItemPropertyView* _activeView;

		//! \brief Contains all property views, indexed by block id.
		QMap<PipelineItemID, PipelineItemPropertyView*> _views;

	};

}

#endif // PROPERTYVIEW_H
