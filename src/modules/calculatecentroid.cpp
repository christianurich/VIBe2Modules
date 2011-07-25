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


#include "calculatecentroid.h"
#include "vectordatahelper.h"

VIBe_DECLARE_NODE_NAME(CalculateCentroid, Modules)

CalculateCentroid::CalculateCentroid()
{
    this->Identifier = "";
    this->addParameter("Identifier", VIBe2::STRING, & this->Identifier);
    this->addParameter("Input", VIBe2::VECTORDATA_IN, & this->input);
    this->addParameter("Output", VIBe2::VECTORDATA_OUT, & this->output);
}

void CalculateCentroid::run() {
    *(output) = *(input);
    std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(this->Identifier, this->input->getFaceNames());

    foreach(std::string name, names) {
        std::vector<Face> faces = this->input->getFaces(name);
        std::vector<Point> points = this->input->getPoints(name);
        Attribute attr = this->input->getAttributes(name);
        Point p;
        double area = 0;
        foreach (Face f, faces) {
            p = VectorDataHelper::caclulateCentroid(f, points);
            area+= abs(VectorDataHelper::calculateArea(f, points));

        }
        attr.setAttribute("centroid_x", p.x);
        attr.setAttribute("centroid_y", p.y);
        attr.setAttribute("area", area);
        this->output->setAttributes(name, attr);

    }
}
