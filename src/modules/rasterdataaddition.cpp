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
#include "rasterdataaddition.h"

VIBe_DECLARE_NODE_NAME( RasterDataAddition, Modules )


        RasterDataAddition::RasterDataAddition()
{
    this->addParameter("Landscapes", VIBe2::USER_DEFINED_RASTERDATA_IN, &this->landscapes);
    this->addParameter("OutputMap", VIBe2::RASTERDATA_OUT, &this->output);
}

void RasterDataAddition::run() {
    if (this->landscapes.size() == 0) {
        Logger(Warning) << "No RasterData for Addition";
        return;
    }
    double cellsize = landscapes.begin()->second->getCellSize();
    long width = landscapes.begin()->second->getWidth();
    long height = landscapes.begin()->second->getHeight();
    this->output->setSize(width, height,cellsize);
    this->output->clear();
    for (long i = 0; i < width; i++) {
        for (long j = 0; j < height; j++) {
            double val;
            val = 0;
            for (std::map<std::string, RasterData *>::iterator it = landscapes.begin(); it != landscapes.end(); ++it) {
                RasterData * r = it->second;
                double val_tmp =  r->getValue(i,j);
                if (r->getNoValue() != val_tmp)
                    val+=val_tmp;
            }
            output->setValue(i,j,val);
        }
    }

}


