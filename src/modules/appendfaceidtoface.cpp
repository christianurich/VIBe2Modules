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

#include "vectordatahelper.h"
#include "appendfaceidtoface.h"
#include "QPolygonF"
VIBe_DECLARE_NODE_NAME(AppendFaceIDToFace,Modules)
AppendFaceIDToFace::AppendFaceIDToFace()
{
    this->IdentifierFaceIDs  = "";
    this->IdentifierFaces = "";
    this->addParameter("IdentifierFaces", VIBe2::STRING, & this->IdentifierFaces);
    this->addParameter("IdentifierFaceIDs", VIBe2::STRING, & this->IdentifierFaceIDs);

    this->addParameter("Faces", VIBe2::VECTORDATA_IN, & this->Faces);
    this->addParameter("FaceIDs", VIBe2::VECTORDATA_IN, & this->FaceIDs);

    this->addParameter("Output", VIBe2::VECTORDATA_OUT, & this->Output);

}

void AppendFaceIDToFace::run() {

    *(Output) = *(Faces);

    std::vector<std::string> faceids = VectorDataHelper::findElementsWithIdentifier(this->IdentifierFaceIDs, this->FaceIDs->getFaceNames());

    foreach (std::string faceid, faceids){
        Logger(Debug) << faceid;
        std::vector<Face> fs = this->FaceIDs->getFaces(faceid);
        std::vector<Point> pointlist = this->FaceIDs->getPoints(faceid);

        foreach (Face f, fs) {
            QPolygonF poly;

            for (int i = 0; i < f.getIDs().size(); i++) {
                poly.append(QPointF(pointlist[f.getIDs()[i]].x, pointlist[f.getIDs()[i]].y));
            }

            std::vector<std::string> faces = VectorDataHelper::findElementsWithIdentifier(this->IdentifierFaces, this->Faces->getAttributeNames());

            foreach (std::string f, faces) {
                Attribute attr = this->Faces->getAttributes(f);
                QPointF p(attr.getAttribute("centroid_x"),attr.getAttribute("centroid_y") );
                if ( poly.containsPoint(p, Qt::WindingFill) ) {
                    std::stringstream ss;
                    ss << this->IdentifierFaceIDs << "ID";
                    attr.setAttribute(ss.str(), faceid);
                    this->Output->setAttributes(f, attr);
                }

            }

        }




    }
}
