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

#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

namespace ysm
{
	enum class BlockType
	{
		Undefined = -1, /** Indicates an erroneous type */

		// Main blocks
		Mixer = 100,
		Buffer,
		VertexPuller,
		VertexArrayObject,
		Display,
		TransformFeedback,
		TextureSampler,
		Texture,
		TextureView,
		FrameBufferObject,
		RenderBuffer,
		CodeGenerator,

		// Shader
		Shader_Vertex = 200,
		Shader_TessellationControl,
		Shader_TessellationEvaluation,
		Shader_Geometry,
		Shader_Fragment,

		// Data source blocks
		MeshGenerator = 1000,
		ModelLoader,
		ImageLoader,
		TextureLoader,
		Array,

		// Fixed function blocks
		Rasterization = 2000,
		FragmentTests,
		TessellationPrimitiveGenerator,

		// Uniform blocks
		Uniform_Int = 3000,
		Uniform_UInt,
		Uniform_Float,
		Uniform_Double,
		Uniform_Vec2,
		Uniform_Vec3,
		Uniform_Vec4,
		Uniform_Mat3x3,
		Uniform_Mat4x4,
		Uniform_ElapsedTime,

		// Miscellaneous blocks
		ModelViewProjection = 10000,
		CameraControl,
		LightSource,
		Material,

		Test = 0xFFFF, /** For testing purposes */
	};
}

#endif
