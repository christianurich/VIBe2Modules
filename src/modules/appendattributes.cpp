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

#include "appendattributes.h"
#include <rasterdatahelper.h>

VIBe_DECLARE_NODE_NAME( AppendAttributes ,Modules )


AppendAttributes::AppendAttributes() {
    this->median = false;
    this->multiplier = 1;
    this->addParameter("Identifier", VIBe2::STRING, &identifier);
    this->addParameter("Faces", VIBe2::VECTORDATA_IN, &inputVec);
    this->addParameter("Attributes", VIBe2::USER_DEFINED_RASTERDATA_IN, &attribueMaps);
    this->addParameter("OutPut", VIBe2::VECTORDATA_OUT, &outPutVec);
    this->addParameter("Multiplier", VIBe2::DOUBLE, &this->multiplier);
    this->addParameter("Median", VIBe2::BOOL, &this->median);

}

void AppendAttributes::run() {
    std::stringstream ss;
    ss << identifier << "_";
    *outPutVec = *inputVec;
    std::vector<std::string> Names = outPutVec->getFaceNames();
    foreach( std::string name, Names ) {
        if (name.compare(0, ss.str().size(), ss.str() )) {
            for (std::map<std::string, RasterData*>::iterator it = attribueMaps.begin();
                 it != attribueMaps.end();
                 it++) {
                RasterData * r = it->second;
                std::string attrName = it->first;
                std::vector<Point> edgePoints = inputVec->getPoints(name);
                Attribute attributes;
                std::vector<std::string> attributeNames;
                attributeNames = outPutVec->getAttributeNames();
                std::vector<std::string>::iterator n = std::find(attributeNames.begin(), attributeNames.end(), name);
                if (n != attributeNames.end())
                    attributes = outPutVec->getAttributes(name);
                if (median) {
                    attributes.setAttribute(attrName,  RasterDataHelper::meanOverArea(r,edgePoints) * multiplier);
                } else {
                    attributes.setAttribute(attrName,  RasterDataHelper::sumOverArea(r,edgePoints,0) * multiplier);
                }
                outPutVec->setAttributes(name, attributes);
            }
        }
    }
}
