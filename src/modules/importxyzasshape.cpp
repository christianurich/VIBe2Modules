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
#include "importxyzasshape.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
VIBe_DECLARE_NODE_NAME( ImportXYZAsShape, Modules)
ImportXYZAsShape::ImportXYZAsShape()
{
    this->fileName = "";
    this->moveToO = false;
    this->addParameter("Filename", VIBe2::FILENAME, &this->fileName);
    this->addParameter("moveToO", VIBe2::BOOL, &this->moveToO);
    this->addParameter("Vectordata", VIBe2::VECTORDATA_OUT, &this->vec);
}

void ImportXYZAsShape::run()
{
    QFile file(QString::fromStdString(this->fileName));
    QTextStream stream(&file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger(Error) << "warning, read input file " ;
        return;
    }
    QString line("NULL");

    std::vector<Point> points;
    int LineCounter  = 0;
    double min_x;
    double min_y;
    while (!line.isNull() ) {
        LineCounter++;
        line = stream.readLine();
        if (LineCounter == 1)
            continue;
        line.replace(",", ".");
        QStringList list = line.split(QRegExp("\\s+"));
        if (list.size() != 3)
            continue;
        QString sx = list[0];
        QString sy = list[1];
        QString sz = list[2];

        double x = sx.toDouble();
        double y = sy.toDouble();
        double z = 0;//sz.toDouble();
        if (LineCounter == 2) {
            min_x = x;
            min_y = y;
        } else {
            if (min_x > x)
                min_x = x;
            if (min_y > y)
                min_y = y;
        }

        points.push_back(Point(x,y ,z));
        LineCounter++;
    }
    std::vector<Edge> edgelist;
    //Create Lines


    if (this->moveToO) {
        Point Offset(min_x, min_y, 0);
        std::vector<Point> newList;
        for (int i = 0 ; i < points.size(); i++) {
            newList.push_back(points[i] - Offset);
        }
        points = newList;
    }

    if (points.size() == 0)
        return;
    Point startPoint = points[0];
    points.push_back(points[0]);
    for (int i = 0; i < points.size()-1; i++) {
        edgelist.push_back(Edge(i, i+1));
        if (startPoint.compare2d(points[i+1])) {
            startPoint = points[i+1];
            i=i++;
        }
    }

    this->vec->setPoints("Lines", points);
    this->vec->setEdges("Lines", edgelist);
}
