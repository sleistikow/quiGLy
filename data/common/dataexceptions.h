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

#ifndef DATAEXCEPTIONS_H
#define DATAEXCEPTIONS_H

#include <exception>
#include <QString>

namespace ysm
{
	class Pipeline;
	class Block;
	class Port;
	class Connection;
	class RenderCommand;

	/**
	 * @brief Exception thrown by a pipeline
	 */
	class PipelineException : public std::exception
	{
	public:
		// Construction
		explicit PipelineException(const Pipeline* pipeline, const QString& reason);

	public:
		/**
		 * @brief Gets a description of this exception
		 */
		const char* what() const Q_DECL_NOEXCEPT override;

		/**
		 * @brief Gets the causing pipeline
		 */
		const Pipeline* getPipeline() const Q_DECL_NOEXCEPT;

	protected:
		const Pipeline* _pipeline;

		std::string _reason;
	};

	/**
	 * @brief Exception thrown by a pipeline block
	 */
	class BlockException : public PipelineException
	{
	public:
		// Construction
		explicit BlockException(const Block* block, const QString& reason);

	public:
		/**
		 * @brief Gets the causing block
		 */
		const Block* getBlock() const Q_DECL_NOEXCEPT;

	private:
		const Block* _block;
	};

	/**
	 * @brief Exception thrown by a block port
	 */
	class PortException : public PipelineException
	{
	public:
		// Construction
		explicit PortException(const Port* port, const QString& reason);

	public:
		/**
		 * @brief Gets the causing port
		 */
		const Port* getPort() const Q_DECL_NOEXCEPT;

	private:
		const Port* _port;
	};

	/**
	 * @brief Exception thrown by a port connection
	 */
	class ConnectionException : public PipelineException
	{
	public:
		// Construction
		explicit ConnectionException(const Connection* connection, const QString& reason);

	public:
		/**
		 * @brief Gets the causing connection
		 */
		const Connection* getConnection() const Q_DECL_NOEXCEPT;

	private:
		const Connection* _connection;
	};

	/**
	 * @brief Exception thrown by a render command
	 */
	class RenderCommandException : public PipelineException
	{
	public:
		// Construction
		explicit RenderCommandException(const RenderCommand* command, const QString& reason);

	public:
		/**
		 * @brief Gets the causing render command
		 */
		const RenderCommand* getRenderCommand() const Q_DECL_NOEXCEPT;

	private:
		const RenderCommand* _command;
	};

	/**
	 * @brief Exception thrown during (de)serialization
	 */
	class SerializationException : public PipelineException
	{
	public:
		// Construction
		explicit SerializationException(const QString& reason);
	};
}

#endif
