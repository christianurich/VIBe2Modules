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
#include "simpleagent.h"

#include <boost/foreach.hpp>
#include <QtGlobal>
#include <sstream>

SimpleAgent::SimpleAgent(Point & startpos,
                         SARules * rules,
                         SimpleAgentModel   * controller)
{

    this->pos = startpos;

    this->rules =  rules;
    this->controller =  controller;
    this->neighbourhoud = 9;
    this->age = 0;
    this->Direction = 0;
    this->angelError = 0;
    this->siblingID = 0;
    this->parentID = 0;
    this->success = -1;
    this->generation = 1;

    //Create Status Rules
    BOOST_FOREACH(Status_map::value_type s, this->rules->StatusVariable) {
        this->status[s.first] = 0;
    }


    this->id = controller->newAgentID();
    this->ProbabilityVector = ublas::vector<double>(this->neighbourhoud);

    PathHistory.push_back(Point(pos.x * rules->cellSize+rules->cellSize/2., pos.y* rules->cellSize+rules->cellSize/2., pos.z* rules->cellSize+rules->cellSize/2.));

}
int SimpleAgent::convertDeegreeToCells(double angel) {


    angelError = angelError + angel;

    /* if (angelError < 0) {
        angelError+=360.0;
    }*/
    while (angelError > 360) {
        angelError=angelError - 360.0;
    }

    while (angelError < 0) {
        angelError=angelError + 360.0;
    }

    int direction = -1;
    if (angelError >= 337.5 || angelError < 22.5) {
        direction =  5;
        //if (angelError >= 337.5)
        // angelError =angelError - 360.;
    }
    if (angelError >= 22.5 && angelError < 67.5) {
        direction =  8;
        angelError =angelError - 45.;
    }
    if (angelError >= 67.5 && angelError < 112.5) {
        direction =  7;
        angelError =angelError - 90.;
    }
    if (angelError >= 112.5 && angelError < 157.5) {
        direction =  6;
        angelError =angelError - 135.;
    }
    if (angelError >= 157.5 && angelError < 202.5) {
        direction =  3;
        angelError =angelError - 180.;
    }
    if (angelError >= 202.5 && angelError < 247.5) {
        direction =  0;
        angelError =angelError - 225.;
    }
    if (angelError >= 247.5 && angelError < 292.5) {
        direction =  1;
        angelError =angelError - 270.;
    }
    if (angelError >= 292.5 && angelError < 337.5) {
        direction =  2;
        angelError =angelError - 315.;
    }

    return direction;
}

void SimpleAgent::calculateProbabilityVector() {
    int counter = 0;
    std::vector<ublas::vector<double> > attractionVectors;
    ublas::vector<double> mf(rules->Moving.size());
    ublas::vector<double> probvec(9);
    probvec*=0;

    BOOST_FOREACH (std::vector<SimpleAgentRule*>  r, rules->Moving ) {
        if ( rules->MovingFactors.size() > 0 ) {
            mf[counter] = rules->MovingFactors[counter]->evaluateExpression();
            counter++;
        } else {
            mf[counter++] = 1;
        }
        ublas::vector<double> vec(this->neighbourhoud);
        for ( int i = 0; i < this->neighbourhoud; i++ ) {
            if(i != 4) {
                vec[i] = r[i]->evaluateExpression();
            } else {
                vec[i] = 0;
            }
        }
        attractionVectors.push_back(vec);
        //std::cout << vec << std::endl;

    }

    counter = 0;
    double denumerator = 0;
    BOOST_FOREACH(ublas::vector<double> v, attractionVectors) {
        double w = mf[counter++];
        probvec += v * w;
        denumerator+= ublas::norm_1( v ) * w;
    }
    if (denumerator!= 0) {
        probvec/=denumerator;
    }
    for ( int i = 0; i < this->neighbourhoud; i++ ) {
        ProbabilityVector[i] = 100 * probvec[i];
        if ( i > 0)
            ProbabilityVector[i]+=ProbabilityVector[i-1];
    }
}
Point SimpleAgent::calculateNewPosition( int direction ) {
    Point p;
    switch (direction) {
    case 0:
        p.x = -1;
        p.y = -1;
        break;
    case 1:
        p.x = 0;
        p.y = -1;
        break;
    case 2:
        p.x = 1;
        p.y = -1;
        break;
    case 3:
        p.x = -1;
        p.y = 0;
        break;
    case 4:
        p.x = 0;
        p.y = 0;
        break;
    case 5:
        p.x = 1;
        p.y = 0;
        break;
    case 6:
        p.x = -1;
        p.y = 1;
        break;
    case 7:
        p.x = 0;
        p.y = 1;
        break;
    case 8:
        p.x = 1;
        p.y = 1;
        break;
    }
    return p;
}
void SimpleAgent::Acting() {



    for ( unordered_map<std::string, SimpleAgentRule*   >::const_iterator it = rules->Mark.begin(); it != rules->Mark.end(); ++it){
        controller->getMarkerMap(it->first)->setValue(pos.x, pos.y, it->second->evaluateExpression());
    }



}
std::vector<SimpleAgent*>  SimpleAgent::run() {
    std::vector<SimpleAgent*> agents;

    //Update Status Maps
    controller->refreshNeighbourhood(this->pos);
    controller->refreshStatus(this);
    unordered_map<std::string, double> tmp;
    BOOST_FOREACH ( Status_map::value_type r, rules->StatusVariable) {
        double d =r.second->evaluateExpression();
        tmp[r.first] = d;
    }
    this->status = tmp;
    controller->refreshStatus(this);

    Acting();

    //AliveRule
    double status = 0;
    int clones = 0;
    BOOST_FOREACH(SimpleAgentRule * r, rules->Alive ) {
        status = r->evaluateExpression();
    }



    if (pos.x < 0 || pos.y < 0 || pos.x > rules->Width || pos.y > rules->Height) {
        status = -1;
    } else {
        //Evaluate SuccessRule
        BOOST_FOREACH(SimpleAgentRule * r, rules->SuccessRule ) {
            int success = r->evaluateExpression();
            //std::cout <<"SUC " << success << std::endl;
            if (success == 0 ) {
                this->success = 1;
            }

        }

    }

    if (status > -1 ) {
        agents.push_back(this);
    }
    if (success == 1 ) {
        if (rules->ExportPathToVectorData.compare("") != 0) {
            std::vector<Point> ps;
            if (this->rules->ExportPathOption.compare("EndPoints") == 0 ) {
                ps.push_back( this->PathHistory.front());
                //this->PathHistory.pop_back();
                ps.push_back( this->PathHistory.back());

            } else {
                //ps.push_back( this->PathHistory.front());
                //ps.push_back( this->PathHistory.back());
                ps = this->PathHistory;
            }
            std::vector<Edge> ve;
            for (int i = 1; i < ps.size(); i++) {
                ve.push_back(Edge(i-1,i));
            }
            std::stringstream s;
            s << "Agent" << "_" << this->id << rand() % 10 ;



            controller->vecExportPath.setPoints(s.str(), ps);
            controller->vecExportPath.setEdges(s.str(), ve);
            controller->vecExportPath.setDoubleAttributes(s.str(), attributeAge);

        }
    }

    BOOST_FOREACH(SimpleAgentRule * r, rules->Clone ) {
        clones =(int) r->evaluateExpression();
    }

    for ( int i = 0; i < clones; i++ ) {
        SimpleAgent * a =new SimpleAgent(*this, i);
        agents.push_back(a);
    }



    if(status > -1) {
        Movement();

        this->age++;
    }
    return agents;
}
void SimpleAgent::Movement() {

    this->ProbabilityVector = ublas::vector<double>(this->neighbourhoud);
    this->ProbabilityVector*=0;
    double r = rand()%100;
    double angle = 0;
    int direction = 0;
    //Choose direction

    if (rules->RuleType.compare("Attraction") == 0) {
        calculateProbabilityVector();
        bool directionset = false;
        double buffer = 0;

        for ( int i = 0; i < this->neighbourhoud; i++ ) {
            if ( r > ProbabilityVector[i] ) {
                direction = i+1;
            } else {
                directionset = true;
                break;
            }
        }
        if (directionset == false ) {
            direction = 4;
        }
    }

    if (rules->RuleType.compare("Direct") == 0) {
        BOOST_FOREACH (std::vector<SimpleAgentRule*>  r, rules->Moving ) {

            if (rules->CellBased == true) {
                direction = r[0]->evaluateExpression();
            } else {
                angle  = r[0]->evaluateExpression();
                direction = this->convertDeegreeToCells( r[0]->evaluateExpression());
            }


        }

    }

    if (rules->RuleType.compare("Direct") == 0) {
        if (rules->CellBased == false) {
            this->Direction = angle;
        }
    }

    Point deltaP = calculateNewPosition(direction);
    pos.x += deltaP.x;
    pos.y += deltaP.y;
    PathHistory.push_back(Point(pos.x * rules->cellSize+rules->cellSize/2., pos.y* rules->cellSize+rules->cellSize/2., this->age));
    attributeAge.push_back(age);

    this->Direction = direction;
}

SimpleAgent::SimpleAgent(const SimpleAgent &a , int siblingID) {
    std::vector<Point> emptyPath;
    PathHistory = emptyPath;
    Point StartPos;
    /*  if(a.PathHistory.size() > 1)
        StartPos = a.PathHistory.at(a.PathHistory.size()-2);
    else*/
    StartPos = a.PathHistory.at(a.PathHistory.size()-1);
    PathHistory.push_back(StartPos);

    this->pos = Point(StartPos.x/a.rules->cellSize-0.5,  StartPos.y/a.rules->cellSize-0.5, StartPos.z/a.rules->cellSize-0.5 );
    this->age = 0;
    this->success = -1;
    this->neighbourhoud = a.neighbourhoud;
    this->Direction = a.Direction;
    this->generation = a.generation+1;
    this->controller = a.controller;
    this->rules = a.rules;
    this->angelError = a.angelError;
    BOOST_FOREACH(Status_map::value_type s, this->rules->StatusVariable) {
        this->status[s.first] = a.status.at(s.first);
    }
    this->ProbabilityVector = a.ProbabilityVector;
    this->siblingID = siblingID;
    this->id = controller->newAgentID();
    this->parentID = a.id;
}
SimpleAgent::~SimpleAgent() {

}
