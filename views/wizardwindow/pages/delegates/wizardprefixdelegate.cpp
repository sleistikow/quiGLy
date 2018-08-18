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

#include "wizardprefixdelegate.h"

using namespace ysm;

WizardPrefixDelegate::WizardPrefixDelegate(IWizardDynamicDelegate* delegate, const QString& prefix) :
	WizardForwardingDelegate(delegate),
	_prefix(prefix)
{ }

WizardPrefixDelegate::WizardPrefixDelegate() : _prefix() { }
WizardPrefixDelegate::~WizardPrefixDelegate() { }

void WizardPrefixDelegate::registerDynamicField(const QString& name, QWidget* widget, const char* property)
{
	//Prefix the field name.
	QString prefixName = QString("%1.%2").arg(_prefix).arg(name);
	_delegate->registerDynamicField(prefixName, widget, property);
}

QVariant WizardPrefixDelegate::dynamicField(const QString& name) const
{
	//Prefix the field name.
	QString prefixName = QString("%1.%2").arg(_prefix).arg(name);
	return _delegate->dynamicField(prefixName);
}

QString WizardPrefixDelegate::dynamicName(const QString &name) const
{
	//Prefix the name.
	QString prefixName = QString("%1.%2").arg(_prefix).arg(name);
	return _delegate->dynamicName(prefixName);
}
