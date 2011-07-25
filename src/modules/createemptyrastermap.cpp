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
#include "createemptyrastermap.h"
#include <QString>
#include <QThread>

VIBe_DECLARE_NODE_NAME( CreateEmptyRasterMap, Modules )
CreateEmptyRasterMap::CreateEmptyRasterMap()
{

    param.Height = 200;
    param.Width = 200;
    param.CellSize = 20;

    this->addParameter("Height", VIBe2::LONG, & param.Height);
    this->addParameter("Width", VIBe2::LONG, & param.Width);
    this->addParameter("CellSize", VIBe2::DOUBLE, & param.CellSize);
    this->addParameter("RasterData", VIBe2::RASTERDATA_OUT, & param.r);
}

CreateEmptyRasterMap::~CreateEmptyRasterMap()
{
}

void CreateEmptyRasterMap::run() {
    param.r->setSize(param.Width, param.Height, param.CellSize);
    param.r->clear();

}

