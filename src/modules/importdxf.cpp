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
#include "importdxf.h"
#include "QVector"
#include "QFile"
#include "QLineF"
#include "QTextStream"
#include <QThread>
#define PI 3.14159265
VIBe_DECLARE_NODE_NAME( ImportDXF, Modules )
ImportDXF::ImportDXF()
{

    param.FileName = "";
    param.ID = "Lines";

    this->addParameter("Filename", VIBe2::FILENAME, &param.FileName);
    this->addParameter("ID", VIBe2::STRING, &param.ID );
    this->addParameter("Import", VIBe2::VECTORDATA_OUT, &param.Import);
}

void ImportDXF::run()
{

    QVector<QLineF> lines;
    QFile file(QString::fromStdString(param.FileName));
    QTextStream *stream = new QTextStream(&file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger(Warning) << "warning, read input file "   << " not in directory";
    }

    QString line("NULL");
    bool LineController = false;
    bool ArcController = false;

    int LineCounter = 0;
    float x1 = 0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;

    float mx = 0;
    float my = 0;
    float r = 0;
    float sa = 0;
    float ea = 0;

    while (!line.isNull()) {

        line =stream->readLine();
        if ( LineController == true || ArcController == true ) {
            LineCounter++;
        }
        if (LineController == true ) {
            switch ( LineCounter ) {
            case 6:
                x1 = line.toFloat();
                break;
            case 8:
                y1 = line.toFloat();
                break;
            case 12:
                x2 = line.toFloat();
                break;
            case 14:
                y2 = line.toFloat();
                LineController = false;
                LineCounter = 0;
                lines.append(QLineF(x1, y1, x2, y2));
                break;
            }
        }
        if (ArcController == true) {
            switch ( LineCounter ) {
            case 6:
                mx = line.toFloat();
                break;
            case 8:
                my = line.toFloat();
                break;
            case 12:
                r = line.toFloat();
                break;
            case 14:
                sa = line.toFloat();
                break;
            case 16:
                ea = line.toFloat();

                //Cacluate Points

                x1 = cos(sa*PI/180.)*r + mx;
                y1 = sin(sa*PI/180.)*r + my;
                x2 = cos(ea*PI/180.)*r + mx;
                y2 = sin(ea*PI/180.)*r + my;

                LineController = false;
                LineCounter = 0;

                lines.append(QLineF(x1, y1, x2, y2));
                break;
            }
        }
        if (line == QString("LINE") ) {
            LineController = true;
            LineCounter = 0;
        }
        if (line == QString("ARC") ) {
            ArcController = true;
            LineCounter = 0;

        }
    }
    file.close();

    //Create QVector

    std::vector<Point> vp;
    std::vector<Edge> ve;
    int id = 0;
    for (int i = 0; i < lines.size(); i++) {

        Point p1;
        Point p2;
        p1.x = lines[i].p1().x();
        p1.y = lines[i].p1().y();
        p1.z = 0;

        p2.x = lines[i].p2().x();
        p2.y = lines[i].p2().y();
        p2.z = 0;

        vp.push_back(p1);
        vp.push_back(p2);
        Edge e;
        e.id1 = id;
        id++;
        e.id2 = id;
        id++;
        ve.push_back(e);


    }

    param.Import->setPoints(param.ID, vp);
    param.Import->setEdges(param.ID, ve);

    Logger(Debug) << "Imported Lines: "<< ve.size() ;
    Logger(Debug)<< "Import done";


}

