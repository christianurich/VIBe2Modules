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
#include "copyrastermap.h"
#include <QThread>
VIBe_DECLARE_NODE_NAME( CopyRasterMap,Modules )

CopyRasterMap::CopyRasterMap()
{
}
CopyRasterMap::~CopyRasterMap()
{
}

void CopyRasterMap::run() {
    std::cout << "Running Copy RasterMap" << std::endl;
    for ( std::vector<std::string>::const_iterator it=param.Copies.begin(); it != param.Copies.end(); ++it) {

        this->setRasterData(*it, this->getRasterData(param.Original));
        std::cout <<*it << "\t <-" << param.Original << std::endl;
    }
}
void CopyRasterMap::init(const parameter_type &parameters) {

    if (parameters.find("Original") != parameters.end()) {
        param.Original = parameters.at("Original");
    } else {
        std::cout << "Error: parameter Original not set" << std::endl;
        QThread::currentThread()->exit();
    }
    if (parameters.find("Copies") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("Copies"));

        while (ss >> buf)
            param.Copies.push_back(buf);
    }

}
boost::shared_ptr<Module> CopyRasterMap::clone() const {
    return boost::shared_ptr<Module>(new CopyRasterMap);
}


