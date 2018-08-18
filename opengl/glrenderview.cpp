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

#include "glrenderview.h"
#include "glrenderpass.h"
#include "glrenderpassset.h"
#include "glcontroller.h"
#include "glwrapper.h"

#include "evaluation/setuprenderingevaluator.h"
#include "evaluation/evaluationexception.h"

#include "data/irendercommand.h"
#include "data/iblock.h"
#include "data/iconnection.h"
#include "data/iconnection.h"
#include "data/rendercommands/drawrendercommand.h"
#include "data/blocks/framebufferobjectblock.h"

#include <QOpenGLShaderProgram>
#include <QMouseEvent>

namespace ysm
{

// TODO: Check wether parent view can be specified correctly
// Currently document is set manually by GLController after creation
GLRenderView::GLRenderView(GLController* controller, IBlock* displayBlock)
	: QOpenGLWidget(),
	  View(NULL),
	  _controller(controller),
	  _evaluator(nullptr),
	  _renderPassSet(nullptr),
	  _valid(false),
	  _cameraControl(nullptr)
{
	// Enable partial update
	setUpdateBehavior(PartialUpdate);

	// Set focus policy for receiving key events
	setFocusPolicy(Qt::StrongFocus);

	// Create the Set of RenderPasses
	_evaluator = controller->getEvaluator();
	_renderPassSet = new GLRenderPassSet(displayBlock);

	// Search for a camera control block in one of the passes
	for(GLRenderPass* pass : _renderPassSet->getRenderPasses())
	{
		if((_cameraControl = pass->getUniqueBlock(BlockType::CameraControl)))
		{
			// As default, move camera away from center
			_cameraTransform = QVector3D(0, 0, 1);
			break;
		}
	}
}

GLRenderView::~GLRenderView()
{
	delete _renderPassSet;
}

GLRenderPassSet* GLRenderView::getRenderPassSet() const
{
	return _renderPassSet;
}

IBlock* GLRenderView::getDisplayBlock() const
{
	return _renderPassSet->getOutputBlock();
}

void GLRenderView::onRegistrationSuccessful()
{
	_valid = true;
}

void GLRenderView::onRenderingAborted()
{
	_valid = false;
}

QVector3D GLRenderView::getCamera() const
{
	return _cameraTransform;
}

void GLRenderView::setCamera(QVector3D camera)
{
	_cameraTransform = camera;
}

void GLRenderView::initializePass(GLRenderPass* pass)
{
	// Bind the shaderprogram
	QOpenGLShaderProgram* program = _evaluator->getShaderProgram(pass);
	program->bind();

	// Look, if a rasterization stage exists in the current pass
	if(pass->getUniqueBlock(BlockType::Rasterization))
		f->glDisable(GL_RASTERIZER_DISCARD);
	else
		f->glEnable(GL_RASTERIZER_DISCARD);

	for(IBlock* block : pass->getInvolvedBlocks())
	{
		switch(block->getType())
		{
		case BlockType::Texture:
		case BlockType::TextureView:
			bindTexture(block, pass);
			break;

		case BlockType::Uniform_ElapsedTime:
			updateElapsedTimeUniform(block, pass);
			break;

		case BlockType::Rasterization:
		case BlockType::FragmentTests:
		case BlockType::TessellationPrimitiveGenerator:
		case BlockType::ModelViewProjection:
		case BlockType::LightSource:
		case BlockType::Material:
			_evaluator->initializeContext(block, pass);
			break;
		default:
			// Nothing to do
			break;
		}
	}
}

void GLRenderView::initializeGL()
{
	// Register the view in the controller
	_controller->registerView(this);

	// Initialize GL-Functions Object
	// Should not fail, because this one was called and catched in the evaluator before
	f = context()->versionFunctions<GLConfiguration::Functions>();
}

//TODO: Thread synchronization problems with stopping rendering process.
//setRendering(false) should ensure, that the rendering is actually halted.
void GLRenderView::paintGL()
{
	// Don't draw, if not initialized correctly
	if(!_valid)
		return;

	// Let's draw! - but safe
	try
	{
		// At first, update Camera control data
		setupCameraControl();

		// Iterate over all commands stored in the underlying pipeline
		for(IRenderCommand* command : _renderPassSet->getPipeline()->getRenderCommands())
		{
			bool clearCommand = false;
			for(GLRenderPass* pass : _renderPassSet->getRenderPasses())
			{
				if(pass->getInvolvedRenderCommands().contains(command))
				{
					// Look for a framebuffer object to be bound
					IBlock* fbo = pass->getUniqueBlock(BlockType::FrameBufferObject);
					if(fbo)
						f->glBindFramebuffer(GL_FRAMEBUFFER, _evaluator->getEvaluatedData(fbo)->getValue());

					// Execute command
					switch (command->getCommand()) {
					case RenderCommandType::Clear:
						callClearSettings(command);
						clearCommand = true;
						break;
					case RenderCommandType::Draw:
						callDrawCommand(command, pass);
						break;
					default:
						break;
					}

					// Release framebuffer
					if(fbo)
						f->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

					// Sync between passes
					GLsync sync = f->glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
					f->glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, SYNC_TIMEOUT);
					f->glDeleteSync(sync);

					// In case we have a clear command, we break here, because otherwise the command could be called
					// multiple times, since a FBO or Display can be part of multiple passes.
					if(clearCommand)
						break;
				}
			}
		}
	}
	catch(EvaluationException exception)
	{
		// Don't forget to bind Default FBO
		f->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

		// Tell the Controller it should stop rendering
		_controller->signalAbortRendering(this, exception.what());
	}
}

void GLRenderView::resizeGL(int w, int h)
{
	Q_UNUSED(w);
	Q_UNUSED(h);
}

void GLRenderView::setupCameraControl()
{
	if(!_cameraControl)
		return;

	for(IConnection* connection : _cameraControl->getOutConnections())
	{
		IBlock* mvp = connection->getDest();
		mvp->beginPropertyUpdate();

		// Create a new up vector which we transform as well
		QVector3D upVector = QVector3D(0, 1, 0);

		// Move away from center
		QVector3D cameraPosition(0, 0, _cameraTransform.z());

		// Create rotation matrix for the camera arm
		QMatrix4x4 transformation;
		transformation.rotate(-_cameraTransform.x(), 0, 1, 0);
		transformation.rotate(-_cameraTransform.y(), 1, 0, 0);

		// Apply the transformation
		mvp->getProperty<Vec3Property>(PropertyID::MVP_CameraPosition)->setValue(transformation * cameraPosition);
		mvp->getProperty<Vec3Property>(PropertyID::MVP_CameraUpVector)->setValue(transformation * upVector);
		mvp->endPropertyUpdate();
	}
}

void GLRenderView::updateElapsedTimeUniform(IBlock* block, GLRenderPass* pass)
{
	QOpenGLShaderProgram* program = _evaluator->getShaderProgram(pass);
	GLint value = *block->getProperty<IntProperty>(PropertyID::Uniform_Value);

	for(IConnection* connection : pass->getOutConnections(block))
	{
		// In case this uniform is connected to a Buffer, we don't need to setup OpenGL for it
		if(connection->getDest()->getType() == BlockType::Buffer)
			continue;

		QString name = *connection->getProperty<StringProperty>(PropertyID::Uniform_Name);
		if(connection->getProperty<BoolProperty>(PropertyID::Uniform_ExplicitLocation)->getValue())
			program->setUniformValue(*connection->getProperty<UIntProperty>(PropertyID::Uniform_Location), value);
		else
			program->setUniformValue(program->uniformLocation(name), value);
	}
}


void GLRenderView::bindTexture(IBlock* block, GLRenderPass* pass) const
{
	GLTextureWrapper* texture = _evaluator->getEvaluatedData<GLTextureWrapper>(block);
	for(const TextureBindingParameter& binding : texture->getBindings(pass))
	{
		// Bind the texture.
		f->glActiveTexture(GL_TEXTURE0 + binding.unit);
		f->glBindTexture(texture->getTarget(), texture->getValue());
		if(binding.location >= 0)
			f->glUniform1i(binding.location, binding.unit);

		// Bind sampler, or automatically unbind old one
		f->glBindSampler(binding.unit, texture->getSampler());
	}
}

void GLRenderView::callDrawCommand(IRenderCommand* command, GLRenderPass* pass)
{
	// Initialize this pass for being drawn
	initializePass(pass);

	// Bind the vao
	IBlock* vao = pass->getUniqueBlock(BlockType::VertexArrayObject);
	f->glBindVertexArray(_evaluator->getEvaluatedData(vao)->getValue());

	int primitiveMode = *command->getProperty<EnumProperty>(PropertyID::Draw_PrimitiveMode);
	switch (primitiveMode)
	{
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_Points:
		primitiveMode = GL_POINTS;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_Lines:
		primitiveMode = GL_LINES;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_LinesAdjacency:
		primitiveMode = GL_LINES_ADJACENCY;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_LineStrip:
		primitiveMode = GL_LINE_STRIP;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_LineStripAdjacency:
		primitiveMode = GL_LINE_STRIP_ADJACENCY;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_LineLoop:
		primitiveMode = GL_LINE_LOOP;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_Triangles:
		primitiveMode = GL_TRIANGLES;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_TrianglesAdjacency:
		primitiveMode = GL_TRIANGLES_ADJACENCY;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_TriangleStrip:
		primitiveMode = GL_TRIANGLE_STRIP;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_TriangleStripAdjacency:
		primitiveMode = GL_TRIANGLE_STRIP_ADJACENCY;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_TriangleFan:
		primitiveMode = GL_TRIANGLE_FAN;
		break;
	case DrawRenderCommand::PrimitiveMode::PrimitiveMode_Patches:
		primitiveMode = GL_PATCHES;
		break;
	default:
		break;
	}

	// Look for transform feedback to be enabled
	IBlock* tfb = pass->getUniqueBlock(BlockType::TransformFeedback);
	if(tfb) // TODO: limit selection here
		f->glBeginTransformFeedback(primitiveMode);

	// Get the element count either from auto detection or user input
	int elementCount = 0;
	if(command->getProperty<BoolProperty>(PropertyID::Draw_AutoElementCount)->getValue())
	{
		// Per design by contract, we can guarantee exactly one Vertex Puller
		IBlock* puller = pass->getUniqueBlock(BlockType::VertexPuller);
		elementCount = *puller->getProperty<UIntProperty>(PropertyID::VertexPuller_ElementCount);
	}
	else
		elementCount = *command->getProperty<UIntProperty>(PropertyID::Draw_ElementCount);

	switch (*command->getProperty<EnumProperty>(PropertyID::Draw_DrawMode))
	{
	case DrawRenderCommand::DrawMode_Elements:
	{
		GLWrapper* ibo = _evaluator->getEvaluatedData(pass->getIndexBufferObjectBlock());
		f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->getValue());
		if(*command->getProperty<BoolProperty>(PropertyID::Draw_Instanced))
		{
			unsigned int instanceCount = *command->getProperty<UIntProperty>(PropertyID::Draw_InstanceCount);
			f->glDrawElementsInstanced(primitiveMode, elementCount, GL_UNSIGNED_INT, 0, instanceCount);
		}
		else
			f->glDrawElements(primitiveMode, elementCount, GL_UNSIGNED_INT, 0);
		f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
		break;
	case DrawRenderCommand::DrawMode_Arrays:
	{
		int firstIndex = *command->getProperty<UIntProperty>(PropertyID::Draw_FirstIndex);
		if(*command->getProperty<BoolProperty>(PropertyID::Draw_Instanced))
		{
			unsigned int instanceCount = *command->getProperty<UIntProperty>(PropertyID::Draw_InstanceCount);
			f->glDrawArraysInstanced(primitiveMode, firstIndex, elementCount, instanceCount);
		}
		else
			f->glDrawArrays(primitiveMode, firstIndex, elementCount);
	}
		break;
	default:
		break;
	}

	// Reset state
	if(tfb)
		f->glEndTransformFeedback();

	f->glBindVertexArray(0);
}


void GLRenderView::callClearSettings(IRenderCommand* command)
{
	//Viewport settings
	if(command->getProperty<BoolProperty>(PropertyID::Clear_ViewportAutoSize)->getValue())
	{
		//TODO: which size to choose for a FBO?
		f->glViewport(0, 0, width(), height());
	}
	else
	{
		QVector2D lowerLeft = command->getProperty<Vec2Property>(PropertyID::Clear_ViewportLowerLeftCorner)->getValue();
		int width = command->getProperty<UIntProperty>(PropertyID::Clear_ViewportWidth)->getValue();
		int height = command->getProperty<UIntProperty>(PropertyID::Clear_ViewportHeight)->getValue();
		f->glViewport(lowerLeft.x(), lowerLeft.y(), width, height);
	}

	// initialize bitmask, to be able to use a single API call
	GLbitfield field = 0;

	// initialize color buffer
	if(*command->getProperty<BoolProperty>(PropertyID::Clear_ColorEnabled))
	{
		const QVector4D& cc = command->getProperty<ColorProperty>(PropertyID::Clear_Color)->getValue();
		f->glClearColor(cc.x(), cc.y(), cc.z(), cc.w());
		field |= GL_COLOR_BUFFER_BIT;
	}

	// initialize depth buffer
	if(*command->getProperty<BoolProperty>(PropertyID::Clear_DepthEnabled))
	{
		GLdouble depth = *command->getProperty<DoubleProperty>(PropertyID::Clear_Depth);
		f->glClearDepth(depth);
		field |= GL_DEPTH_BUFFER_BIT;
	}

	// initialize stencil buffer
	if(*command->getProperty<BoolProperty>(PropertyID::Clear_StencilEnabled))
	{
		GLint stencil = *command->getProperty<IntProperty>(PropertyID::Clear_Stencil);
		f->glClearStencil(stencil);
		field |= GL_STENCIL_BUFFER_BIT;
	}

	// Clear selected buffers
	f->glClear(field);
}


void GLRenderView::mouseMoveEvent(QMouseEvent* event)
{
	if (!_cameraControl || event->buttons() != Qt::LeftButton)
		return;

	// Calculate delta movement
	QPoint delta = event->globalPos() - _grabMousePosition;
	QPointF angles = delta * _cameraControl->getProperty<FloatProperty>(PropertyID::CameraControl_RotationSpeed)->getValue();

	// Reset cursor position
	QCursor::setPos(_grabMousePosition);

	// Transform by parameters
	_cameraTransform += QVector3D(angles.x(), angles.y(), 0);
}

void GLRenderView::mousePressEvent(QMouseEvent* event)
{
	if(!_cameraControl)
		return;

	_grabMousePosition = event->globalPos();

	// accept the event
	event->accept();
}

void GLRenderView::wheelEvent(QWheelEvent* event)
{
	if(!_cameraControl || event->angleDelta().isNull())
		return;

	// Calculate delta zoom
	int delta = -(event->angleDelta().y() / 60);
	float zoom = delta * _cameraControl->getProperty<FloatProperty>(PropertyID::CameraControl_TranslationSpeed)->getValue();

	// Set zoom value, but allow positive values, only
	_cameraTransform.setZ(qMax(_cameraTransform.z() + zoom, 0.1f));

	// accept the event
	event->accept();
}

// TODO: Document change should not be allowed
void GLRenderView::updateDocument() { }
void GLRenderView::updateItem() { }

} // namespace ysm
