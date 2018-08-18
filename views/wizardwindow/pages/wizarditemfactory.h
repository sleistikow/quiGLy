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

#ifndef WIZARDITEMFACTORY_H
#define WIZARDITEMFACTORY_H

#include <QMap>
#include <QFormLayout>
#include <QDomElement>

namespace ysm
{
	class WizardDynamicItem;
	class WizardItemCollection;

	//! \brief Factory that creates wizard dynamic items from XML data.
	class WizardItemFactory
	{

	private:

		//! \brief Method declaration used to create an item.
		using ItemCreator = WizardDynamicItem* (*)(WizardItemCollection*, QDomElement, QFormLayout*);

	public:

		//! \brief Initialize new instance.
		WizardItemFactory();

		/*!
		 * \brief Register the given type using the given key.
		 * \param name The type's identifier.
		 */
		template<typename T> void registerItem(const QString& name)
		{
			//Append the create method to the registry.
			_registry[name] = createGeneric<T>;
		}

		/*!
		 * \brief Create the given item element.
		 * \param collection The collection that manages the item.
		 * \param element The XML item data.
		 * \param layout The layout where to add the item.
		 */
		WizardDynamicItem* createItem(WizardItemCollection* collection, QDomElement element,
									  QFormLayout* layout);

	protected:

		/*!
		 * \brief Creates a new dynamic item of the given type.
		 * \param collection The collection that manages the item.
		 * \param element The XML item data.
		 * \param layout The layout where to add the item.
		 */
		template<typename T>
		static WizardDynamicItem* createGeneric(WizardItemCollection* collection, QDomElement element,
												QFormLayout* layout)
		{
			//Just call the constructor, which must match the given style.
			return new T(collection, element, layout);
		}

	private:

		//! \brief The factory's data.
		QMap<QString, ItemCreator> _registry;

	};

}

#endif // WIZARDITEMFACTORY_H
