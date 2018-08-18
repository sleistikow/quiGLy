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

#include "wizardpagedelegate.h"
#include "../wizarddependencyobject.h"

#include <QDebug>

using namespace ysm;

WizardPageDelegate::WizardPageDelegate(QWidget* parent) :
	QWizardPage(parent)
{ }

WizardPageDelegate::~WizardPageDelegate() { }

void WizardPageDelegate::registerDynamicField(const QString& name, QWidget* widget, const char* property)
{
	//Adjust the name and forward to the internal implementation.
	QString fieldName = dynamicName(name);
	registerField(fieldName, widget, property);

	//Log the field registration.
	qDebug() << "Registered field " << fieldName;
}

QVariant WizardPageDelegate::dynamicField(const QString& name) const
{
	//Adjust the name and forward to the internal implementation.
	return field(dynamicName(name));
}

void WizardPageDelegate::notifyCompleteChanged() { emit completeChanged(); }

int WizardPageDelegate::nextId() const
{
	//Check all following pages until one page is found that has all dependencies set.
	QWizardPage* nextPage = 0;
	int nextId = wizard()->currentId();
	while(nextPage = wizard()->page(++nextId))
	{
		//Check if next page has dependencies at all.
		WizardDependencyObject* dependencyPage = dynamic_cast<WizardDependencyObject*>(nextPage);
		if(!dependencyPage)
			return nextId;

		//Iterate over the page's dependencies and validate them.
		bool dependenciesFulfilled = true;
		foreach(QString dependency, dependencyPage->getDependencies())
		{
			//Retrieve the dependency's value, compare as string.
			QString fieldValue = field(dependency).toString();
			if(!dependencyPage->checkDependency(dependency, fieldValue))
				dependenciesFulfilled = false;
		}

		//All dependencies fulfilled, show next page.
		if(dependenciesFulfilled)
			return nextId;
	}

	//No page found.
	return -1;
}
