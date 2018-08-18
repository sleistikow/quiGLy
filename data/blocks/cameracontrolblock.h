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

#ifndef CAMERACONTROLBLOCK_H
#define CAMERACONTROLBLOCK_H

#include "data/blocks/block.h"
#include "data/properties/property.h"
#include "data/cache/icacheable.h"

namespace ysm
{
	class CameraControlBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::CameraControl};

	public:
		// Construction
		explicit CameraControlBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the current tranlation speed
		 */
		FloatProperty* getTranslationSpeed();

		/**
		 * @brief Gets the current rotation speed
		 */
		FloatProperty* getRotationSpeed();

		// Port access
		/**
		 * @brief Gets the single out-port
		 */
		Port* getGenericOutPort();

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createPorts() override;
		void createProperties() override;

	protected:
		// Properties
		FloatProperty* _translationSpeed{nullptr};
		FloatProperty* _rotationSpeed{nullptr};

		// Ports
		Port* _outPort{nullptr};
	};
}

#endif // CAMERACONTROLBLOCK_H
