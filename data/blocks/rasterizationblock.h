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

#ifndef RASTERIZATIONBLOCK_H
#define RASTERIZATIONBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	/**
	 * @brief Fixed-function Block representing the rasterization stage
	 */
	class RasterizationBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::Rasterization};

	public:
		// Types
		/**
		 * @brief The various cull face modes
		 */
		enum CullFaceMode
		{
			CullFace_Front = 0x01,
			CullFace_Back = 0x02,
			CullFace_Both = CullFace_Front|CullFace_Back,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getCullFaceModeNames();

		enum PolygonMode
		{
			PolyMode_Point,
			PolyMode_Line,
			PolyMode_Fill,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getPolygonModeNames();

		enum FrontFace
		{
			FrontFace_Clockwise,
			FrontFace_Counterclockwise,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getFrontFaceNames();

	public:
		// Construction
		explicit RasterizationBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the cull face mode
		 */
		EnumProperty* getCullFaceMode();
		BoolProperty* getCullingEnabled();
		EnumProperty* getPolygonMode();
		EnumProperty* getFrontFace();
		FloatProperty* getLineWidth();
		FloatProperty* getPointSize();
		BoolProperty* getLineAntialiasingEnabled();
		BoolProperty* getPointAntialiasingEnabled();

		// Port access
		/**
		 * @brief Gets the single in-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the single out-port
		 */
		Port* getGenericOutPort();

    public:
        bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		// Properties
		BoolProperty* _cullingEnabled{nullptr};
		EnumProperty* _cullFaceMode{nullptr};
		EnumProperty* _polygonMode{nullptr};
		FloatProperty* _lineWidth{nullptr};
		FloatProperty* _pointSize{nullptr};
		EnumProperty* _frontFace{nullptr};
		BoolProperty* _lineAntialiasingEnabled{nullptr};
		BoolProperty* _pointAntialiasingEnabled{nullptr};


		// Ports
		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
	};
}

#endif
