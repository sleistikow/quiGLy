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

#ifndef PIPELINEITEMLIST_H
#define PIPELINEITEMLIST_H

#include "pipelinemanager.h"
#include "data/common/objectvector.h"
#include "data/common/dataexceptions.h"

namespace ysm
{
	class Pipeline;

	/**
	 * @brief Basic pipeline item list
	 */
	template<typename T>
	class PipelineItemList : public ObjectVector<T>
	{
	public:
		explicit PipelineItemList(Pipeline* parent);

	public:
		/**
		 * @brief Finds the item with the given @p id
		 * @return The item with the matching ID (if one exists; null otherwise)
		 */
		T* findItem(const PipelineItemID id) const;

	public:
		void append(const T* item) override;
		void insert(const int i, const T* item) override;

	protected:
		Pipeline* _pipeline{nullptr};
	};

	// Template member functions

	template<typename T>
	PipelineItemList<T>::PipelineItemList(Pipeline* parent) : _pipeline{parent}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};
	}

	template<typename T>
	T* PipelineItemList<T>::findItem(const PipelineItemID id) const
	{
		return ObjectVector<T>::find([id](const T* item) { return (item->getID() == id); });
	}

	template<typename T>
	void PipelineItemList<T>::append(const T* item)
	{
		// Need to set the item ID
		T* itemNC = const_cast<T*>(item);

		if (itemNC->getID() == 0)
		{
			// Assign the item a unique ID
			itemNC->setID(PipelineManager::requestNextItemID());
		}

		ObjectVector<T>::append(item);
	}

	template<typename T>
	void PipelineItemList<T>::insert(const int i, const T* item)
	{
		// Need to set the item ID
		T* itemNC = const_cast<T*>(item);

		if (itemNC->getID() == 0)
		{
			// Assign the item a unique ID
			itemNC->setID(PipelineManager::requestNextItemID());
		}

		ObjectVector<T>::insert(i, item);
	}
}

#endif
