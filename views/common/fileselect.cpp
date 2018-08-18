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

#include "fileselect.h"

#include <QHBoxLayout>

using namespace ysm;

FileSelect::FileSelect(QWidget* parent) :
	QWidget(parent)
{
	//Create edit widget.
	_lineEdit = new QLineEdit(this);
	_lineEdit->setReadOnly(true);

	//Create browse button.
	_browseButton = new QPushButton("...", this);
	_browseButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_browseButton->setMaximumWidth(30);

	//Configure the main layout.
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(_lineEdit);
	layout->addWidget(_browseButton);
	layout->setMargin(0);

	//Connect to Slots
	connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(emitFilenameChanged()));
	connect(_browseButton, SIGNAL(clicked(bool)), this, SLOT(emitBrowseFile()));
}

QString FileSelect::getFilename() const { return _lineEdit->text(); }
void FileSelect::setFilename(const QString &selectedFile) { _lineEdit->setText(selectedFile); emitFilenameChanged(); }

bool FileSelect::isReadOnly() const { return !_browseButton->isEnabled(); }
void FileSelect::setReadOnly(bool isReadOnly)
{
	_browseButton->setEnabled(!isReadOnly);
	_browseButton->setVisible(!isReadOnly);
}

void FileSelect::emitFilenameChanged() { emit filenameChanged(); }
void FileSelect::emitBrowseFile() { emit browseFile(); }
