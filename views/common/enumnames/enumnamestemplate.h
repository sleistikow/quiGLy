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

#ifndef ENUMNAMESTEMPLATE_H
#define ENUMNAMESTEMPLATE_H

#include "data/properties/property.h"

#include <QMap>

namespace ysm
{

	//Macro should be used in headers to start the enum's helper class.
	#define INTERFACE_START(name, type) \
		class name##Helper \
		{ \
		private: \
			static void initialize(); \
			static bool _initialized; \

	//Macro can be used to add text groups.
	#define INTERFACE_GROUP(type, group) \
		public: \
			static QMap<type, QString> getTyped##group(); \
			static QMap<int, QString> getUntyped##group(); \
		private: \
			static QMap<type, QString> _typed##group; \
			static QMap<int, QString> _untyped##group; \

	//Macro should be used in headers to end the enum's helper class.
	#define INTERFACE_END() \
		}; \

	//Macro should be used to start enum's helper class implementation.
	#define IMPLEMENT(name) bool name##Helper::_initialized = false;

	//Macro should be used to implement text groups.
	#define IMPLEMENT_GROUP(name, type, group) \
		QMap<type, QString> name##Helper::_typed##group; \
		QMap<int, QString> name##Helper::_untyped##group; \
		\
		QMap<type, QString> name##Helper::getTyped##group() { if(!_initialized) initialize(); return _typed##group; } \
		QMap<int, QString> name##Helper::getUntyped##group() { if(!_initialized) initialize(); return _untyped##group; }

	//Macro should be placed before any ADD() macros of one implementation.
	#define IMPLEMENTATION_START_ADD(name) void name##Helper::initialize() {

	//Macro adds a name to the group's text list.
	#define IMPLEMENTATION_ADD(group, key, value) \
		_typed##group[key] = value; \
		_untyped##group[static_cast<int>(key)] = value;

	//Macro should be used to after ADD() macros of one implementation.
	#define IMPLEMENTATION_END_ADD() _initialized = true; }

	//! \brief Static class the can convert enum properties to display names.
	class EnumPropertyHelper
	{
	public:

		/*!
		 * \brief Convert an enum property's current value to string.
		 * \param The property.
		 * \return The user readable name of the current value.
		 */
		static QString getName(const EnumProperty* enumProperty);

		/*!
		 * \brief Get enum property value names.
		 * \param enumProperty The enum property.
		 * \return The user readable names of the enum's values.
		 */
		static QMap<int, QString> getNames(const EnumProperty* enumProperty);

	private:

		//! \brief Initialize new instance.
		explicit EnumPropertyHelper();
	};
}

#endif // ENUMNAMESTEMPLATE_H
