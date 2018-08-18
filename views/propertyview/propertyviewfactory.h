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

#ifndef PROPERTYVIEWFACTORY_H
#define PROPERTYVIEWFACTORY_H

#include "data/ipipelineitem.h"
#include "data/iblock.h"
#include "data/irendercommand.h"
#include "data/iconnection.h"
#include "data/iproperty.h"

#include <QObject>
#include <QMap>

namespace ysm
{
	class PipelineItemPropertyView;
	class IPropertyViewItem;
	class IView;

	//! \brief Factory, that is able to create property views, and property view items.
	class PropertyViewFactory : public QObject
	{
		Q_OBJECT

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		explicit PropertyViewFactory(QObject* parent = 0);

		/*!
		 * \brief Creates a new property view, that is able to represent the given item.
		 * \param pipelineItem The data item.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		PipelineItemPropertyView* createBlockPropertyView(IBlock* pipelineItem,
														  IView* parentView) const;

		/*!
		 * \brief Creates a new property view, that is able to represent the given item.
		 * \param pipelineItem The data item.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		PipelineItemPropertyView* createRenderCommandPropertyView(IRenderCommand* pipelineItem,
																  IView* parentView) const;

		/*!
		 * \brief Creates a new property view, that is able to represent the given item.
		 * \param pipelineItem The data item.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		PipelineItemPropertyView* createConnectionPropertyView(IConnection* pipelineItem,
															   IView* parentView) const;

		/*!
		 * \brief Creates a new property view item, that is able to represent the given property.
		 * \param property The data item.
		 * \param parentView The parent item.
		 * \return The property view item.
		 */
		IPropertyViewItem* createPropertyViewItem(IProperty* property,
												  IView* parentView) const;

		/*!
		 * \brief Register a block type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param constructor The constructor, or null to unregister.
		 * \param registrator Executed after construction, to register specialized property view items.
		 */
		void setBlockConstructor(const BlockType itemType,
								 PipelineItemPropertyView* (* constructor)(IBlock*, IView*),
								 void(* registrator)(PropertyViewFactory*) = 0);

		/*!
		 * \brief Register a render command type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param constructor The constructor, or null to unregister.
		 * \param registrator Executed after construction, to register specialized property view items.
		 */
		void setRenderCommandConstructor(const RenderCommandType itemType,
										 PipelineItemPropertyView* (* constructor)(IRenderCommand*, IView*),
										 void(* registrator)(PropertyViewFactory*) = 0);

		/*!
		 * \brief Register the connection type.
		 * If it's already registered, it's overwritten.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setConnectionConstructor(PipelineItemPropertyView* (* constructor)(IConnection*, IView*));

		/*!
		 * \brief Register a property type for any pipeline item type.
		 * If it's already registered, it's overwritten.
		 * \param propertyType The property type.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setItemConstructor(const PropertyType propertyType,
								IPropertyViewItem* (* constructor)(IProperty*, IView*));

		/*!
		 * \brief Register a special property id for any pipeline item type.
		 * If it's already registered, it's overwritten.
		 * \param propertyID The property id.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setItemConstructor(const PropertyID propertyID,
								IPropertyViewItem* (* constructor)(IProperty*, IView*));

		/*!
		 * \brief Regster a property type for a special block type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param propertyType The property type.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setBlockItemConstructor(const BlockType itemType, const PropertyType propertyType,
									 IPropertyViewItem* (*constructor)(IProperty*, IView*));

		/*!
		 * \brief Regster a property type for a special render command type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param propertyType The property type.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setRenderCommandItemConstructor(const RenderCommandType itemType, const PropertyType propertyType,
											 IPropertyViewItem* (* constructor)(IProperty*, IView*));

		/*!
		 * \brief Register a special property id for a special block type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param propertyID The property id.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setBlockItemConstructor(const BlockType itemType, const PropertyID propertyType,
									 IPropertyViewItem* (*constructor)(IProperty*, IView*));

		/*!
		 * \brief Register a special property id for a special render command type.
		 * If it's already registered, it's overwritten.
		 * \param itemType The item type.
		 * \param propertyID The property id.
		 * \param constructor The constructor, or null to unregister.
		 */
		void setRenderCommandItemConstructor(const RenderCommandType itemType, const PropertyID propertyID,
											 IPropertyViewItem* (* constructor)(IProperty*, IView*));

		/*!
		 * \brief Register a block type, use the default creation method.
		 * \param itemType The item type.
		 */
		template<typename T> void setDefaultBlockConstructor(const BlockType itemType)
		{ setBlockConstructor(itemType, &defaultBlockConstructor<T>, T::registerPropertyItems); }

		/*!
		 * \brief Register a render command type, use the default creation method.
		 * \param itemType The item type.
		 */
		template<typename T> void setDefaultRenderCommandConstructor(const RenderCommandType itemType)
		{ setRenderCommandConstructor(itemType, &defaultRenderCommandConstructor<T>, T::registerPropertyItems); }

		/*!
		 * \brief Register a connection type, use the default creation method.
		 * \param itemType The item type.
		 */
		template<typename T> void setDefaultConnectionConstructor()
		{ setConnectionConstructor(&defaultConnectionConstructor<T>); }

		/*!
		 * \brief Register a property type, use the default creation method.
		 * \param propertyType The property type.
		 */
		template<typename T> void setDefaultItemConstructor(const PropertyType propertyType)
		{ setItemConstructor(propertyType, defaultItemConstructor<T>); }

		/*!
		 * \brief Register a special property id, use the default creation method.
		 * \param propertyID The property id.
		 */
		template<typename T> void setDefaultItemConstructor(const PropertyID propertyID)
		{ setItemConstructor(propertyID, defaultItemConstructor<T>); }

		/*!
		 * \brief Register a property type for a sepcial block type, use the default creation method.
		 * \param itemType The item type.
		 * \param propertyType The property type.
		 */
		template<typename T> void setDefaultBlockItemConstructor(const BlockType itemType,
																 const PropertyType propertyType)
		{ setBlockItemConstructor(itemType, propertyType, defaultItemConstructor<T>); }

		/*!
		 * \brief Register a property type for a special render command type, use the default creation method.
		 * \param itemType The item type.
		 * \param propertyType The property type.
		 */
		template<typename T> void setDefaultRenderCommandItemConstructor(const RenderCommandType itemType,
																		 const PropertyType propertyType)
		{ setRenderCommandItemConstructor(itemType, propertyType, defaultItemConstructor<T>); }

		/*!
		 * \brief Register a special property id for a sepcial block type, use the default creation method.
		 * \param itemType The item type.
		 * \param propertyID The property id.
		 */
		template<typename T> void setDefaultBlockItemConstructor(const BlockType itemType,
																 const PropertyID propertyID)
		{ setBlockItemConstructor(itemType, propertyID, defaultItemConstructor<T>); }

		/*!
		 * \brief Register a special property id for a special render command type, use the default creation method.
		 * \param itemType The item type.
		 * \param propertyID The property id.
		 */
		template<typename T> void setDefaultRenderCommandItemConstructor(const RenderCommandType itemType,
																		 const PropertyID propertyID)
		{ setRenderCommandItemConstructor(itemType, propertyID, defaultItemConstructor<T>); }

	protected:

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param block The item parameter.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		template<typename T>
		static PipelineItemPropertyView* defaultBlockConstructor(IBlock* block, IView* parentView)
		{ return new T(block, NULL, parentView); }

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param renderCommand The item parameter.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		template<typename T> static
		PipelineItemPropertyView* defaultRenderCommandConstructor(IRenderCommand* renderCommand, IView* parentView)
		{ return new T(renderCommand, NULL, parentView); }

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param connection The item parameter.
		 * \param parentView The parent item.
		 * \return The property view.
		 */
		template<typename T> static
		PipelineItemPropertyView* defaultConnectionConstructor(IConnection* connection, IView* parentView)
		{ return new T(connection, NULL, parentView); }

		/*!
		 * \brief Calls the constructor of the given type and passes the item parameter.
		 * \param property The item parameter.
		 * \param parentView The parent item.
		 * \return The property view item.
		 */
		template<typename T> static
		IPropertyViewItem* defaultItemConstructor(IProperty* property, IView* parentView)
		{ return new T(property, parentView); }

	private:

		//! \brief Contains all block constructors indexed by block type.
		QMap<BlockType, PipelineItemPropertyView* (*)(IBlock*, IView*)>
		_blockConstructors;

		//! \brief Contains all render command constructors indexed by render command type.
		QMap<RenderCommandType, PipelineItemPropertyView* (*)(IRenderCommand*, IView*)>
		_renderCommandConstructors;

		//! \brief The connection constructor.
		PipelineItemPropertyView* (*_connectionConstructor)(IConnection*, IView*);

		//! \brief Contains all default item constructors indexed by property type.
		QMap<PropertyType, IPropertyViewItem* (*)(IProperty*, IView*)>
		_defaultItemConstructors;

		//! \brief Contains all specialized default item constructors indexed by property type.
		QMap<PropertyID, IPropertyViewItem* (*)(IProperty*, IView*)>
		_specialDefaultItemConstructors;

		//! \brief Contains all block item constructors indexed by block type and property type.
		QMap<BlockType, QMap<PropertyType, IPropertyViewItem* (*)(IProperty*, IView*)>>
		_blockItemConstructors;

		//! \brief Contains all render command item constructors indexed by render command type and property type.
		QMap<RenderCommandType, QMap<PropertyType, IPropertyViewItem* (*)(IProperty*, IView*)>>
		_renderCommandItemConstructors;

		//! \brief Contains all specialized block item constructors indexed by block type and property type.
		QMap<BlockType, QMap<PropertyID, IPropertyViewItem* (*)(IProperty*, IView*)>>
		_specialBlockItemConstructors;

		//! \brief Contains all specialized render command item constructors indexed by render command type and property type.
		QMap<RenderCommandType, QMap<PropertyID, IPropertyViewItem* (*)(IProperty*, IView*)>>
		_specialRenderCommandItemConstructors;
	};

}

#endif // PROPERTYVIEWFACTORY_H
