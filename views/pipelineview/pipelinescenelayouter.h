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

#ifndef PIPELINESCENELAYOUTER_H
#define PIPELINESCENELAYOUTER_H

#include "commands/ichangeable.h"
#include "data/iblock.h"
#include "data/irendercommand.h"
#include "data/ipipelineitem.h"
#include "data/iserializable.h"

#include <QObject>
#include <QPointF>
#include <QMap>

namespace ysm
{

	//! \brief Responsible for layouting pipeline scenes. Stores the layout information scene independently.
	class PipelineSceneLayouter : public QObject, public ISerializable, public IChangeable
	{
		Q_OBJECT

	private:

		//! \brief Contains layout information about a single pipeline item.
		struct LayoutInfo
		{
			//! \brief The item's positions.
			QPointF _previewPosition, _basePosition, _currentPosition, _autoPosition;

			//! \brief Logical auto layout position.
			QPoint _logicalPosition;

			//! \brief Block that defines the item's auto layout anchor.
			IBlock* _rootBlock;

			//! \brief Wether the item is selected or not.
			bool _selected;
		};

	public:

		/*!
		 * \brief Initialize new instance.
		 * \param parent The parent.
		 */
		explicit PipelineSceneLayouter(QObject* parent = 0);

		/*!
		 * \brief Returns the position of the given item. If the item is not found, it's registered at default position.
		 * \param pipelineItem The pipeline item.
		 * \param fallbackPosition The position that is used, in case the item is not yet registered.
		 * \return The item's position, which might be adjusted to the layouter's restrictions.
		 */
		QPointF getItemPosition(IPipelineItem* pipelineItem, QPointF fallbackPosition);

		/*!
		 * \brief Returns the item's latest calculated auto layout position.
		 * \param pipelineItem The pipeline item.
		 * \return The item's auto layout position.
		 */
		QPointF getAutoLayoutItemPosition(IPipelineItem* pipelineItem);

		/*!
		 * \brief Sets the item's position.
		 * \param pipelineItem The pipeline item.
		 * \param position The position.
		 * \return The item's position, which might be adjusted to the layouter's restrictions.
		 */
		QPointF setItemPosition(IPipelineItem* pipelineItem, QPointF position);

		/*!
		 * \brief Move the item's position by the given offset.
		 * Uses the position as anchor and clears the exact data.
		 * \param pipelineItem The pipeline item.
		 * \param offset The offset.
		 * \param previewOnly If preview mode is enabled, the base position is kept and can be restored.
		 * \return The item's position, which might be adjusted to the layouter's restrictions.
		 */
		QPointF moveItem(IPipelineItem* pipelineItem, QPointF offset, bool previewOnly);

		/*!
		 * \brief Returns the selection state of the given item.
		 * \param pipelineItem The pipeline item.
		 * \return The item's selection state.
		 */
		bool getItemSelected(IPipelineItem* pipelineItem);

		/*!
		 * \brief Sets the item's selection state.
		 * \param pipelineItem The pipeline item.
		 * \param selected The selection state.
		 * \return The item's selection state, which might be adjusted to the layouter's restrictions.
		 */
		bool setItemSelected(IPipelineItem* pipelineItem, bool selected);

		/*!
		 * \brief Auto layout the given pipeline.
		 * NOTE: This stores auto layout information for every item in the pipeline, but does not yet set the actual
		 * position. This needs to be done manually.
		 * \param pipeline The pipeline.
		 */
		void updateAutoLayout(IPipeline* pipeline);

		/*!
		 * \brief Unregisters a block from the layouter.
		 * \param pipelineItem The pipeline item.
		 */
		void unregisterItem(IPipelineItem* pipelineItem);

	public:

		/*!
		 * \brief Serialize the layout.
		 * \param base The base XML element.
		 * \param context The context.
		 */
		void serialize(QDomElement* base, SerializationContext* context) const override;

		/*!
		 * \brief Deserialize the layout.
		 * \param base The base XML element.
		 * \param context The context.
		 */
		void deserialize(const QDomElement* base, SerializationContext* context) override;

	signals:

		//! \brief An item's layout data changed.
		void layoutChanged(IPipelineItem*);

	protected:

		/*!
		 * \brief Gets or creates the layout information for the given item.
		 * \param pipelineItem The pipeline item.
		 * \param layoutInfo The layout info.
		 * \return True, if the layout info was created.
		 */
		bool getLayoutInfo(IPipelineItem* pipelineItem, LayoutInfo** layoutInfo);

		/*!
		 * \brief Store the given position to the layout info and adjust it.
		 * \param layoutInfo The layout info.
		 * \param position The position.
		 */
		void setItemPosition(LayoutInfo* layoutInfo, QPointF position, bool previewOnly);

		/*!
		 * \brief Auto layout all blocks, that are connected to the source blocks.
		 * \param sourceBlocks The source blocks.
		 * \param direction The direction: -1 for left, 1 for right.
		 * \return All affected blocks.
		 */
		QSet<IBlock*> autoLayoutHorizontally(QSet<IBlock*> sourceBlocks, int direction);

		/*!
		 * \brief Layout the blocks vertically, after the horizontal layout is done.
		 * \param rootBlocks The root blocks blocks.
		 */
		void autoLayoutVertically(QSet<IBlock*> rootBlocks);

	private:

		//! \brief Contains all pipeline item positions indexed by the pipeline item id.
		QMap<IPipelineItem*, LayoutInfo*> _itemLayoutInfos;
	};

}

#endif // PIPELINESCENELAYOUTER_H
