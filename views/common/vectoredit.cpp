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

#include "vectoredit.h"

#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QFontDatabase>

using namespace ysm;

VectorEdit::VectorEdit(int dimensions, QWidget* parent) :
	QWidget(parent),
	_dimensions(dimensions)
{
	//Use a fixed font.
	const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

	//Create the base layout.
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setMargin(0);

	//Create the line edits.
	for(int i = 0; i < dimensions; i++)
	{
		//Configure the line edit.
		QLineEdit* lineEdit = new QLineEdit(this);
		lineEdit->setAlignment(Qt::AlignRight);
		lineEdit->setValidator(new QDoubleValidator(this));
		lineEdit->setFont(fixedFont);
		lineEdit->setMaximumWidth(50);
		lineEdit->setText("0");

		//Append the edit.
		mainLayout->addWidget(lineEdit);
		_lineEdits.append(lineEdit);

		//Connect to changes.
		connect(lineEdit, &QLineEdit::editingFinished, this, &VectorEdit::updateValue);
	}

	//Do not stretch the control.
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}

QVector4D VectorEdit::getVector() const { return _vector; }
void VectorEdit::setVector(const QVector4D& vector)
{
	//Ensure the vector actually changes.
	if(_vector == vector) return;
	_vector = vector;

	//Update the line edits.
	for(int i = 0; i < _dimensions; i++)
		_lineEdits[i]->setText(QString::number(_vector[i]));

	//Emit change signal.
	emit vectorChanged();
}

QString VectorEdit::serializeVector() const { return QString(); }
void VectorEdit::deserializeVector(const QString& vector) { }

QChar VectorEdit::getDivider() const { return _divider; }
void VectorEdit::setDivider(QChar divider) { _divider = divider; }

bool VectorEdit::isReadOnly() const { return _lineEdits.first()->isReadOnly(); }
void VectorEdit::setReadOnly(bool isReadOnly)
{
	//Set read-only state for all edits.
	foreach(QLineEdit* lineEdit, _lineEdits)
		lineEdit->setReadOnly(isReadOnly);
}

void VectorEdit::updateValue()
{
	//Build a vector from line edits.
	QVector4D currentVector;
	for(int i = 0; i < _dimensions; i++)
		currentVector[i] = _lineEdits[i]->text().toFloat();

	//Set the new vector.
	setVector(currentVector);
}
