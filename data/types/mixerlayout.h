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

#ifndef MIXERLAYOUT_H
#define MIXERLAYOUT_H

#include <QVector>
#include "data/iserializable.h"
#include "data/types/types.h"

namespace ysm
{
	class IConnection;
	class Connection;
	class MixerLayoutProperty;
	class IBlock;

	/**
	 * @brief Type holding the layout defined in a mixer block
	 */
	class MixerLayout : public ISerializable
	{
		friend class MixerLayoutProperty;

	public:
		struct MixerLayoutEntry
		{
			IConnection* dataConnection{nullptr};
			TypeConversion::ConversionOptions typeConversion;
			QString name;
		};

	public:
		// Construction
		MixerLayout();
		MixerLayout(const MixerLayout& src);

	public:
		// Assignment
		MixerLayout& operator =(const MixerLayout& src);

		// Compare values
		bool operator ==(const MixerLayout& src) const;
		bool operator !=(const MixerLayout& src) const;

	public:
		// Verification
		/**
		 * @brief Checks whether the layout is valid
		 */
		bool verifyLayout(QString& error) const;

		// Entry management
		/**
		 * @brief Convenience function for adding new entries to the layout
		 * @param index An optional index at where to add the index (if -1, the entry will be appended)
		 */
		void addEntry(IConnection* con, const QString& name, const int index = -1);

		/**
		 * @brief Convenience function for adding new entries to the layout
		 * @param index An optional index at where to add the index (if -1, the entry will be appended)
		 */
		void addEntry(IConnection* con, const TypeConversion::ConversionOptions& typeConv, const QString& name, const int index = -1);

		/**
		 * @brief Automatically configures the Mixer layout
		 */
		bool autoConfigureLayout(IBlock* block);

		// Object access
		/**
		 * @brief Direct access to the entry list
		 */
		QVector<MixerLayoutEntry>* getEntries();

		// General attributes
		/**
		 * @brief Gets if the entries should be treated as a struct
		 */
		bool getEntriesAsStruct() const;

		/**
		 * @brief Sets whether the entries should be treated as a struct
		 */
		void setEntriesAsStruct(const bool entriesAsStruct);

	public: // ISerializable
		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	private:
		/**
		 * @brief Removes all entries that use the connection @p con as their data connection
		 */
		void removeConnectionEntries(const Connection* con);


		/**
		 * @brief Check for entries that use the connection @p con as their data connection
		 * @return True if entries exist
		 */
		bool hasConnectionEntries(const Connection* con) const;

		/**
		 * @brief Compares two entries
		 * @return true if both are equal
		 */
		bool compareEntries(const MixerLayoutEntry& entry1, const MixerLayoutEntry& entry2) const;

		// Simple element copy
		void copyMixerLayout(const MixerLayout& src);

	private:
		QVector<MixerLayoutEntry> _entries;
		bool _entriesAsStruct{false};
	};
}

#endif
