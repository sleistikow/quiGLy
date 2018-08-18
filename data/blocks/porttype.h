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

#ifndef PORTTYPE_H
#define PORTTYPE_H

namespace ysm
{
	enum class PortType
	{
		Undefined = -1, /** Indicates an erroneous type */

		// Data ports
		Data_In = 100,
		Data_Out,
		Data_VertexPositions,
		Data_VertexNormals,
		Data_VertexTangents,
		Data_VertexBitangents,
		Data_VertexColors,
		Data_TextureCoordinates,
		Data_IndexList,
		Data_TexelColors,

		// Vertex Puller ports
		VertexPuller_IndexList = 1000,

		// Shader ports
		Shader_Texture = 2000,
		Shader_Uniform,
		Shader_TransformFeedback,
		Shader_UBO,
		Shader_SSBOIn,
		Shader_SSBOOut,
		Shader_AtomicCounterIn,

		// Textures & Texture Views
		Texture_Sampler = 3000,
		Texture_FrameBuffer,
		TextureView_Texture,

		// FrameBufferObject ports
		FrameBufferObject_RenderBuffer = 4000,
		FrameBufferObject_Texture,

		// ModelViewProjection ports
		MVP_MatM = 5000,
		MVP_MatV,
		MVP_MatP,
		MVP_MatMV,
		MVP_MatMVP,
		MVP_MatNormal,

		//Light Source ports
		Light_Position,
		Light_Direction,
		Light_SpotExponent,
		Light_SpotAngle,
		Light_ColorAmbient,
		Light_ColorDiffuse,
		Light_ColorSpecular,
		Light_AttenuationConstant,
		Light_AttenuationLinear,
		Light_AttenuationQuadratic,

		//Material Ports
		Material_Ambient,
		Material_Diffuse,
		Material_Specular,
		Material_Shininess,


		// Generic ports
		GenericIn = 50000,
		GenericOut,		

		Test = 0xFFFF, /** For testing purposes */
	};

	enum class PortDirection
	{
		Dry = 0x00, /** Port doesn't accept anything; usually indicating an erroneous port */
		In = 0x01,
		Out = 0x02,
		Both = In|Out,
	};
}

#endif
