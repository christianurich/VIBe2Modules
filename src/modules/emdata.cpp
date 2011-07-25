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
#include "emdata.h"
#include "iostream"
#include "rasterdata.h"
VIBe_DECLARE_NODE_NAME( EMData,Modules )
EMData::EMData() {
}

boost::shared_ptr<Module> EMData::clone() const {
    return boost::shared_ptr<Module>(new EMData);
}
void EMData::run() {
    std::cout << "Hello from Example Module Data" << std::endl;
    RasterData Topo(50,100,20);
    Topo.setValue(1,1,10);
    this->setRasterData("Topo", Topo);
    std::cout << "Topo " << Topo.getValue(1,1) << std::endl;
    RasterData PopDensity = this->getRasterData("PopDensity");
    std::cout << "Value from Python " <<  PopDensity.getValue(1,1) << std::endl;
}
