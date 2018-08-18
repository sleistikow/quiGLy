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

#include "wizardenumerationdelegate.h"

using namespace ysm;

WizardEnumerationDelegate::WizardEnumerationDelegate(IWizardDynamicDelegate* delegate, int index) :
	_delegate(delegate),
	_index(index)
{ }

WizardEnumerationDelegate::WizardEnumerationDelegate() : _delegate(NULL), _index(0) { }
WizardEnumerationDelegate::~WizardEnumerationDelegate() { }

void WizardEnumerationDelegate::registerDynamicField(const QString& name, QWidget* widget, const char* property)
{
	//Enumerate the field name.
	QString prefixName = QString("[%1].%2").arg(_index).arg(name);
	_delegate->registerDynamicField(prefixName, widget, property);
}

QVariant WizardEnumerationDelegate::dynamicField(const QString& name) const
{
	//Enumerate the field name.
	QString prefixName = QString("[%1].%2").arg(_index).arg(name);
	return _delegate->dynamicField(prefixName);
}

QString WizardEnumerationDelegate::dynamicName(const QString &name) const
{
	//Enumerate the name.
	QString prefixName = QString("[%1].%2").arg(_index).arg(name);
	return _delegate->dynamicName(prefixName);
}

void WizardEnumerationDelegate::notifyCompleteChanged() { _delegate->notifyCompleteChanged(); }
