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

#include "colorselect.h"
#include "data/common/utils.h"

#include <QHBoxLayout>
#include <QColorDialog>

using namespace ysm;

ColorSelect::ColorSelect(QWidget* parent) :
	QWidget(parent)
{
	//Create the controls.
	_selectButton = new QPushButton(this);
	_colorEdit = new QLineEdit(this);
	_colorEdit->setReadOnly(true);

	//Create the main layout.
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(_selectButton);
	mainLayout->addWidget(_colorEdit);

	//Adjust the layout. There is no need for the control to be stretched.
	_selectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	_colorEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	mainLayout->setMargin(0);

	//Connect the signals.
	connect(_selectButton, &QPushButton::clicked, this, &ColorSelect::pickColor);

	//Set the initial color.
	setColor(Qt::black);
}

QColor ColorSelect::getColor() const { return _color; }
void ColorSelect::setColor(const QColor& color)
{
	//Ensure the color actually changes.
	if(color == _color) return;
	_color = color;

	//Update the button.
	_selectButton->setStyleSheet(
		QString("* { background-color: %1; border: 2px solid black; }")
		.arg(_color.name(QColor::HexArgb)));

	//Update the line edit.
	_colorEdit->setText(_color.name(QColor::HexArgb));

	//Emit the change signal.
	emit colorChanged();
}

QVector4D ColorSelect::getColorVector() const
{
	//Convert color to vector.
	return QVector4D(_color.redF(), _color.greenF(), _color.blueF(), _color.alphaF());
}

void ColorSelect::setColorVector(const QVector4D& vector)
{
	//Convert vector to color.
	setColor(QColor::fromRgbF(vector.x(), vector.y(), vector.z(), vector.w()));
}

QString ColorSelect::serializeColor() const
{
	//Convert color to string.
	return QString("%1;%2;%3;%4").arg(_color.redF()).arg(_color.greenF()).arg(_color.blueF()).arg(_color.alphaF());
}

void ColorSelect::deserializeColor(const QString& color)
{
	//Use the property utils to convert string to color.
	QVector<float> vector;
	if(Utils::convertStringToFloatArray(color, vector))
		setColor(QColor::fromRgbF(vector[0], vector[1], vector[2], vector[3]));
}

void ColorSelect::pickColor()
{
	//Get new color from dialog.
	QColor color = QColorDialog::getColor(_color, this, QString(), QColorDialog::ShowAlphaChannel);
	if(color.isValid())
		setColor(color);
}

QSize ColorSelect::sizeHint() const
{
	//TODO: Useful size could be calculated.
	return QSize(160, 0);
}
