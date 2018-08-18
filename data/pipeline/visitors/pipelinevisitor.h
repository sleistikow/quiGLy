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

#ifndef PIPELINEVISITOR_H
#define PIPELINEVISITOR_H

namespace ysm
{
	class Pipeline;
	class Block;
	class Port;
	class Connection;
	class RenderCommand;

	/**
	 * @brief The base pipeline visitor class
	 */
	class PipelineVisitor
	{
	public:
		// Construction
		explicit PipelineVisitor();

	public:
		// Callbacks
		/**
		 * @brief Called when a pipeline has been visited
		 * @return Returns false to abort the iteration
		 */
		virtual bool pipelineVisited(Pipeline* pipeline);

		/**
		 * @brief Called when a block has been visited
		 * @return Returns false to abort the iteration
		 */
		virtual bool blockVisited(Block* block);

		/**
		 * @brief Called when a port has been visited
		 * @return Returns false to abort the iteration
		 */
		virtual bool portVisited(Port* port);

		/**
		 * @brief Called when a connection has been visited
		 * @return Returns false to abort the iteration
		 */
		virtual bool connectionVisited(Connection* connection);

		/**
		 * @brief Called when a render command has been visited
		 * @return Returns false to abort the iteration
		 */
		virtual bool renderCommandVisited(RenderCommand* command);
	};
}

#endif
