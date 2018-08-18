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

#ifndef WIZARDWINDOW_H
#define WIZARDWINDOW_H

#include "wizardfile.h"
#include "parser/wizardparser.h"
#include "wizardpagefactory.h"

#include <QLabel>
#include <QWizard>
#include <QStackedWidget>

namespace ysm
{
	class MainWindow;
	class WizardPageWidget;

	//! \brief Main window for the project wizard.
	class WizardWindow : public QWizard
	{
		Q_OBJECT

	public:

		//! \brief Simple struct holding a source's data.
		//! Provides some helper methods to access field data.
		struct Source
		{
			//! \brief Input properties.
			QString _name, _type, _primary, _disable, _naming; WizardWindow* _parent;

			/*!
			 * \brief Gets the primary field.
			 * \return The primary field value.
			 */
			QString getPrimary() const;

			/*!
			 * \brief Returns a user readable name.
			 * \return The user name.
			 */
			QString getUserName() const;

			/*!
			 * \brief Returns a unique identifier.
			 * \return The identifier.
			 */
			QString getID() const;

			/*!
			 * \brief Gets an internal field.
			 * \param field The field name.
			 * \return The field's value.
			 */
			QVariant getField(const QString& field) const;

			/*!
			 * \brief Checks if source is enabled.
			 * \return True, if enabled.
			 */
			bool isEnabled() const;

			/*!
			 * \brief Checks if source is a reference.
			 * \return True, if reference.
			 */
			bool isReference() const;
		};

		//! \brief Simple struct holding an output's data.
		//! Provides some helper methods to access field data.
		struct Output
		{
			//! \brief Output properties.
			QString _name, _type, _source, _naming; WizardWindow* _parent;

			/*!
			 * \brief Returns the output's exact name.
			 * \return The prefix (name.type).
			 */
			QString getTypeName() const;

			/*!
			 * \brief Returns a user readable name.
			 * \return The user name.
			 */
			QString getUserName() const;

			/*!
			 * \brief Returns a unique identifier.
			 * \return The identifier.
			 */
			QString getID() const;

			/*!
			 * \brief Gets an internal field.
			 * \param field The field name.
			 * \return The field's value.
			 */
			QVariant getField(const QString& field) const;

			/*!
			 * \brief Checks if source is enabled.
			 * \return True, if enabled.
			 */
			bool isEnabled() const;
		};

		//! \brief Simple struct holding a reference's data.
		struct Reference { QString _name, _identifier; bool _output; };

	private:

		//! \brief The next unique identifier.
		static int _nextID;

	public:

		/*!
		 * \brief Get a default label.
		 * \param caption The label's caption.
		 * \return The label with the given caption.
		 */
		static QLabel* getLabel(const QString& caption);

		/*!
		 * \brief Get a default divider.
		 * \return The divider widget.
		 */
		static QWidget* getDivider();

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param wizardFile The wizard to execute.
		 * \param singlePass False, if wizard is part of multi pass wizard.
		 * \param parent The parent widget.
		 */
		WizardWindow(const WizardFile& wizardFile, bool singlePass, QWidget* parent);

	public:

		/*!
		 * \brief Gets the window's underlying data.
		 * \return The underlying wizard data.
		 */
		WizardFile getWizard() const;

		/*!
		 * \brief Returns a unique ID for this wizard.
		 * \return The wizard ID.
		 */
		int getID() const;

		/*!
		 * \brief Checks wether this wizard is part of a multi pass wizard.
		 * \return True, if wizard is multi pass.
		 */
		bool isMultiPass() const;

		/*!
		 * \brief Checks wether the project should be imported.
		 * \return True for import, false for new project.
		 */
		bool getImportOption() const;

		/*!
		 * \brief Retrieves the selected target file.
		 * \return The target file or an empty string.
		 */
		QString getTargetFile() const;

		/*!
		 * \brief Gets the wizard's user name.
		 * \return The wizard's name.
		 */
		QString getUserName() const;

	public:

		/*!
		 * \brief Gets the length of the item array.
		 * \param name The item array.
		 * \return The array's length.
		 */
		int getItemCount(const QString& name);

		/*!
		 * \brief Gets the next available item from the array.
		 * \param name The item array.
		 * \param item Returns the item's identifier.
		 * \param index The iteration index, which is adjusted.
		 */
		bool nextItem(QString& name, QString& item, int& index) const;

		/*!
		 * \brief Appends an item to the item array.
		 * \param name The item array.
		 * \return The item's identifier.
		 */
		QString appendItem(const QString& name);

		/*!
		 * \brief Removes an item from the item array.
		 * \param name The item array.
		 * \param item The item's identifier.
		 */
		void removeItem(const QString& name, const QString& item);

	public:

		/*!
		 * \brief Retrieves all available sources.
		 * \return The sources.
		 */
		QList<Source> getSources() const;

		/*!
		 * \brief Retrieves the source for the given identifier.
		 * \param source The source identifier.
		 * \return The source.
		 */
		Source getSource(const QString& source) const;

		/*!
		 * \brief Register a new source.
		 * \param source The source.
		 */
		void registerSource(const Source& source);

	public:

		/*!
		 * \brief Retrieves all available output names.
		 * \return The output names.
		 */
		QList<QString> getOutputs() const;

		/*!
		 * \brief Retrieves the outputs for the given identifier.
		 * \param output The output identifier.
		 * \return The outputs.
		 */
		QList<Output> getOutputs(const QString& output) const;

		/*!
		 * \brief Register a new output.
		 * \param output The output.
		 */
		void registerOutput(const Output& output);

	public:

		/*!
		 * \brief Retrieves all available references.
		 * \param The source type to reference.
		 * \return The references.
		 */
		QList<Reference> getReferences(const QString& type) const;

		/*!
		 * \brief Registers a new reference.
		 * \param source The source to reference.
		 */
		void registerReference(const Source& source);

		/*!
		 * \brief Registers a new reference.
		 * \param source The source to reference.
		 */
		void registerReference(const Output& output);

	public slots:

		/*!
		 * \brief Generate the project file and load the project.
		 * \param mainWindow The main window.
		 */
		void createProject(MainWindow* mainWindow);

	protected:

		//! \brief Generate the wizard's pages.
		void createPages();

	private:

		//! \Brief The wizard's unique identifier.
		int _identifier;

		//! \brief Factory to build pages.
		WizardPageFactory _pageFactory;

		//! \brief The underlying wizard data.
		WizardFile _wizardFile;

		//! \brief True, if wizard is part of multi pass wizard.
		bool _multiPass;

		//! \brief The registered outputs.
		QMap<QString, QMap<QString, Output>> _outputs;

		//! \brief The registered sources.
		QMap<QString, Source> _sources;

		//! \brief The referenced sources by type.
		QMap<QString, QList<Reference>> _references;

		//! \brief The registered item arrays.
		QMap<QString, QList<QString>> _items;
		QMap<QString, int> _itemIDs;
	};

}

#endif // WIZARDWINDOW_H
