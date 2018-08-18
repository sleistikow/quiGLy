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

#ifndef VERSIONSELECT_H
#define VERSIONSELECT_H

#include "views/mainwindow/mainwindow.h"
#include "views/mainwindow/rendermanager.h"

#include <QComboBox>

namespace ysm
{

	//! \brief Combo box that allows to select an OpenGL version.
	class VersionSelect : public QComboBox
	{
		Q_OBJECT

		Q_PROPERTY(RenderManager::OpenGLVersion currentVersion READ getCurrentVersion)
		Q_PROPERTY(int numericVersion READ getNumericVersion)

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param minimumVersion The minimum version that can be selected.
		 * \param document The document to retrieve data from.
		 */
		VersionSelect(int minimumVersion, QWidget* parent);

		/*!
		 * \brief Return the selected version.
		 * \return The selected OpenGL version.
		 */
		RenderManager::OpenGLVersion getCurrentVersion() const;

		/*!
		 * \brief Return the selected version as numeric value.
		 * \return The selected OpenGL version.
		 */
		int getNumericVersion() const;

	private:

		//! \brief Available version info.
		QMap<QString, QPair<RenderManager::OpenGLVersion, bool>> _versionInfo;
	};

}

#endif // VERSIONSELECT_H
