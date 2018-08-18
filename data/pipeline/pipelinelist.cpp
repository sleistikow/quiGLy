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

#include "pipelinelist.h"
#include "pipeline.h"

namespace ysm
{
	PipelineList::PipelineList(PipelineManager* manager) : _manager{manager}
	{
		_xmlElementName = "Pipeline";
	}

	void PipelineList::addPipeline(const Pipeline* pipeline)
	{
		if (!pipeline)
			throw std::invalid_argument{"pipeline may not be null"};

		append(pipeline);
	}

	Pipeline* PipelineList::newPipeline()
	{
		Pipeline* pipeline = new Pipeline(_manager);

		if (!pipeline)
			throw std::runtime_error{"Out of memory"};

		append(pipeline);
		return pipeline;
	}

	void PipelineList::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		// Keep existing data if in import mode
		if (!ctx->isImportMode())
			clear();

		int curPipeline = 0;

		for (QDomElement& elem : ctx->getElements(xmlElement, _xmlElementName))
		{
			Pipeline* pipeline{nullptr};

			// In Import mode, we deserialize all data into the existing pipelines
			if (ctx->isImportMode())
			{
				// We can only import as many pipelines as we already have in our list
				if (curPipeline >= _objects.size())
					break;

				pipeline = _objects[curPipeline++];
			}
			else
				pipeline = newPipeline();

			ctx->setObjectID(pipeline, &elem);

			// Prevent memory leaks in case of deserialization failure
			try
			{
				pipeline->deserialize(&elem, ctx);
			}
			catch (...)
			{
				QString msg = QString("Failed to deserialize a pipeline");
				ctx->addMessage(msg);

				if (!ctx->isImportMode())
					remove(pipeline);
			}
		}
	}
}
