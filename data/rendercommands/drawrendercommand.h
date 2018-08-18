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

#ifndef DRAWRENDERCOMMAND_H
#define DRAWRENDERCOMMAND_H

#include "rendercommand.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Render command for drawing
	 */
	class DrawRenderCommand : public RenderCommand
	{
		Q_OBJECT

	public:
		// Types
		enum DrawMode
		{
			DrawMode_Arrays,
			DrawMode_Elements,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getDrawModeNames();

		enum PrimitiveMode
		{
			PrimitiveMode_Points,
			PrimitiveMode_Lines,
			PrimitiveMode_LinesAdjacency,
			PrimitiveMode_LineStrip,
			PrimitiveMode_LineStripAdjacency,
			PrimitiveMode_LineLoop,
			PrimitiveMode_Triangles,
			PrimitiveMode_TrianglesAdjacency,
			PrimitiveMode_TriangleStrip,
			PrimitiveMode_TriangleStripAdjacency,
			PrimitiveMode_TriangleFan,
			PrimitiveMode_Patches,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getPrimitiveModeNames();

	public:
		static const RenderCommandType command_type{RenderCommandType::Draw};

	public:
		// Construction
		explicit DrawRenderCommand(Pipeline* parent);

	public:
		// Property access

		/**
		 * @brief Gets the drawing mode (which glDraw* function should be used)
		 */
		EnumProperty* getDrawMode();

		/**
		 * @brief Gets the primitive mode
		 */
		EnumProperty* getPrimitiveMode();

		/**
		 * @brief Determines whether to auto detect element count
		 */
		BoolProperty* getAutoElementCount();

		/**
		 * @brief Gets the element/vertex count
		 */
		UIntProperty* getElementCount();

		/**
		 * @brief Gets the starting index/offset
		 */
		UIntProperty* getFirstIndex();

		/**
		 * @brief Determines whether to draw instanced
		 */
		BoolProperty* getInstanced();

		/**
		 * @brief Gets the instance count to be drawn
		 */
		UIntProperty* getInstanceCount();

	public:
		bool canAcceptBlockAssignment(IBlock* block, QString& denialReason) override;

	protected:
		void createProperties() override;

	private:
		// Properties
		EnumProperty* _drawMode{nullptr};
		EnumProperty* _primitiveMode{nullptr};
		BoolProperty* _autoElementCount{nullptr};
		UIntProperty* _elementCount{nullptr};
		UIntProperty* _firstIndex{nullptr};
		BoolProperty* _instanced{nullptr};
		UIntProperty* _instanceCount{nullptr};
	};
}

#endif
