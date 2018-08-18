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

#ifndef IPIPELINEITEMPRIVATE_H
#define IPIPELINEITEMPRIVATE_H

#include <QObject>
#include "ipipelineitem.h"

namespace ysm
{
	class PropertyList;

	//! @brief Interface that expands pipeline items by data internal methods and QObject support.
	class IPipelineItemPrivate : public QObject
	{
		Q_OBJECT

	public:

		//! @brief Destruct instance.
		virtual ~IPipelineItemPrivate() { }

		//! @brief Initialize new instance.
		virtual void initPipelineItem() = 0;

		/**
		 * @brief Sets the ID of this item.
		 * @param id The new ID.
		 */
		virtual void setID(const PipelineItemID id) = 0;

		/**
		 * @brief Determines whether multiple properties are being updated and notifications are suppressed.
		 * @return True, if multiple properties are being updated.
		 */
		virtual bool isInPropertyUpdate() const = 0;

		/**
		 * @brief Called when a property has been changed.
		 * @param prop The property that has changed (or null to refresh all properties).
		 */
		virtual void applyPropertyChanges(IProperty* prop = nullptr) = 0;

	protected:

		//! @brief Initialize new instance.
		IPipelineItemPrivate() { }
	};
}

#endif
