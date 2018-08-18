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

#ifndef UPDATEPROPERTIESCOMMAND_H
#define UPDATEPROPERTIESCOMMAND_H

#include "commands/uidatachangingcommand.h"
#include "data/ipipelineitem.h"
#include "data/iproperty.h"

#include <QDebug>

namespace ysm
{
	//! \brief Command that updates a variable number of properties.
	class UpdatePropertiesCommand : public UIDataChangingCommand
	{

	private:

		//! \brief Interface that acts as base class for templated child classes.
		class IUpdatePropertyCommand
		{
		public:

			//! \brief Destruct instance.
			virtual ~IUpdatePropertyCommand() { }

			//! \brief Forward execution.
			virtual void redo() = 0;

			//! \brief Backwards execution.
			virtual void undo() = 0;

		protected:

			//! \brief Initialize new instance.
			IUpdatePropertyCommand() { }
		};

		//! \brief Generic property updater.
		template<typename T, typename S> class UpdatePropertyCommand : public IUpdatePropertyCommand
		{

		public:

			/*!
			 * \brief Initialize new instance.
			 * \param pipelineItem The item to operate on.
			 * \param property The property to operate on.
			 * \param value The value to set.
			 */
			UpdatePropertyCommand(IPipelineItem* pipelineItem, T* property, S value) :
				_pipelineItem(pipelineItem),
				_property(property),
				_newValue(value)
			{ }

			//! \brief Forward execution.
			void redo() Q_DECL_OVERRIDE
			{
				_oldValue = static_cast<S>(_property->getValue());
				if(!_property->isReadOnly())
					_property->setValue(_newValue);
			}

			//! \brief Backwards execution.
			void undo() Q_DECL_OVERRIDE
			{
				if(!_property->isReadOnly())
					_property->setValue(_oldValue);
			}

		private:

			//! \brief The item to operate on.
			IPipelineItem* _pipelineItem;

			//! \brief The property to operate on.
			T* _property;

			//! \brief Old and new value.
			S _newValue, _oldValue;
		};


		//! \brief Generic property updater from string.
		template<typename T> class UpdatePropertyFromStringCommand : public IUpdatePropertyCommand
		{

		public:

			/*!
			 * \brief Initialize new instance.
			 * \param pipelineItem The item to operate on.
			 * \param property The property to operate on.
			 * \param value The value to set.
			 */
			UpdatePropertyFromStringCommand(IPipelineItem* pipelineItem, T* property, QString value) :
				_pipelineItem(pipelineItem),
				_property(property),
				_newValue(value),
				_oldValue(property->toString())
			{ }

			//! \brief Forward execution.
			void redo() Q_DECL_OVERRIDE { _property->fromString(_newValue); }

			//! \brief Backwards execution.
			void undo() Q_DECL_OVERRIDE { _property->fromString(_oldValue); }

		private:

			//! \brief The item to operate on.
			IPipelineItem* _pipelineItem;

			//! \brief The property to operate on.
			T* _property;

			//! \brief Old and new value.
			QString _newValue, _oldValue;
		};

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param pipelineItem The item to operate on.
		 */
		UpdatePropertiesCommand(IPipelineItem* pipelineItem);

		/*!
		 * \brief Initialize new instance, add single property update.
		 * \param pipelineItem The item to operate on.
		 * \param property The property to change.
		 * \param value The new value.
		 */
		template<typename T, typename S> UpdatePropertiesCommand(IPipelineItem* pipelineItem, T* property, S value) :
			UpdatePropertiesCommand(pipelineItem)
		{ addOperation<T, S>(property, value); }

		//! \brief Destruct instance.
		virtual ~UpdatePropertiesCommand();

		/*!
		 * \brief Execute the command.
		 * \return True, on success.
		 */
		bool execute() Q_DECL_OVERRIDE;

		/*!
		 * \brief Undo the command.
		 * \return True, on success.
		 */
		bool undo() Q_DECL_OVERRIDE;

		/*!
		 * Adds an update operation.
		 * \param property The property to change.
		 * \param value The new value.
		 */
		template<typename T, typename S> void addOperation(T* property, S value)
		{
			//Make sure, command has not been executed.
			if(_wasExecuted)
				qDebug() << "Can not add operation, command was already executed.";

			else if(property->getOwner() != _pipelineItem)
				qDebug() << "Can not add operation, property does not belong to correct pipeline item.";

			//Append the property update operation.
			else
			{
				_commands.append(new UpdatePropertyCommand<T, S>(_pipelineItem, property, value));
				addChangedObject(property, IChangeable::Change);
			}
		}

		/*!
		 * Adds an update from string operation.
		 * \param property The property to change form string.
		 * \param value The new value as string.
		 */
		template<typename T> void addFromStringOperation(T* property, QString value)
		{
			//Make sure, command has not been executed.
			if(_wasExecuted)
				qDebug() << "Can not add operation, command was already executed.";

			else if(property->getOwner() != _pipelineItem)
				qDebug() << "Can not add operation, property does not belong to correct pipeline item.";

			//Append the property update operation.
			else
			{
				_commands.append(new UpdatePropertyFromStringCommand<T>(_pipelineItem, property, value));
				addChangedObject(property, IChangeable::Change);
			}
		}

	private:

		//! \brief Internal commands, used to execute this command.
		QList<IUpdatePropertyCommand*> _commands;

		//! \brief Pipeline item to operate on.
		IPipelineItem* _pipelineItem;

		//! \brief Flag, checks if the process has been executed.
		bool _wasExecuted;
	};
}

#endif // UPDATEPROPERTIESCOMMAND_H
