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
#include "marker.h"

#include <QString>
#include "vectordata.h"
#include <sstream>
#include "userdefinedfunctions.h"
#include <boost/foreach.hpp>
#include <QStringList>
#include <QThread>
#include <guimarker.h>
#include <vectordatahelper.h>
VIBe_DECLARE_NODE_NAME( Marker, Modules )
        Marker::Marker()
{
    param.Width = 200;
    param.Height = 200;
    param.CellSize = 20;
    param.PlacementOption = "";
    param.RExpression = "";
    param.rExpression = "";
    param.maxExpression = "";
    param.Identifier = "";
    param.Points = true;
    param.Edges = false;

    this->addParameter("Width", VIBe2::LONG, & param.Width);
    this->addParameter("Height", VIBe2::LONG, & param.Height);
    this->addParameter("CellSize", VIBe2::DOUBLE, &param.CellSize);
    this->addParameter("RExpression", VIBe2::STRING, &param.RExpression);
    this->addParameter("rExpression", VIBe2::STRING, &param.rExpression);
    this->addParameter("maxExpression", VIBe2::STRING, &param.maxExpression);
    this->addParameter("InputRasterData", VIBe2::USER_DEFINED_RASTERDATA_IN, &inputRasterData);
    this->addParameter("PlacementOption", VIBe2::STRING, &param.PlacementOption);
    this->addParameter("Points", VIBe2::INT, &param.Points);
    this->addParameter("Edges", VIBe2::INT, &param.Edges);
    this->addParameter("VectorData", VIBe2::VECTORDATA_IN, & vectorData);
    this->addParameter("OutPutMap", VIBe2::RASTERDATA_OUT, &OutputMap);
    this->addParameter("Identifier", VIBe2::STRING, &param.Identifier);
}

double Marker::calculater(const Point &sp, const Point &cp) {
    double x = cp.x - sp.x;
    double y = cp.y - sp.y;

    double l = sqrt(x*x+y*y);
    return l;
}

bool Marker::createInputDialog() {
    QWidget * w = new GUIMarker(this);
    w->show();
    return true;
}


void Marker::initRExpression () {
    R = new double;
    mu::Parser * p  = new mu::Parser();
    long Vcounter = 0;
    long Rcounter = 0;
    /*for ( std::vector<std::string>::const_iterator it = param.RVariables.begin(); it != param.RVariables.end(); ++it) {
        std::cout << "Try: " << *it << std::endl;
        if (paramRaw.find(*it) != paramRaw.end()) {
            std::string buf;
            std::stringstream ss(paramRaw.at(*it));
            std::vector<std::string> tokens;
            while (ss >> buf)
                tokens.push_back(buf);

            if (tokens[0].compare("value") <= 0) {
                if ( tokens[1].compare("DoubleAttributes")<= 0) {
                    RDoubleAttributes.push_back(& this->getVectorData(tokens[2]).getDoubleAttributes(tokens[3]) );
                    RVariables.push_back(new double);
                    RExpression->DefineVar(*it, RVariables[Vcounter++] );

                }
            }
            if (tokens[0].compare("RasterData") <= 0) {
                RRasterData.push_back(& this->getRasterData(tokens[1], this->getT()) );
                RRasterVariables.push_back(new double);
                RExpression->DefineVar(*it, RRasterVariables[Rcounter++] );
            }

        } else {
            std::cout << "Error: Variable not defined" << std::endl;
            QThread::currentThread()->exit();;
        }
    }*/
    RRasterData.clear();
    RRasterVariables.clear();
    for ( std::vector<std::string>::const_iterator it = param.RData.begin(); it != param.RData.end(); ++it) {
        std::cout << "Try: " << *it << std::endl;
        if (paramRaw.find(*it) != paramRaw.end()) {
            std::string buf;
            std::stringstream ss(paramRaw.at(*it));
            std::vector<std::string> tokens;
            while (ss >> buf)
                tokens.push_back(buf);
            RRasterData.push_back(& this->getRasterData(tokens[0]) );
            RRasterVariables.push_back(new double);
            p->DefineVar(*it, RRasterVariables[Rcounter++] );

        } else {
            std::cout << "Error: Variable not defined" << std::endl;
            QThread::currentThread()->exit();;
        }
    }




    p->DefineVar("CellValue", this->CellValue);
    p->DefineFun("nov", mu::numberOfValues);
    p->DefineFun("rand", mu::random), false;
    p->SetExpr(param.RExpression);
    RExpression = p;


}

double Marker::evaluateExpresion_R(int index, Point & p) {
    try {


        for ( unsigned int i = 0; i < RDoubleAttributes.size(); i++ ) {
            *RVariables.at(i) = RDoubleAttributes.at(i)->at(index);
        }
        for ( unsigned int i = 0; i < RRasterData.size(); i++ ) {
            *RRasterVariables.at(i) = RRasterData.at(i)->getValue(p.x, p.y);
        }

        *R = RExpression->Eval();
        return *R;

    }
    catch (mu::Parser::exception_type &e) {
        std::cout << e.GetMsg() << std::endl;
    }

}
void Marker::initMaxMinExpression() {
    this->maxSet = false;
    this->minSet = false;
    if ( param.maxExpression.size() > 0 ) {
        maxExpression = new mu::Parser();
        maxExpression->DefineVar("R", R);
        maxExpression->DefineVar("r", r);
        maxExpression->SetExpr(param.maxExpression);
        maxSet = true;
    }
}

void Marker::initrExpression () {
    mu::Parser * p  = new mu::Parser();

    long Vcounter = 0;
    long rcounter = 0;
    /*for ( std::vector<std::string>::const_iterator it = param.rVariables.begin(); it != param.rVariables.end(); ++it) {
        if (paramRaw.find(*it) != paramRaw.end()) {
            std::string buf;
            std::stringstream ss(paramRaw.at(*it));
            std::vector<std::string> tokens;
            while (ss >> buf)
                tokens.push_back(buf);

            if (tokens[0].compare("value") <= 0) {
                if ( tokens[1].compare("DoubleAttributes")<= 0) {
                    rDoubleAttributes.push_back(& this->getVectorData(tokens[2]).getDoubleAttributes(tokens[3]) );
                    rVariables.push_back(new double);
                    rExpression->DefineVar(*it, rVariables[Vcounter++] );
                }

            }
            if (tokens[0].compare("RasterData") <= 0) {
                rRasterData.push_back(& this->getRasterData(tokens[1], this->getT() ) ) ;
                rRasterVariables.push_back(new double);
                rExpression->DefineVar(*it, rRaste    p->rVariables[Rcounter++] );
            }
        } else {
            std::cout << "Error: Variable not defined" << std::endl;
            QThread::currentThread()->exit();;
        }
    }*/
    rRasterData.clear();
    rRasterVariables.clear();
    for ( std::vector<std::string>::const_iterator it = param.RData.begin(); it != param.RData.end(); ++it) {
        std::cout << "Try: " << *it << std::endl;
        if (paramRaw.find(*it) != paramRaw.end()) {
            std::string buf;
            std::stringstream ss(paramRaw.at(*it));
            std::vector<std::string> tokens;
            while (ss >> buf)
                tokens.push_back(buf);
            rRasterData.push_back(& this->getRasterData(tokens[0]) );
            rRasterVariables.push_back(new double);
            p->DefineVar(*it, rRasterVariables[rcounter] );
            rcounter++;

        } else {
            std::cout << "Error: Variable not defined" << std::endl;
            QThread::currentThread()->exit();;
        }
    }

    p->DefineFun("nov", mu::numberOfValues);
    p->DefineFun("rand", mu::random, false);
    p->DefineVar("r", r);
    p->DefineVar("R", R);
    p->DefineVar("CellValue", this->CellValue);
    p->SetExpr(param.rExpression);
    this->rExpression = p;
}


double Marker::evaluateExpresion_r(int index, Point & p ) {
    try  {

        for ( unsigned int i = 0; i < rDoubleAttributes.size(); i++ ) {
            *rVariables.at(i) = rDoubleAttributes.at(i)->at(index);
        }
        for (unsigned int i = 0; i < rRasterData.size(); i++ ) {
            *rRasterVariables.at(i) = rRasterData.at(i)->getValue(p.x, p.y);
        }
        //Calculate
        return rExpression->Eval();
    }
    catch (mu::Parser::exception_type &e) {
        std::cout << e.GetMsg() << std::endl;
    }

}
void Marker::run() {
    this->OutputMap->setSize(param.Width, param.Height, param.CellSize);
    this->OutputMap->clear();

    //Init MuParser
    std::map<std::string, double*> VariableMap;
    mu::Parser * RExpression = new mu::Parser();
    mu::Parser * rExpression = new mu::Parser();
    mu::Parser * maxExpression = new mu::Parser();
    std::vector<std::string> VariableNames;
    double * R = new double;
    double * r = new double;
    double * V = new double;
    for (std::map<std::string, RasterData * >::iterator it = inputRasterData.begin(); it != inputRasterData.end(); ++it) {
        VariableMap[it->first] = new double;
        RExpression->DefineVar(it->first, VariableMap[it->first]);
        rExpression->DefineVar(it->first, VariableMap[it->first]);
        maxExpression->DefineVar(it->first, VariableMap[it->first]);
        VariableNames.push_back(it->first);
    }
    rExpression->DefineVar("R", R);
    rExpression->DefineVar("r", r);
    rExpression->DefineFun("rand", mu::random, false);
    rExpression->DefineVar("Value", V);
    RExpression->DefineVar("Value", V);
    RExpression->DefineFun("rand", mu::random, false);
    maxExpression->DefineVar("R", R);
    maxExpression->DefineVar("r", r);
    maxExpression->DefineVar("Value", V);
    maxExpression->DefineFun("rand", mu::random, false);
    //Set Expressions
    rExpression->SetExpr(param.rExpression);
    RExpression->SetExpr(param.RExpression);
    maxExpression->SetExpr(param.maxExpression);
    //Init Points
    std::vector<Point> points;
    //AddPoints
    if ( param.Points == true ) {
        foreach (std::string pname, VectorDataHelper::findElementsWithIdentifier(param.Identifier, vectorData->getPointsNames())) {
            std::vector<Point>  points_tmp = vectorData->getPoints(pname);
            foreach(Point p, points_tmp ) {
                points.push_back(Point((long) p.getX()/param.CellSize,(long) p.getY()/param.CellSize,p.getZ()));
            }
        }
    }
    //AddLines as Points
    if ( param.Edges == true ) {
        foreach (std::string ename, VectorDataHelper::findElementsWithIdentifier(param.Identifier,vectorData->getEdgeNames())) {
            std::vector<Edge>  edges= vectorData->getEdges(ename);
            std::vector<Point> coordinates;
            foreach(Point p, vectorData->getPoints(ename)) {
                coordinates.push_back(Point((long) p.getX()/param.CellSize, (long)p.getY()/param.CellSize,p.getZ()));
            }

            BOOST_FOREACH(Edge e, edges) {
                Point p1 = coordinates[e.id1];
                Point p2 = coordinates[e.id2];
                double dx = p2.x - p1.x;
                double dy = p2.y - p1.y;

                long steps = (long) sqrt(dx*dx+dy*dy);

                long x0 = (long) p1.x;
                long y0 = (long) p1.y;
                for ( int i = 0; i < steps; i++ ) {
                    Point p;
                    p.x =  x0 + i* (dx /  steps);
                    p.y =  y0 + i* (dy /  steps);
                    points.push_back(p);
                }
            }
        }
    }
    //Create Mark for every point
    foreach(Point p, points) {
        //Set Values

        *(V) = p.z;
        foreach(std::string s, VariableNames) {
            double * d = VariableMap[s];
            *(d) = inputRasterData[s]->getValue(p.x, p.y);
        }
        //Evaluate R
        try {
            *(R) = RExpression->Eval();
        } catch (mu::Parser::exception_type &e) {
            Logger(Error) << e.GetMsg();
            //std::cout <<  e.GetMsg() << std::endl;
        }

        //Mark Celss
        Point CenterPoint = p;
        long  minI = p.x - *R-2;
        long  minJ = p.y - *R-2;
        long  maxI = p.x + *R+2;
        long  maxJ = p.y + *R+2;
        if (minI < 0)
            minI = 0;
        if (minJ < 0)
            minJ = 0;
        if (maxI > param.Width)
            maxI = param.Width;
        if (maxJ > param.Height)
            maxJ = param.Height;
        Point currentPoint;

        for ( unsigned long  i = minI; i < maxI; i++ ) {
            for ( unsigned long j =  minJ; j < maxJ; j++ ) {
                currentPoint.x = (double) i+0.5;
                currentPoint.y = (double) j+0.5;

                *r = calculater(CenterPoint, currentPoint);


                if (*r <= *R ) {
                    double value;
                    try {
                        value = rExpression->Eval();
                    } catch (mu::Parser::exception_type &e) {
                        Logger(Error) << e.GetMsg();
                        //std::cout <<  e.GetMsg() << std::endl;
                    }
                    if (param.PlacementOption.compare("KeepHigherValue") == 0) {
                        if (  this->OutputMap->getValue(i,j) > value ) {
                            value = this->OutputMap->getValue(i,j);
                        }
                    } else if ( param.PlacementOption.compare("KeepLowerValue") == 0 ) {
                        if ( this->OutputMap->getValue(i,j) < value && this->OutputMap->getValue(i,j) != 0 ) {
                            value = this->OutputMap->getValue(i,j);
                        }
                    } else if ( param.PlacementOption.compare("KeepValue") == 0 ) {
                        if ( this->OutputMap->getValue(i,j) != 0) {
                            value = this->OutputMap->getValue(i,j);

                        }
                    } else if ( param.PlacementOption.compare("Add") == 0 ) {
                        double val =  this->OutputMap->getValue(i,j);

                        value = value + val;
                    }
                    if (!this->param.maxExpression.empty()) {
                        double maxValue = maxExpression->Eval();
                        value = (value > maxValue ) ?  maxValue : value;
                    }

                    if (value > 1000) {
                        Logger(Debug) << value;
                    }

                    this->OutputMap->setValue(i,j,value);
                }
            }
        }
    }
    delete R;
    delete r;
    delete V;
    delete RExpression;
    delete rExpression;
    delete maxExpression;
    foreach(std::string s, VariableNames)
        delete VariableMap[s];


}

void Marker::run1() {
    r = new double;
    std::cout << "running Marker" << std::endl;
    //Check Marker Map
    RasterData MarkerMap;
    if ( param.KeepMap.compare("true") == 0 ) {
        try {
            MarkerMap = this->getRasterData(param.MarkerMap);
        } catch(int e) {
            MarkerMap = RasterData(param.Width, param.Height, param.CellSize);
        }

    }else {

        MarkerMap = RasterData(param.Width, param.Height, param.CellSize);
    }
    initRExpression();
    initrExpression();
    initMaxMinExpression();
    double cellsize = param.CellSize;
    std::vector<Point> points;
    if (param.VectorData.compare("")  != 0) {
        VectorData vData;
        vData = this->getVectorData(param.VectorData);
        //if ( param.Points.compare("true") == 0 ) {
        foreach (std::string pname, vData.getPointsNames()) {
            std::vector<Point>  points_tmp = vData.getPoints(pname);
            foreach(Point p, points_tmp ) {
                points.push_back(Point(p.getX()/cellsize,p.getY()/cellsize,p.getZ()));


            }

        }
        //}
        // if ( param.Edges.compare("true") == 0 ) {

        foreach (std::string ename, vData.getEdgeNames()) {
            std::vector<Edge>  edges= vData.getEdges(ename);
            std::vector<Point> coordinates;
            foreach(Point p, vData.getPoints(ename)) {
                coordinates.push_back(Point(p.getX()/cellsize,p.getY()/cellsize,p.getZ()));

            }


            BOOST_FOREACH(Edge e, edges) {
                Point p1 = coordinates[e.id1];
                Point p2 = coordinates[e.id2];
                double dx = p2.x - p1.x;
                double dy = p2.y - p1.y;

                long steps = (long) sqrt(dx*dx+dy*dy);



                long x0 = (long) p1.x;
                long y0 = (long) p1.y;

                for ( int i = 0; i < steps; i++ ) {
                    Point p;
                    p.x =  x0 + i* (dx /  steps);
                    p.y =  y0 + i* (dy /  steps);
                    points.push_back(p);
                }
            }
        }
        //}
    } /*else if (param.RData.compare("")  != 0) {
        RasterData rData(this->getRasterData(param.RData, this->getT()+1));

        //Find Points
        for ( int y = 0; y < rData.getHeight(); y++) {
            for (long x = 0;  x < rData.getWidth(); x++ ){
                double val = rData.getValue(x,y);
                if ( val != 0) {
                    points.push_back(Point(x,y,val));
                }
            }
        }


    }*/

    int counter = 0;
    for (std::vector<Point>::const_iterator it = points.begin(); it!=points.end(); ++it) {
        Point p = *it;
        *(this->CellValue) = p.z;

        double R = this->evaluateExpresion_R(counter, p);

        //if ( R > 0)
        //MarkerMap.setValue(p.x, p.y, this->evaluateExpresion_r(counter, p););
        Point pc;

        long  minI = p.x - R-2;
        long  minJ = p.y - R-2;
        long  maxI = p.x + R+2;
        long  maxJ = p.y + R+2;
        if (minI < 0)
            minI = 0;
        if (minJ < 0)
            minJ = 0;
        if (maxI > param.Width)
            maxI = param.Width;
        if (maxJ > param.Height)
            maxJ = param.Height;

        for ( unsigned long  i = minI; i < maxI; i++ ) {
            for ( unsigned long j =  minJ; j < maxJ; j++ ) {
                pc.x = (double) i+0.5;
                pc.y = (double) j+0.5;

                *r = calculater(*it, pc);
                if (*r <= R ) {
                    double value = this->evaluateExpresion_r(counter, p);
                    double max = 0;
                    if (maxSet == true) {
                        max = maxExpression->Eval();
                        value = (value > max ) ?  max : value;

                    }
                    if (param.PlacementOption.compare("KeepHigherValue") == 0) {
                        if ( MarkerMap.getValue(i,j) < value ) {
                            MarkerMap.setValue(i,j,value);
                        }
                    } else if ( param.PlacementOption.compare("KeepLowerValue") == 0 ) {
                        if ( MarkerMap.getValue(i,j) > value || MarkerMap.getValue(i,j) == 0 ) {
                            MarkerMap.setValue(i,j,value);
                        }
                    } else if ( param.PlacementOption.compare("Replace") == 0 ) {
                        MarkerMap.setValue(i,j,value);
                    } else if ( param.PlacementOption.compare("CropAdd") == 0 ) {
                        double val = MarkerMap.getValue(i,j);
                        value = value + val;
                        if (maxSet == true)
                            value = (value > max ) ?  max : value;
                        if (val < value)
                            MarkerMap.setValue(i,j,value);
                    } else if ( param.PlacementOption.compare("KeepValue") == 0 ) {
                        if (MarkerMap.getValue(i,j) == 0) {
                            MarkerMap.setValue(i,j,value);
                        }
                    }

                }

            }
        }
        counter++;
    }

    this->setRasterData(param.MarkerMap, MarkerMap);
    std::vector<double * >  RVariables;
    std::vector<double * >  rVariables;
    int size = RVariables.size();
    for ( int i = size-1; i >= 0; i-- ) {
        delete RVariables.at(i);
        delete rVariables.at(i);
    }
    size = RRasterVariables.size();
    for ( int i = size-1; i >= 0; i-- ) {
        delete RRasterVariables.at(i);
        delete rRasterVariables.at(i);
    }

    delete rExpression;
    delete RExpression;
    delete r;
    delete R;

    if (maxSet == true )
        delete maxExpression;
}




/*void Marker::init(const parameter_type &parameters) {
    CellValue = new double;

    paramRaw = parameters;
    if (parameters.find("Width") != parameters.end()) {
        param.Width = QString::fromStdString(parameters.at("Width")).toLong();
    } else {
        std::cout << "Error: parameter Width not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("Height") != parameters.end()) {
        param.Height = QString::fromStdString(parameters.at("Height")).toLong();
    } else {
        std::cout << "Error: parameter Height not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("CellSize") != parameters.end()) {
        param.CellSize = QString::fromStdString(parameters.at("CellSize")).toLong();
    } else {
        std::cout << "Error: CellSize Width not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("MarkerMap") != parameters.end()) {
        param.MarkerMap = parameters.at("MarkerMap");
    } else {
        std::cout << "Error: MarkerMap not set" << std::endl;
        QThread::currentThread()->exit();;
    }

    if (parameters.find("VectorData") != parameters.end()) {
        param.VectorData = parameters.at("VectorData");
    } else {
        param.VectorData = "";
    }

    if (parameters.find("Points") != parameters.end()) {

        QString s=QString::fromStdString(parameters.at("Points"));

        QStringList sl = s.split(QRegExp("\\s+"));
        if (sl.size() > 1) {
            param.Points = sl[1].toStdString();
        } else {
            param.Points = sl[0].toStdString();
        }
    }

    if (parameters.find("Edges") != parameters.end()) {
        param.Edges = parameters.at("Edges");
    }
    if (parameters.find("KeepMap") != parameters.end()) {
        param.KeepMap = parameters.at("KeepMap");
    }
    if (parameters.find("RExpression") != parameters.end()) {
        param.RExpression = parameters.at("RExpression");

    } else {
        std::cout << "Error: RExpression not set" << std::endl;
        QThread::currentThread()->exit();;

    }
    if (parameters.find("RVariables") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("RVariables"));

        while (ss >> buf)
            param.RVariables.push_back(buf);
    }

    if (parameters.find("rExpression") != parameters.end()) {
        param.rExpression = parameters.at("rExpression");

    } else {
        std::cout << "Error: rExpression not set" << std::endl;
        QThread::currentThread()->exit();;

    }

    if (parameters.find("maxExpression") != parameters.end()) {
        if ( parameters.at("maxExpression").compare("") != 0) {
            param.maxExpression = parameters.at("maxExpression");
        }

    }
    if (parameters.find("minExpression") != parameters.end()) {
        param.rExpression = parameters.at("minExpression");

    }
    if (parameters.find("rVariables") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("rVariables"));

        while (ss >> buf)
            param.rVariables.push_back(buf);
    }
    if (parameters.find("RasterData") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("RasterData"));

        while (ss >> buf)
            param.RData.push_back(buf);
    }
    if (parameters.find("PlacementOption") != parameters.end()) {
        param.PlacementOption = parameters.at("PlacementOption");

    } else {
        std::cout << "Error: PlacementOption not set" << std::endl;
        QThread::currentThread()->exit();;

    }

}*/
