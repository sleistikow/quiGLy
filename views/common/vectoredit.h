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

#ifndef VECTOREDIT_H
#define VECTOREDIT_H

#include <QLineEdit>
#include <QWidget>
#include <QVector4D>

namespace ysm
{

	//! \brief Allows to use multiple line edits as vector input field.
	class VectorEdit : public QWidget
	{
		Q_OBJECT

		Q_PROPERTY(QVector4D vector READ getVector WRITE setVector NOTIFY vectorChanged)
		Q_PROPERTY(QString serializedVector READ serializeVector WRITE deserializeVector NOTIFY vectorChanged)

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param dimensions The dimension count.
		 * \param parent The parent widget.
		 */
		VectorEdit(int dimensions, QWidget* parent = 0);

		/*!
		 * \brief Gets the current vector.
		 * \return The vector.
		 */
		QVector4D getVector() const;

		/*!
		 * \brief Set the current vector.
		 * \param vector The vector.
		 */
		void setVector(const QVector4D& vector);

		/*!
		 * \brief Serialize the vector to string.
		 * \return The string, depends on the divider.
		 */
		QString serializeVector() const;

		/*!
		 * \brief Deserialize the vector from string.
		 * \return vector The string, depends on the divider.
		 */
		void deserializeVector(const QString& vector);

		/*!
		 * \brief Gets the divider used in serialization.
		 * \return The divider.
		 */
		QChar getDivider() const;

		/*!
		 * \brief Sets the divider used in serialization.
		 * \param divider The divider.
		 */
		void setDivider(QChar divider);

		/*!
		 * \brief Get read only state.
		 * \return Read only state.
		 */
		bool isReadOnly() const;

		/*!
		 * \brief Set read only state.
		 * \param isReadOnly Read only state.
		 */
		void setReadOnly(bool isReadOnly);

	signals:

		//! \brief The vector changed.
		void vectorChanged();

	protected slots:

		//! \brief Update the value, if changed.
		void updateValue();

	private:

		//! \brief The dimension count.
		int _dimensions;

		//! \brief The current vector.
		QVector4D _vector;

		//! \brief The divider for serialization.
		QChar _divider;

		//! \brief The line edits.
		QList<QLineEdit*> _lineEdits;
	};

}

#endif // VECTOREDIT_H
