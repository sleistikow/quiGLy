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

#ifndef PIPELINELIST_H
#define PIPELINELIST_H

#include "data/common/objectvector.h"

namespace ysm
{
	class Pipeline;
	class PipelineManager;

	class PipelineList : public ObjectVector<Pipeline>
	{
	public:
		// Construction
		PipelineList(PipelineManager* manager);

	public:
		/**
		 * @brief Adds an existing pipeline to the list
		 */
		void addPipeline(const Pipeline* pipeline);

		/**
		 * @brief Creates a new pipeline and adds it to the list
		 * If no pipeline could be created, an exception is thrown.
		 */
		Pipeline* newPipeline();

	public: // ISerializable
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		PipelineManager* _manager{nullptr};
	};
}

#endif
