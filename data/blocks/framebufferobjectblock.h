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

#ifndef FRAMEBUFFEROBJECTBLOCK_H
#define FRAMEBUFFEROBJECTBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief FBO block
	 */
	class FrameBufferObjectBlock : public Block
	{
		Q_OBJECT

	public:
		// Types
		/**
		 * @brief The texture Target type
		 */
		enum AccessType
		{
			Access_Read,
			Access_Draw,
			Access_Both,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getAccessTypeNames();

		/**
		 * @brief The texture Target type
		 */
		enum AttachmentType
		{
			Attachment_Color0,
			Attachment_Depth,
			Attachment_Stencil,
			Attachment_DepthStencil,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getAttachmentTypeNames();

	public:
		static const BlockType block_type{BlockType::FrameBufferObject};

	public:
		// Construction
		explicit FrameBufferObjectBlock(Pipeline* parent);

	public:
		// Property access
		// TODO: Map out location to connection

		// Port access
		/**
		 * @brief Gets the generic in-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the render buffer out port
		 */
		Port* getRenderBufferPort();

		/**
		 * @brief Gets the texture out port
		 */
		Port* getTexturePort();

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	protected slots:
		void prepareConnection(Connection* con) override;

	private:
		// Properties
		// TODO: Map out location to connection

		// Ports
		Port* _inPort{nullptr};
		Port* _renderBufferPort{nullptr};
		Port* _texturePort{nullptr};
	};
}

#endif // FRAMEBUFFEROBJECTBLOCK_H
