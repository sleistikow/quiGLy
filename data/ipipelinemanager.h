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

#ifndef IPIPELINEMANAGER_H
#define IPIPELINEMANAGER_H

#include <QVector>
#include "iserializable.h"

namespace ysm
{
	class IPipeline;

	/// @brief Interface to manage a group of pipelines.
	class IPipelineManager : public ISerializable
	{
	public:

		/// @brief Initialize new instance.
		virtual ~IPipelineManager() { }

		/**
		 * @brief Gets all pipelines.
		 * @return List of all pipelines.
		 */
		virtual QVector<IPipeline*> getPipelines() const = 0;

		/**
		 * @brief Adds a new pipeline.
		 * @return The newly created pipeline.
		 */
		virtual IPipeline* addPipeline() = 0;

		/**
		 * @brief Deletes an existing pipeline.
		 * @pipeline The pipeline.
		 */
		virtual void deletePipeline(const IPipeline* pipeline) = 0;

		/// @brief Resets all pipeline data.
		virtual void reset() = 0;

	protected:

		/// @brief Initialize new instance.
		IPipelineManager() { }
	};
}

#endif
