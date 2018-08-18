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

#ifndef VARYINGS_H
#define VARYINGS_H


#include <QVector>
#include <QString>

#include "data/iserializable.h"
#include "data/types/gltypes.h"

namespace ysm
{
	class IBlock;
	class VaryingsProperty;

	/**
	 * @brief Type holding the layout configuration of a VaoBlock
	 */
	class Varyings : public ISerializable
	{
		friend class VaryingsProperty;

	public:
		/**
		 * @brief Struct representing a transform feedback varying
		 */
		struct VaryingEntry
		{
			GLSLDataType type;
			QString name;
		};

		Varyings();
		Varyings(const Varyings& src);

		/**
		 * @brief addEntry Function for adding new entries
		 * @param entryIndex An optional index that specifies, where to add the entry (-1 = append)
		 */
		void addEntry(QString name, GLSLDataType type, int entryIndex = -1);

		/**
		 * @brief Direct access to entry list
		 */
		QVector<VaryingEntry>* getEntries();

		/**
		 * @brief Returns the size, all Varying entrys need to be stored in bytes
		 */
		unsigned int getSize() const;

	public:
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

		Varyings& operator =(const Varyings& src);

		bool operator ==(const Varyings& src) const;
		bool operator !=(const Varyings& src) const;

	private:
		bool compareEntries(const VaryingEntry& entry1, const VaryingEntry& entry2) const;
		void copyTfbVarying(const Varyings& src);

	private:
		QVector<VaryingEntry> _entries;
	};
}


#endif // VARYINGS_H
