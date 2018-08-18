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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "types.h"
#include "data/cache/cacheableobject.h"

namespace ysm
{
	class Block;

	/**
	 * @brief Base class for data sources (classes that generate Mesh data, texture information and similar)
	 */
	class DataSource : public CacheableObject
	{
	public:
		using DataSourceOutput = unsigned int;

		enum
		{
			NoOutput = 0,
		};

	public:
		// Construction
		explicit DataSource(Pipeline* pipeline, Block* block);
		virtual ~DataSource();

	public:
		// General attributes
		/**
		 * @brief Gets all data source attributes
		 */
		unsigned int getOutputs() const;

		/**
		 * @brief Checks whether the data source provides the given attributes
		 * @return true if the data source provides all @p outputs
		 */
		bool hasOutputs(const unsigned int outputs) const;		

	protected:
		Block* _block{nullptr};

		unsigned int _outputs{NoOutput};
	};
}

#endif
