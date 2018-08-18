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

#ifndef VAOLAYOUT_H
#define VAOLAYOUT_H

#include <QVector>
#include <QString>

#include "data/iserializable.h"
#include "data/types/gltypes.h"

namespace ysm
{
	class IBlock;
	class IConnection;
	class Connection;
	class VaoLayoutProperty;
	class BufferBlock;
	class MixerBlock;
	class DataSourceBlock;

	/**
	 * @brief Type holding the layout configuration of a VaoBlock
	 */
	class VaoLayout : public ISerializable
	{
		friend class VaoLayoutProperty;

	public:
		/**
		 * @brief Enum representing a single entry of the VaoLayout
		 */
		struct VaoLayoutEntry
		{
			//Connection to Buffer
			IConnection* bufferConnection{nullptr};

			QString name;

			//glAttribPointer parameters
			int index;
			int size;
			GLDataType type;
			GLSLDataType glslType;
			bool normalized;
			int stride;
			int offset;
		};

		VaoLayout();
		VaoLayout(const VaoLayout& src);

		/**
		 * @brief addEntry Function for adding new entries
		 * @param entryIndex An optional index that specifies, where to add the entry (-1 = append)
		 */
		void addEntry(IConnection* con, QString name,  int index, int size, GLDataType type, bool normalized, int stride, int offset, GLSLDataType glslType, int entryIndex = -1);

		/**
		 * @brief Direct access to entry list
		 */
		QVector<VaoLayoutEntry>* getEntries();

		/**
		 * @brief Automatically configures the VAO layout
		 */
		bool autoConfigureLayout(IBlock* block);

	public:
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;		

		VaoLayout& operator =(const VaoLayout& src);

		bool operator ==(const VaoLayout& src) const;
		bool operator !=(const VaoLayout& src) const;

	private:
		bool compareEntries(const VaoLayoutEntry& entry1, const VaoLayoutEntry& entry2) const;
		void copyVaoLayout(const VaoLayout& src);

	private:
		/**
		 * @brief Automatically creates the VAO layout for a single buffer block
		 */
		bool autoConfigureLayout(BufferBlock* block, IConnection* conVaoIn);

		bool autoConfigureLayout_Mixer(IConnection* conVaoIn, MixerBlock* mixer, IConnection* conData, int& offset, int& stride);
		bool autoConfigureLayout_DataSource(IConnection* conVaoIn, DataSourceBlock* dataSource, IConnection* conData, TypeConversion::ConversionOptions* convOptions);

		/**
		 * @brief Gets the GL types of a data connection
		 */
		bool getConnectionGLTypes(IConnection* con, GLDataType& glType, int& count, GLSLDataType& glslType, TypeConversion::ConversionOptions* convOptions);

		/**
		 * @brief Removes all entries using the connection @p con
		 */
		void removeConnectionEntries(Connection* con);

		/**
		 * @brief Check for entries using the connection @p con
		 * @return True if entries exist
		 */
		bool hasConnectionEntries(const Connection* con) const;

	private:
		QVector<VaoLayoutEntry> _entries;
	};
}

#endif // VAOLAYOUT_H
