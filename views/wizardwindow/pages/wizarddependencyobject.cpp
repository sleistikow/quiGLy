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

#include "wizarddependencyobject.h"

using namespace ysm;

WizardDependencyObject::WizardDependencyObject(const QDomElement& element)
{
	//Retrieve all dependencies.
	for(QDomElement dependency = element.firstChildElement("dependency"); !dependency.isNull();
		dependency = dependency.nextSiblingElement("dependency"))
	{
		//Get the data.
		QString source = dependency.attribute("source");
		QString value = dependency.attribute("value");
		bool inverse = dependency.attribute("inverse") == "true";

		//Store the dependency.
		_dependencies[source].append({ source, value, inverse });
	}
}

WizardDependencyObject::WizardDependencyObject() { }

QList<QString> WizardDependencyObject::getDependencies() const { return _dependencies.keys(); }
bool WizardDependencyObject::checkDependency(const QString& name, const QString& value) const
{
	//Ensure that the item depends on the given field.
	if(!_dependencies.contains(name)) return true;

	//Evaluate the dependency.
	foreach(Dependency dependency, _dependencies[name])
		if((dependency._value == value) == dependency._inverse)
			return false;

	//Dependency evaluated.
	return true;
}
