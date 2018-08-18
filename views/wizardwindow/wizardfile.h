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

#ifndef WIZARDFILE_H
#define WIZARDFILE_H

#include <QFile>
#include <QDomDocument>
#include <QDomElement>

namespace ysm
{

	//! \brief Contains data about a single project wizard.
	class WizardFile
	{
	public:

		//! \brief Simple helper structure to store page data.
		struct Page
		{
			//! \brief The page's name.
			QString _name;

			//! \brief The page's DOM element.
			QDomElement _root;
		};

	public:

		//! \brief Constant to access the name attribute.
		static const QString Name;

		//! \brief Constant to access the description attribute.
		static const QString Description;

		//! \brief Shortcut to invalid instance.
		static const WizardFile Invalid;

	public:

		/*!
		 * \brief Get the default base directory.
		 * \return The default base directory.
		 */
		static QString getBaseDirectory();

		/*!
		 * \brief Get the wizards directory relative to the given base directory.
		 * \param baseDirectory The base directory.
		 * \return The wizards directory.
		 */
		static QString getWizardsDirectory(const QString& baseDirectory);

		/*!
		 * \brief Get the sources directory relative to the given base directory.
		 * \param baseDirectory The base directory.
		 * \return The sources directory.
		 */
		static QString getSourcesDirectory(const QString& baseDirectory);

		/*!
		 * \brief Get the outputs directory relative to the given base directory.
		 * \param baseDirectory The base directory.
		 * \return The outputs directory.
		 */
		static QString getOutputsDirectory(const QString& baseDirectory);

		/*!
		 * \brief Get the include directory relative to the given base directory.
		 * \param baseDirectory The base directory.
		 * \return The include directory.
		 */
		static QString getIncludeDirectory(const QString& baseDirectory);

	public:

		/*!
		 * \brief Gets the given file's root element and logs problems to the console.
		 * \param configurationFile The configuration file.
		 * \param rootElement Returns the root element on success.
		 * \return True on success.
		 */
		static bool getRootElement(QFile& configurationFile, QDomElement& rootElement);

		/*!
		 * \brief Finds a single child element in the given DOM element and returns it's content.
		 * \param rootElement The DOM element.
		 * \param propertyKey The property's key.
		 * \param defaultValue The default value.
		 * \return The property's value.
		 */
		static QString getProperty(QDomElement rootElement, const QString& propertyKey,
								   const QString& defaultValue = QString());

		/*!
		 * \brief Finds a single child element in the given DOM element and returns it's value attribute.
		 * \param rootElement The DOM element.
		 * \param propertyKey The property's key.
		 * \param defaultValue The default value.
		 * \return The attribute's value.
		 */
		static QString getAttribute(QDomElement rootElement, const QString& propertyKey,
									const QString& defaultValue = QString(),
									const QString& attributeName = "value");

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param baseDirectory The wizard's base directory.
		 */
		WizardFile(const QString& baseDirectory, const QString& wizardName);

		/*!
		 * \brief Copy instance.
		 * \param wizardFile The source instance.
		 */
		WizardFile(const WizardFile& wizardFile);

		//! \brief Initialize invalid instance.
		WizardFile();

		/*!
		 * \brief Copy instance.
		 * \param other The source instance.
		 * \return Reference to this.
		 */
		WizardFile& operator=(const WizardFile& wizardFile);

		/*!
		 * \brief Returns true, if the configuration file was parsed successfully.
		 * \return True, if wizard is valid.
		 */
		bool isValid() const;

		/*!
		 * \brief Return all available pages.
		 * \return List of pages.
		 */
		QList<Page> getPages() const;

		/*!
		 * \brief Return the wizard's project template file.
		 * \return The template file.
		 */
		QString getTemplate() const;

	public:

		/*!
		 * \brief Convencience method to access a property of the wizard.
		 * \param propertyKey The property's key.
		 * \return The property's value.
		 */
		QString getProperty(const QString& propertyKey) const;

		/*!
		 * \brief Covenience method to access the wizards directory of the wizard.
		 * \return The wizards directory.
		 */
		QString getWizardsDirectory() const;

		/*!
		 * \brief Covenience method to access the sources directory of the wizard.
		 * \return The sources directory.
		 */
		QString getSourcesDirectory() const;

		/*!
		 * \brief Covenience method to access the outputs directory of the wizard.
		 * \return The outputs directory.
		 */
		QString getOutputsDirectory() const;

		/*!
		 * \brief Covenience method to access the include directory of the wizard.
		 * \return The outputs directory.
		 */
		QString getIncludeDirectory() const;

	protected:

		//! \brief Parse the configuration file.
		void parseConfiguration();

	private:

		//! \brief The wizard's base directory.
		QString _baseDirectory;

		//! \brief The wizard's name.
		QString _wizardName;

		//! \brief The wizard's state.
		bool _isValid;

		//! \brief The XML's root element.
		QDomElement _rootElement;
	};
}

#endif // WIZARDFILE_H
