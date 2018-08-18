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

#include "wizarditemfactory.h"
#include "collections/wizarditemcollection.h"

#include "pageitems/wizardcheckboxitem.h"
#include "pageitems/wizardcomboboxitem.h"
#include "pageitems/wizardfileitem.h"
#include "pageitems/wizardcodeitem.h"
#include "pageitems/wizardsourceitem.h"
#include "pageitems/wizardstackeditem.h"
#include "pageitems/wizardnumericitem.h"
#include "pageitems/wizardvecitem.h"
#include "pageitems/wizardcoloritem.h"

using namespace ysm;

WizardItemFactory::WizardItemFactory()
{
	//Register all default page types.
	registerItem<WizardCheckBoxItem>("bool");
	registerItem<WizardComboBoxItem>("select");
	registerItem<WizardFileItem>("file");
	registerItem<WizardCodeItem>("code");
	registerItem<WizardSourceItem>("source");
	registerItem<WizardStackedItem>("stack");
	registerItem<WizardNumericItem<QDoubleSpinBox, float>>("float");
	registerItem<WizardNumericItem<QSpinBox, int>>("int");
	registerItem<WizardVecItem<2>>("vec2");
	registerItem<WizardVecItem<3>>("vec3");
	registerItem<WizardVecItem<4>>("vec4");
	registerItem<WizardColorItem>("color");
}

WizardDynamicItem* WizardItemFactory::createItem(WizardItemCollection* collection, QDomElement element,
												 QFormLayout* layout)
{
	//Retrieve the item data.
	QString typeName = element.attribute("type");

	//Ensure the page is registered.
	if(!_registry.contains(typeName)) return NULL;

	//Call the construction method and return the new instance.
	return _registry[typeName](collection, element, layout);
}
