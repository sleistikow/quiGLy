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

#ifndef PIPELINEPROJECTCONTEXT_H
#define PIPELINEPROJECTCONTEXT_H

#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QMultiMap>
#include <QPair>
#include <QDir>

namespace ysm
{
	class ISerializable;
	class IPipelineManager;

	/// @brief Context class for serialization. Stores global information.
	class SerializationContext
	{
	public:

		/// @brief The deferring priority.
		enum class DeferringPriority
		{
			Lowest,
			Low,
			Normal,
			High,
			Highest,
		};

		/// @brief The file path adjustment mode.
		enum class PathAdjustMode
		{
			Load,
			Keep,
			Move,
		};

	public:

		/**
		 * @brief Initialize new instance.
		 * @param document The target document.
		 * @param root The root element.
		 */
		explicit SerializationContext(QDomDocument* document, QDomElement* root);

		/**
		 * @brief Initialize the context.
		 * @param adjustMode The path adjustment mode.
		 * @param baseDirectory The base directory.
		 * @param assetsDirectory Assets directory if exists.
		 * @param offsetID Possible offset id.
		 * @param importMode True, if imported into existing document.
		 */
		void initContext(PathAdjustMode adjustMode, QDir baseDirectory, QDir assetsDirectory,
						 unsigned int offsetID = 0, bool importMode = false);

	public:

		/**
		 * @brief Get the base directory.
		 * @return The base directory.
		 */
		QDir getBaseDirectory() const;

		/**
		 * @brief Gets the project assets directory.
		 * @return The assets directory.
		 */
		QDir getAssetsDirectory() const;

		/**
		 * @brief Gets the XML document.
		 * @return the XML document.
		 */
		QDomDocument* getDocument();

		/**
		 * @brief Gets the XML root element.
		 * @return the XML root element.
		 */
		QDomElement* getRoot();

		/**
		 * @brief Gets the ID offset, if available.
		 * @return The offset ID or zero.
		 */
		unsigned int getOffsetID() const;

		/*!
		 * \brief Gets the path adjustment mode.
		 * \return The path adjustment mode.
		 */
		PathAdjustMode getPathAdjustMode() const;

		/**
		 * @brief Checks, if the the data is imported into an existing document or opened as a new one.
		 * @return True, if the data is imported.
		 */
		bool isImportMode() const;

	public:

		/**
		 * @brief Gets the unique ID for given object.
		 * If the object has not been pooled, the ID is generated and stored.
		 * @param serializableObject The serializable object.
		 * @return The unique ID.
		 */
		unsigned int getObjectID(ISerializable* serializableObject);

		/**
		 * @brief Manually sets the unique ID for given object and pools the object.
		 * If an offset ID is given, the object's ID will be adjusted.
		 * @param serializableObject The serializable object.
		 * @param objectID The unique ID.
		 * @return The adjusted unique ID.
		 */
		unsigned int setObjectID(ISerializable* serializableObject, const unsigned int objectID);

		/**
		 * @brief Extract the unique ID for the given object from the given element.
		 * If an offset ID is given, the object's ID will be adjusted.
		 * @param serializableObject The serializable object.
		 * @param element The element holding the unique ID.
		 * @return The adjusted unique ID.
		 */
		unsigned int setObjectID(ISerializable* serializableObject, const QDomElement* element);

		/**
		 * @brief Finds the object that matches the given ID.
		 * @param objectID The object's unique ID without possible offset adjustment.
		 * @return The serializable object.
		 */
		ISerializable* findObject(unsigned int objectID) const;

		/**
		 * @brief Finds the first object that matches the given type.
		 * @return The serializable object as T.
		 */
		template<typename T> T* findObject() const
		{
			// Iterate over all available objects, until a match is found.
			for (auto it = _objectIDs.cbegin(); it != _objectIDs.cend(); ++it)
			{
				// Try to cast the object, if possible return it.
				T* castedObject = dynamic_cast<T*>(it.value());
				if (castedObject)
					return castedObject;
			}

			// No object was found.
			return nullptr;
		}

		/**
		 * @brief Add all IDs from the given pipeline manager to the ID pool.
		 * @param pipelineManager The pipeline manager.
		 */
		void poolPipelineItems(IPipelineManager* pipelineManager);

	public:

		/**
		 * @brief Adds the given object to the deferred deserialization queue.
		 * @param serializableObject The serializable object.
		 * @param element The element to deserialize later.
		 * @param priority The priority in the queue.
		 */
		void deferElement(ISerializable* serializableObject, const QDomElement& element, DeferringPriority priority);

		/// @brief Process all deferred elements.
		void processDeferredElements();

		/**
		 * @brief Checks if the deferred elements are being processed.
		 * @return True, if processing deferred elements.
		 */
		bool isProcessingDeferredElements() const;

	public:

		/**
		 * @brief Resolves a filename according to the given path adjustment mode.
		 * @param sourceFile The original file name.
		 * @return The adjusted file name.
		 */
		QString resolveFilename(QString sourceFile) const;


		/**
		 * @brief Gets all serialization messages.
		 * @return List of all messages that occured during serialization.
		 */
		QStringList getMessages() const;

		/**
		 * @brief Adds a serialization message.
		 * @param message The message.
		 */
		void addMessage(const QString& message);

	public:

		/**
		 * @brief Gets all child-elements of the given parent with matching name.
		 * @param parent The parent element.
		 * @param name The name to match against.
		 * @return List of all elements with the given name in parent.
		 */
		QVector<QDomElement> getElements(const QDomElement* parent, const QString& name) const;

		/**
		 * @brief Asserts that the given element has all gievn attributes.
		 * @param element The element to check.
		 * @param attributes The required attributes.
		 * @return True, if all attributes are available.
		 */
		bool assertAttributes(const QDomElement* element, const QStringList& attributes);

	public:

		/**
		 * @brief Creates a new XML element.
		 * @param name The element's name.
		 * @param serializableObject If specified, the element will have the object's ID.
		 * @return The created XML element.
		 */
		QDomElement createElement(const QString& name, ISerializable* serializableObject = nullptr);

		/**
		 * @brief Creates a new text XML element.
		 * @param text The text.
		 * @return The created XML element.
		 */
		QDomText createTextElement(const QString& text);

	private:

		/**
		 * @brief Process all deferred elements of the given priority.
		 * @param priority The priority.
		 */
		void processDeferredElements(DeferringPriority priority);

	private:		

		/// The XML document.
		QDomDocument* _xmlDocument{nullptr};

		/// The XML root element.
		QDomElement* _xmlRoot{nullptr};

		/// The base directory.
		QDir _baseDirectory;

		/// The assets directory.
		QDir _assetsDirectory;

		/// The path adjustment mode.
		PathAdjustMode _adjustMode;

		/// The object ID pool, indexing all available IDs by the matching object.
		QMap<ISerializable*, unsigned int> _objectIDs;

		/// The last used ID.
		unsigned int _currentID{0};

		/// The offset ID.
		unsigned int _offsetID{0};

		/// The working mode.
		bool _importMode{false};

		/// List of all deferred elements, indexed by priority.
		using DeferredElement = QPair<ISerializable*, QDomElement>;
		QMultiMap<DeferringPriority, DeferredElement> _deferredElements;

		/// Flag identifying the current deferring state.
		bool _processingDeferredElements{false};

		/// List of all messages thrown during serialization.
		QStringList _messages;
	};
}

#endif
