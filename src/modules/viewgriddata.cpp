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
#include "viewgriddata.h"

VIBe_DECLARE_NODE_NAME(ViewGridData, Modules)
ViewGridData::ViewGridData()
{
    this->addParameter("Map", VIBe2::USER_DEFINED_RASTERDATA_IN, &Maps);
}

void ViewGridData::run() {
    vibens::Logger(vibens::Debug) << "Run Module ViewGrid";
    std::map<std::string, std::string> resultMap;
    for(std::map<std::string, RasterData *>::iterator it = Maps.begin(); it != Maps.end(); ++it) {
        RasterData * r = it->second;
        resultMap[r->getUUID()+"%"+r->getName()] =  r->getName();
    }
    this->sendRasterDataToResultViewer(resultMap);

}
