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

#include "dataexceptions.h"
#include "data/blocks/block.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"

namespace ysm
{
	PipelineException::PipelineException(const Pipeline* pipeline, const QString& reason) : _pipeline{pipeline}
	{
		_reason = reason.toStdString();
	}

	const char* PipelineException::what() const Q_DECL_NOEXCEPT
	{
		return _reason.c_str();
	}

	const Pipeline* PipelineException::getPipeline() const Q_DECL_NOEXCEPT
	{
		return _pipeline;
	}

	BlockException::BlockException(const Block* block, const QString& reason) : PipelineException(nullptr, reason), _block{block}
	{
		if (block)
			_pipeline = const_cast<Block*>(block)->getPipeline();
	}

	const Block* BlockException::getBlock() const Q_DECL_NOEXCEPT
	{
		return _block;
	}

	ConnectionException::ConnectionException(const Connection* connection, const QString& reason) : PipelineException(nullptr, reason), _connection{connection}
	{
		Connection* con = const_cast<Connection*>(connection);

		if (con && con->getSource())
			_pipeline = con->getSource()->getPipeline();
	}

	RenderCommandException::RenderCommandException(const RenderCommand* command, const QString& reason) : PipelineException(nullptr, reason), _command{command}
	{
		if (command)
			_pipeline = const_cast<RenderCommand*>(command)->getPipeline();
	}

	const RenderCommand* RenderCommandException::getRenderCommand() const Q_DECL_NOEXCEPT
	{
		return _command;
	}

	PortException::PortException(const Port* port, const QString& reason) : PipelineException(nullptr, reason), _port{port}
	{
		Port* p = const_cast<Port*>(port);

		if (p && p->getBlock())
			_pipeline = p->getBlock()->getPipeline();
	}

	const Port* PortException::getPort() const Q_DECL_NOEXCEPT
	{
		return _port;
	}

	SerializationException::SerializationException(const QString& reason) : PipelineException(nullptr, reason)
	{

	}
}
