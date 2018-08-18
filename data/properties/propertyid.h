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

#ifndef PROPERTYID_H
#define PROPERTYID_H

namespace ysm
{
	enum class PropertyID
	{
		None = -1, /** Invalid property */
		Default = 0,

		// General properties
		MessageLog = 1,

		// Mixers
		Mixer_Layout = 100,

		// VAO
		Vao_Layout = 200,

		// Buffers
		Buffer_Data = 300,
		Buffer_Size,
		Buffer_UsageFrequency,
		Buffer_UsageAccess,
		Buffer_Binding,
		Buffer_Name,
		Buffer_EntryCount,

		// Data
		Data_Outputs = 400,
		Data_OutputType,
		Data_VertexPositions,
		Data_VertexNormals,
		Data_VertexTangents,
		Data_VertexBitangents,
		Data_VertexColors,
		Data_TextureCoordinates,
		Data_IndexList,
		Data_TexelColors,

		// Shader
		Shader_Code = 500,
		Shader_Type,

		// Vertex Puller
		VertexPuller_RestartIndex = 600,
		VertexPuller_ElementCount,

		// Uniforms
		Uniform_Name = 700,
		Uniform_Value,
		Uniform_Location,
		Uniform_ExplicitLocation,

		// Tessellation Primitive Generator
		TessellationPrimitiveGenerator_PatchVertices = 800,
		TessellationPrimitiveGenerator_PatchDefaultInnerLevel,
		TessellationPrimitiveGenerator_PatchDefaultOuterLevel,

		// FrameBufferObject
		FrameBufferObject_Attachment = 900,
		FrameBufferObject_ColorIndex,
		FrameBufferObject_CubeMapSide,
		FrameBufferObject_TextureLevel,
		FrameBufferObject_TextureLayer,
		FrameBufferObject_AllLayers,

		// RenderBuffer
		RenderBuffer_Samples = 1000,
		RenderBuffer_InternalFormat,
		RenderBuffer_Width,
		RenderBuffer_Height,
		RenderBuffer_ViewPortAutoSize,

		// ModelViewProjection
		MVP_ModelPosition = 1100,
		MVP_ModelRotationAxis,
		MVP_ModelRotationAngle,
		MVP_ModelScaling,
		MVP_CameraPosition,
		MVP_CameraUpVector,
		MVP_CameraTarget,
		MVP_ProjVerticalAngle,
		MVP_ProjAspectRation,
		MVP_ProjNear,
		MVP_ProjFar,
		MVP_MatM,
		MVP_MatV,
		MVP_MatP,
		MVP_MatMV,
		MVP_MatMVP,
		MVP_MatNormal,

		//Light Sources
		Light_Position = 1200,
		Light_Direction,
		Light_SpotExponent,
		Light_SpotAngle,
		Light_ColorAmbient,
		Light_ColorDiffuse,
		Light_ColorSpecular,
		Light_AttenuationConstant,
		Light_AttenuationLinear,
		Light_AttenuationQuadratic,

		//Materials
		Material_Ambient = 1300,
		Material_Diffuse,
		Material_Specular,
		Material_Shininess,


		// 2D Geometry
		Mesh_Type = 10000,
		Mesh_Center,
		Mesh_Vertex1,
		Mesh_Vertex2,
		Mesh_Vertex3,
		Mesh_Width,
		Mesh_Height,
		Mesh_Depth,
		Mesh_Radius,
		Mesh_Sections,
		Mesh_VertexCount,
		Mesh_ElementCount,
		Mesh_InterpolateNormals,

		// Textures, Texture Views, Samplers & Images
		Img_FileName = 10100,
		Img_Size,
		Img_Grid,
		Img_Cells,
		Img_PixelDataFormat,
		Img_PixelDataType,
		Sampler_Type,
		Texture_FileName,
		Texture_Texture,
		Texture_Unit,
		Texture_Mipmaps,
		Texture_TargetType,
		Texture_Binding,
		Texture_Name,
		Texture_BaseLevel,
		Texture_MaxLevel,
		Texture_SwizzleRed,
		Texture_SwizzleGreen,
		Texture_SwizzleBlue,
		Texture_SwizzleAlpha,
		Texture_DepthStencilMode,
		Texture_Width,
		Texture_Height,
		Texture_RenderBufferAutoSize,
		TextureBase_MinFilter,
		TextureBase_MagFilter,
		TextureBase_MinLOD,
		TextureBase_MaxLOD,
		TextureBase_WrapModeS,
		TextureBase_WrapModeT,
		TextureBase_WrapModeR,
		TextureBase_Anisotropy,
		TextureBase_BorderColor,
		TextureBase_CompareMode,
		TextureBase_CompareFunc,
		TextureBase_InternalFormat,
		TextureView_MinimumLevel,
		TextureView_LevelCount,
		TextureView_MinimumLayer,
		TextureView_LayerCount,

		// Models
		Model_FileName = 10200,
		Model_MeshCount,
		Model_MeshNames,
		Model_CombineMeshes,
		Model_ActiveMesh,
		Model_ActiveMeshOutputs,
		Model_ActiveMeshVertexCount,
		Model_ActiveMeshElementCount,

		// Array
		Array_ElementCount,
		Array_DataType,
		Array_Data,
		Array_Byte,

		// Rasterization
		Rasterization_CullFaceMode = 11000,
		Rasterization_EnableCulling,
		Rasterization_PolygonMode,
		Rasterization_FrontFace,
		Rasterization_LineWidth,
		Rasterization_PointSize,
		Rasterization_EnableLineAntialiasing,
		Rasterization_EnablePointAntialiasing,

		// Fragment Tests
		FragmentTests_DepthFunc = 12000,
		FragmentTests_DepthTest,
		FragmentTests_DepthMask,
		FragmentTests_StencilFuncFront,
		FragmentTests_StencilFuncBack,
		FragmentTests_StencilMask,
		FragmentTests_StencilRef,
		FragmentTests_StencilOpSFailFront,
		FragmentTests_StencilOpDpFailFront,
		FragmentTests_StencilOpDpPassFront,
		FragmentTests_StencilOpSFailBack,
		FragmentTests_StencilOpDpFailBack,
		FragmentTests_StencilOpDpPassBack,
		FragmentTests_Blending,
		FragmentTests_BlendFuncSourceColor,
		FragmentTests_BlendFuncSourceAlpha,
		FragmentTests_BlendFuncDestinationColor,
		FragmentTests_BlendFuncDestinationAlpha,
		FragmentTests_BlendEquation,
		FragmentTests_StencilTest,
		FragmentTests_ScissorTest,
		FragmentTests_LowerLeftPosition,
		FragmentTests_RectangleHeight,
		FragmentTests_RectangleWidth,
		FragmentTests_ColorMaskRed,
		FragmentTests_ColorMaskGreen,
		FragmentTests_ColorMaskBlue,
		FragmentTests_ColorMaskAlpha,

		// Buffer Clearing
		Clear_ColorEnabled = 13000,
		Clear_Color,
		Clear_DepthEnabled,
		Clear_Depth,
		Clear_StencilEnabled,
		Clear_Stencil,
		Clear_ViewportLowerLeftCorner,
		Clear_ViewportWidth,
		Clear_ViewportHeight,
		Clear_ViewportAutoSize,

		// Drawing commands
		Draw_DrawMode = 14000,
		Draw_PrimitiveMode,
		Draw_ElementCount,
		Draw_FirstIndex,
		Draw_Instanced,
		Draw_InstanceCount,
		Draw_AutoElementCount,

		// Transform Feedback
		TransformFeedback_BufferMode = 15000,
		TransformFeedback_Index,

		// Display
		Display_Visible = 16000,
		Display_MultiSample,
		Display_Samples,
		Display_GammaCorrection,

		// Code Generator
		CodeGenerator_Language = 17000,
		CodeGenerator_API,
		CodeGenerator_FileName,

		// Camera Control
		CameraControl_RotationSpeed = 18000,
		CameraControl_TranslationSpeed,

		// Varyings //TODO: split into SSBO, TFB
		Varyings = 19000,

		// User properties
		User1 = 99000,
		User2,
		User3,
		User4,
		User5,
		User6,
		User7,
		User8,
		User9,
		User10,
		User11,
		User12,
		User13,
		User14,
		User15,
		User16,
		User17,
		User18,
		User19,
		User20,
	};
}

#endif
