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
#include "simpleagentmodel.h"
#include <QString>
#include <boost/foreach.hpp>
#include <algorithm>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <sstream>

#include <QThread>

VIBe_DECLARE_NODE_NAME( SimpleAgentModel,Modules )
SimpleAgentModel::SimpleAgentModel()
{
    
}
SimpleAgentModel::~SimpleAgentModel(){
    std::cout << "Remove SimpleAgentModel" << std::endl;
    if (init_called){
        
        delete this->rules;
        BOOST_FOREACH( std::string m, param.RData ) {
            delete Maps[m];
        }
        delete this->as.age;
        delete this->as.dir;
        delete this->as.gen;
        delete this->as.siblingID;
        delete this->as.id;
        delete this->as.parentID;
        delete this->as.success;
    }
}

void SimpleAgentModel::initPointerMaps() {
    std::sort(param.RData.begin(), param.RData.end());
    //Neighbourhood
    BOOST_FOREACH( std::string m, param.RData ) {
        if ( paramRaw.find(m)!= paramRaw.end() ) {
            double * d = new double[this->NeighbourhoodSize];
            this->NeighbourhoodPointerMap[m] = d;
            Maps[m] =  & this->getRasterData(paramRaw[m]);
        }  else {
            std::cout << "Error: parameter " << m << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
    //StatusDataMap
    std::sort(param.Status.begin(), param.Status.end());
    BOOST_FOREACH( std::string s, param.Status) {
        if ( paramRaw.find(s)!= paramRaw.end() ) {
            double * d = new double;
            this->StatusPointerMap[s] = d;
        }  else {
            std::cout << "Error: parameter " << s << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
    
    BOOST_FOREACH( std::string s, param.MarkingMaps ) {
        RasterData *rdata;
        std::string mapName;
        if ( paramRaw.find(s)!= paramRaw.end() ) {
            mapName = paramRaw.at(s);
        } else {
            std::cout << "Error: parameter " << s << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
        try {            
            rdata = new RasterData( this->getRasterData(mapName)  ) ;
        }  catch(int e) {
            rdata = new RasterData( param.Width, param.Height, param.CellSize );
            this->setRasterData(mapName, *rdata);
        }
        MarkingMaps[s] = rdata;
        double * d = new double[this->NeighbourhoodSize];
        this->NeighbourhoodPointerMap[s] = d;
    }
    
}
void SimpleAgentModel::initRules() {
    this->rules->ExportPathToVectorData = param.ExportPathToVectorData;
    this->rules->ExportPathOption = param.ExportPathOption;
    
    //Init Status Variables
    BOOST_FOREACH(std::string r, param.Status) {
        if ( paramRaw.find(r)!= paramRaw.end() ) {
            this->rules->StatusVariable[r] = (new SimpleAgentRule(paramRaw[r], &this->as, & this->NeighbourhoodPointerMap,  this->NeighbourhoodSize, & this->StatusPointerMap));
        } else {
            std::cout << "Rule " << r << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
    //Init Movement Rules
    //First get MovementRuleType
    if ( param.MovingRuleType.compare("Attraction") == 0 )  {
        BOOST_FOREACH(std::string s, param.MovingRule) {
            if ( paramRaw.find(s)!= paramRaw.end() ) {
                std::vector<SimpleAgentRule*> rules;
                for ( int i = 0; i < this->NeighbourhoodSize; i++ ) {
                    std::string r = paramRaw[s];
                    for (PointerMap::const_iterator it = NeighbourhoodPointerMap.begin(); it != NeighbourhoodPointerMap.end(); ++it) {
                        std::string n = it->first;
                        std::stringstream variablelist;
                        variablelist << n << i;
                        int counter = 0;
                        while (r.find(n, counter) != std::string::npos) {
                            counter = r.find(n, counter);
                            std::string s(n);
                            r.replace(counter, s.length(), variablelist.str());
                            counter = counter+variablelist.str().length();
                        }
                    }
                    rules.push_back(new SimpleAgentRule(r, &this->as, & this->NeighbourhoodPointerMap, this->NeighbourhoodSize, & this->StatusPointerMap) );
                }
                this->rules->Moving.push_back(rules);
                this->rules->RuleType = "Attraction";
                this->rules->CellBased =param.CellBased;
                this->rules->Height = param.Height;
                this->rules->Width = param.Width;
                this->rules->cellSize = param.CellSize;
            }  else {
                std::cout << "Rule " << s << " not set" << std::endl;
                QThread::currentThread()->exit();;
            }
        }
        
        
        BOOST_FOREACH(std::string r, param.MovingRuleFactors) {
            if ( paramRaw.find(r)!= paramRaw.end() ) {
                this->rules->MovingFactors.push_back(new SimpleAgentRule(paramRaw[r], &this->as, & this->NeighbourhoodPointerMap, this->NeighbourhoodSize, & this->StatusPointerMap));
            } else {
                std::cout << "Rule " << r << " not set" << std::endl;
                QThread::currentThread()->exit();;
            }
        }
        
        
        
        
    } else if ( param.MovingRuleType.compare("Direct") == 0 )  {
        BOOST_FOREACH(std::string s, param.MovingRule) {
            std::vector<SimpleAgentRule*> rules;
            if ( paramRaw.find(s)!= paramRaw.end() ) {
                std::string r = paramRaw[s];
                rules.push_back(new SimpleAgentRule(r, &this->as, & this->NeighbourhoodPointerMap, this->NeighbourhoodSize, & this->StatusPointerMap) );
            }
            this->rules->Moving.push_back(rules);
            this->rules->CellBased =param.CellBased;
            this->rules->RuleType = "Direct";
            this->rules->Height = param.Height;
            this->rules->Width = param.Width;
            this->rules->cellSize = param.CellSize;
            
        }
        
    }
    else {
        std::cout << "Unknown moving type" << std::endl;
        QThread::currentThread()->exit();;
    }
    
    //Init Alive Rule
    BOOST_FOREACH(std::string r, param.AliveRule) {
        if ( paramRaw.find(r)!= paramRaw.end() ) {
            this->rules->Alive.push_back(new SimpleAgentRule(paramRaw[r],  &this->as,& this->NeighbourhoodPointerMap,  this->NeighbourhoodSize, & this->StatusPointerMap));
        } else {
            std::cout << "Rule " << r << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
    //Init Clone Rule
    BOOST_FOREACH(std::string r, param.CloneRule) {
        if ( paramRaw.find(r)!= paramRaw.end() ) {
            this->rules->Clone.push_back(new SimpleAgentRule(paramRaw[r],  &this->as,& this->NeighbourhoodPointerMap,  this->NeighbourhoodSize, & this->StatusPointerMap));
        } else {
            std::cout << "Rule " << r << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
    //InitMarkerRules
    BOOST_FOREACH(std::string r, param.MarkingMaps) {
        std::stringstream ss;
        ss << "Rule_" << r;
        //if ( paramRaw.find(ss.str())!= paramRaw.end() ) {
        //std::string r1 = paramRaw[ss.str()];
        if ( paramRaw.find(ss.str())!= paramRaw.end() ) {
            this->rules->Mark[r]=new SimpleAgentRule(paramRaw[ss.str()],  &this->as, & this->NeighbourhoodPointerMap,  this->NeighbourhoodSize, & this->StatusPointerMap);
        } else {
            std::cout << "Marker Rule " << ss.str() << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
        /*} else {
            std::cout << "Map " << r << " has no marks" << std::endl;
            QThread::currentThread()->exit();;
        }*/
    }    
    //Init Success Rule
    BOOST_FOREACH(std::string r, param.SuccessRule) {
        if ( paramRaw.find(r)!= paramRaw.end() ) {
            this->rules->SuccessRule.push_back(new SimpleAgentRule(paramRaw[r],  &this->as,& this->NeighbourhoodPointerMap,  this->NeighbourhoodSize, & this->StatusPointerMap));
        } else {
            std::cout << "Rule " << r << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }
    }
}

void SimpleAgentModel::initRunTime() {
    this->NeighbourhoodSize = 9;
    this->initPointerMaps();
    this->initRules();  
}

void SimpleAgentModel::run() {
    std::cout << "Running SimpleAgentModel" << std::endl;

    
    if ( firsttime == true ){

        initRunTime();        
        
        std::vector<Point> startpos = this->getVectorData(param.StartPositions[0]).getPoints(param.StartPositions[1]);
        //Create Agents
        double CellSize = param.CellSize;
        BOOST_FOREACH( Point p, startpos ) {
            Point p1(p.x/CellSize-0.5,p.y/CellSize-0.5, 0);

            SimpleAgent * a = new SimpleAgent (p1,this->rules, this);
            a->setGeneration(1);
            agents.push_back(a);
        }
        firsttime = false;
    } else {
        if (param.NewStartPositions){
            std::vector<Point> startpos = this->getVectorData(param.StartPositions[0]).getPoints(param.StartPositions[1]);
            //Create Agents
            double CellSize = param.CellSize;
            BOOST_FOREACH( Point p, startpos ) {
                Point p1(p.x/CellSize-0.5,p.y/CellSize-0.5, 0);

                SimpleAgent * a = new SimpleAgent (p1,this->rules, this);
                a->setGeneration(1);
                agents.push_back(a);
            }
        }
        refreshPointerOnRasterData();
        if (param.ExportPathToVectorData.compare("") != 0) {
            this->vecExportPath = this->getVectorData(param.ExportPathToVectorData);
        }
        
    }
    this->vecExportPath = VectorData();
    int c = 0;
    if(param.Simultaneously) {
        while ( agents.size() > 0 && c < param.Steps) {

            c++;
            std::vector<SimpleAgent*> newAgents;
            BOOST_FOREACH( SimpleAgent *agent , agents ) {
                bool alive = false;
                std::vector<SimpleAgent*> agents = agent->run();
                BOOST_FOREACH (SimpleAgent *a, agents) {
                    if ( a == agent ) {
                        alive = true;
                    }
                    newAgents.push_back(a);
                }


                if ( alive == false ) {
                    delete agent;
                }
            }
            agents = newAgents;
        }
    }




    std::vector<int> vrands;
    for (int i = 0; i < agents.size(); i++) {
        vrands.push_back(i);
    }
    std::vector<SimpleAgent*> randomAgents;
    int range = vrands.size();
    for (int i = 0; i < range; i++) {
        int index =  (int) rand() % vrands.size();
        int id = vrands[index];
        std::vector< int >::iterator location;
        location = std::find( vrands.begin(), vrands.end(), id );
        vrands.erase(location);
        randomAgents.push_back(agents[id]);
    }


    std::cout <<"Number of Agents " << agents.size() << std::endl;
    agents = randomAgents;
    bool status = false;
    if(!param.Simultaneously) {

        while ( agents.size() > 0 && c < param.Steps && status == false) {
            std::cout <<"Number of Agents " << agents.size() << std::endl;
            std::vector<SimpleAgent*> newAgents = agents;
            for(int i = 0; i < agents.size(); i++) {
                SimpleAgent *agent  = agents[i];
                c++;
                bool alive = false;
                int age = 0;
                do {
                    age++;
                    std::vector<SimpleAgent*> agents_tmp ;
                    agents_tmp = agent->run();
                    if (agent->getSuccess() > 0 )
                        status = true;
                    alive = false;
                    BOOST_FOREACH (SimpleAgent *a, agents_tmp) {
                        if ( a == agent ) {
                            alive = true;

                        } else {
                            newAgents.push_back(a);
                        }

                    }
                    if ( alive == false ) {
                        std::vector< SimpleAgent* >::iterator location;
                        location = std::find( newAgents.begin(), newAgents.end(), agent );
                        newAgents.erase(location);
                        delete agent;
                        agent = 0;
                    }
                } while(alive == true );

                if (c >= param.Steps || status == true) {
                    break;
                }

            }
            agents = newAgents;

        }

    }

    for ( unordered_map<std::string, RasterData*>::const_iterator it = MarkingMaps.begin(); it != MarkingMaps.end(); ++it) {
        std::string mapName;
        if ( paramRaw.find(it->first)!= paramRaw.end() ) {
            mapName = paramRaw.at(it->first);
        } else {
            std::cout << "Error: parameter " << it->first << " not set" << std::endl;
            QThread::currentThread()->exit();
        }

        this->setRasterData(mapName, *it->second);
    }
    /*if (param.ExportPathToVectorData.compare("") != 0) {
        //Flaten Data
        VectorData vec;
        std::vector<Point> vp;
        std::vector<Edge> ve;
        std::vector<std::string> names = this->vecExportPath.getVectorDataNames();
        for ( int i = 0; i < names.size(); i++ ) {
            if ( QString::fromStdString(names[i]).contains("PathHistory") == true ) {
                std::vector<Point> vp_tmp = this->vecExportPath.getVectorData(names[i]);
                std::vector<Edge> ve_tmp = this->vecExportPath.getEdges(names[i]);
                long startID = vp.size();
                for (int j = 0; j < vp_tmp.size(); j++) {
                    vp.push_back(vp_tmp[j]);
                }
                for (int j = 0; j < ve_tmp.size(); j++) {
                    ve.push_back(Edge(ve_tmp[j].id1 + startID, ve_tmp[j].id2 + startID));
                }
            }
        }
        vec.setVectorData("PathHistory", vp);
        vec.setEdges("PathHistory", ve);
        
        this->setVectorData(param.ExportPathToVectorData, vec);
    }*/
    /* VectorData vec;
    std::vector<Point> vp;
    std::vector<Edge> ve;
    std::vector<std::string> names = this->vecExportPath.getVectorDataNames();
    for ( int i = 0; i < names.size(); i++ ) {

        std::vector<Point> vp_tmp = this->vecExportPath.getVectorData(names[i]);
        std::vector<Edge> ve_tmp = this->vecExportPath.getEdges(names[i]);
        if (vp_tmp.size() > 1) {
            vp.push_back(vp_tmp.at(0));
            vp.push_back(vp_tmp.at(vp_tmp.size()-1));
            ve.push_back(Edge(0,1));
            vec.setVectorData(names[i], vp_tmp);
            vec.setEdges(names[i], ve_tmp);
        }

    }*/
    this->setVectorData(param.ExportPathToVectorData, this->vecExportPath);
}
void SimpleAgentModel::refreshPointerOnRasterData() {
    
    BOOST_FOREACH( std::string m, param.RData ) {
        if ( paramRaw.find(m)!= paramRaw.end() ) {
            Maps[m] =   & this->getRasterData(paramRaw[m]);
        }  else {
            std::cout << "Error: parameter " << m << " not set" << std::endl;
            QThread::currentThread()->exit();
        }
    }
    
    BOOST_FOREACH( std::string s, param.MarkingMaps ) {
        RasterData *rdata;
        try {
            rdata = new RasterData( this->getRasterData(s)  ) ;
        }  catch(int e) {
            rdata = new RasterData( param.Width, param.Height, param.CellSize );
            this->setRasterData(s, *rdata);
        }
        MarkingMaps[s] = rdata;
    }
}
void SimpleAgentModel::refreshNeighbourhood(Point p){
    
    for (unordered_map<std::string, const RasterData * >::const_iterator it = Maps.begin(); it != Maps.end(); ++it) {
        ublas::vector<double> vec ( it->second->getMoorNeighbourhood(p.x, p.y) );
        for ( int i = 0; i < vec.size(); i++ ) {
            NeighbourhoodPointerMap.at(it->first)[i]= vec[i];
        }
    }
    for (unordered_map<std::string, RasterData * >::const_iterator it = MarkingMaps.begin(); it != MarkingMaps.end(); ++it) {
        ublas::vector<double> vec ( it->second->getMoorNeighbourhood(p.x, p.y) );
        for ( int i = 0; i < vec.size(); i++ ) {
            NeighbourhoodPointerMap.at(it->first)[i]= vec[i];
        }
    }
    
}
void SimpleAgentModel::refreshStatus(const SimpleAgent * agent) {
    //Refresh Status
    int counter = 0;
    *(this->as.age) = agent->getAge();
    *(this->as.dir) = agent->getDirection();
    *(this->as.gen) = agent->getGeneration();
    *(this->as.siblingID) = agent->getSiblingID();
    *(this->as.id) = agent->getID();
    *(this->as.parentID) = agent->getParentID();
    *(this->as.success) = agent->getSuccess();
    
    for (unordered_map<std::string, double * >::const_iterator it = StatusPointerMap.begin(); it != StatusPointerMap.end(); ++it) {
        *StatusPointerMap.at(it->first)= agent->getStatus().at(it->first);
        counter++;
    }
}

int SimpleAgentModel::newAgentID() {
    return this->AgentIDs++;
}

void SimpleAgentModel::init(const parameter_type &parameters) {
    srand((unsigned)time(0));
    this->rules = new SARules;
    this->as.age = new double;
    this->as.dir = new double;
    this->as.gen = new double;
    this->as.siblingID = new double;
    this->as.id = new double;
    this->as.parentID = new double;
    this->as.success= new double;
    
    AgentIDs = 1;
    firsttime = true;
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
        std::cout << "Error: parameter Width not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("CellSize") != parameters.end()) {
        param.CellSize = QString::fromStdString(parameters.at("CellSize")).toDouble();
    } else {
        std::cout << "Error: parameter CellSize not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("Steps") != parameters.end()) {
        param.Steps = QString::fromStdString(parameters.at("Steps")).toInt();
    } else {
        std::cout << "Error: parameter Steps not set" << std::endl;
        QThread::currentThread()->exit();;
    }
    if (parameters.find("ExportPathToVectorData") != parameters.end()) {
        param.ExportPathToVectorData = parameters.at("ExportPathToVectorData");
    } else {
        param.ExportPathToVectorData = "";
    }
    
    if (parameters.find("MovingRuleType") != parameters.end()) {
        param.MovingRuleType = parameters.at("MovingRuleType");
    } else {
        std::cout << "Error: parameter MovingRuleType not set" << std::endl;
        QThread::currentThread()->exit();
    }
    
    if (parameters.find("StartPositions") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("StartPositions"));
        
        while (ss >> buf)
            param.StartPositions.push_back(buf);
        if (param.StartPositions.size()<2) {
            std::cout << "Error: parameter StartPositions" << std::endl;
            QThread::currentThread()->exit();
        }
    }
    
    if (parameters.find("MarkingMaps") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("MarkingMaps"));
        
        while (ss >> buf)
            param.MarkingMaps.push_back(buf);
    }
    
    if (parameters.find("MovingRule") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("MovingRule"));
        while (ss >> buf)
            param.MovingRule.push_back(buf);
    }
    if (parameters.find("MovingRuleFactors") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("MovingRuleFactors"));
        while (ss >> buf)
            param.MovingRuleFactors.push_back(buf);
    }
    
    if (parameters.find("AliveRule") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("AliveRule"));
        while (ss >> buf)
            param.AliveRule.push_back(buf);
    }
    if (parameters.find("CloneRule") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("CloneRule"));
        while (ss >> buf)
            param.CloneRule.push_back(buf);
    }
    if (parameters.find("SuccessRule") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("SuccessRule"));
        while (ss >> buf)
            param.SuccessRule.push_back(buf);
    }
    if (parameters.find("MarkingRule") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("MarkingRule"));
        while (ss >> buf)
            param.MarkingRule.push_back(buf);
    }
    if (parameters.find("RasterData") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("RasterData"));
        while (ss >> buf)
            param.RData.push_back(buf);
    }
    if (parameters.find("Status") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("Status"));
        while (ss >> buf)
            param.Status.push_back(buf);
    }
    if (parameters.find("ExportPathOption") != parameters.end()) {
        param.ExportPathOption = parameters.at("ExportPathOption");
    } else {
        param.ExportPathOption = "";
    }
    if (parameters.find("CellBased") != parameters.end()) {
        if (parameters.at("CellBased").compare("true") == 0) {
            param.CellBased = true;
        } else if  (parameters.at("CellBased").compare("false") == 0) {
            param.CellBased = false;
        } else {
            std::cout << "Parameter: CellBased true|false not set" << std::endl;
            QThread::currentThread()->exit();;
        }
        
    } else {
        param.CellBased = true;
    }
    if (parameters.find("NewStartPositions") != parameters.end()) {
        if (parameters.at("NewStartPositions").compare("true") == 0) {
            param.NewStartPositions = true;
        } else if  (parameters.at("NewStartPositions").compare("false") == 0) {
            param.NewStartPositions = false;
        } else {
            std::cout << "Parameter: NewStartPositions true|false not set" << std::endl;
            QThread::currentThread()->exit();;
        }
        
    } else {
        param.NewStartPositions = false;
    }
    if (parameters.find("Simultaneously") != parameters.end()) {
        if (parameters.at("Simultaneously").compare("true") == 0) {
            param.Simultaneously = true;
        } else if  (parameters.at("Simultaneously").compare("false") == 0) {
            param.Simultaneously = false;
        } else {
            std::cout << "Parameter: Simultaneously true|false not set" << std::endl;
            QThread::currentThread()->exit();;
        }

    } else {
        param.Simultaneously = true;
    }
}

boost::shared_ptr<Module> SimpleAgentModel::clone() const{
    return boost::shared_ptr<SimpleAgentModel>(new SimpleAgentModel);
}


