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

#ifndef ANTTRAILMODEL_H
#define ANTTRAILMODEL_H
#include "module.h"
#include "simpleagent.h"
#include "simpleagentrule.h"
#include <boost/unordered_map.hpp>


using namespace boost;
#include <map>
#include "muParser.h"
typedef unordered_map<std::string, SimpleAgentRule*> Status_map;


struct VIBE_HELPER_DLL_EXPORT SARules {

     Status_map StatusVariable;
    std::vector< std::vector<SimpleAgentRule*> > Moving;
    std::string RuleType;
    bool CellBased;

    std::string ExportPathToVectorData;
    std::string ExportPathOption;
    std::vector<SimpleAgentRule*> MovingFactors;
    std::vector<SimpleAgentRule*> Alive;
    std::vector<SimpleAgentRule*> Clone;
    std::vector<SimpleAgentRule*> SuccessRule;
    unordered_map<std::string, SimpleAgentRule*> Mark;
    unsigned long Width;
    unsigned long Height;
    double cellSize;

};


struct VIBE_HELPER_DLL_EXPORT AgentStatusMap {
    double * dir;
    double * age;
    double * gen;
    double * siblingID;
    double * id;
    double * parentID;
    double * success;
};
using namespace vibens;
class VIBE_HELPER_DLL_EXPORT SimpleAgentModel : public  Module {
VIBe_DECLARE_NODE( SimpleAgentModel)

    struct Parameters {
        std::vector<std::string> Maps;
        unsigned long Width;
        unsigned long Height;
        double CellSize;

        int Steps;

        std::vector<std::string> RData;

        std::vector<std::string> StartPositions;
        std::vector<std::string> Status;               
        std::vector<std::string> MarkingMaps;

        std::vector<std::string> MovingRule;
        std::vector<std::string> MovingRuleFactors;

        std::string MovingRuleType;
        std::vector<std::string> MarkingRule;
        std::vector<std::string> AliveRule;

        std::vector<std::string> CloneRule;

        std::vector<std::string> SuccessRule;
        bool CellBased;
        std::string ExportPathToVectorData;
        bool NewStartPositions;
        bool Simultaneously;
        std::string ExportPathOption;




    };



private:
    unordered_map<std::string, const RasterData*> Maps;
    unordered_map<std::string, RasterData*> MarkingMaps;

    SARules * rules;
    PointerMap  NeighbourhoodPointerMap;
    int NeighbourhoodSize;
    PointerMap StatusPointerMap;
    std::vector<SimpleAgent*> agents;

    bool firsttime;
    int AgentIDs;
    Parameters param;
    parameter_type paramRaw;
    AgentStatusMap as;
    void initRunTime();
    void initPointerMaps();
    void initRules();
    void refreshPointerOnRasterData();


public:

    RasterData* getMarkerMap(std::string s){return MarkingMaps[s] ;}
    SimpleAgentModel();
    virtual ~SimpleAgentModel();
    boost::shared_ptr<Module>clone() const;

    virtual void init(const parameter_type &parameters);
    void run();

    void refreshNeighbourhood(Point p);
    void refreshStatus(const SimpleAgent * agent);
    int newAgentID();

    VectorData vecExportPath;


};

#endif // ANTTRAILMODEL_H
