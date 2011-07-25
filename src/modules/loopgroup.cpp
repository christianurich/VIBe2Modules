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
#include "loopgroup.h"

VIBe_DECLARE_GROUP_NAME(LoopGroup, Loops)
        LoopGroup::LoopGroup() {
    i = 0;
    Runs = 1;
    this->addParameter("RasterDataIn", vibens::VIBe2::USER_DEFINED_RASTER_TUPLE_IN, &InRasterData);
    this->addParameter("RasterDataOut", vibens::VIBe2::USER_DEFINED_RASTER_TUPLE_OUT,&OutRasterData);
    this->addParameter("VectorDataIn", vibens::VIBe2::USER_DEFINED_VECTORDATA_TUPLE_IN, &InVectorData);
    this->addParameter("VectorDataOut", vibens::VIBe2::USER_DEFINED_VECTORDATA_TUPLE_OUT,&OutVectorData);
    this->addParameter("DoubleDataIn", vibens::VIBe2::USER_DEFINED_DOUBLEDATA_TUPLE_IN, &InDoubleData);
    this->addParameter("DoubleDataOut", vibens::VIBe2::USER_DEFINED_DOUBLEDATA_TUPLE_OUT,&OutDoubleData);
    this->addParameter("Runs", vibens::VIBe2::INT, &Runs);


}

void LoopGroup::run() {
    this->Steps = Runs;
    Group::run();
}

