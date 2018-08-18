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

#ifndef DATASOURCEBLOCK_H
#define DATASOURCEBLOCK_H

#include "block.h"
#include "port.h"
#include "portlist.h"
#include "data/properties/property.h"
#include "data/properties/propertylist.h"
#include "data/types/datasource.h"
#include "data/common/utils.h"

namespace ysm
{
	/**
	 * @brief Base block for all kinds of data sources (model and texture loaders, geometry generators etc.)
	 */
	class DataSourceBlock : public Block
	{
		Q_OBJECT

	public:
		// Construction
		explicit DataSourceBlock(DataSource* dataSource, Pipeline* parent, BlockType type, const QString& name);
		virtual ~DataSourceBlock();

	public:
		// Initialization
		void initBlock() override;

		// Notifications
		void applyPropertyChanges(IProperty* prop) override;

		// Connections
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	public:
		// Helper functions
		/**
		 * @brief Finds the output type of the given port
		 * @return If the port doesn't exist, DataSource::NoOutput is returned
		 */
		DataSource::DataSourceOutput findOutputByPort(IPort* port);

		// Output functions
		/**
		 * @brief Retrieves the output data for output type @p output at @p index, applying any type conversions given in @p typeConv
		 * @return If no data could be retrieved, an empty QByteArray is returned
		 */
		virtual QByteArray retrieveOutput(DataSource::DataSourceOutput output, const TypeConversion::ConversionOptions& typeConv, const int index = -1) = 0;

		/**
		 * @brief Retrieves the number of elements for output type @p output
		 */
		virtual unsigned int retrieveOutputCount(DataSource::DataSourceOutput output) = 0;

		/**
		 * @brief Gets the size (in bytes) of the specified @p output (for a single element)
		 */
		unsigned int getOutputSize(DataSource::DataSourceOutput output, const TypeConversion::ConversionOptions* typeConversion = nullptr);				

	public:
		// Object access
		/**
		 * @brief Gets the data source object
		 */
		DataSource* getDataSource();

		// Property access
		/**
		 * @brief Gets the data outputs
		 */
		UIntProperty* getDataOutputs() const;

		/**
		 * @brief Gets the data property for the given output type (or null if none exists)
		 */
		const IProperty* getOutput(DataSource::DataSourceOutput output) const;

		// Port access
		/**
		 * @brief Gets the port for the given output type (or null if none exists)
		 */
		Port* getOutputPort(DataSource::DataSourceOutput output);

	public:
		// PipelineItem
		bool isOverridingStatus() const override;

	protected:
		void createProperties() override;

		/**
		 * @brief Reloads the data source (when a property has been changed, for example)
		 * If an error occurs, an exception is thrown.
		 */
		virtual void reloadDataSource() = 0;

		/**
		 * @brief Checks wether the cache key changed since the last call.
		 * @param clear If true, the change flag is cleared instead of checked.
		 * @return True, if changed.
		 */
		bool hasCacheKeyChanged(bool clear);

	protected:
		// Output units
		/**
		 * @brief Structure to save an output unit (property + port)
		 */
		struct OutputUnit
		{
			IProperty* property{nullptr};
			Port* port{nullptr};
			DataType outputType{DataType::NoType};
			unsigned int outputSize{0};
		};

		/**
		 * @brief Rebuilds all output units (property + port for an output)
		 * If something goes wrong, an exception is thrown.
		 */
		virtual void rebuildOutputUnits() = 0;

		/**
		 * @brief Rebuilds a single output unit (property + port for an output); deletes an existing unit if @p dataType isn't supported by the data source
		 * If an error occurs, an exception is thrown.
		 * @arg T The property type
		 * @arg F The property getter function type (automatically deduced)
		 */
		template<typename T, typename F>
		void rebuildOutputUnit(const DataSource::DataSourceOutput dataType, DataType outputType, const PropertyID propID,
							   const PortType portType, const QString& name, F getterGate);

		// Output retrieval
		/**
		 * @brief This function retrieves output from the given output unit, using @p typeConv for type conversions
		 * @param index If index is -1, the entire data is retrieved
		 */
		template<typename T>
		QByteArray retrieveOutput(const OutputUnit& outputUnit, const TypeConversion::ConversionOptions& typeConv, const int index);

		/**
		 * @brief This function retrieves the output element count from the given output unit
		 */
		template<typename T>
		unsigned int retrieveOutputCount(const OutputUnit& outputUnit);

	protected:
		DataSource* _dataSource{nullptr};
		QString _cacheKey;

		QMap<DataSource::DataSourceOutput, OutputUnit> _outputUnits;

		// Properties
		UIntProperty* _dataOutputs{nullptr};

	protected slots:
		void prepareConnection(Connection* con) override;
	};

	// Template member functions

	template<typename T, typename F>
	void DataSourceBlock::rebuildOutputUnit(const DataSource::DataSourceOutput dataType, DataType outputType, const PropertyID propID,
											const PortType portType, const QString& name, F getterGate)
	{
		if (_dataSource->hasOutputs(dataType))
		{
			// The spurce supports the given dataType, so add the corresponding output unit if necessary
			if (!_outputUnits.contains(dataType))
			{
				OutputUnit unit;

				T* prop = _properties->newProperty<T>(propID, name + " Data", true);
				prop->delegateValue(getterGate, nullptr,
									[this](bool clear)->bool { return hasCacheKeyChanged(clear); });
				prop->setSerializable(false);

				unit.property = prop;
				unit.port = _ports->newPort(portType, PortDirection::Out, name);
				unit.outputType = outputType;
				unit.outputSize = sizeof(typename T::value_type::value_type);

				_outputUnits[dataType] = unit;
			}
		}
		else
		{
			// The source doesn't support the given dataType, so remove any existing property and port
			if (_outputUnits.contains(dataType))
			{
				OutputUnit unit = _outputUnits[dataType];

				_outputUnits.remove(dataType);

				_properties->remove(unit.property);
				_ports->remove(unit.port);
			}
		}
	}

	template<typename T>
	QByteArray DataSourceBlock::retrieveOutput(const OutputUnit& outputUnit, const TypeConversion::ConversionOptions& typeConv, const int index)
	{
		const T* dataProp = dynamic_cast<const T*>(outputUnit.property);

		if (!dataProp)
			throw std::runtime_error{"The property could not be cast to the proper type"};

		QByteArray output;

		// Check if we need to convert types
		if (typeConv.targetType != DataType::NoType && typeConv.targetType != outputUnit.outputType && TypeConversion::canConvertBetweenTypes(outputUnit.outputType, typeConv.targetType))
		{
			const typename T::value_type* data = &dataProp->getValue();
			typename T::value_type tmpData;
			int dataIndex = index;

			if (index >= 0)
			{
				if (index >= dataProp->getValue().size())
					return QByteArray();

				// Add to our temporary vector and use that for conversion
				tmpData.append(dataProp->getValue()[index]);
				data = &tmpData;
				dataIndex = 0;
			}

			// Perform the actual conversions
			switch (typeConv.targetType)
			{
			case DataType::Vec2:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToVec2(*data), dataIndex, typeConv);
				break;

			case DataType::Vec3:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToVec3(*data), dataIndex, typeConv);
				break;

			case DataType::Vec4:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToVec4(*data), dataIndex, typeConv);
				break;

			case DataType::Int:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToInt(*data), dataIndex, typeConv);
				break;

			case DataType::UInt:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToUInt(*data), dataIndex, typeConv);
				break;

			case DataType::Float:
				output = TypeConversion::convertVectorToByteArray(TypeConversion::convertToFloat(*data), dataIndex, typeConv);
				break;

			default:
				break;
			}
		}
		else
			output = TypeConversion::convertVectorToByteArray(dataProp->getValue(), index, typeConv);

		return output;
	}

	template<typename T>
	unsigned int DataSourceBlock::retrieveOutputCount(const OutputUnit& outputUnit)
	{
		const T* dataProp = dynamic_cast<const T*>(outputUnit.property);

		if (!dataProp)
			throw std::runtime_error{"The property could not be cast to the proper type"};

		return dataProp->getValue().size();
	}
}

#endif
