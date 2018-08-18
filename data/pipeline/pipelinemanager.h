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

#ifndef PIPELINEMANAGER_H
#define PIPELINEMANAGER_H

#include <QObject>
#include "data/ipipelinemanager.h"
#include "data/iblock.h"
#include "pipelineobjectfactory.h"
#include "pipeline.h"
#include "data/cache/cachepool.h"

namespace ysm
{
	class PipelineList;
	class PipelineVisitor;

	/**
	 * @brief Manages a group of pipelines.
	 * Note that a pipeline item ID is unique between multiple pipeline manager's. Therefore all manager's register in
	 * a static list. There are static methods to provide access.
	 */
	class PipelineManager : public QObject, public IPipelineManager
	{
		Q_OBJECT

	public:

		/**
		 * @brief Initialize new instance.
		 * @param parent The parent instance.
		 */
		explicit PipelineManager(QObject* parent = 0);

		/// @brief Destruct instance.
		virtual ~PipelineManager();

	public:

		/**
		 * @brief Searches globally through all pipelines for the item with the given identifier.
		 * @param itemId The item's identifier.
		 * @return The item matching the given identifier or NULL.
		 */
		static IPipelineItem* findGlobalPipelineItem(const PipelineItemID itemId);

		/**
		 * @brief Searches all pipelines for the block with the given identifier.
		 * @param itemId The block's identifier.
		 * @return The block matching the given identifier or NULL.
		 */
		static Block* findGlobalBlock(const PipelineItemID itemId);

		/**
		 * @brief Searches all pipelines for the port with the given identifier.
		 * @param itemId The port's identifier.
		 * @return The port matching the given identifier or NULL.
		 */
		static Port* findGlobalPort(const PipelineItemID itemId);

		/**
		 * @brief Searches all pipelines for the connection with the given identifier.
		 * @param itemId The connection's identifier.
		 * @return The connection matching the given identifier or NULL.
		 */
		static Connection* findGlobalConnection(const PipelineItemID itemId);

		/**
		 * @brief Searches all pipelines for the render command with the given identifier.
		 * @param itemId The render command's identifier.
		 * @return The render command matching the given identifier or NULL.
		 */
		static RenderCommand* findGlobalRenderCommand(const PipelineItemID itemId);

		/**
		 * @brief Takes a global visitor and applies it to all pipelines of all managers.
		 * @param visitor The visitor to execute.
		 * @return True on success.
		 */
		static bool takeGlobalVisitor(PipelineVisitor* visitor);

		/**
		 * @brief Takes a visitor and applies it to all pipelines of this manager.
		 * @param visitor The visitor to execute.
		 * @return True on success.
		 */
		bool takeVisitor(PipelineVisitor* visitor);

	public:

		/**
		 * @brief Gets a list of all pipelines of this manager.
		 * @return List of pipelines.
		 */
		PipelineList* getPipelineList();

		/**
		 * @brief Gets the pipeline manager's object factory.
		 * @return The object factory.
		 */
		PipelineObjectFactory* getObjectFactory();

		/**
		 * @brief Gets the pipeline manager's cache pool.
		 * @return The cache pool.
		 */
		CachePool* getCachePool();

	public:

		/**
		 * @brief Requires a new free and globally available item identifier.
		 * @return The item identifier.
		 */
		static PipelineItemID requestNextItemID();

		/**
		 * @brief Gets the currently highest item identifier.
		 * @return The item identifier.
		 */
		static PipelineItemID getHighestItemID();

		/**
		 * @brief Adjusts the currently highest item identifier.
		 * @param identifier The new identifier to set.
		 * @param higherOnly If true, changes are only applied if the new identifier is higher than the current one.
		 */
		static void setHighestItemID(const PipelineItemID identifier, bool higherOnly = false);

	public:

		/**
		 * @brief Gets all pipelines.
		 * @return List of all pipelines.
		 */
		QVector<IPipeline*> getPipelines() const override;

		/**
		 * @brief Adds a new pipeline.
		 * @return The newly created pipeline.
		 */
		Pipeline* addPipeline() override;

		/**
		 * @brief Deletes an existing pipeline.
		 * @pipeline The pipeline.
		 */
		void deletePipeline(const IPipeline* pipeline) override;

		/// @brief Resets all pipeline data.
		void reset() override;

	public:

		/**
		 * @brief Serialize the pipeline manager and all it's pipelines.
		 * @param root The root XML element.
		 * @param context The context.
		 */
		void serialize(QDomElement* root, SerializationContext* context) const override;

		/**
		 * @brief Deserialize the pipeline manager and all it's pipelines.
		 * @param root The root XML element.
		 * @param context The context.
		 */
		void deserialize(const QDomElement* root, SerializationContext* context) override;

	private:

		/// @brief List of all pipelines.
		PipelineList* _pipelines;

		/// @brief Object factory used to create new objects.
		PipelineObjectFactory _objectFactory;

		/// @brief Cache pool holding cache data.
		CachePool _cachePool;

	private:

		/// @brief The currently highest item identifier.
		static PipelineItemID _highestItemID;

		/// @brief List of all registered managers.
		static QList<PipelineManager*> _registry;
	};
}

#endif
