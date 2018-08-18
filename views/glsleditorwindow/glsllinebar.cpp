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

#include "glsllinebar.h"
#include "glsllinebarblock.h"
#include "iglsllinebardelegate.h"

#include <QPainter>
#include <QPaintEvent>
#include <QFontDatabase>

using namespace ysm;

GLSLLineBar::GLSLLineBar(QWidget* parent) :
	QWidget(parent),
	_delegate(NULL)
{
	//Require 10px space at both sides.
	setContentsMargins(10, 0, 15, 0);

	//Set the font.
	setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

IGLSLLineBarDelegate* GLSLLineBar::getDelegate() const { return _delegate; }

void GLSLLineBar::setDelegate(IGLSLLineBarDelegate* delegate)
{
	if(_delegate)
		disconnect(dynamic_cast<QObject *>(_delegate), SIGNAL(lineBarBlocksChanged()),
				   this, SLOT(updateLineBarBlocks()));
	_delegate = delegate;
	if(_delegate)
		connect(dynamic_cast<QObject *>(_delegate), SIGNAL(lineBarBlocksChanged()),
				this, SLOT(updateLineBarBlocks()));
}

void GLSLLineBar::updateLineBarBlocks()
{
	//Calculate the size minimal required width and set it.
	int marginSpacing = contentsMargins().left() + contentsMargins().right();
	setFixedWidth(getLineNumberDigitCount() * fontMetrics().maxWidth() + marginSpacing);

	//Redraw the widget.
	updateGeometry();
	update();
}

int GLSLLineBar::getLineNumberDigitCount() const
{
	//Delegate must be available.
	if(!_delegate)
		return 0;

	//Get the line numbers, minimum 1, then divide by 10 as long as possible to calculate the number of digits.
	int lineNumbers = qMax(_delegate->getLineBarBlockCount(this), 1), digitCount = 0;
	for( ; lineNumbers > 0; lineNumbers /= 10)
		digitCount++;
	return digitCount;
}

void GLSLLineBar::paintEvent(QPaintEvent* event)
{
	//Create the painter.
	QPainter painter(this);
	painter.fillRect(event->rect(), Qt::lightGray);
	painter.setPen(Qt::darkGray);

	//Make sure delegate is specified.
	if(_delegate)
	{
		//Retrieve the data from the delegate.
		QVector<GLSLLineBarBlock> lineBlocks = _delegate->getLineBarBlocks(this);

		//Iterate over the line blocks, and draw them.
		for(int i = 0; i < lineBlocks.size(); i++)
		{
			//Adjust the rect.
			QRectF lineNumberRect = lineBlocks[i].getBlockBoundingRect();
			lineNumberRect.setLeft(contentsMargins().left());
			lineNumberRect.setWidth(getLineNumberDigitCount() * fontMetrics().maxWidth());
			lineNumberRect.setHeight(fontMetrics().lineSpacing());

			//Draw right bound.
			painter.drawText(lineNumberRect, QString::number(lineBlocks[i].getBlockNumber() + 1),
							 QTextOption(Qt::AlignVCenter | Qt::AlignRight));
		}
	}
}
