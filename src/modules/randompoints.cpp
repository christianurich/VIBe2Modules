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
#include "randompoints.h"
#include <QString>
#include <QThread>
#include <QTime>
VIBe_DECLARE_NODE_NAME( RandomPoints,Modules )

RandomPoints::RandomPoints() {

    param.Width = 200;
    param.Height = 200;
    param.CellSize = 20;
    param.NumberOfPoints = 1;
    this->addParameter("Width", VIBe2::LONG, & param.Width);
    this->addParameter("Height", VIBe2::LONG, & param.Height);    
    this->addParameter("CellSize", VIBe2::DOUBLE, & param.CellSize);
    this->addParameter("NumberOfPoints", VIBe2::LONG, & param.NumberOfPoints);
    this->addParameter("VectorData", VIBe2::VECTORDATA_OUT, & param.vec);


}

void RandomPoints::run() {
    std::cout << param.NumberOfPoints<< std::endl;
    srand ( QTime::currentTime ().msec() );
    //std::cout << "Running RandomPoints" << std::endl;
    std::vector<Point> points;

    for ( long i = 0; i < param.NumberOfPoints; i++ ) {
       double x =  (long) rand() % param.Width;
       long y =  (long) rand() % param.Height;
       Point p;
       p.x = x*param.CellSize +param.CellSize/2. ;
       p.y = y*param.CellSize +param.CellSize/2.;
       p.z = 0;
       points.push_back(p);

    }
    param.vec->setPoints("Points", points);


}



