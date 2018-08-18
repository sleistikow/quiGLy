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

#include <QDebug>
#include <QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLDebugLogger>

#include "commands/iuicommandqueue.h"
#include "commands/pipeline/change/updatestatuscommand.h"
#include "commands/pipeline/change/validatepipelinecommand.h"

#include "views/logview/logview.h"
#include "views/document.h"
#include "views/view.h"

#include "data/ipipelinemanager.h"
#include "data/ipipeline.h"
#include "data/iblock.h"
#include "data/blocks/displayblock.h"
#include "data/blocks/uniforms/elapsedtimeuniformblock.h"

#include "glcontroller.h"
#include "glrenderview.h"
#include "glrenderpass.h"
#include "glrenderpassset.h"
#include "abortrenderingevent.h"
#include "evaluation/evaluationexception.h"
#include "evaluation/iglrenderpassevaluator.h"
#include "evaluation/setuprenderingevaluator.h"

#include "views/document.h"

namespace ysm
{

GLController::GLController(QObject* parent) :
	QObject(parent)
{
	// Within this implementation, a global shared context must be present
	if(!QCoreApplication::testAttribute(Qt::AA_ShareOpenGLContexts))
		throw std::runtime_error("Attribute Qt::AA_ShareOpenGLContexts not set correctly!");

	// Try to get the best out of our machine!
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#ifdef QT_DEBUG
	format.setOption(QSurfaceFormat::DebugContext);
#endif

	// Try to create the context
	QOpenGLContext versionTestContext;
	versionTestContext.setFormat(format);
	if(!versionTestContext.create())
		throw std::runtime_error("No OpenGL context could be created at all.");

	// Store the actually created format
	_surfaceFormat = versionTestContext.format();

	// Check, whether OpenGL 3.3 is supported
	if(!isOpenGLVersionSupported(3, 3))
		throw std::runtime_error("Your system does not support OpenGL 3.3 which has to be present.");

	// Finally, we can initialize our Evaluators
	_setupRenderingEvaluator = new SetupRenderingEvaluator();

	// Setup timer for the view refresh
	_timer.setInterval(0);
	_timer.start();
}

GLController::~GLController()
{
	// Delete all the evaluators
	delete _setupRenderingEvaluator;
}

bool GLController::isOpenGLVersionSupported(int major, int minor)
{
	// We support nothing lower than 3.3
	if(major < 3 || (major == 3 && minor < 3))
		return false;

	const QPair<int, int>& version = getOpenGLVersion();
	if(major < version.first)
		return true;

	if(major > version.first)
		return false;

	return (minor <= version.second);
}

QPair<int, int> GLController::getOpenGLVersion() const
{
	return _surfaceFormat.version();
}

SetupRenderingEvaluator* GLController::getEvaluator() const
{
	return _setupRenderingEvaluator;
}

void GLController::registerView(GLRenderView* view)
{

#ifdef QT_DEBUG
	// Create logger for the views context
	QOpenGLDebugLogger logger;
	logger.initialize();
	connect(&logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(onMessageLogged(QOpenGLDebugMessage)));
	logger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
#endif

	// Try evaluating view
	try
	{
		// Log success
		IBlock* displayBlock = view->getDisplayBlock();
		QString widgetName = QString("%1 #%2").arg(displayBlock->getName()).arg(displayBlock->getID());
		LogView::log(QString("\n=== RENDERING STARTED [%1] ===\n").arg(widgetName));

		GLRenderPassSet* renderPassSet = view->getRenderPassSet();

		// Check, whether everything went fine
		if(!renderPassSet->isValid())
			throw EvaluationException("Pipeline Graph contains cycles.");

		for(GLRenderPass* pass : renderPassSet->getRenderPasses())
		{
			if(!pass->isValid())
				throw EvaluationException("Pipeline was not setup correctly!\n"
										  "Check that all necessary Blocks exist.");
		}

		// Evaluate Pipeline
		_setupRenderingEvaluator->evaluate(renderPassSet);

		// Add the warnings to the blocks
		for(const SetupRenderingEvaluator::Warning& warning : _setupRenderingEvaluator->getWarnings())
		{
			if(warning.block)
				view->executeCommand(new UpdateStatusCommand(warning.block, PipelineItemStatus::Chilled, warning.message));
		}

		// Log success
		LogView::log(QString("\n=== RENDERING SUCCESSFUL [%1] ===\n").arg(widgetName));

		// Tell the view that the registration was successful
		view->onRegistrationSuccessful();

		// Connect the timer to the update event for a constant framerate
		connect(&_timer, SIGNAL(timeout()), view, SLOT(update()));
	}
	catch (EvaluationException exception)
	{
		// Store the reason
		QString reason;

		// Get the block, the error occurd on
		IBlock* block = exception.getBlock();
		if(block)
		{
			view->executeCommand(new UpdateStatusCommand(block, PipelineItemStatus::Sick, exception.what()));
			reason = QString("Error while evaluating block #%1").arg(block->getID());
		}
		else
			reason = QString("Error during evaluation: %1").arg(exception.what());

		// Signal that something went wrong
		signalAbortRendering(view, reason, exception.getLog());
	}

	// Tell the world we might have some messages left to be read

#ifdef QT_DEBUG
	logger.stopLogging();
#endif
}

void GLController::signalAbortRendering(GLRenderView* view, const QString& reason, const QString& log)
{
	// We need to ensure that nothing is rendered on the view anymore
	view->onRenderingAborted();

	// Log the rendering failure
	IBlock* displayBlock = view->getDisplayBlock();
	QString widgetName = QString("%1 #%2").arg(displayBlock->getName()).arg(displayBlock->getID());
	LogView::log(QString("\n=== RENDERING ABORTED [%1] ===\n").arg(widgetName));

	// Append reason and detail log
	LogView::log(reason);
	LogView::log(log);

	// Create an event which will be handled last in the eventqueue
	QCoreApplication::postEvent(parent(), new AbortRenderingEvent(view, reason, log));
}

#ifdef QT_DEBUG
void GLController::onMessageLogged(QOpenGLDebugMessage message)
{
	//Set breakpoint here
	LogView::log(message.message());
}
#endif

} // namespace ysm
