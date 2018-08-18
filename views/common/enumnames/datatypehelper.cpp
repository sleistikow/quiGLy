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

#include "datatypehelper.h"

using namespace ysm;

IMPLEMENT(DataType)
IMPLEMENT_GROUP(DataType, DataType, Names)
IMPLEMENT_GROUP(DataType, DataType, Conversions)

IMPLEMENTATION_START_ADD(DataType)
IMPLEMENTATION_ADD(Names, DataType::NoType, "No Type")
IMPLEMENTATION_ADD(Names, DataType::Vec2, "2D Vector")
IMPLEMENTATION_ADD(Names, DataType::Vec3, "3D Vector")
IMPLEMENTATION_ADD(Names, DataType::Vec4, "4D Vector")
IMPLEMENTATION_ADD(Names, DataType::Int, "Int")
IMPLEMENTATION_ADD(Names, DataType::UInt, "UInt")
IMPLEMENTATION_ADD(Names, DataType::Float, "Float")
IMPLEMENTATION_ADD(Conversions, DataType::NoType, "No Conversion")
IMPLEMENTATION_ADD(Conversions, DataType::Vec2, "To 2D Vector")
IMPLEMENTATION_ADD(Conversions, DataType::Vec3, "To 3D Vector")
IMPLEMENTATION_ADD(Conversions, DataType::Vec4, "To 4D Vector")
IMPLEMENTATION_ADD(Conversions, DataType::Int, "To Int")
IMPLEMENTATION_ADD(Conversions, DataType::UInt, "To UInt")
IMPLEMENTATION_ADD(Conversions, DataType::Float, "To Float")
IMPLEMENTATION_END_ADD()
