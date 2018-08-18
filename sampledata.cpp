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

#include "sampledata.h"

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/add/addblockcommand.h"
#include "commands/pipeline/add/addconnectioncommand.h"
#include "commands/pipeline/add/addrendercommandcommand.h"
#include "commands/pipeline/change/updatepropertiescommand.h"

#include "views/document.h"

#include "data/ipipeline.h"
#include "data/iblock.h"
#include "data/iproperty.h"
#include "data/iconnection.h"
#include "data/irendercommand.h"

#include "data/properties/property.h"
#include "data/properties/mixerlayoutproperty.h"
#include "data/properties/vaolayoutproperty.h"
#include "data/properties/filenameproperty.h"
#include "data/properties/glsldocumentlistproperty.h"

#include "data/blocks/texturebaseblock.h"
#include "data/blocks/meshgeneratorblock.h"
#include "data/rendercommands/drawrendercommand.h"

#include <QOpenGLFunctions>

using namespace ysm;

//Create a block.
#define BLOCK(name, type) \
	AddBlockCommand* name##Command = new AddBlockCommand(pipeline, BlockType::type); \
	commandQueue->execute(name##Command); \
	IBlock* name = name##Command->getBlock(); \
	Q_UNUSED(name);

//Create a render command.
#define COMMAND(name, type, block) \
	AddRenderCommandCommand* name##Command = new AddRenderCommandCommand(block, RenderCommandType::type); \
	commandQueue->execute(name##Command); \
	IRenderCommand* name = name##Command->getRenderCommand(); \
	Q_UNUSED(name);

//Begin property update.
#define BEGIN_PROPERTY_UPDATE(name) \
	IPipelineItem* item = name; \
	UpdatePropertiesCommand* updateCommand = new UpdatePropertiesCommand(item);

//Set a property value.
#define PROPERTY(id, type, value) \
	updateCommand->addOperation(item->getProperty<type##Property>(PropertyID::id), value);

//End property update.
#define END_PROPERTY_UPDATE \
	commandQueue->execute(updateCommand);

//Add a connection.
#define CONNECT(name, sourceName, sourcePort, targetName, targetPort) \
	IPort* name##Source = sourceName->getPort(PortType::sourcePort); \
	IPort* name##Target = targetName->getPort(PortType::targetPort); \
	AddConnectionCommand* name##Command = new AddConnectionCommand(name##Source, name##Target); \
	commandQueue->execute(name##Command); \
	IConnection* name = name##Command->getConnection(); \
	Q_UNUSED(name);

const QString vertexShaderSourceA = "\
#version 150 core\n\
#extension GL_ARB_explicit_attrib_location: enable\n\
\n\
out vec2 uv;\n\
out float z;\n\
\n\
mat4 calculateMatrix();\n\
\n\
void main()\n\
{\n\
	uv = TextureCoordinates.xy;\n\
	gl_Position = calculateMatrix() * vec4(VertexPositions, 1);\n\
	z = gl_Position.z;\n\
}\n\
";

const QString vertexShaderSourceB = "\
#version 150 core\n\
#extension GL_ARB_explicit_attrib_location: enable\n\
\n\
mat4 rotationMatrix(vec3 axis, float angle)\n\
{\n\
	axis = normalize(axis);\n\
	float s = sin(angle);\n\
	float c = cos(angle);\n\
	float oc = 1.0 - c;\n\
	\n\
	return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,\n\
				oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,\n\
				oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,\n\
				0.0, 0.0, 0.0, 1.0);\n\
}\n\
\n\
mat4 modl()\n\
{\n\
   float angle = time * 0.001f;\n\
   vec3 axis = vec3(0.0f, 1.0f, 0.0f);\n\
   return rotationMatrix(axis, angle);\n\
}\n\
\n\
mat4 calculateMatrix()\n\
{\n\
   return mvp * modl();\n\
}\n\
";

const QString fragmentShaderASource = "\
#version 150 core\n\
\n\
in vec2 uv;\n\
out vec4 color;\n\
void main()\n\
{\n\
	color = texture(textureSampler, uv);\n\
}\n\
";

const QString fragmentShaderBSource = "\
#version 150 core \n\
\n\
in float z;\n\
out vec4 color;\n\
void main()\n\
{\n\
	color = vec4(1.0 * (abs(z) / 4.5), 0.0, 0.0, 1.0);\n\
}\n\
";

SampleData::SampleData() { }

void SampleData::createSampleData(Document *document)
{
	//Access the pipeline. Required to use the macros.
	IPipeline* pipeline = document->getPipeline();
	IUICommandQueue* commandQueue = document->getCommandQueue();

	//Create data source.
	BLOCK(dataSource, MeshGenerator);
	{
		BEGIN_PROPERTY_UPDATE(dataSource);
		PROPERTY(Mesh_Type, Enum, MeshGeneratorBlock::Geom_Cuboid);
		PROPERTY(Mesh_Width, Float, 1.5f);
		PROPERTY(Mesh_Height, Float, 1.5f);
		PROPERTY(Mesh_Depth, Float, 1.5f);
		END_PROPERTY_UPDATE;
	}

	//Create mixer and connect to data source.
	BLOCK(mixer, Mixer);
	CONNECT(dataSourceMixerPosition, dataSource, Data_VertexPositions, mixer, Data_In);
	CONNECT(dataSourceMixerTexture, dataSource, Data_TextureCoordinates, mixer, Data_In);
	{
		//Create mixer layout.
		MixerLayout mixerLayout;
		mixerLayout.autoConfigureLayout(mixer);

		BEGIN_PROPERTY_UPDATE(mixer);
		PROPERTY(Mixer_Layout, MixerLayout, mixerLayout);
		END_PROPERTY_UPDATE;
	}

	//Create VBO and connect to mixer.
	BLOCK(vbo, Buffer);
	CONNECT(mixerVBO, mixer, Data_Out, vbo, Data_In);

	//Create VAO and connect to VBO.
	BLOCK(vao, VertexArrayObject);
	CONNECT(vboVAO, vbo, Data_Out, vao, Data_In);
	{
		//Create VAO layout.
		VaoLayout vaoLayout;
		vaoLayout.autoConfigureLayout(vao);

		BEGIN_PROPERTY_UPDATE(vao);
		PROPERTY(Vao_Layout, VaoLayout, vaoLayout);
		END_PROPERTY_UPDATE;
	}

	//Create IBO and connect to data source.
	BLOCK(ibo, Buffer);
	CONNECT(dataSourceIBO, dataSource, Data_IndexList, ibo, Data_In);

	//Create vertex puller and connect to IBO and VAO.
	BLOCK(vertexPuller, VertexPuller);
	CONNECT(vaoVertexPuller, vao, GenericOut, vertexPuller, GenericIn);
	CONNECT(iboVertexPuller, ibo, Data_Out, vertexPuller, VertexPuller_IndexList);

	//Create uniforms.
	BLOCK(elapsedTime, Uniform_ElapsedTime);
	BLOCK(mvp, ModelViewProjection);
	{
		BEGIN_PROPERTY_UPDATE(mvp);
		PROPERTY(MVP_CameraPosition, Vec3, QVector3D(0, 0, 4));
		END_PROPERTY_UPDATE;
	}

	//Create vertex shader and connect to vertex puller and uniforms.
	BLOCK(vertexShader, Shader_Vertex);
	CONNECT(vertexPullerVertexShader, vertexPuller, GenericOut, vertexShader, GenericIn);
	CONNECT(elapsedTimeVertexShader, elapsedTime, GenericOut, vertexShader, Shader_Uniform);
	CONNECT(mvpVertexShader, mvp, MVP_MatMVP, vertexShader, Shader_Uniform);
	{
		//Create sahder document list.
		GLSLDocumentList documentList;
		documentList.addDocument(vertexShaderSourceA, vertexShader);
		documentList.addDocument(vertexShaderSourceB, vertexShader);

		BEGIN_PROPERTY_UPDATE(vertexShader);
		PROPERTY(Shader_Code, GLSLDocumentList, documentList);
		END_PROPERTY_UPDATE;
	}
	{
		BEGIN_PROPERTY_UPDATE(elapsedTimeVertexShader);
		PROPERTY(Uniform_Name, String, QString("time"));
		END_PROPERTY_UPDATE;
	}
	{
		BEGIN_PROPERTY_UPDATE(mvpVertexShader);
		PROPERTY(Uniform_Name, String, QString("mvp"));
		END_PROPERTY_UPDATE;
	}

	//Primary render path.
	{
		//Create rasterization and connect to vertex shader.
		BLOCK(rasterization, Rasterization);
		CONNECT(vertexShaderRasterization, vertexShader, GenericOut, rasterization, GenericIn);

		//Create image loader block.
		BLOCK(imageLoader, ImageLoader);
		{
			BEGIN_PROPERTY_UPDATE(imageLoader);
			PROPERTY(Img_FileName, Filename, QString("test.jpg"));
			END_PROPERTY_UPDATE;
		}

		//Create texture sampler block.
		BLOCK(sampler, TextureSampler);
		{
			BEGIN_PROPERTY_UPDATE(sampler);
			PROPERTY(TextureBase_MinFilter, Enum, TextureBaseBlock::Filter_Linear);
			PROPERTY(TextureBase_MagFilter, Enum, TextureBaseBlock::Filter_Linear);
			END_PROPERTY_UPDATE;
		}

		//Create texture block and connect to image loader block and sampler.
		BLOCK(texture, Texture);
		CONNECT(imageLoaderTexture, imageLoader, Data_TexelColors, texture, Data_In);
		CONNECT(samplerTexture, sampler, GenericOut, texture, Texture_Sampler);

		//Create fragment shader and connect to rasterization and uniforms.
		BLOCK(fragmentShader, Shader_Fragment);
		CONNECT(rasterizationFragmentShader, rasterization, GenericOut, fragmentShader, GenericIn);
		CONNECT(textureFragmentShader, texture, GenericOut, fragmentShader, Shader_Texture);
		{
			//Create sahder document list.
			GLSLDocumentList documentList;
			documentList.addDocument(fragmentShaderASource, fragmentShader);

			BEGIN_PROPERTY_UPDATE(fragmentShader);
			PROPERTY(Shader_Code, GLSLDocumentList, documentList);
			END_PROPERTY_UPDATE;
		}
		{
			BEGIN_PROPERTY_UPDATE(textureFragmentShader);
			PROPERTY(Texture_Name, String, QString("textureSampler"));
			PROPERTY(Texture_Unit, UInt, 0);
			END_PROPERTY_UPDATE;
		}

		//Create fragment test block and connect to fragment shader.
		BLOCK(fragmentTest, FragmentTests);
		CONNECT(fragmentShaderFragmentTest, fragmentShader, GenericOut, fragmentTest, GenericIn);

		//Create display block and connect to fragment test.
		BLOCK(display, Display);
		CONNECT(fragmentTestDisplay, fragmentTest, GenericOut, display, GenericIn);

		//Create clear command.
		COMMAND(clear, Clear, display);
		{
			BEGIN_PROPERTY_UPDATE(clear);
			PROPERTY(Clear_ColorEnabled, Bool, true);
			PROPERTY(Clear_DepthEnabled, Bool, true);
			PROPERTY(Clear_StencilEnabled, Bool, false);
			PROPERTY(Clear_Color, Color, QVector4D(0.0, 0.0, 0.5, 1.0));
			END_PROPERTY_UPDATE;
		}
	}

	//Secondary render path.
	{
		//Create rasterization and connect to vertex shader.
		BLOCK(rasterization, Rasterization);
		CONNECT(vertexShaderRasterization, vertexShader, GenericOut, rasterization, GenericIn);

		//Create fragment shader and connect to rasterization and uniforms.
		BLOCK(fragmentShader, Shader_Fragment);
		CONNECT(rasterizationFragmentShader, rasterization, GenericOut, fragmentShader, GenericIn);
		{
			//Create sahder document list.
			GLSLDocumentList documentList;
			documentList.addDocument(fragmentShaderBSource, fragmentShader);

			BEGIN_PROPERTY_UPDATE(fragmentShader);
			PROPERTY(Shader_Code, GLSLDocumentList, documentList);
			END_PROPERTY_UPDATE;
		}

		//Create fragment test block and connect to fragment shader.
		BLOCK(fragmentTest, FragmentTests);
		CONNECT(fragmentShaderFragmentTest, fragmentShader, GenericOut, fragmentTest, GenericIn);

		//Create display block and connect to fragment test.
		BLOCK(display, Display);
		CONNECT(fragmentTestDisplay, fragmentTest, GenericOut, display, GenericIn);

		//Create clear command.
		COMMAND(clear, Clear, display);
		{
			BEGIN_PROPERTY_UPDATE(clear);
			PROPERTY(Clear_ColorEnabled, Bool, true);
			PROPERTY(Clear_DepthEnabled, Bool, true);
			PROPERTY(Clear_StencilEnabled, Bool, false);
			PROPERTY(Clear_Color, Color, QVector4D(0.0, 0.0, 0.0, 1.0));
			END_PROPERTY_UPDATE;
		}
	}

	COMMAND(draw, Draw, vertexPuller);
	{
		BEGIN_PROPERTY_UPDATE(draw);
		PROPERTY(Draw_DrawMode, Enum, DrawRenderCommand::DrawMode_Elements);
		PROPERTY(Draw_PrimitiveMode, Enum, DrawRenderCommand::PrimitiveMode_Triangles);
		PROPERTY(Draw_ElementCount, UInt, 0);
		END_PROPERTY_UPDATE;
	}
}

