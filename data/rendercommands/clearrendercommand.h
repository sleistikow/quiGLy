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

#ifndef CLEARRENDERCOMMAND_H
#define CLEARRENDERCOMMAND_H

#include "rendercommand.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Render command for buffer clearing
	 */
	class ClearRenderCommand : public RenderCommand
	{
		Q_OBJECT

	public:
		static const RenderCommandType command_type{RenderCommandType::Clear};

	public:
		// Construction
		explicit ClearRenderCommand(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets whether the color buffer should be cleared
		 */
		BoolProperty* getClearColorEnabled();

		/**
		 * @brief Gets the clear color
		 */
		ColorProperty* getClearColor();

		/**
		 * @brief Gets whether the depth buffer should be cleared
		 */
		BoolProperty* getClearDepthEnabled();

		/**
		 * @brief Gets the clear depth
		 */
		DoubleProperty* getClearDepth();

		/**
		 * @brief Gets whether the stencil buffer should be cleared
		 */
		BoolProperty* getClearStencilEnabled();

		/**
		 * @brief Gets the clear stencil value
		 */
		IntProperty* getClearStencil();

		/**
		 * @brief Gets the clear stencil value
		 */
		Vec2Property* getLowerLeftCorner();

		/**
		 * @brief Gets the clear stencil value
		 */
		UIntProperty* getWidth();

		/**
		 * @brief Gets the clear stencil value
		 */
		UIntProperty* getHeight();

		/**
		 * @brief Determines, whether to use size of the framebuffer
		 */
		BoolProperty* getAutoSize();

	public:
		bool canAcceptBlockAssignment(IBlock* block, QString& denialReason) override;

	protected:
		void createProperties() override;

	private:
		BoolProperty* _clearColorEnabled{nullptr};
		ColorProperty* _clearColor{nullptr};

		BoolProperty* _clearDepthEnabled{nullptr};
		DoubleProperty* _clearDepth{nullptr};

		BoolProperty* _clearStencilEnabled{nullptr};
		IntProperty* _clearStencil{nullptr};

		//Viewport settings
		Vec2Property* _lowerLeftCorner{nullptr};
		UIntProperty* _width{nullptr};
		UIntProperty* _height{nullptr};
		BoolProperty* _autoSize{nullptr};

	};
}

#endif
