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

#ifndef BUFFERBLOCK_H
#define BUFFERBLOCK_H

#include "block.h"
#include "mixerblock.h"
#include "datasourceblock.h"
#include "uniforms/uniformbaseblock.h"
#include "data/properties/property.h"
#include "data/cache/cacheableobject.h"
#include "data/common/dataexceptions.h"

#include <QMap>

namespace ysm
{
	/**
	 * @brief Buffer block representing the various buffer types
	 */
	class BufferBlock : public Block, public CacheableObject
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Buffer};

	public:
		// Types
		/**
		 * @brief The buffer usage frequency type
		 */
		enum BufferUsageFrequency
		{
			Usage_Stream,
			Usage_Static,
			Usage_Dynamic,
		};

		/**
		 * @brief The buffer usage access type
		 */
		enum BufferUsageAccess
		{
			Usage_Draw,
			Usage_Read,
			Usage_Copy,
		};

	public:
		// Construction
		/**
		 * @param parent The owning pipeline
		 */
		explicit BufferBlock(Pipeline* parent, BlockType type, const QString& name, CachePool* pool);
		explicit BufferBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the buffer data
		 */
		ByteArrayProperty* getBufferData() const;

		/**
		 * @brief Gets the buffer size
		 */
		UIntProperty* getBufferSize() const;

		/**
		 * @brief Gets the buffer element count
		 */
		UIntProperty* getBufferEntryCount() const;

		/**
		 * @brief Gets the usage frequency setting
		 */
		EnumProperty* getUsageFrequency();

		/**
		 * @brief Gets the usage access setting
		 */
		EnumProperty* getUsageAccess();

		// Port access
		/**
		 * @brief Gets the data in-port
		 */
		Port* getDataInPort();

		/**
		 * @brief Gets the data out-port
		 */
		Port* getDataOutPort();

	public:
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

	protected slots:
		void prepareConnection(Connection* con) override;

	private:
		struct BufferData : CacheObject::CacheObjectData
		{
			QByteArray data;
			unsigned int elementCount{0};
		} _emptyData;

		/**
		 * @brief Gets the cached buffer data
		 * If no data could be cached, _emptyData is returned.
		 */
		const BufferData* getBufferData();

		/**
		 * @brief Gets the cache key for a cacheable object
		 */
		QString getCacheKey_DataSource(DataSourceBlock* dataSrc);

		/**
		 * @brief Gets the cache key for a Mixer
		 */
		QString getCacheKey_Mixer(MixerBlock* mixer);

		/**
		 * @brief Gets the cache key for a Uniform
		 */
		QString getCacheKey_Uniform(UniformBaseBlock* uniform);

		/**
		 * @brief Processes the connected block of this buffer, using two callbacks
		 */
		template<typename M, typename D, typename U>
		void processConnectedBlock(IBlock* block, M mixerCb, D dataSrcCb, U uniformCb);

	protected:
		// Properties
		ByteArrayProperty* _bufferData{nullptr};
		UIntProperty* _bufferSize{nullptr};
		UIntProperty* _bufferEntryCount{nullptr};

		EnumProperty* _usageFrequency{nullptr};
		EnumProperty* _usageAccess{nullptr};

		// Ports
		Port* _dataInPort{nullptr};
		Port* _dataOutPort{nullptr};

		QString _cacheKey;
	};

	// Template member functions

	template<typename M, typename D, typename U>
	void BufferBlock::processConnectedBlock(IBlock* block, M mixerCb, D dataSrcCb, U uniformCb)
	{
		if (!block)
			throw std::runtime_error{"block may not be null"};

		// See which block we're connected to
		if (dynamic_cast<MixerBlock*>(block))
		{
			MixerBlock* mixer = dynamic_cast<MixerBlock*>(block);
			mixerCb(mixer);
		}
		else if (dynamic_cast<UniformBaseBlock*>(block))
		{
			UniformBaseBlock* uniform = dynamic_cast<UniformBaseBlock*>(block);
			uniformCb(uniform);
		}
		else if (dynamic_cast<DataSourceBlock*>(block))
		{
			DataSourceBlock* dataSrc = dynamic_cast<DataSourceBlock*>(block);
			dataSrcCb(dataSrc);
		}
	}
}

#endif
