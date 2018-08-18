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

#ifndef ITRACEABLE_H
#define ITRACEABLE_H

#include "ichangeable.h"

namespace ysm
{
	//! \brief Interface for type independent value tracing.
	class ITraceableValue
	{
	public:

		//! \brief Destruct instance.
		virtual ~ITraceableValue () { }

		/*!
		 * \brief Creates a new copy of this value.
		 * NOTE: The caller becomes owner of the returned instance.
		 * \return The newly created copy.
		 */
		virtual ITraceableValue* cloneValue() const = 0;

	protected:

		//! \brief Initialize new instance.
		ITraceableValue() { }
	};

	//! \brief Changeable interface that allows to trace the latest occured changes.
	class ITraceable : public IChangeable
	{
	public:

		/*!
		 * \brief Returns the current value of this changeable type independently.
		 * NOTE: The caller becomes owner of the returned instance.
		 * \return The changeable's value.
		 */
		virtual ITraceableValue* toTraceableValue() const = 0;

		/*!
		 * \brief Sets the changeable's value type independently.
		 * NOTE: The caller stays owner of the given instance.
		 * \param value The changeable's value.
		 */
		virtual void fromTraceableValue(const ITraceableValue* value) = 0;

		/*!
		 * \brief Checks wether the changeable changed since the change flag was cleared.
		 * \return The current state of the change flag.
		 */
		virtual bool hasChanged() const = 0;

		//! \brief Clears the change flag.
		virtual void clearChanged() = 0;

		/*!
		 * \brief Returns the value that the changeable had, when the change flag was cleared.
		 * NOTE: The instance is removed when the change flag is cleared.
		 * The caller DOES NOT become the owner of the returned instance.
		 * \return The original value.
		 */
		virtual ITraceableValue* getUnchangedValue() const = 0;
	};
}

#endif // ITRACEABLE_H
