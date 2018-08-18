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

#include "connectioncombobox.h"
#include "data/iblock.h"

using namespace ysm;

ConnectionComboBox::ConnectionComboBox(IPort* port, IView* parentView) :
	QComboBox(),
	ViewItem(parentView),
	_port(port)
{
	//Retrieve the currently available connections.
	updateConnections();

	//Listen for connection changes.
	notifyStatic(this, &ConnectionComboBox::updateConnections);
	notifyStatic(IChangeable::Add, this, &ConnectionComboBox::addConnection);
	notifyStatic(IChangeable::Remove, this, &ConnectionComboBox::removeConnection);
}

IConnection* ConnectionComboBox::getCurrentConnection() const
{
	//Ensure an item is selected.
	if(currentIndex() < 0) return NULL;

	//Find the actual connection with the currently selected identifier.
	int connectionID = currentData().toInt();
	foreach(IConnection* connection, _port->getInConnections())
		if(connection->getID() == (unsigned int) connectionID)
			return connection;

	//Connection not found.
	return NULL;
}

void ConnectionComboBox::setCurrentConnection(IConnection* connection)
{
	//Find the selected connection's index.
	int targetIndex = -1;
	for(int i = 0; i < count(); i++)
		if(connection && itemData(i) == connection->getID())
			targetIndex = i;

	//Block the signals as the change must not be propagated, then change the selected index.
	blockSignals(true);
	setCurrentIndex(targetIndex);
	blockSignals(false);
}

void ConnectionComboBox::addConnection(IConnection* connection)
{
	//Check if the connection targets the port.
	if(connection->getDestPort() == _port)
	{
		//Block the signals, the change must not be propagated.
		blockSignals(true);

		//Add the connection item using the descriptor and identifier.
		addItem(getDescriptor(connection), connection->getID());

		//Unblock the signals.
		blockSignals(false);
	}
}

void ConnectionComboBox::removeConnection(IConnection* connection)
{
	//Check if the connection targets the layout's data port.
	if(connection->getDestPort() == _port)
	{
		//Block the signals, the change must not be propagated.
		blockSignals(true);

		//Remove the connection item, if available.
		int connectionIndex = findData(connection->getID());
		if(connectionIndex >= 0)
			removeItem(connectionIndex);

		//Unblock the signals.
		blockSignals(false);
	}
}

void ConnectionComboBox::updateConnections()
{
	//Block the signals, the change must not be propagated.
	blockSignals(true);

	//Get the current connection, so it can be restored afterwards.
	IConnection* currentConnection = getCurrentConnection();

	//Iterate over all available connections and re-add them.
	clear();
	foreach(IConnection* connection, _port->getInConnections())
		addItem(getDescriptor(connection), connection->getID());

	//Try to restore the previous selection.
	setCurrentConnection(currentConnection);

	//Unblock the signals.
	blockSignals(false);
}

QString ConnectionComboBox::getDescriptor(IConnection* connection) const
{
	//Format: [#blockID] blockName :: portName.
	return QString("[#%2] %1 :: %3")
			.arg(connection->getSource()->getName())
			.arg(connection->getSource()->getID())
			.arg(connection->getSourcePort()->getName());
}
