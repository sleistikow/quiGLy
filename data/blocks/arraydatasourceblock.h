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

#ifndef ARRAYDATASOURCE_H
#define ARRAYDATASOURCE_H

#include "uniforms/uniformbaseblock.h"

namespace ysm
{
	/**
	 * @brief Data source block for creating arrays of any desired size.
	 * This class is treated and implemented as UniformBaseBlock, because
	 * the data implementation does not differ.
	 * However, in UI it is displayed as pure Data source, since there
	 * are no ArrayUniforms in OpenGL.
	 */
	class ArrayDataSourceBlock : public UniformBaseBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Array};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getDataTypeNames();

	public:
		// Construction
		explicit ArrayDataSourceBlock(Pipeline* parent);

	public:
		// Port access
		/**
		 * @brief Gets the single out-port
		 */
		Port* getGenericOutPort();

		// Property access
		/**
		 * @brief Gets the element count stored in the array
		 */
		UIntProperty* getElementCount();

		/**
		 * @brief Gets the element count stored in the array
		 */
		UIntProperty* getByteCount();

		/**
		 * @brief Gets the datatype the array contains
		 */
		EnumProperty* getDataType();

		/**
		 * @brief Gets the data stored by the array block
		 */
		FloatDataProperty* getData();

	public:
		// Raw data functions
		unsigned int getOutputSize(IPort* port) const override;
		QByteArray retrieveUniformData(IPort* port) const override;

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createPorts() override;
		void createProperties() override;

	protected slots:
		void prepareConnection(Connection* con) override;

	private:
		// Ports
		Port* _outPort{nullptr};

		// Properties
		UIntProperty* _elementCount{nullptr};
		UIntProperty* _byteCount{nullptr};
		EnumProperty* _dataType{nullptr};
		FloatDataProperty* _data{nullptr};
	};
}

#endif // ARRAYDATASOURCE_H
