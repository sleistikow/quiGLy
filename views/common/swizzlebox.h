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

#ifndef SWIZZLEBOX_H
#define SWIZZLEBOX_H

#include "data/types/types.h"

#include <QWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>

namespace ysm
{

	//! \brief User control that helps specifying a swizzle configuration.
	class SwizzleBox : public QWidget
	{
		Q_OBJECT

	public:

		//! \brief Initialize new instance.
		SwizzleBox();

		//! \brief Destruct instance.
		virtual ~SwizzleBox();

		/*!
		 * \brief Add a new dimension to swizzle.
		 * \param dimension The dimension's user readable name.
		 */
		void addDimension(const QString& dimension);

		/*!
		 * \brief Set the source type.
		 * The source type defines the incoming type and is defined by the incoming connection.
		 * \param sourceType The source type.
		 */
		void setSourceType(const DataType& sourceType);

		/*!
		 * \brief Set the target type.
		 * The target type defines the outgoing type and can be defined by the user.
		 * \param targetType The target type.
		 */
		void setTargetType(const DataType& targetType);

		/*!
		 * \brief Get the current swizzling.
		 * \return The type conversion that is currently defined by the swizzlebox.
		 */
		TypeConversion::ConversionOptions getSwizzling() const;

		/*!
		 * \brief Set the current swizzling.
		 * \param swizzling The type conversion that should be shown by the swizzlebox.
		 */
		void setSwizzling(const TypeConversion::ConversionOptions& swizzling);

	signals:

		//! \brief Swizzling configuration was changed.
		void swizzled();

	protected:

		/*!
		 * \brief Store the conversion option, used to avoid indexing problems.
		 * \param optionIndex The option's index (x, y, z, w).
		 * \param targetValue The option's value to be set (will be converted to VectorComponent).
		 */
		void storeConversionOption(int optionIndex, int targetValue);

		/*!
		 * \brief Read the conversion option, used to avoid indexing problems.
		 * \param optionIndex The option's index (x, y, z, w).
		 * \return The option's current value (converted from VectorComponent).
		 */
		int readConversionOption(int optionIndex) const;

		//! \brief Recreate the combo boxes.
		void recreateComboBoxes();

	protected slots:

		//! \brief A combo box changed it's value.
		void swizzle();

	private:

		//! \brief Source type.
		DataType _sourceType;

		//! \brief The current conversion.
		TypeConversion::ConversionOptions _conversionOptions;

		//! \brief The internal layout.
		QFormLayout* _formLayout;

		//! \brief The dimensions.
		QList<QString> _dimensions;

		//! \brief The labeling for the combo boxes.
		QList<QLabel*> _boxLabels;

		//! \brief The combo boxes used for swizzling.
		QList<QComboBox*> _comboBoxes;
	};

}

#endif // SWIZZLEBOX_H
