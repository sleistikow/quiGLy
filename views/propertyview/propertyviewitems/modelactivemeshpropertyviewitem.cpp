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

#include "modelactivemeshpropertyviewitem.h"

#include "../pipelineitempropertyview.h"
#include "data/properties/property.h"

#include <QVBoxLayout>

using namespace ysm;

ModelActiveMeshPropertyViewItem::ModelActiveMeshPropertyViewItem(IProperty* property, IView* parentView) :
	GenericPropertyViewItem(property, parentView)
{
	//Set label to property name.
	setLabel(new QLabel(property->getName()));

	//Create the combo box.
	_comboBox = new ActiveMeshComboBox(dynamic_cast<IBlock*>(property->getOwner()), this);
	_comboBox->setMinimumWidth(120);
	connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModelValue()));
	setEdit(_comboBox);
}

int ModelActiveMeshPropertyViewItem::getViewValue() const { return _comboBox->currentIndex(); }
void ModelActiveMeshPropertyViewItem::updateViewValue() { _comboBox->setCurrentIndex(getModelValue()); }
