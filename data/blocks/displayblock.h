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

#ifndef DISPLAYBLOCK_H
#define DISPLAYBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Block representing the very final stage of everything - connects a fragment shader to the OpenGL preview window
	 */
	class DisplayBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Display};

	public:
		// Construction
		explicit DisplayBlock(Pipeline* parent);

	public:
        // Property access
        /**
         * @brief Gets the visibility
         */
        BoolProperty* getVisibility();

		/**
		 * @brief Gets the visibility
		 */
		BoolProperty* getMultiSample();

		/**
		 * @brief Gets the visibility
		 */
		UIntProperty* getSamples();


		// Port access
		/**
		 * @brief Gets the single in-port
		 */
		Port* getGenericInPort();

    protected:
        void createProperties() override;
        void createPorts() override;

    private:
        // Properties
		BoolProperty* _visible{nullptr};
		BoolProperty* _multiSample{nullptr};
		BoolProperty* _gammaCorrection{nullptr};
		UIntProperty* _samples{nullptr};

		// Ports
		Port* _inPort{nullptr};			
	};
}

#endif
