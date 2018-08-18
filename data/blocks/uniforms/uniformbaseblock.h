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

#ifndef UNIFORMBASEBLOCK_H
#define UNIFORMBASEBLOCK_H

#include "data/blocks/block.h"
#include "data/cache/icacheable.h"

namespace ysm
{
	class IPort;

	class UniformBaseBlock : public Block, public ICacheable
	{
	public:
		explicit UniformBaseBlock(Pipeline *parent, BlockType type, const QString &name);
		virtual ~UniformBaseBlock();

	public:
		// Raw data functions
		/**
		 * @brief Retrieves the output size of this uniform
		 */
		virtual unsigned int getOutputSize(IPort* port) const = 0;

		/**
		 * @brief Retrieves the raw data as a byte array of this uniform
		 */
		virtual QByteArray retrieveUniformData(IPort* port) const = 0;

	public: // ICacheable
		CacheObject::Key getCacheKey(bool retrieveForeignKey) override;
		CacheObject::CacheObjectData* createCacheData() override;

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected slots:
		void prepareConnection(Connection* con) override;

	protected:
		template<typename T, typename V>
		QByteArray retrieveData(const V& val) const;
	};

	// Template member functions

	template<typename T, typename V>
	QByteArray UniformBaseBlock::retrieveData(const V& val) const
	{
		QByteArray data;
		T v = *val;

		data.append(reinterpret_cast<char*>(&v), sizeof(T));
		return data;
	}
}

#endif
