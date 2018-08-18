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

#ifndef WIZARDPAGEFACTORY_H
#define WIZARDPAGEFACTORY_H

#include "wizardfile.h"

#include <QMap>
#include <QDomElement>
#include <QWizardPage>

namespace ysm
{
	class WizardWindow;

	//! \brief Factory that creates wizard pages from XML data.
	class WizardPageFactory
	{

	public:

		//! \brief Initialize new instance.
		WizardPageFactory();

		/*!
		 * \brief Register the given type using the given key.
		 * \param name The type's identifier.
		 */
		template<typename T> void registerPage(const QString& name)
		{
			//Append the create method to the registry.
			_registry[name] = createPage<T>;
		}

		/*!
		 * \brief Create the given page element.
		 * \param page The XML page data.
		 * \param parent The parent window.
		 * \return New wizard page widget.
		 */
		QWizardPage* createPage(WizardFile::Page page, WizardWindow* parent);

	protected:

		/*!
		 * \brief Creates a new page widget of the given type.
		 * \param rootElement The root element.
		 * \param parent The parent window.
		 */
		template<typename T> static QWizardPage* createPage(QDomElement rootElement, WizardWindow* parent)
		{
			//Just call the constructor and forward the element.
			return new T(rootElement, parent);
		}

	private:

		//! \brief The factory's data.
		QMap<QString, QWizardPage* (*)(QDomElement, WizardWindow*)> _registry;

	};

}

#endif // WIZARDPAGEFACTORY_H
