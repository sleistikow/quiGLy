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

#ifndef VERTEXPULLERBLOCK_H
#define VERTEXPULLERBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Configurable block representing the vertex puller stage.
	 */
	class VertexPullerBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::VertexPuller};
		/**
			 * @param The owning Pipeline
			 */
		explicit VertexPullerBlock(Pipeline* parent);

		UIntProperty* getRestartIndex();

		/**
		 * @brief Returns the calculated number of elements in the VAO
		 */
		UIntProperty* getElementCount() const;

		/**
		 * @brief Gets the single in-port
		 * @return In-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the single out-port
		 * @return Out-port
		 */
		Port* getGenericOutPort();

		/**
		 * @brief Gets the port which is possibly connected to an IBO
		 */
		Port* getIboPort();

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		unsigned int calculateElementCount() const;

	private:
		// Properties
		UIntProperty* _restartIndex{nullptr};
		UIntProperty* _elementCount{nullptr};

		// Ports
		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
		Port* _iboPort{nullptr};
	};
}
#endif
