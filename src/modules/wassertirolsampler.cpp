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
#include "wassertirolsampler.h"
#include "vectordatahelper.h"
#include "QPolygonF"

VIBe_DECLARE_NODE_NAME(WassertirolSampler, PowerVIBe)
WassertirolSampler::WassertirolSampler()
{
    cellSize = 100;
    this->addParameter("cellSize", VIBe2::DOUBLE, &cellSize);
    this->addParameter("SampleArea", VIBe2::VECTORDATA_IN, &SampleArea);
    this->addParameter("Sample", VIBe2::VECTORDATA_IN, &Sample);
    this->addParameter("Result", VIBe2::VECTORDATA_OUT, &Result);
    this->addParameter("NumberOfFaces", VIBe2::DOUBLEDATA_OUT, &NumberOfFaces);
}
std::vector<int> WassertirolSampler::buildRandomIndices(int size) {
    Logger(Debug) << "Start Build  Random List "<< size;
    //std::vector<int> vec;
    //vec.reserve(size);
    std::vector<int> indices;
    indices.resize(size);

    for (int i = 0; i < size; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < size; i++) {
        int ra = rand() % size;
        int tmp_index = indices[ra];
        indices[ra] =  indices[size-1-ra];
        indices[size-1-ra] = tmp_index;
    }
    //horrible slow
   /* while (indices.size() > 0) {
        int ra = rand() % indices.size();
        vec.push_back(indices[ra]);
        indices.erase(find(indices.begin(), indices.end(), indices[ra]));
        //std::cout << indices.size() << std::endl;


    }*/
    Logger(Debug) << "End Build  Random List";
    return indices;


}

void WassertirolSampler::run() {
    std::vector<QPolygonF> existingAreas;
    std::vector<std::string> names = this->SampleArea->getFaceNames();
    bool unset = true;
    double minx = 0;
    double miny = 0;
    double maxx = 0;
    double maxy = 0;
    //Find max and min Values;
    if (this->Sample->getFaceNames().size() == 0) {
        Logger(Error) << "No Face Found for Sampling";
        return;
    }
    this->Result->clear();

    std::string sample = this->Sample->getFaceNames()[0];

    std::string BigestArea = "";
    double area = 0;
    foreach (std::string name, names) {
        double a_t = VectorDataHelper::calculateArea(this->SampleArea->getFaces(name)[0], this->SampleArea->getPoints(name));
        if (area < a_t) {
            area = a_t;
            BigestArea = name;
        }
    }
    this->Result->setFaces(BigestArea, this->SampleArea->getFaces(BigestArea));
    this->Result->setPoints(BigestArea, this->SampleArea->getPoints(BigestArea));

    foreach (std::string name, names) {
        if (name.compare(BigestArea) != 0) {
            QPolygonF f;
            std::vector<Point> points = this->SampleArea->getPoints(name);
            foreach(Point p, points) {
                f.push_back(QPointF(p.x, p.y));
            }
            existingAreas.push_back(f);
        }
    }

    names = this->Result->getFaceNames();
    //Check If Inside
    QPolygonF sarea;

    foreach (std::string name, names) {
        std::vector<Point> points = this->Result->getPoints(name);
        foreach(Point p, points) {
            if (unset) {
                unset = false;
                minx = p.x;
                miny = p.y;
                maxx = p.x;
                maxy = p.y;
            }
            if(minx > p.x)
                minx = p.x;
            if(miny > p.y)
                miny = p.y;
            if(maxx < p.x)
                maxx = p.x;
            if(maxy < p.x)
                maxy = p.x;

            sarea.push_back(QPointF(p.x, p.y));
        }

    }



    //Create PointList;
    std::vector<Point> plist;
    for(double x = minx; x < maxx; x = x+cellSize) {
        for(double y = miny; y < maxy; y = y+cellSize) {
            plist.push_back(Point(x,y,0));


        }
    }
    std::vector<Point> p_new;
    std::vector<bool> checked;
    foreach(Point p, plist) {
        if (sarea.containsPoint(QPointF(p.x, p.y), Qt::WindingFill)) {
            p_new.push_back(p);
            checked.push_back(false);
        }

    }

    this->Result->setPoints("PointList", p_new);
    bool successful = true;

    int counter = 0;

    while (successful == true && p_new.size() > 0) {
        //Create Random PointList for Start
        std::vector<int> indices = buildRandomIndices(p_new.size());

        std::vector<Point> RandomList = p_new;
        for (int i = 0; i < RandomList.size(); i++) {
            int index = indices[i];
            successful = false;
            //Check if Samle its
            Point p= RandomList[index];
            QPolygonF s;

            foreach(Point ps, this->Sample->getPoints(sample)) {
                s.push_back(QPointF(ps.x+p.x, ps.y+p.y));
            }
            s.push_back(QPointF(this->Sample->getPoints(sample)[0].x +p.x, this->Sample->getPoints(sample)[0].y+p.y));


            if (s.subtracted(sarea).size() > 0) {
                checked[index] = true;
                continue;
            }



            //Check Intersection with existing Areas;
            bool interseted = false;
            for( int j = 0; j < existingAreas.size();j++ ){

                if (s.intersected(existingAreas[j]).size() > 0) {
                    interseted = true;
                    j = existingAreas.size();
                }
            }
            if (interseted) {
                checked[index] = true;
                continue;                
            }

            //Checks passed
            existingAreas.push_back(s);

            counter++;
            std::vector<Point> newPoints;
            foreach(QPointF pf, s)
                newPoints.push_back(Point(pf.x(), pf.y(), 0));

            stringstream ss;
            ss << "Areas_" << counter;
            Logger(Debug) << "Place Temperatur Zone" ;
            this->Result->setPoints(ss.str(),newPoints );
            this->Result->setFaces(ss.str(), this->Sample->getFaces(sample));


            std::vector<Point> p_new_tmp;
            std::vector<bool> checked_tmp;
            for (int j = 0 ; j < p_new.size(); j++) {
                Point p1 = p_new[j];
                if (p == p1)
                    continue;
                if (!s.containsPoint(QPointF(p1.x, p1.y), Qt::WindingFill) && !checked[j]){
                    p_new_tmp.push_back(p1);
                    checked_tmp.push_back(false);
                }
            }
            p_new = p_new_tmp;
            checked = checked_tmp;
            successful = true;

            i = RandomList.size();

        }

    }

    NumberOfFaces = counter;



}
