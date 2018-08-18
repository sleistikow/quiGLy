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

#ifndef GLSLTEXTEDITLINEBAR_H
#define GLSLTEXTEDITLINEBAR_H

#include <QWidget>

namespace ysm
{
	class IGLSLLineBarDelegate;

	/*!
	 * \brief Line bar widget, which paints a line bar.
	 * The displayed data is retrieved from a delegate, which is usually the text edit field, as it knows about the
	 * lines and blocks.
	 */
	class GLSLLineBar : public QWidget
	{
		Q_OBJECT

	public:

		//! \brief Initialize new instance.
		explicit GLSLLineBar(QWidget* parent = 0);

		/*!
		 * \brief Gets the line bar's GLSLLineBarDelegate, which is responsible for providing the data.
		 * \returns The line bar's delegate.
		 */
		IGLSLLineBarDelegate* getDelegate() const;

		/*!
		 * \brief Sets the line bar's GLSLLineBarDelegate, which is responsible for providing the data.
		 * \param blocks The line bar's delegate.
		 */
		void setDelegate(IGLSLLineBarDelegate* delegate);

	protected:

		/*!
		 * \brief Paint the line bar.
		 * \param event The event args.
		 */
		void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

		/*!
		 * \brief Calculates the maximum line number's digit count.
		 * \return The number of digits in the highest line number.
		 */
		int getLineNumberDigitCount() const;

	protected slots:

		//! \brief Updates the line bar.
		void updateLineBarBlocks();

	private:

		//! \brief The line bar's GLSLLineBarDelegate, which is responsible for providing the data.
		IGLSLLineBarDelegate* _delegate;
	};
}

#endif // GLSLTEXTEDITLINEBAR_H
