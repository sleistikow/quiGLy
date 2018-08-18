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

#ifndef EVALUATIONUTILS_H
#define EVALUATIONUTILS_H

#include <QOpenGLFunctions>
#include <QString>

#include "data/types/gltypes.h"
#include "data/blocks/blocktype.h"

namespace ysm
{
	class EvaluationUtils
	{
	public:
		// To String - to be used by CodeGenerationEvaluator
		static QString mapTextureTargetToString(int value);
		static QString mapFilterTypeToString(int value);
		static QString mapWrapModeToString(int value);
		static QString mapUsagePatternToString(int frequency, int access);
		static QString mapDataTypeToString(GLDataType value);
		static QString mapAttachmentToString(int value, unsigned int increment);
		static QString mapFrameBufferTargetToString(int value);
		static QString mapInternalFormatToString(int value);
		static QString mapBufferModeToString(int value);
		static QString mapShaderTypeToString(BlockType value);
		static QString mapFunctionTypeToString(int value);
		static QString mapTextureModeTypeToString(int value);
		static QString mapSwizzleTypeToString(int value);
		static QString mapCubeMapSideToString(int value);
		static QString mapPixelDataFormatToString(int value);
		static QString mapPixelDataTypeToString(int value);


		// To OpenGL - to be used by SetupRenderingEvaluator
		static GLenum mapTextureTargetToOpenGL(int value);
		static GLint  mapFilterTypeToOpenGL(int value);
		static GLint  mapWrapModeToOpenGL(int value);
		static GLenum mapUsagePatternToOpenGL(int frequency, int access);
		static GLenum mapDataTypeToOpenGL(GLDataType value);
		static GLenum mapAttachmentToOpenGL(int value, unsigned int increment);
		static GLenum mapFrameBufferTargetToOpenGL(int value);
		static GLenum mapInternalFormatToOpenGL(int value);
		static GLenum mapBufferModeToOpenGL(int value);
		static GLenum mapShaderTypeToOpenGL(BlockType value);
		static GLint  mapFunctionTypeToOpenGL(int value);
		static GLenum mapTextureModeTypeToOpenGL(int value);
		static GLint  mapSwizzleTypeToOpenGL(int value);
		static GLenum mapCubeMapSideToOpenGL(int value);
		static GLenum mapPixelDataFormatToOpenGL(int value);
		static GLenum mapPixelDataTypeToOpenGL(int value);


	private:

		// Static class
		EvaluationUtils();

	};

}

#endif // EVALUATIONUTILS_H
