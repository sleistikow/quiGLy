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

#ifndef UIDATACHANGINGCOMMAND_H
#define UIDATACHANGINGCOMMAND_H

#include "uicommand.h"
#include "data/ipipelineitem.h"
#include "data/iproperty.h"

#include <QObject>
#include <QMap>

namespace ysm
{
	//! \brief Abstract base interface for commands, that operate on the data.
	//! Purpose of this class is to reduce the boilerplate code in commands, that change data in any case.
	class UIDataChangingCommand : public UICommand
	{

	private:

		//! \brief Helper class that stores information about a property's state.
		class PropertyBackup
		{
		public:

			/*!
			 * \brief Initialize new instance.
			 * \param property The property to backup.
			 */
			PropertyBackup(IProperty* property);

			//! \brief Destruct instance.
			virtual ~PropertyBackup();

			//! \brief Restore the property's value.
			void restoreProperty() const;

		private:

			//! \brief The target property to backup.
			IProperty* _target;

			//! \brief The property's backup value.
			ITraceableValue* _backup;
		};

	public:

		//! \brief Destruct instance.
		virtual ~UIDataChangingCommand();

		/*!
		 * \brief List of all objects, that are changed during execution.
		 * \param operation The type of operation.
		 * \return List of changed objects.
		 */
		QList<IChangeable*> getChangedObjects(IChangeable::Operation operation) Q_DECL_OVERRIDE;

		/*!
		 * \brief Returns wether the command is undoable.
		 * \return True, if undoable.
		 */
		bool isUndoable() const Q_DECL_OVERRIDE;

		/*!
		 * \brief Check if the rendering needs to be re-evaluted after execution.
		 * \return True, if rendering could have changed.
		 */
		bool didChangeRendering() const Q_DECL_OVERRIDE;

	protected:

		/// Initialize new instance.
		UIDataChangingCommand();

		/*!
		 * \brief Adds an object to the list of changed objects.
		 * \param object The object.
		 * \param operation The operation that is executed on the object.
		 */
		void addChangedObject(IChangeable* object, IChangeable::Operation operation);

		//! \brief Clears the added and removed object list.
		void resetChangedObjects(IChangeable::Operation operation);

		/*!
		 * \brief Adds a property to the backup list. The property's value can be restored using restoreProperties().
		 * This is useful to handle dependent properties, that change implicitly on execution of this command.
		 * - The method DOES NOT check, wether the property actually changed.
		 * - The method must be called AFTER the change occured, so that the previous value is then stored.
		 * - If the command can affect any properties (e.g. on deletion, if a block is referenced), use one of the
		 *   overloads that catches all possible changes.
		 * \param property The property to add to backup list.
		 */
		void backupProperty(IProperty* property);

		/*!
		 * \brief Adds all changed property's of the item to the backup list.
		 * - The method does check which of the properties actually changed.
		 * - The method must be called AFTER the change occured, so that the changes can be identified.
		 * \param pipelineItem The item that is changed.
		 */
		void backupProperties(IPipelineItem* pipelineItem);

		/*!
		 * \brief Adds all changed property's to the backup list.
		 * - The method does check which of the properties actually changed.
		 * - The method must be called AFTER the change occured, so that the changes can be identified.
		 * \param pipeline The pipeline that is changed.
		 */
		void backupProperties(IPipeline* pipeline);

		/*!
		 * \brief Clears all changed property's of the item.
		 * \param pipelineItem The item that is changed.
		 */
		void clearProperties(IPipelineItem* pipelineItem);

		/*!
		 * \brief Clears all changed property's.
		 * \param pipeline The pipeline that is changed.
		 */
		void clearProperties(IPipeline* pipeline);

		//! \brief After this method is called, all further backupProperties() calls will be ignored.
		//! This method can be used to ensure backups are only created upon the first execution of the command.
		void propertyBackupsDone();

		//! \brief Restores all backed up properties.
		void restoreProperties();

	private:

		//! \brief List of changed objects.
		QMap<IChangeable::Operation, QList<IChangeable*>> _changedObjects;

		//! \brief List of property backups.
		QList<PropertyBackup*> _propertyBackups;

		//! \brief Flag that indicates first execution.
		bool _backupsDone;
	};

}

#endif // UIDATACHANGINGCOMMAND_H
