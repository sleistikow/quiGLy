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

#ifndef COLORSELECT_H
#define COLORSELECT_H

#include <QPushButton>
#include <QLineEdit>
#include <QVector4D>

namespace ysm
{

	//! \brief Color selection widget.
	class ColorSelect : public QWidget
	{
		Q_OBJECT

		Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
		Q_PROPERTY(QVector4D colorVector READ getColorVector WRITE setColorVector NOTIFY colorChanged)
		Q_PROPERTY(QString serializedColor READ serializeColor WRITE deserializeColor NOTIFY colorChanged)

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent widget.
		 */
		ColorSelect(QWidget* parent);

		/*!
		 * \brief Gets the current color.
		 * \return The color.
		 */
		QColor getColor() const;

		/*!
		 * \brief Sets the current color.
		 * \param color The color.
		 */
		void setColor(const QColor& color);

		/*!
		 * \brief Gets the color as vector.
		 * Used by the project wizard.
		 * \return The color vector.
		 */
		QVector4D getColorVector() const;

		/*!
		 * \brief Sets the color as vector.
		 * Used by the project wizard.
		 * \param vector The color vector.
		 */
		void setColorVector(const QVector4D& vector);

		/*!
		 * \brief Serialize the color to string.
		 * \return The string (R;G;B;A).
		 */
		QString serializeColor() const;

		/*!
		 * \brief Deserialize the color from string.
		 * \param color The string (R;G;B;A).
		 */
		void deserializeColor(const QString& color);

		/*!
		 * \brief Get the desired size.
		 * \return The desired size.
		 */
		QSize sizeHint() const Q_DECL_OVERRIDE;

	signals:

		//! \brief Notify about color changes.
		void colorChanged();

	protected slots:

		//! \brief Display the color select dialog.
		void pickColor();

	private:

		//! \brief The current color.
		QColor _color;

		//! \brief The button to select the color.
		QPushButton* _selectButton;

		//! \brief The text edit containing the color.
		QLineEdit* _colorEdit;
	};

}

#endif // COLORSELECT_H
