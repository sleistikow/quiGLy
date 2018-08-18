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

#include "logview.h"
#include "../mainwindow/mainwindow.h"
#include "../mainwindow/viewmanager.h"

using namespace ysm;

LogView::LogView(QWidget* parentWidget, IView* parentView) :
	QDockWidget(parentWidget),
	View(parentView)
{
	//Initialize the output widget.
	_textLog = new QPlainTextEdit(this);
	_textLog->setReadOnly(true);

	//Use the output widget.
	setWidget(_textLog);
}

void LogView::clear()
{
	//Access the active log view to clear it.
	LogView* activeLog = MainWindow::getInstance()->getViewManager()->getLogView();
	activeLog->_textLog->clear();
}

void LogView::log(const QString &log)
{
	//Also log to debug console.
	qDebug() << log;

	//Access the active log view to log the message.
	LogView* activeLog = MainWindow::getInstance()->getViewManager()->getLogView();
	activeLog->_textLog->appendPlainText(log);
}

void LogView::log(const QStringList &log)
{
	//Also log to debug console.
	qDebug() << log;

	//Access the active log view to log the message.
	LogView* activeLog = MainWindow::getInstance()->getViewManager()->getLogView();
	foreach(QString message, log)
		activeLog->_textLog->appendPlainText(message);
}

void LogView::updateDocument() { }
void LogView::updateItem() { }
