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

#include "filenamepropertyviewitem.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

using namespace ysm;

FilenamePropertyViewItem::FilenamePropertyViewItem(IProperty* property, IView* parentView) :
	GenericPropertyViewItem(property, parentView)
{
	//Set label to property name.
	setLabel(new QLabel(property->getName()));

	//Create edit and connect signal.
	_fileSelect = new FileSelect(NULL);
	setEdit(_fileSelect);

	//Connect to state changes.
	connect(_fileSelect, SIGNAL(filenameChanged()), this, SLOT(updateModelValue()));
	connect(_fileSelect, SIGNAL(browseFile()), this, SLOT(browseFile()));
}

void FilenamePropertyViewItem::browseFile()
{
	//Show file dialog.
	QString filename = QFileDialog::getOpenFileName();
	if(!filename.isEmpty())
		_fileSelect->setFilename(filename);
}

QString FilenamePropertyViewItem::getViewValue() const { return _fileSelect->getFilename(); }
void FilenamePropertyViewItem::updateViewValue()
{
	_fileSelect->setFilename(getModelValue());
	_fileSelect->setReadOnly(getProperty()->isReadOnly());
}
