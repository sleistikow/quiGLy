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

#ifndef PROPERTY_H
#define PROPERTY_H

#include <functional>

#include "data/common/serializationcontext.h"
#include "propertybase.h"
#include "data/common/utils.h"
#include "data/ipipelineitemprivate.h"

#include <QSet>

namespace ysm
{
	/// @brief Concrete implementation of a typeless traceable value for properties.
	template<typename T>
	class PropertyValue : public ITraceableValue
	{
	public:

		/**
		 * @brief Initialize new instance.
		 * @param value The represented value.
		 */
		PropertyValue(const T& value);

		/**
		 * @brief Copy new instance.
		 * @param value The source instance.
		 */
		PropertyValue(const PropertyValue& value);

		/// @brief Initialize new instance.
		PropertyValue() { }

		/**
		 * @brief Creates a new copy of this value.
		 * NOTE: The caller becomes owner of the returned instance.
		 * @return The newly created copy.
		 */
		ITraceableValue* cloneValue() const override;

		/**
		 * @brief Gets the represented value.
		 * @return The represented value.
		 */
		T getValue() const;

	private:

		/// @brief The represented value.
		T _value;
	};

	/**
	 * @brief Representing a single property
	 * Note: Pointer value types can be accessed by using the -> operator.
	 * Note: The traceable getUnchangedValue() only applies to writable properties.
	 *       Because of the possibility to delegate property values, it's possible that clearing one property's change
	 *       flag also clears another property's change flag. To avoid problems, always check all required flags before
	 *       clearing any of them.
	 * @arg T The property value type
	 * @arg P The property type
	 */
	template<typename T, PropertyType P>
	class Property : public PropertyBase
	{
	public:
		using base_type = Property<T, P>;
		using value_type = T;
		static const PropertyType property_type{P};

	private:
		using get_callback_delegate = std::function<const T&()>;
		using set_callback_delegate = std::function<void(const T&)>;
		using change_callback_delegate = std::function<bool(bool)>;

	public:
		// Construction
		explicit Property(const PropertyID id = PropertyID::None, const QString& name = "", bool isReadOnly = false);
		explicit Property(const Property<T, P>& prop);

		explicit Property(const T &val);

		template<typename Arg, typename... Args>
		explicit Property(const Arg& arg, const Args&... args);

	public:
		// Assignment
		Property& operator =(const Property<T, P>& prop);
		Property& operator =(const T& val);
		operator const T&() const;

		// Easy object access
		const T* operator ->() const;

		// Compare type and value
		bool operator ==(const Property<T,P>& prop) const;

	public:
		// Value access
		/**
		 * @brief Gets the property value
		 * @return The property value
		 */
		virtual const T& getValue() const;

		/**
		 * @brief Sets the property value
		 * @param val The new property value
		 */
		virtual void setValue(const T& val);

		/**
		 * @brief Called before the property is changed
		 * If the contents of the property have changed due to other means than using setValue or operator =, call this function to notify
		 * its owner about these changes.
		 */
		void valueChanging();

		/**
		 * @brief Called when the property has been changed
		 * If the contents of the property have changed due to other means than using setValue or operator =, call this function to notify
		 * its owner about these changes.
		 */
		void valueChanged();

		// Value delegating
		/**
		 * @brief Sets value delegates for getting and setting the property value
		 * Value delegates are used to forward the getValue and setValue functions to an outside function/object.
		 */
		void delegateValue(get_callback_delegate getter, set_callback_delegate setter, change_callback_delegate change);

		// Copying
		void copyProperty(const IProperty* source) override;
		IProperty* cloneProperty() const override;

		// Property flags
		bool isReadOnly() const override;
		void setReadOnly(const bool readOnly = true) override;

		// Typeless representation
		ITraceableValue* toTraceableValue() const override;
		void fromTraceableValue(const ITraceableValue* value) override;

		// String representation
		QString toString() const override;
		bool fromString(const QString& data) override;

	public: // IProperty
		// General attributes
		PropertyType getType() const override;
		PropertyID getID() const override;

		QString getName() const override;

		/**
		 * @brief Sets the name of this property
		 */
		void setName(const QString& name);

		bool isEqual(const IProperty* prop) const override;

		bool hasChanged() const override;
		void clearChanged() override;
		ITraceableValue* getUnchangedValue() const override;

	public: // ISerializable
		inline void serialize(QDomElement* xmlElement, SerializationContext* ctx) const override;
		inline void deserialize(const QDomElement* xmlElement, SerializationContext* ctx) override;

	protected:
		// Copy construction
		inline void copyConstructProperty(const Property<T, P>& prop);

	protected:
		PropertyType _type{P};
		PropertyID _id{PropertyID::None};
		QString _name;

	protected:
		T _value;
		bool _isReadOnly{false};
		bool _isInitialized{false};

		// Change tracing
		ITraceableValue* _unchangedValue{nullptr};
		bool _wasReadOnly{false};

	private:
		get_callback_delegate _getValueDelegate{nullptr};
		set_callback_delegate _setValueDelegate{nullptr};
		change_callback_delegate _hasChangedDelegate{nullptr};
	};

	// Template member functions

	template<typename T>
	PropertyValue<T>::PropertyValue(const T &value) : _value{value}
	{

	}

	template<typename T>
	PropertyValue<T>::PropertyValue(const PropertyValue &value) : _value{value._value}
	{

	}

	template<typename T>
	ITraceableValue* PropertyValue<T>::cloneValue() const
	{
		return new PropertyValue<T>(_value);
	}

	template<typename T>
	T PropertyValue<T>::getValue() const
	{
		return _value;
	}

	// Template member functions

	template<typename T, PropertyType P>
	Property<T,P>::Property(const PropertyID id, const QString& name, bool isReadOnly) : _id{id}, _name{name}, _isReadOnly{isReadOnly}
	{

	}

	template<typename T, PropertyType P>
	Property<T,P>::Property(const Property<T, P>& prop)
	{
		copyConstructProperty(prop);
	}

	template<typename T, PropertyType P>
	Property<T,P>::Property(const T& val) : _value{val}
	{

	}

	template<typename T, PropertyType P>
	const T& Property<T,P>::getValue() const
	{
		if (_getValueDelegate && _privateOwner)
			return _getValueDelegate();

		return _value;
	}

	template<typename T, PropertyType P>
	void Property<T,P>::setValue(const T& val)
	{
		valueChanging();

		if (_isReadOnly)
			throw std::runtime_error{"Trying to modify a read-only property"};

		if (_setValueDelegate && _privateOwner)
			_setValueDelegate(val);
		else
			_value = val;

		valueChanged();
	}

	template<typename T, PropertyType P>
	void Property<T,P>::valueChanging()
	{
		// Do not overwrite value until changedValue() is called
		if(!_unchangedValue && _isInitialized)
			_unchangedValue = toTraceableValue();
	}

	template<typename T, PropertyType P>
	void Property<T,P>::valueChanged()
	{
		_isInitialized = true;
		if (_privateOwner && !_isReadOnly)
		{
			if (!_privateOwner->isInPropertyUpdate())
				_privateOwner->applyPropertyChanges(this);
		}
	}

	template<typename T, PropertyType P>
	void Property<T,P>::delegateValue(get_callback_delegate getter, set_callback_delegate setter, change_callback_delegate change)
	{
		_getValueDelegate = getter;
		_setValueDelegate = setter;
		_hasChangedDelegate = change;
	}

	template<typename T, PropertyType P>
	void Property<T,P>::copyProperty(const IProperty* source)
	{
		if (_isReadOnly)
			throw std::runtime_error{"Trying to modify a read-only property"};

		const base_type* propSource = dynamic_cast<const base_type*>(source);

		if (!propSource)
			throw std::runtime_error{"Trying to copy from a property of a different type"};

		setValue(propSource->getValue());
	}

	template<typename T, PropertyType P>
	IProperty* Property<T,P>::cloneProperty() const
	{
		base_type* propClone = new base_type;

		if (!propClone)
			throw std::runtime_error{"Out of memory"};

		propClone->copyConstructProperty(*this);
		propClone->_privateOwner = nullptr;

		return propClone;
	}

	template<typename T, PropertyType P>
	bool Property<T,P>::isReadOnly() const
	{
		return _isReadOnly;
	}

	template<typename T, PropertyType P>
	void Property<T,P>::setReadOnly(const bool readOnly)
	{
		_isReadOnly = readOnly;
	}

	template<typename T, PropertyType P>
	Property<T,P>& Property<T,P>::operator =(const Property<T, P>& prop)
	{
		copyConstructProperty(prop);
		return *this;
	}

	template<typename T, PropertyType P>
	Property<T,P>& Property<T,P>::operator =(const T& val)
	{
		setValue(val);
		return *this;
	}

	template<typename T, PropertyType P>
	const T* Property<T,P>::operator ->() const
	{
		return &getValue();
	}

	template<typename T, PropertyType P>
	bool Property<T,P>::operator ==(const Property<T,P>& prop) const
	{
		return (_value == prop._value);
	}

	template<typename T, PropertyType P>
	PropertyType Property<T,P>::getType() const
	{
		return _type;
	}

	template<typename T, PropertyType P>
	PropertyID Property<T,P>::getID() const
	{
		return _id;
	}

	template<typename T, PropertyType P>
	QString Property<T,P>::getName() const
	{
		return _name;
	}

	template<typename T, PropertyType P>
	void Property<T,P>::setName(const QString& name)
	{
		_name = name;
	}

	template<typename T, PropertyType P>
	bool Property<T,P>::isEqual(const IProperty* prop) const
	{
		const base_type* propOther = dynamic_cast<const base_type*>(prop);

		if (propOther)
			return (propOther->_value == _value);

		return false;
	}

	template<typename T, PropertyType P>
	bool Property<T,P>::hasChanged() const
	{
		//Check if read only flag changed
		if(_isReadOnly != _wasReadOnly)
			return true;

		// Use change delegate if possible
		if(_hasChangedDelegate && _privateOwner)
			return _hasChangedDelegate(false);

		// Check if the stored value changed
		return _unchangedValue;
	}

	template<typename T, PropertyType P>
	void Property<T,P>::clearChanged()
	{
		// Use change delegate if possible
		if(_hasChangedDelegate && _privateOwner)
			_hasChangedDelegate(true);

		// Remove the reference to the unchanged value, it's not needed anymore
		delete _unchangedValue;
		_unchangedValue = nullptr;
		_wasReadOnly = _isReadOnly;
	}

	template<typename T, PropertyType P>
	ITraceableValue* Property<T,P>::getUnchangedValue() const
	{
		return _unchangedValue;
	}

	template<typename T, PropertyType P>
	ITraceableValue* Property<T,P>::toTraceableValue() const
	{
		// Caller owns the returned property value
		return new PropertyValue<T>(getValue());
	}

	template<typename T, PropertyType P>
	void Property<T,P>::fromTraceableValue(const ITraceableValue* value)
	{
		// Ensure property value type is correct
		const PropertyValue<T>* castedValue = dynamic_cast<const PropertyValue<T>*>(value);
		if(castedValue)
			setValue(castedValue->getValue());
	}

	template<typename T, PropertyType P>
	QString Property<T,P>::toString() const
	{
		// Default implementation
		return Utils::encodeEscapeCharacters(QString("%1").arg(getValue()));
	}

	template<typename T, PropertyType P>
	bool Property<T,P>::fromString(const QString& data)
	{
		// Default implementation
		T newVal = Utils::decodeEscapeCharacters(data);
		setValue(newVal);
		return true;
	}

	template<typename T, PropertyType P>
	Property<T,P>::operator const T&() const
	{
		return getValue();
	}

	template<typename T, PropertyType P>
	template<typename Arg, typename... Args>
	Property<T,P>::Property(const Arg& arg, const Args&... args) : _value{arg, std::forward<Args>(args)...}
	{

	}

	template<typename T, PropertyType P>
	void Property<T,P>::serialize(QDomElement* xmlElement, SerializationContext* ctx) const
	{
		xmlElement->setAttribute("id", static_cast<int>(_id));

		QString textRepr = toString();

		if (!textRepr.isEmpty())
		{
			QDomText data = ctx->createTextElement(textRepr);
			xmlElement->appendChild(data);
		}
	}

	template<typename T, PropertyType P>
	void Property<T,P>::deserialize(const QDomElement* xmlElement, SerializationContext* ctx)
	{
		Q_UNUSED(ctx);

		QString data = xmlElement->text();

		if (!data.isEmpty())
		{
			bool isReadOnly = _isReadOnly;

			setReadOnly(false);
			fromString(data);
			setReadOnly(isReadOnly);
		}
	}

	template<typename T, PropertyType P>
	void Property<T,P>::copyConstructProperty(const Property<T, P>& prop)
	{
		_id = prop._id;
		_name = prop._name;
		_isReadOnly = prop._isReadOnly;

		if (!_isReadOnly)
			setValue(prop.getValue());
	}
}

#include "standardproperties.h"

#endif
