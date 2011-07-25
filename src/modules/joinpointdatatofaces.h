/**
 * @file joinpointdatatofaces.h
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef JOINPOINTDATATOFACES_H
#define JOINPOINTDATATOFACES_H
#include "module.h"

using namespace vibens;

/** @brief Joins point data and their attributes to the intersection faces.
  *
  * As input vectordata of the points and the faces are requiered. The module
  * Intersects every single face with the point dataset. The attributes of the points that are inside the polygon
  * are summed up and joined the the face and send to the output vector.
  *
  * @ingroup Modules
  * @author Christian Urich
  *
  */
class VIBE_HELPER_DLL_EXPORT JoinPointDataToFaces : public Module
{
    VIBe_DECLARE_NODE(JoinPointDataToFaces)
private:
       VectorData * pointData;
       VectorData * faces;
       VectorData * faces_out;
       std::string IdentifierPoints;
       std::string IdentifierFaces;
public:
    JoinPointDataToFaces();
    void run();
};

#endif // JOINPOINTDATATOFACES_H
