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
#include "markcell.h"
#include <QThread>
VIBe_DECLARE_NODE_NAME( MarkCell,Modules )
MarkCell::MarkCell()
{
    x = 0;
    y = 0;
    height = 200;
    width = 200;
    cellsize = 20;
    this->output = &this->createRasterData("OutPut");
    this->addParameter("OutPut", VIBe2::RASTERDATA_OUT, &this->output);
    this->addParameter("Height", VIBe2::LONG, & this->height);
    this->addParameter("Width", VIBe2::LONG, & this->width);
    this->addParameter("CellSize", VIBe2::DOUBLE, &this->cellsize);
    this->addParameter("X", VIBe2::DOUBLE, & this->x);
    this->addParameter("Y", VIBe2::DOUBLE, & this->y);
}

void MarkCell::run()
{

    this->output->setSize(this->width, this->height, this->cellsize);
    this->output->clear();
    Logger(Debug) << (long)(this->x-this->cellsize/2.)/this->cellsize;
    output->setValue((long)(this->x-this->cellsize/2.)/this->cellsize,(this->y-this->cellsize/2.)/this->cellsize,1 );
}


