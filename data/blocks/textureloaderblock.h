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

#ifndef TEXTURELOADERBLOCK_H
#define TEXTURELOADERBLOCK_H

#include "block.h"
#include "data/properties/textureproperty.h"
#include "data/properties/filenameproperty.h"
#include "data/cache/cacheableobject.h"
#include "opengl/gli.h"

namespace ysm
{
	class TextureLoaderBlock : public Block, public CacheableObject
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::TextureLoader};

	public:
		// Construction
		/**
		 * @param parent The owning pipeline
		 */
		explicit TextureLoaderBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the texture file
		 */
		FilenameProperty* getTextureFile() const;

		/**
		 * @brief Gets the actual texture
		 */
		TextureProperty* getTexture() const;

	public:
		void applyPropertyChanges(IProperty* prop) override;

		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	public:
		// ICacheable
		CacheObject::Key getCacheKey(bool retrieveForeignKey) override;
		CacheObject::CacheObjectData* createCacheData() override;

	protected:
		void createProperties() override;
		void createPorts() override;

		/**
		 * @brief Checks wether the cache key changed since the last call.
		 * @param clear If true, the change flag is cleared instead of checked.
		 * @return True, if changed.
		 */
		bool hasCacheKeyChanged(bool clear);

	private:
		struct TextureData : CacheObject::CacheObjectData
		{
			gli::texture texture;

			TextureData() { }
			TextureData(gli::texture&& tex) : texture{tex} { }
		} _emptyData;

		/**
		 * @brief Gets the cached texture data
		 * If no data could be cached, _emptyData is returned.
		 */
		const TextureData* getTextureData();

	private:
		// Properties
		FilenameProperty* _textureFile{nullptr};
		TextureProperty* _texture{nullptr};

		// Ports
		Port* _outPort{nullptr};

		QString _cacheKey;
	};
}

#endif
