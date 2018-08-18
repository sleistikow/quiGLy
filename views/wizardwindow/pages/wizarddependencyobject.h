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

#ifndef WIZARDDEPENDENCYOBJECT_H
#define WIZARDDEPENDENCYOBJECT_H

#include <QDomElement>
#include <QList>
#include <QMap>

namespace ysm
{

	//! \brief Adds support for dependencies parsed by XML.
	class WizardDependencyObject
	{

	private:

		//! \brief Internal struct that holds dependency data.
		struct Dependency { QString _source, _value; bool _inverse; };

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param element The element that provides the dependencies.
		 */
		WizardDependencyObject(const QDomElement& element);

		//! \brief Initialize new instance.
		WizardDependencyObject();

		/*!
		 * \brief Returns the item's dependencies.
		 * \return The names of the dependencies of this item.
		 */
		QList<QString> getDependencies() const;

		/*!
		 * \brief Checks, wether the dependency is fulfilled.
		 * \param name The field's name.
		 * \param value The field's value.
		 * \return True, if dependency is matched.
		 */
		bool checkDependency(const QString& name, const QString& value) const;

	private:

		//! \brief List of all dependencies.
		QMap<QString, QList<Dependency>> _dependencies;
	};

}

#endif // WIZARDDEPENDENCYOBJECT_H
