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

#include "wizardpagefactory.h"

#include "pages/wizardfilepage.h"
#include "pages/wizardcustompage.h"
#include "pages/wizardsourcepage.h"
#include "pages/wizardoutputpage.h"
#include "pages/wizardstackedpage.h"
#include "pages/wizarditemspage.h"

using namespace ysm;

WizardPageFactory::WizardPageFactory()
{
	//Register all default page types.
	registerPage<WizardFilePage>("file");
	registerPage<WizardCustomPage>("custom");
	registerPage<WizardSourcePage>("source");
	registerPage<WizardOutputPage>("output");
	registerPage<WizardStackedPage>("stack");
	registerPage<WizardItemsPage>("items");
}

QWizardPage* WizardPageFactory::createPage(WizardFile::Page page, WizardWindow *parent)
{
	//Ensure the page is registered.
	if(!_registry.contains(page._name)) return NULL;

	//Call the construction method and return the new instance.
	return _registry[page._name](page._root, parent);
}
