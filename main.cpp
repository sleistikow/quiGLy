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

#include <QApplication>
#include <QOpenGLFunctions>

#include "data/pipeline/pipelinemanager.h"

#include "views/mainwindow/mainwindow.h"
#include "views/mainwindow/documentmanager.h"
#include "views/mainwindow/maindelegate.h"

#include "views/document.h"
#include "sampledata.h"

using namespace ysm;

/*!
 * \brief Main.
 * \param argc The argument count.
 * \param argv The arguments.
 * \return The exit code.
 */
int main(int argc, char** argv)
{
	// Define default format.
	QSurfaceFormat defaultFormat;
	defaultFormat.setVersion(4, 5);
	defaultFormat.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(defaultFormat);

	//Enable global context sharing flag.
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);

	//Create global objects.
	QApplication app(argc, argv);

	//Set application data.
	app.setApplicationDisplayName("quiGLy 1.0");
	app.setApplicationName("quiGLy");
	app.setApplicationVersion("1.0.0");
	app.setOrganizationName("WWU Muenster");
	app.setOrganizationDomain("http://www.uni-muenster.de");

	//Create window and locale.
	QLocale locale(QLocale::English, QLocale::UnitedStates);
	MainWindow* mainWindow = MainWindow::getInstance();

#ifdef QT_DEBUG

	//Create debug document.
	SampleData::createSampleData(mainWindow->getDocumentManager()->createDocument());
	mainWindow->getDelegate()->onAutoLayout();

#endif

	//Create empty document.
	mainWindow->getDocumentManager()->createDocument();

	//Initialize and show main window.
	mainWindow->setLocale(locale);
	mainWindow->show();

	//Run the application.
	int exitCode = app.exec();

	//Return.
	delete mainWindow;
	return exitCode;
}
