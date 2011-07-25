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
#include "sumoverrasterdata.h"

VIBe_DECLARE_NODE_NAME( SumOverRasterData, Modules)

SumOverRasterData::SumOverRasterData()
{
   this->multiplier = 1;
    this->addParameter("Sum", vibens::VIBe2::DOUBLEDATA_OUT, & this->d);
    this->addParameter("RasterData", vibens::VIBe2::RASTERDATA_IN, & this->r);
    this->addParameter("Multiplier", VIBe2::DOUBLE, &this->multiplier);
}

void SumOverRasterData::run() {
    d = 0;
    long height;
    long width;

    height = this->r->getHeight();
    width = this->r->getWidth();

    for (long i = 0; i < height; i++) {
        for (long j = 0; j < width; j++) {
            d+=this->r->getValue(j,i)*multiplier;
        }
    }


}
