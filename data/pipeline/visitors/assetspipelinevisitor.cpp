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

#include "assetspipelinevisitor.h"
#include "data/properties/propertylist.h"
#include "data/properties/filenameproperty.h"
#include "data/blocks/block.h"
#include "data/blocks/port.h"
#include "data/blocks/connection.h"
#include "data/rendercommands/rendercommand.h"

namespace ysm
{
	AssetsPipelineVisitor::AssetsPipelineVisitor()
	{

	}

	QStringList AssetsPipelineVisitor::getAssetFiles() const
	{
		return _assetFiles;
	}

	bool AssetsPipelineVisitor::blockVisited(Block* block)
	{
		processProperties(block->getPropertyList());
		return true;
	}

	bool AssetsPipelineVisitor::portVisited(Port* port)
	{
		processProperties(port->getPropertyList());
		return true;
	}

	bool AssetsPipelineVisitor::connectionVisited(Connection* connection)
	{
		processProperties(connection->getPropertyList());
		return true;
	}

	bool AssetsPipelineVisitor::renderCommandVisited(RenderCommand* command)
	{
		processProperties(command->getPropertyList());
		return true;
	}

	void AssetsPipelineVisitor::processProperties(PropertyList* properties)
	{
		// Add all filenames to the assets list
		for (IProperty* prop : *properties)
		{
			if (prop->getType() == PropertyType::Filename)
			{
				const FilenameProperty* fileProp = dynamic_cast<FilenameProperty*>(prop);

				if (!fileProp)
					throw std::runtime_error{"Thought I had a Filename property, but I was wrong"};

				QString file = *fileProp;
				Qt::CaseSensitivity caseSens = Qt::CaseSensitive;

#ifdef Q_OS_WIN32
				caseSens = Qt::CaseInsensitive;
#endif
				if (!_assetFiles.contains(file, caseSens))
					_assetFiles << file;
			}
		}
	}
}
