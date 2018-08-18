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

#include "textureloaderblock.h"
#include "port.h"
#include "portlist.h"
#include "connection.h"
#include "data/properties/propertylist.h"

namespace ysm
{
	TextureLoaderBlock::TextureLoaderBlock(Pipeline* parent) : Block(parent, block_type, "Texture Loader"), CacheableObject(parent->getManager()->getCachePool())
	{

	}

	FilenameProperty* TextureLoaderBlock::getTextureFile() const
	{
		return _textureFile;
	}

	TextureProperty* TextureLoaderBlock::getTexture() const
	{
		return _texture;
	}

	bool TextureLoaderBlock::canAcceptConnection(IPort* src, IPort* dest, QString& denialReason)
	{
		if (!Block::canAcceptConnection(src, dest, denialReason))
			return false;

		if (src == _outPort)
		{
			ConnectionPoints conPoints;

			conPoints << qMakePair(BlockType::Texture, PortType::Data_In);

			if (!checkConnectionPoints(dest, conPoints))
			{
				denialReason = "Texture Loader output must be connected to a Texture block";
				return false;
			}

			return true;
		}

		// Nope, we don't like this connection
		return false;
	}

	CacheObject::Key TextureLoaderBlock::getCacheKey(bool retrieveForeignKey)
	{
		Q_UNUSED(retrieveForeignKey);

		CacheObject::Key key;

		// Key consists of class qualifier + texture filename
		key = QString("TextureLoaderBlock/%1").arg(*_textureFile);
		return key;
	}

	CacheObject::CacheObjectData* TextureLoaderBlock::createCacheData()
	{
		setStatus(PipelineItemStatus::Healthy);

		TextureData* data = nullptr;
		QString textureFile = *_textureFile;

		if (!textureFile.isEmpty())
		{
			// Data must be created on the heap, will be managed by the cache pool
			data = new TextureData{gli::load(textureFile.toStdString())};

			if (data->texture.empty())
			{
				delete data;
				data = nullptr;

				QString msg = QString("The texture failed to load");
				setStatus(PipelineItemStatus::Chilled, msg);
			}
		}

		return data;
	}

	void TextureLoaderBlock::createProperties()
	{
		Block::createProperties();

		_textureFile = _properties->newProperty<FilenameProperty>(PropertyID::Texture_FileName, "Texture File");

		_texture = _properties->newProperty<TextureProperty>(PropertyID::Texture_Texture, "Texture", true);
		_texture->delegateValue(
					[this]()->const gli::texture*& { static const gli::texture* __ret = nullptr; __ret = &getTextureData()->texture; return __ret; },
					nullptr,
					[this](bool clear)->bool { return hasCacheKeyChanged(clear); });
	}

	void TextureLoaderBlock::createPorts()
	{
		Block::createPorts();

		_outPort = _ports->newPort(PortType::GenericOut, PortDirection::Out, "Out");
	}

	const TextureLoaderBlock::TextureData* TextureLoaderBlock::getTextureData()
	{
		const TextureData* data = getCachedData<TextureData>();

		if (!data)
		{
			// No data cached; return an empty dummy
			data = &_emptyData;
		}

		return data;
	}

	void TextureLoaderBlock::applyPropertyChanges(IProperty* prop)
	{
		Block::applyPropertyChanges(prop);

		if (!prop || prop == _textureFile)
		{
			QString textureFile = *_textureFile;

			if (!textureFile.isEmpty())
			{
				// Force loading of the texture file by accessing the texture data
				getTextureData();
			}
		}
	}

	bool TextureLoaderBlock::hasCacheKeyChanged(bool clear)
	{
		// Check if key should be cleared
		QString cacheKey = getCacheKey(true);
		if(clear)
			_cacheKey = cacheKey;

		// Check for change.
		return _cacheKey != cacheKey;
	}
}
