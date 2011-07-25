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
#include "compilersettings.h"

#ifndef SIMPLEAGENT_H
#define SIMPLEAGENT_H

#include "vectordata.h"
#include "rasterdata.h"
#include "muParser.h"
#include "simpleagentrule.h"
#include "simpleagentmodel.h"


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/unordered_map.hpp>

using namespace boost;
namespace   ublas = boost::numeric::ublas;
class SimpleAgentModel;
struct SARules;


class VIBE_HELPER_DLL_EXPORT SimpleAgent
{
private:
    std::vector<Point> PathHistory;
    std::vector<double> attributeAge;
    Point pos;
    int age;
    int id;
    int neighbourhoud;
    double Direction;
    int generation;
    double angelError;
    int siblingID;
    int parentID;
    int success;
    SimpleAgentModel * controller;
    SARules * rules;

    unordered_map<std::string, double> status;

    Point calculateNewPosition(int direction);

    void Acting();
    void Movement();

    void calculateProbabilityVector();

    int convertDeegreeToCells(double angel);


    ublas::vector<double> ProbabilityVector;
public:

    SimpleAgent(Point & startpos,
                SARules * rules,
                SimpleAgentModel   * controller);
    SimpleAgent(const SimpleAgent & a, int siblingID=0);
    virtual std::vector<SimpleAgent*> run();
    int getAge() const {return age;}
    int getDirection() const {return Direction;}
     unordered_map<std::string, double> getStatus() const {return status;}
    Point getPosition() const {return pos;}
    void setGeneration(int g){generation = g;}
    int getGeneration() const {return generation;}
    int getSiblingID() const {return siblingID;}
    int getParentID() const {return parentID;}
    int getSuccess() const {return success;}
    long getID() const {return id;}
    ~SimpleAgent();

};

#endif // SIMPLEAGENT_H
