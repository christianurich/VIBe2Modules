/**
 * @file joinpointdatatofaces.cpp
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

#include "joinpointdatatofaces.h"
#include <QPointF>
#include <QPolygonF>
#include "vectordatahelper.h"

VIBe_DECLARE_NODE_NAME(JoinPointDataToFaces, Modules)
JoinPointDataToFaces::JoinPointDataToFaces()
{
    this->IdentifierFaces = "";
    this->IdentifierPoints = "";
    this->addParameter("IdentifierFaces", VIBe2::STRING, & this->IdentifierFaces);
    this->addParameter("IdentifierPoints", VIBe2::STRING, & this->IdentifierPoints);
    this->addParameter("PointData", VIBe2::VECTORDATA_IN, & this->pointData);
    this->addParameter("Faces", VIBe2::VECTORDATA_IN, & this->faces);
    this->addParameter("FacesOut", VIBe2::VECTORDATA_OUT, & this->faces_out);
}

void JoinPointDataToFaces::run() {

    *(faces_out) = *(faces);
    std::vector<QPointF> pointList;

    std::vector<std::string> pointListIdentifier = VectorDataHelper::findElementsWithIdentifier(this->IdentifierPoints, this->pointData->getPointsNames());

    for (int i = 0; i < pointListIdentifier.size(); i++) {
        //Only on Point should be found
        std::vector<Point>  pl = this->pointData->getPoints(pointListIdentifier[i]);
        if (pl.size() != 1) {
            Logger(Error) << "Only one Point per Identifier is allowed";
            return;
        }

        Point p  = pl[0];
        pointList.push_back(QPointF (p.getX(), p.getY()));

    }

    std::vector<std::string> faceList = VectorDataHelper::findElementsWithIdentifier(this->IdentifierFaces, this->faces->getFaceNames());

    foreach (std::string idface, faceList) {

        std::vector<Face> fl = this->faces->getFaces(idface);

        //Asume that only one Polygon is stored
        if (fl.size() != 1) {
            Logger(Error) << "Only one Ploygon per Identifier is allowed";
            return;
        }
        //Create Polygon to check intersection
        QPolygonF poly;

        Face f = fl[0];
        std::vector<Point> pl = this->faces->getPoints(idface);
        std::vector<long> ids = f.getIDs();
        for (int i = 0; i < ids.size(); i++) {
            poly.push_back(QPointF(pl[i].x, pl[i].y));
        }

        std::vector<string> IntersectedElements;
        //Identifiy intersecting elements
        for (int i = 0; i < pointList.size(); i++) {
            if (poly.containsPoint(pointList[i], Qt::WindingFill)) {
                IntersectedElements.push_back(pointListIdentifier[i]);
            }
        }

        //If no element has been found check next polygon
        if (IntersectedElements.size() == 0)
            continue;

        //Get AttributeList of the Pointdata Therefore all attributes that are within the attribute are summed up and asainged to the polygon
        Attribute attr = this->pointData->getAttributes(IntersectedElements[0]);
        std::vector<std::string> attributeNames = attr.getAttributeNames();
        attr = this->faces->getAttributes(idface);
        foreach (std::string id, IntersectedElements) {
            Attribute pointAttr = this->pointData->getAttributes(id);
            foreach (std::string name, attributeNames) {
                double val = attr.getAttribute(name);
                attr.setAttribute(name, val + pointAttr.getAttribute(name));
            }
        }

        this->faces_out->setAttributes(idface, attr);



    }




}
