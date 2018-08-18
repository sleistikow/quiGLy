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

#include "swizzlebox.h"

#include "data/types/typeutils.h"

using namespace ysm;

SwizzleBox::SwizzleBox() :
	_formLayout(NULL)
{
	//Create the layout to display the combo boxes.
	_formLayout = new QFormLayout(this);
	_formLayout->setMargin(0);
}

SwizzleBox::~SwizzleBox() { qDeleteAll(_comboBoxes); qDeleteAll(_boxLabels); }

TypeConversion::ConversionOptions SwizzleBox::getSwizzling() const { return _conversionOptions; }

void SwizzleBox::addDimension(const QString &dimension) { _dimensions.append(dimension); recreateComboBoxes(); }

void SwizzleBox::storeConversionOption(int optionIndex, int targetValue)
{
	//Switch over the option's index and store it to the correct value.
	switch(optionIndex)
	{
	case 0: _conversionOptions.swizzlingX = static_cast<VectorComponent>(targetValue); break;
	case 1: _conversionOptions.swizzlingY = static_cast<VectorComponent>(targetValue); break;
	case 2: _conversionOptions.swizzlingZ = static_cast<VectorComponent>(targetValue); break;
	case 3: _conversionOptions.swizzlingW = static_cast<VectorComponent>(targetValue); break;
	}
}

int SwizzleBox::readConversionOption(int optionIndex) const
{
	//Switch over the option's index and read the correct value.
	switch(optionIndex)
	{
	case 0: return static_cast<int>(_conversionOptions.swizzlingX);
	case 1: return static_cast<int>(_conversionOptions.swizzlingY);
	case 2: return static_cast<int>(_conversionOptions.swizzlingZ);
	case 3: return static_cast<int>(_conversionOptions.swizzlingW);
	}

	//No value available.
	return 0;
}

void SwizzleBox::recreateComboBoxes()
{
	//Iterate over all obsolete combo boxes and delete them.
	int targetComponentCount = DataTypeUtils::getTypeComponentCount(_conversionOptions.targetType);
	while(targetComponentCount < _comboBoxes.count())
	{
		delete _comboBoxes.takeLast();
		delete _boxLabels.takeLast();
	}

	//Iterate over all missing combo boxes and add them.
	for(int i = _comboBoxes.count(); i < targetComponentCount && i < _dimensions.count(); i++)
	{
		//Create combo box and label.
		_comboBoxes.append(new QComboBox());
		_boxLabels.append(new QLabel(QString("%1 out =").arg(_dimensions[i])));

		//Add combo box to layout and connect to signals.
		_comboBoxes[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		_formLayout->addRow(_boxLabels[i], _comboBoxes[i]);
		connect(_comboBoxes[i], SIGNAL(currentIndexChanged(QString)), this, SLOT(swizzle()));
	}

	//Iterate over all existing combo boxes and reset the options.
	int sourceComponentCount = DataTypeUtils::getTypeComponentCount(_sourceType);
	for(int i = 0; i < _comboBoxes.count(); i++)
	{
		//Do not forward these changes.
		_comboBoxes[i]->blockSignals(true);

		//Update the options.
		_comboBoxes[i]->clear();
		for(int j = 0; j < sourceComponentCount && j < _dimensions.count(); j++)
			_comboBoxes[i]->addItem(QString("%1 in").arg(_dimensions[j]));

		//Set the combo box's current option.
		int currentItem = readConversionOption(i);
		if(_comboBoxes[i]->count() > currentItem)
			_comboBoxes[i]->setCurrentIndex(currentItem);

		//Re-enable updates.
		_comboBoxes[i]->blockSignals(false);
	}
}

void SwizzleBox::swizzle()
{
	//Store the new conversion options.
	for(int i = 0; i < _comboBoxes.count(); i++)
		storeConversionOption(i, _comboBoxes[i]->currentIndex());

	//Emit the update signal.
	emit swizzled();
}

void SwizzleBox::setSwizzling(const TypeConversion::ConversionOptions &swizzling)
{
	//Store the swizzling and recreate the combo boxes.
	_conversionOptions = swizzling;
	recreateComboBoxes();
}

void SwizzleBox::setSourceType(const DataType &sourceType)
{
	//Store the target type and recreate the combo boxes.
	_sourceType = sourceType;
	recreateComboBoxes();
}

void SwizzleBox::setTargetType(const DataType &targetType)
{
	//Store the target type and recreate the combo boxes.
	_conversionOptions.targetType = targetType;
	recreateComboBoxes();
}
