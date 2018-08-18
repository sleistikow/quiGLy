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

#ifndef VERTEXARRAYOBJECTBLOCK_H
#define VERTEXARRAYOBJECTBLOCK_H

#include "block.h"
namespace ysm
{
	class VaoLayoutProperty;	

	/**
	 * @brief A block representing the VAO configuration
	 */
	class VertexArrayObjectBlock : public Block
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::VertexArrayObject};

	public:
		// Construction
		explicit VertexArrayObjectBlock(Pipeline* parent);

	public:
		void initBlock() override;		

	public:
		// Object access
		/**
		 * @brief Gets the VAO layout
		 */
		VaoLayoutProperty* getVaoLayout();		

		/**
		 * @brief Gets the generic in-port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the generic out-port
		 */
		Port* getGenericOutPort();		

	public:
		bool canAcceptConnection(IPort* src, IPort* dest, QString& denialReason) override;

	protected:
		void createProperties() override;
		void createPorts() override;	

	private slots:
		void onConnectionRemoved(Connection* con);

	private:
		VaoLayoutProperty* _vaoLayout{nullptr};		

		Port* _inPort{nullptr};
		Port* _outPort{nullptr};
	};
}
#endif
