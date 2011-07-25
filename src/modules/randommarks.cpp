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
#include "randommarks.h"
#include <QString>
#include <QThread>
VIBe_DECLARE_NODE_NAME( RandomMarks,Modules )
boost::shared_ptr<Module> RandomMarks::clone() const {
    return boost::shared_ptr<Module>(new RandomMarks);
}

RandomMarks::RandomMarks()
{
}
void RandomMarks::run() {
    RasterData rData(param.Width, param.Height, param.CellSize);
    for ( long i = 0; i < param.NumberOfPoints; i++ ) {
       long x =  (long) rand() % param.Width;
       long y =  (long) rand() % param.Height;
       rData.setValue(x,y,1);

    }


    this->setRasterData(param.Map, rData);

}
void RandomMarks::init(const parameter_type &parameters) {
    if (parameters.find("Width") != parameters.end()) {
        param.Width = QString::fromStdString(parameters.at("Width")).toLong();
    } else {
        std::cout << "Error: parameter Width not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("Height") != parameters.end()) {
        param.Height = QString::fromStdString(parameters.at("Height")).toLong();
    } else {
        std::cout << "Error: parameter Width not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("CellSize") != parameters.end()) {
        param.CellSize = QString::fromStdString(parameters.at("CellSize")).toDouble();
    } else {
        std::cout << "Error: parameter CellSize not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("Map") != parameters.end()) {
        param.Map = parameters.at("Map");
    } else {
        std::cout << "Error: parameter Map not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("NumberOfPoints") != parameters.end()) {
        param.NumberOfPoints = QString::fromStdString( parameters.at("NumberOfPoints") ).toLong();
    } else {
        std::cout << "Error: parameter Map not set" << std::endl;
        QThread::currentThread()->exit();;
    }
}
