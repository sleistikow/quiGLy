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

#ifndef CODEGENERATORBLOCK_H
#define CODEGENERATORBLOCK_H

#include "block.h"
#include "data/properties/filenameproperty.h"

namespace ysm
{
	/**
	 * @brief Block representing a final stage of the pipeline, being allowing
	 */
	class CodeGeneratorBlock : public Block
	{
		Q_OBJECT

	public:
		// Types
		/**
		 * @brief The supported languages
		 */
		enum Language
		{
			Language_CPlusPlus,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getLanguageNames();

		/**
		 * @brief The supported APIs
		 */
		enum APIType
		{
			API_OpenGL,
			//API_OpenGLES,
			//API_WebGL,
		};

		/**
		 * @brief Returns the string representations of the given enum
		 */
		static QMap<int, QString> getAPINames();

	public:
		static const BlockType block_type{BlockType::CodeGenerator};

	public:
		// Construction
		explicit CodeGeneratorBlock(Pipeline* parent);

	public:
		// Property access
		/**
		 * @brief Gets the visibility
		 */
		EnumProperty* getLanguage();

		/**
		 * @brief Gets the visibility
		 */
		EnumProperty* getAPI();

		/**
		 * @brief Gets the filename
		 */
		FilenameProperty* getOutFile();

		// Port access
		/**
		 * @brief Gets the single in-port
		 */
		Port* getGenericInPort();

	protected:
		void createProperties() override;
		void createPorts() override;

	private:
		// Properties
		EnumProperty* _language{nullptr};
		EnumProperty* _api{nullptr};
		FilenameProperty* _outFile{nullptr};

		// Ports
		Port* _inPort{nullptr};
	};
}

#endif // CODEGENERATORBLOCK_H
