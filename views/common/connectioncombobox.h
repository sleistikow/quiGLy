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

#ifndef CONNECTIONCOMBOBOX_H
#define CONNECTIONCOMBOBOX_H

#include "data/iport.h"
#include "data/iconnection.h"
#include "views/viewitem.h"

#include <QComboBox>

namespace ysm
{

	//! \brief Combo box to select a connection that is connected to a specific port.
	class ConnectionComboBox : public QComboBox, public ViewItem
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param port The port that provides the connections.
		 * \param parentView The parent view.
		 */
		ConnectionComboBox(IPort* port, IView* parentView);

		/*!
		 * \brief Set the selected item.
		 * \param connection The connection to select.
		 */
		void setCurrentConnection(IConnection* connection);

		/*!
		 * \brief Get the currently selected item's data or NULL.
		 * \return The item's data.
		 */
		IConnection* getCurrentConnection() const;

	protected:

		/*!
		 * \brief Connection was added, update the combo box.
		 * \param connection The connection.
		 */
		void addConnection(IConnection* connection);

		/*!
		 * \brief Connection was removed, update the combo box.
		 * \param connection The connection.
		 */
		void removeConnection(IConnection* connection);

		//! \brief Completely update the combo box.
		void updateConnections();

		/*!
		 * \brief Retrieve the descriptor for the given connection.
		 * \param connection The connection.
		 * \return A unique user readable descriptor string.
		 */
		QString getDescriptor(IConnection* connection) const;

	private:

		//! \brief The target port.
		IPort* _port;
	};
}

#endif // CONNECTIONCOMBOBOX_H
