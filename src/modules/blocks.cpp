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
#include "blocks.h"

VIBe_DECLARE_NODE_NAME( Blocks,Modules )


Blocks::Blocks() {
    width = 2000;
    height = 2000;
    size = 100;

    this->addParameter("Width", VIBe2::LONG, &width);
    this->addParameter("Height", VIBe2::LONG, &height);
    this->addParameter("Size", VIBe2::DOUBLE, &size);
    this->addParameter("block", VIBe2::VECTORDATA_OUT, &block);

}

void Blocks::run () {
    long elementsX = width/size;
    long elementsY = height/size;
    int IDCounter = 0;
    for (int i = 0; i < elementsX-1; i++) {
        for (int j = 0; j < elementsY-1; j++) {
            //Points at the Edge
            Point p1(i*size, j*size, 0);
            Point p2((i+1)*size, j*size, 0);
            Point p3((i+1)*size, (j+1)*size, 0);
            Point p4(i*size, (j+1)*size, 0);            std::vector<Point> edgePoints;

            edgePoints.push_back(p1);
            edgePoints.push_back(p2);
            edgePoints.push_back(p3);
            edgePoints.push_back(p4);


            //Create Edges
            Edge e1(0,1);
            Edge e2(1,2);
            Edge e3(2,3);
            Edge e4(3,0);

            std::vector<Edge> edges;
            edges.push_back(e1);
            edges.push_back(e2);
            edges.push_back(e3);
            edges.push_back(e4);

            //Cretae Face
            std::vector<long> f;
            f.push_back(0);
            f.push_back(1);
            f.push_back(2);
            f.push_back(3);

            Face face(f);
            std::vector<Face> faces;
            faces.push_back(face);

            Point centerPoint((i+0.5)*size, (j+0.5)*size, 0);
            std::vector<Point> centerPoints;
            centerPoints.push_back(centerPoint);

            std::stringstream name;
            name << "Block_" << IDCounter;
            block->setPoints(name.str(), edgePoints);
            block->setEdges(name.str(), edges);
            block->setFaces(name.str(), faces);

            name.clear();
            name << "BlockCenterPoint_" << IDCounter;
            block->setPoints(name.str(), centerPoints);

            IDCounter++;
        }
    }

}
