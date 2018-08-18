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

#ifndef PIPELINEITEM_H
#define PIPELINEITEM_H

#include "data/ipipelineitem.h"
#include "data/ipipelineitemprivate.h"
#include "data/properties/property.h"
#include "data/properties/propertylist.h"
#include "pipeline.h"
#include "pipelinemanager.h"

namespace ysm
{
	class PropertyList;

	/**
	 * @brief Represents a general pipeline item, derived from a specialized interface
	 * Override warnings might be shown in the IDE, due to generic base class.
	 */
	template<typename T>
	class PipelineItem : public IPipelineItemPrivate, public T
	{

	//Construction.
	public:

		explicit PipelineItem(Pipeline* parent, const QString& name = "");
		virtual ~PipelineItem();

	//Property access.
	public:

		PropertyList* getPropertyList();

		QVector<IProperty*> getProperties() const override;
		void deleteProperty(const IProperty* prop) override;
		void clearProperties() override;

		void beginPropertyUpdate() override;
		void endPropertyUpdate(bool applyChanges = true) override;

		bool isInPropertyUpdate() const override;
		void applyPropertyChanges(IProperty* prop = nullptr) override;

	//Pipeline item.
	public:

		void initPipelineItem() override;
		Pipeline* getPipeline() const override;

		PipelineItemID getID() const override;
		void setID(const PipelineItemID id) override;

		QString getName() const override;
		void setName(const QString& name) override;

		/**
		 * @brief Returns true, if the item can be renamed.
		 * Used for serialization. The name is only serialized, if it can be changed.
		 * @return True, if the name should be serialized.
		 */
		virtual bool canRename() const;

		PipelineItemStatus getStatus() const override;
		void setStatus(const PipelineItemStatus status, const QString& msg = "") override;
		bool isOverridingStatus() const override;

	//Versioning.
	public:

		unsigned int getVersion() const override;

	//Serializing.
	public:

		void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	protected:

		IProperty* getPropertyHelper(const PropertyID id) const override;
		IProperty* addPropertyHelper(const PropertyType type, const PropertyID id, const QString& name) override;

		/**
		 * @brief Creates all properties of this item
		 * Any existing properties will be removed when calling this function.
		 * Throws an exception when something goes wrong.
		 */
		virtual void createProperties();

	protected:
		Pipeline* _pipeline{nullptr};

		PipelineItemID _id{0};
		QString _name;

		PropertyList* _properties{nullptr};
		unsigned int _propertyUpdateCounter{0};		

		PipelineItemStatus _status{PipelineItemStatus::Healthy};
		StringProperty* _statusMessage{nullptr};
	};

	template<typename T>
	PipelineItem<T>::PipelineItem(Pipeline* parent, const QString& name) : _pipeline{parent}, _name{name}, _properties{new PropertyList{parent, this}}
	{
		if (!parent)
			throw std::invalid_argument{"parent may not be null"};
	}

	template<typename T>
	PipelineItem<T>::~PipelineItem()
	{
		delete _properties;
	}

	template<typename T>
	void PipelineItem<T>::initPipelineItem()
	{
		beginPropertyUpdate();
		createProperties();
		endPropertyUpdate(false);
	}

	template<typename T>
	bool PipelineItem<T>::isInPropertyUpdate() const
	{
		return (_propertyUpdateCounter > 0);
	}

	template<typename T>
	void PipelineItem<T>::applyPropertyChanges(IProperty* prop)
	{
		Q_UNUSED(prop);
	}

	template<typename T>
	PropertyList* PipelineItem<T>::getPropertyList()
	{
		return _properties;
	}

	template<typename T>
	void PipelineItem<T>::setID(const PipelineItemID id)
	{
		_id = id;

		// Update the highest item ID if necessary
		PipelineManager::setHighestItemID(id, true);
	}

	template<typename T>
	void PipelineItem<T>::setName(const QString& name)
	{
		_name = name;
	}

	template<typename T>
	bool PipelineItem<T>::canRename() const
	{
		return true;
	}

	template<typename T>
	void PipelineItem<T>::createProperties()
	{
		_properties->clear();

		// Message log property
		_statusMessage = _properties->newProperty<StringProperty>(PropertyID::MessageLog, "Status Message");
		_statusMessage->setSerializable(false);
	}

	template<typename T>
	void PipelineItem<T>::beginPropertyUpdate()
	{
		_propertyUpdateCounter += 1;
	}

	template<typename T>
	void PipelineItem<T>::endPropertyUpdate(bool applyChanges)
	{
		if (_propertyUpdateCounter > 0)
			_propertyUpdateCounter -= 1;

		if (_propertyUpdateCounter == 0)
		{
			if (applyChanges)
				applyPropertyChanges();
		}
	}

	template<typename T>
	Pipeline* PipelineItem<T>::getPipeline() const
	{
		return _pipeline;
	}

	template<typename T>
	PipelineItemID PipelineItem<T>::getID() const
	{
		return _id;
	}

	template<typename T>
	QString PipelineItem<T>::getName() const
	{
		return _name;
	}

	template<typename T>
	PipelineItemStatus PipelineItem<T>::getStatus() const
	{
		return _status;
	}

	template<typename T>
	void PipelineItem<T>::setStatus(const PipelineItemStatus status, const QString& msg)
	{
		_status = status;

		if (_status == PipelineItemStatus::Healthy)
		{
			if (!_statusMessage->getValue().isEmpty())
				*_statusMessage = "";
		}
		else
		{
			if (!msg.isEmpty())
				*_statusMessage = msg;
		}
	}

	template<typename T>
	bool PipelineItem<T>::isOverridingStatus() const
	{
		return false;
	}

	template<typename T>
	QVector<IProperty*> PipelineItem<T>::getProperties() const
	{
		return _properties->objects();
	}

	template<typename T>
	void PipelineItem<T>::deleteProperty(const IProperty* prop)
	{
		if (!prop)
			throw std::invalid_argument{"prop may not be null"};

		_properties->remove(prop);
	}

	template<typename T>
	void PipelineItem<T>::clearProperties()
	{
		_properties->clear();
	}

	template<typename T>
	unsigned int PipelineItem<T>::getVersion() const
	{
		return getPipeline()->getOpenGLVersion();
	}

	template<typename T>
	void PipelineItem<T>::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		if(canRename())
			xmlElement->setAttribute("name", _name);

		QDomElement elem = ctx->createElement("Properties");

		xmlElement->appendChild(elem);
		_properties->serialize(&elem, ctx);
	}

	template<typename T>
	void PipelineItem<T>::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		if(canRename())
			_name = xmlElement->attribute("name", _name);

		QDomElement elem = xmlElement->firstChildElement("Properties");

		if (!elem.isNull())
			_properties->deserialize(&elem, ctx);
	}

	template<typename T>
	IProperty* PipelineItem<T>::getPropertyHelper(const PropertyID id) const
	{
		return _properties->findProperty(id);
	}

	template<typename T>
	IProperty* PipelineItem<T>::addPropertyHelper(const PropertyType type, const PropertyID id, const QString& name)
	{
		if (type == PropertyType::Undefined)
			throw std::invalid_argument{"type may not be PropertyType::Undefined"};

		if (id == PropertyID::None)
			throw std::invalid_argument{"id may not be PropertyID::None"};

		return _properties->newProperty(id, type, name);
	}
}

#endif
