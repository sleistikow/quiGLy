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

#ifndef MODELVIEWPROJECTIONBLOCK_H
#define MODELVIEWPROJECTIONBLOCK_H

#include "uniformbaseblock.h"

namespace ysm
{
	/**
	 * @brief Special block for MVP matrices
	 */
	class ModelViewProjectionBlock : public UniformBaseBlock
	{
		Q_OBJECT

	public:
		static const BlockType block_type{BlockType::ModelViewProjection};

	public:
		explicit ModelViewProjectionBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the model position
		 */
		Vec3Property* getModelPosition();

		/**
		 * @brief Gets the model rotation axis
		 */
		Vec3Property* getModelRotationAxis();

		/**
		 * @brief Gets the model rotation angle
		 */
		FloatProperty* getModelRotationAngle();

		/**
		 * @brief Gets the model scaling
		 */
		Vec3Property* getModelScaling();

		/**
		 * @brief Gets the camera target
		 */
		Vec3Property* getCameraTarget();

		/**
		 * @brief Gets the camera center
		 */
		Vec3Property* getCameraCenter();

		/**
		 * @brief Gets the camera up vector
		 */
		Vec3Property* getCameraUpVector();

		/**
		 * @brief Gets the projection vertical angle
		 */
		FloatProperty* getProjVerticalAngle();

		/**
		 * @brief Gets the projection aspect ratio
		 */
		FloatProperty* getProjAspectRatio();

		/**
		 * @brief Gets the projection near plane
		 */
		FloatProperty* getProjNear();

		/**
		 * @brief Gets the projection far plane
		 */
		FloatProperty* getProjFar();

		/**
		 * @brief Gets the model matrix
		 */
		Mat4x4Property* getMatM();

		/**
		 * @brief Gets the view matrix
		 */
		Mat4x4Property* getMatV();

		/**
		 * @brief Gets the projection matrix
		 */
		Mat4x4Property* getMatP();

		/**
		 * @brief Gets the model-view matrix
		 */
		Mat4x4Property* getMatMV();

		/**
		 * @brief Gets the model-view-projection matrix
		 */
		Mat4x4Property* getMatMVP();

		/**
		 * @brief Gets the normal matrix
		 */
		Mat3x3Property* getMatNormal();

		// Port access
		/**
		 * @brief Gets the camera control port
		 */
		Port* getGenericInPort();

		/**
		 * @brief Gets the model matrix port
		 */
		Port* getMatMPort();

		/**
		 * @brief Gets the view matrix port
		 */
		Port* getMatVPort();

		/**
		 * @brief Gets the projection matrix port
		 */
		Port* getMatPPort();

		/**
		 * @brief Gets the model-view matrix port
		 */
		Port* getMatMVPort();

		/**
		 * @brief Gets the model-view-projection matrix port
		 */
		Port* getMatMVPPort();

		/**
		 * @brief Gets the normal matrix port
		 */
		Port* getMatNormalPort();

	public:
		// Raw data functions
		unsigned int getOutputSize(IPort* port) const override;
		QByteArray retrieveUniformData(IPort* port) const override;

	public:
		void applyPropertyChanges(IProperty* prop) override;

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		void updateMatrices();

	private:
		// Properties
		Vec3Property* _modelPosition{nullptr};
		Vec3Property* _modelRotationAxis{nullptr};
		FloatProperty* _modelRotationAngle{nullptr};
		Vec3Property* _modelScaling{nullptr};

		Vec3Property* _cameraPosition{nullptr};
		Vec3Property* _cameraTarget{nullptr};
		Vec3Property* _cameraUpVector{nullptr};

		FloatProperty* _projVerticalAngle{nullptr};
		FloatProperty* _projAspectRatio{nullptr};
		FloatProperty* _projNear{nullptr};
		FloatProperty* _projFar{nullptr};

		Mat4x4Property* _matM{nullptr};
		Mat4x4Property* _matV{nullptr};
		Mat4x4Property* _matP{nullptr};
		Mat4x4Property* _matMV{nullptr};
		Mat4x4Property* _matMVP{nullptr};
		Mat3x3Property* _matNormal{nullptr};

		// Ports
		Port* _inPort{nullptr};
		Port* _matMPort{nullptr};
		Port* _matVPort{nullptr};
		Port* _matPPort{nullptr};
		Port* _matMVPort{nullptr};
		Port* _matMVPPort{nullptr};
		Port* _matNormalPort{nullptr};
	};
}

#endif
