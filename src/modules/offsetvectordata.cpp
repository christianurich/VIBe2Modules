/**
 * @file
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
#include "offsetvectordata.h"
VIBe_DECLARE_NODE_NAME(OffsetVectorData, Modules)

OffsetVectorData::OffsetVectorData()
{
    this->offsetX = 0;
    this->offsetY = 0;

    this->addParameter("vec_in", VIBe2::VECTORDATA_IN, & this->vec_in);
    this->addParameter("vec_out", VIBe2::VECTORDATA_OUT, & this->vec_out);
    this->addParameter("OffsetX", VIBe2::DOUBLE, & this->offsetX);
    this->addParameter("OffsetY", VIBe2::DOUBLE, & this->offsetY);




}

void OffsetVectorData::run() {
    *vec_out = *vec_in;
    Point offsetp(this->offsetX , this->offsetY, 0);
    Logger(Debug) << this->offsetX;
    Logger(Debug) << this->offsetY;

    std::vector<std::string> names = this->vec_out->getPointsNames();
    foreach (std::string name, names) {
        std::vector<Point>points = this->vec_out->getPoints(name);
        for (int i = 0; i < points.size(); i++) {
            points[i] = points[i]+offsetp;
        }
        this->vec_out->setPoints(name, points);
    }
}
