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
#include "linestofaces.h"
#include "geometry.h"
#include <sstream>
#include <QString>
#include <QThread>
#include <vectordatahelper.h>
VIBe_DECLARE_NODE_NAME( LinesToFaces, Modules )
LinesToFaces::LinesToFaces()
{
    /*
    std::string VectorDataFrom;
    std::string VectorDataTo;
    std::string IDFrom;
    std::string IDTo;
    bool Snap;
            bool RemoveLines;
    float SnapTolerance;
*/
    param.IDFrom = "Lines";
    param.IDTo = "Faces";
    param.Snap = true;
    param.RemoveLines = true;
    param.SnapTolerance = 0.;
    param.CheckInsides = false;

    this->addParameter("IDFrom", VIBe2::STRING, &param.IDFrom);
    this->addParameter("IDTo", VIBe2::STRING, &param.IDTo);
    this->addParameter("Snap", VIBe2::BOOL, &param.Snap);
    this->addParameter("RemoveLines", VIBe2::BOOL, &param.RemoveLines);
    this->addParameter("SnapTolerance", VIBe2::DOUBLE, &param.SnapTolerance);
    this->addParameter("VectorDataFrom", VIBe2::VECTORDATA_IN, &param.VectorDataFrom);
    this->addParameter("VectorDataTo", VIBe2::VECTORDATA_OUT, &param.VectorDataTo);
    this->addParameter("CheckInsides", VIBe2::BOOL, &param.CheckInsides);


}

void LinesToFaces::run() {
    VectorData vec = VIBeH::Geometry::ShapeFinder( *(param.VectorDataFrom), param.IDFrom, param.IDTo, param.Snap, param.SnapTolerance, param.RemoveLines);
    param.VectorDataTo->clear();


    std::vector<std::string> names = vec.getFaceNames();

    foreach(std::string n, names) {
        std::vector<Face> faces = vec.getFaces(n);
        std::vector<Face> faces_new;
        std::vector<Point> points = vec.getPoints(n);
        std::vector<Edge> edges = vec.getEdges(n);

        if (!param.CheckInsides) {
            faces_new = faces;
        } else {
            foreach(Face f, faces) {
                int counter = 0;
                foreach(int i, f.getIDs()) {
                    Point p = points[i];
                    if (VectorDataHelper::findConnectedEges(vec, p).size() > 2)
                        counter++;

                }
                if ( counter < f.getIDs().size() || counter < 8 ) {
                    faces_new.push_back(f);
                }
            }
        }
        param.VectorDataTo->setFaces(n, faces_new);
        param.VectorDataTo->setPoints(n, points);
        param.VectorDataTo->setEdges(n, edges);

    }

}

