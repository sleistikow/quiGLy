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

#ifndef DEFAULTPROPERTYVIEW_H
#define DEFAULTPROPERTYVIEW_H

#include "../viewitem.h"
#include "data/ipipelineitem.h"
#include "propertyviewitems/ipropertyviewitem.h"

#include <QScrollArea>
#include <QToolBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMap>

namespace ysm
{
	class PropertyViewFactory;

	//! \brief Default property view for a specific item, shows all properties in a "common" group box.
	class PipelineItemPropertyView : public QScrollArea, public ViewItem
	{
		Q_OBJECT

	private:

		//! \brief Hold settings for each property.
		struct PropertySettings
		{
			QString groupName;
			bool isHidden;
		};

	public:

		/*!
		 * \brief Registers the property view's required items.
		 * \param factory The factory.
		 */
		static void registerPropertyItems(PropertyViewFactory* factory);

		/*!
		 * \brief Initialize new instance.
		 * \param pipelineItem The pipeline item.
		 * \param parentWidget The parent widget.
		 * \param parentView The parent item.
		 */
		PipelineItemPropertyView(IPipelineItem* pipelineItem, QWidget* parentWidget, IView* parentView);

		/*!
		 * \brief Gets the underlying pipeline item.
		 * \return The pipeline item.
		 */
		IPipelineItem* getPipelineItem() const;

		/// Recreate the property view items of this view.
		void recreatePropertyViewItems();

	protected:

		/*!
		 * \brief Sets the given property's group.
		 * \param property The property.
		 * \param groupName The group name.
		 */
		void setPropertyGroup(IProperty* property, QString groupName);

		/*!
		 * \brief Sets the given property's visibility.
		 * \param property The property.
		 * \param hidden Wether the property should be hidden.
		 */
		void setPropertyHidden(IProperty* property, bool hidden = true);

	protected:

		/*!
		 * \brief A property was created, create the property view accordingly.
		 * \param property The property.
		 */
		void createPropertyItemView(IProperty* property);

		/*!
		 * \brief A property was updated, create the property view accordingly.
		 * \param property The property.
		 */
		void updatePropertyItemView(IProperty* property);

		/*!
		 * \brief A property was deleted, create the property view accordingly.
		 * \param property The property.
		 */
		void deletePropertyItemView(IProperty* property);

	private:

		//! \brief Contains all child views.
		QMap<IProperty*, IPropertyViewItem*> _propertyViews;

		//! \brief Toolbox containing all property groups.
		QToolBox* _toolBox;

		//! \brief The pipeline item to operate on.
		IPipelineItem* _pipelineItem;

		//! \brief The layout.
		QFormLayout* _layout;

		//! \brief The default group box layout.
		QFormLayout* _defaultLayout;

		//! \brief The property settigns.
		QMap<IProperty*, PropertySettings> _propertySettings;

		//! \brief the group box's layouts.
		QMap<QString, QFormLayout*> _boxLayouts;
	};

}

#endif // DEFAULTPROPERTYVIEW_H
