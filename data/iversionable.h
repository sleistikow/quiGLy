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

#ifndef IVERSIONABLE_H
#define IVERSIONABLE_H

#define DEFAULT_MINIMUM_VERSION 330

namespace ysm
{
	//! @brief Interface for versionable items (items which only exist for a certain OpenGL version).
	class IVersionable
	{
	public:

		//! @brief Destruct instance.
		virtual ~IVersionable() { }

	public:

		/*!
		 * @brief Returns the versionable's current version.
		 * @return The version as integer (e.g. 330 for 3.3).
		 */
		virtual unsigned int getVersion() const = 0;

		/*!
		 * @brief Returns the versionable's minimum required version.
		 * @return The version as integer (e.g. 330 for 3.3).
		 */
		inline virtual unsigned int getMinimumVersion() const { return DEFAULT_MINIMUM_VERSION; }

		/*!
		 * @brief Returns the versionable's last supported version, or 0 if still supported.
		 * @return The version as integer (e.g. 330 for 3.3).
		 */
		inline virtual unsigned int getDeprecatedVersion() const { return 0; }

		/*!
		 * @brief Returns the versionable's first deprecated version, or 0 if not deprecated.
		 * @return The version as integer (e.g. 330 for 3.3).
		 */
		inline virtual unsigned int getMaximumVersion() const { return 0; }

		/*!
		 * @brief Checks, wether the item is supported by it's current version.
		 * @return True, if supported.
		 */
		inline virtual bool isSupported() const
		{
			//Access required versions.
			unsigned int version = getVersion();
			unsigned int minimumVersion = getMinimumVersion(), maximumVersion = getMaximumVersion();

			//Ensure version is valid.
			return version >= minimumVersion && (maximumVersion == 0 || version <= maximumVersion);
		}

		/*!
		 * @brief Checks, wether the item is deprecated in it's current version.
		 * @return True, if deprecated.
		 */
		inline virtual bool isDeprecated() const
		{
			//Access required versions.
			unsigned int version = getVersion();
			unsigned int deprecatedVersion = getDeprecatedVersion();

			//Ensure version is valid.
			return deprecatedVersion != 0 && version >= deprecatedVersion;
		}

	protected:

		//! @brief Initialize new instance.
		IVersionable() { }
	};
}

#endif
