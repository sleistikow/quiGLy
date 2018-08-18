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

#include "activemeshcombobox.h"
#include "data/properties/property.h"

using namespace ysm;

ActiveMeshComboBox::ActiveMeshComboBox(IBlock* modelLoaderBlock, IView* parentView) :
	QComboBox(),
	ViewItem(parentView),
	_block(modelLoaderBlock)
{
	//Retrieve the currently available meshes.
	updateComboBox();

	//Listen for changes.
	notifyStatic(this, &ActiveMeshComboBox::updateComboBox);
	notifyStatic(IChangeable::Change, this, &ActiveMeshComboBox::updateComboBox);
}

void ActiveMeshComboBox::updateComboBox()
{
	//Block the signals, the change must not be propagated.
	blockSignals(true);

	//Store the active value.
	unsigned int activeMesh = currentIndex();

	//Retrieve the data.
	unsigned int meshCount = _block->getProperty<UIntProperty>(PropertyID::Model_MeshCount)->getValue();
	QStringList meshNames = _block->getProperty<StringListProperty>(PropertyID::Model_MeshNames)->getValue();

	//Refill the combo box.
	clear();
	for (unsigned int i = 0; i < meshCount; ++i)
	{
		//Access the mesh's name, if available.
		QString meshName = QString("Unnamed Mesh #%1").arg(i + 1);
		if (i < (unsigned int) meshNames.size() && !meshNames[i].trimmed().isEmpty())
			meshName = meshNames[i].trimmed();

		//Add the mesh, check if it's the current mesh.
		addItem(meshName);
		if (i == activeMesh)
			setCurrentIndex(i);
	}

	//Disable combo box, if no meshes are available.
	if (meshCount == 0)
		setEnabled(false);

	//Unblock the signals.
	blockSignals(false);
}

void ActiveMeshComboBox::updateComboBox(IProperty* property)
{
	//If the mesh names change, update the combo box's content.
	if(property->getOwner() == _block && property->getID() == PropertyID::Model_MeshNames)
		updateComboBox();
}
