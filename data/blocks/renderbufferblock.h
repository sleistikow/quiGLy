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

#ifndef RENDERBUFFERBLOCK_H
#define RENDERBUFFERBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Render Buffer block
	 */
	class RenderBufferBlock : public Block
	{
		Q_OBJECT

	public:
		// Types
		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getInternalFormatNames();

	public:
		static const BlockType block_type{BlockType::RenderBuffer};

	public:
		// Construction
		explicit RenderBufferBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the sample count
		 */
		UIntProperty* getSamples();

		/**
		 * @brief Gets the internally used format
		 */
		EnumProperty* getInternalFormat();

		/**
		 * @brief Gets the width of the renderbuffer
		 */
		UIntProperty* getWidth();

		/**
		 * @brief Gets the height of the renderbuffer
		 */
		UIntProperty* getHeight();

		/**
		 * @brief Determines, whether to use size of the framebuffer
		 */
		BoolProperty* getAutoSize();

		// Port access
		/**
		 * @brief Gets the single in-port
		 */
		Port* getDataInPort();

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		// Properties
		UIntProperty* _samples{nullptr};
		EnumProperty* _internalFormat{nullptr};
		UIntProperty* _width{nullptr};
		UIntProperty* _height{nullptr};
		BoolProperty* _autoSize{nullptr};

		// Ports
		Port* _dataInPort{nullptr};
	};
}

#endif // RENDERBUFFERBLOCK_H
