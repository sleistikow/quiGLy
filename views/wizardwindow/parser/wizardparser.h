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

#ifndef WIZARDPARSER_H
#define WIZARDPARSER_H

#include "../wizardfile.h"

#include <QTextStream>
#include <QString>
#include <QMap>

namespace ysm
{
	class WizardWindow;

	//! \brief Parser that generates an actual project file from project template and wizard.
	class WizardParser
	{

	private:

		//! \brief Internal enum to categorize include files.
		enum IncludeType { Default, Source, Output };

		//! \brief Internal data structure used to create ports by macros.
		struct Connect { QString _connection, _block, _port; bool _out; QMap<QString, QString> _properties; };

		//! \brief Internal data structure used to evaluate sources.
		struct Include { QFile* _file; QTextStream* _stream; IncludeType _type; QString _name, _output; };

		//! \brief Internal data structure used to evaluate eachs.
		struct Each { QString _name, _current; int _seek, _counter, _index; };

	public:

		//! \brief Initialize new instance.
		WizardParser();

		/*!
		 * \brief Append wizard data to the project.
		 * \param wizardWindow The wizard window.
		 */
		void appendWizard(const WizardWindow* wizardWindow);

		/*!
		 * \brief Finish the project.
		 * \param targetFile The target file.
		 * \return True on success.
		 */
		bool finishProject(const QString& targetFile);

	protected:

		/*!
		 * \brief Adjusts the item name according to this/each/...
		 * \param name The item name.
		 * \return The adjusted name.
		 */
		QString adjustName(QString name);

		/*!
		 * \brief Return or genrate an ID with given index.
		 * \param index The index or an empty string.
		 * \return The generated ID.
		 */
		QString requireID(QString index = QString(), const QString& minor = QString());

		/*!
		 * \brief Retrieves the correct field name or value for the given name.
		 * \param name The field's name, which is adjusted.
		 * \return The field's value.
		 */
		QVariant getField(const QString& name);

	protected:

		/*!
		 * \brief Try to read the next line or pop the stream.
		 * \param currentLine The line's content.
		 * \return True, if line could be read.
		 */
		bool readLine(QString& currentLine);

		/*!
		 * \brief Include the given file into the parsing process.
		 * \param includePath The path.
		 * \param includeType The file's type.
		 */
		void includeFile(const QString& includePath, IncludeType includeType = Default);

	protected:

		/*!
		 * \brief Parse sources in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseSources(QString& currentLine);

		/*!
		 * \brief Parse outputs in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseOutputs(QString& currentLine);

		/*!
		 * \brief Parse includes in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseIncludes(QString& currentLine);

		/*!
		 * \brief Parse conditionals in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseConditionals(QString& currentLine);

		/*!
		 * \brief Parse loops in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseLoops(QString& currentLine);

		/*!
		 * \brief Parse identifiers in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseIdentifiers(QString& currentLine);

		/*!
		 * \brief Parse variables in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseVariables(QString& currentLine);

		/*!
		 * \brief Parse connects in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parseConnects(QString& currentLine);

		/*!
		 * \brief Parse ports in the current line.
		 * \param currentLine The current line.
		 * \return False if current line should be skipped.
		 */
		bool parsePorts(QString& currentLine);

	protected:

		/*!
		 * \brief Expand ports macro in the given buffer.
		 * \param currentLine The current line.
		 * \param outputStream The stream to use.
		 * \return True if the line was handled.
		 */
		bool expandPorts(const QString& currentLine, QTextStream& outputStream);

	private:

		//! \brief Temporary buffer stream.
		QTextStream* _bufferStream;
		QString* _bufferString;

		//! \brief The completed wizard to retrieve the data from.
		const WizardWindow* _wizardWindow;

		//! \brief Next free pipeline item ID.
		int _nextID;

		//! \brief All search paths used for includes.
		QStringList _searchPaths;

		//! \brief Sources and outputs search path.
		QString _sourcesPath;
		QString _outputsPath;

		//! \brief Stack used to evaluate includes.
		QList<Include> _includeStack;

		//! \brief Map of all includes that were marked as once.
		QMap<QString, QString> _includeOnces;

		//! \brief Lists of item identifiers.
		QMap<QString, QString> _itemIDs;

		//! \brief Conditional stack to evaluate if/else.
		//! - 0: Current conditional has not been matched.
		//! - 1: Current conditional is matched.
		//! - 2: Current conditional was matched.
		QList<int> _conditionalStack;

		//! \brief Each stack to evaluate loops.
		QList<Each> _eachStack;

		//! \brief Data structure used to create ports.
		using ConnectData = QMap<QString, QList<Connect>>;
		QMap<QString, ConnectData> _connects;
	};

}

#endif // WIZARDPARSER_H
