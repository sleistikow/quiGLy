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

#ifndef TRANSFORMFEEDBACKBLOCK_H
#define TRANSFORMFEEDBACKBLOCK_H

#include "block.h"
#include "data/properties/property.h"

namespace ysm
{
	class VaryingsProperty;

	class TransformFeedbackBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::TransformFeedback};
		explicit TransformFeedbackBlock(Pipeline* parent);

		enum BufferMode
		{
			BufferMode_Interleaved,
			BufferMode_Separate,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getBufferModeNames();

		EnumProperty* getBufferMode();
		VaryingsProperty* getVaryings();

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

    public:
        bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	protected slots:
		void prepareConnection(Connection* con) override;

	private:
		EnumProperty* _bufferMode{nullptr};
		VaryingsProperty* _varyings{nullptr};

		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
	};
}
#endif // TRANSFORMFEEDBACKBLOCK_H
