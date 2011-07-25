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
#include "cellularautomata.h"
#include <QString>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include "userdefinedfunctions.h"
#include <QThread>
#include <guicellularautomata.h>
#include <QWidget>
#include <QStringList>
#include <boost/foreach.hpp>

using namespace boost;
namespace   ublas = boost::numeric::ublas;

VIBe_DECLARE_NODE_NAME( CellularAutomata, Modules )

        CellularAutomata::CellularAutomata()
{
    runinit = false;
    param.Width = 200;
    param.Height = 200;
    param.CellSize = 20;    
    param.Steps = 1;
    //param.OutputMap = & this->createRasterData("OutputMap");
    this->addParameter("Width", VIBe2::LONG, &this->param.Width);
    this->addParameter("Height", VIBe2::LONG, &this->param.Height);
    this->addParameter("CellSize", VIBe2::DOUBLE, &this->param.CellSize);
    this->addParameter("Steps", VIBe2::INT, &this->param.Steps);
    this->addParameter("Neighs", VIBe2::STRING_MAP, &this->param.neighs);
    this->addParameter("Rules", VIBe2::STRING_MAP, &this->param.rules);
    this->addParameter("Landscapes", VIBe2::USER_DEFINED_RASTERDATA_IN, &this->landscapes);
    this->addParameter("OutputMap", VIBe2::RASTERDATA_OUT, &this->param.OutputMap);
    this->addParameter("Desicion", VIBe2::STRING, &this->param.Desicion);
}

bool CellularAutomata::createInputDialog() {
    QWidget * w = new GUICellularAutomata(this);
    w->show();
    return true;
}

void CellularAutomata::run()  {
    this->param.OutputMap->setSize(param.Width, param.Height, param.CellSize);
  /*  for (std::map<std::string, RasterData *>::iterator it = this->landscapes.begin(); it != this->landscapes.end(); ++it) {
         Logger(Debug) << it->first  ;
        RasterData * r = it->second;
         Logger(Debug) << r->getValue(10,10)  ;
    }
    for (std::map<std::string, std::string>::iterator it = this->param.neighs.begin(); it != this->param.neighs.end(); ++it) {
         Logger(Debug) << it->first <<  it->second  ;
    }*/

    if (runinit == false) {
        this->initRuntime();
    }

    //Eval Expressions
    for (int x = 0; x < this->param.Width; x++) {
        for (int y = 0; y < this->param.Height; y++) {
            //Update RasterData
            BOOST_FOREACH(std::string s, this->NeighboorhoodList) {
                RasterData * r = landscapes[this->NeighboorhoodMapName[s]];
                r->getNeighboorhood(this->NeighboorhoodMaps[s], this->NeighboohoodDimensions[s].widht,this->NeighboohoodDimensions[s].height, x, y );
            }
            for (int counter = 0; counter < this->Rules.size(); counter++) {

                Parser * p=this->Rules.at(counter);
                try
                {
                     *(this->RulesResults.at(counter)) =  p->Eval();
                } catch (Parser::exception_type &e)
                {
                     Logger(Error) << p->GetExpr()  ;
                     Logger(Error) << e.GetMsg()  ;
                }


            }
            this->param.OutputMap->setValue(x,y,this->Desicion->Eval());
        }

    }


}


void CellularAutomata::initRuntime() {
    runinit =true;
    //init Pointer Maps;
    for ( std::map<std::string, std::string>::iterator it = param.neighs.begin(); it != param.neighs.end(); ++it) {
        std::string name= it->first;
        QString  value = QString::fromStdString(it->second);
        QStringList list = value.split("+|+");
        std::string dataFromPort = list[0].toStdString();

        //Find Matrix Dimension;
        list[2]  = list[2].remove("|");
        QStringList rows = list[2].split(";");
        double **Neighbourhood;
        int **Neighbourhood_Stamp;
        int width = rows.size();
        Neighbourhood = new double*[width];
        Neighbourhood_Stamp = new int*[width];
        int height;
        int i = 0;
        foreach(QString s, rows) {
             //Logger(Debug) << s.toStdString()  ;
            height = s.split(",").size();
            Neighbourhood[i] = new double[height];
            Neighbourhood_Stamp[i] = new int[height];
            int j = 0;
            foreach(QString v, s.split(",")) {
                // Logger(Debug) << v.toInt();
                Neighbourhood_Stamp[i][j] = v.toInt();
                j++;
            }
            i++;
        }
        this->NeighboorhoodMaps[name] = Neighbourhood;
        this->NeighboorhoodStamps[name] = Neighbourhood_Stamp;
        this->NeighboorhoodMapName[name] = dataFromPort;
        Dimension dim;
        dim.widht = width;
        dim.height = height;


        //Calculate Neighboorhoud PointerMap
        int sum = 0;
        for (int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                sum = sum + Neighbourhood_Stamp[x][y];
            }
        }
         //Logger(Debug) << sum  ;
        double **d = new double*[sum];
        i = 0;
        for (int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                if (Neighbourhood_Stamp[x][y] > 0 ) {
                    d[i] = & Neighbourhood[x][y];
                    i++;
                }
            }
        }
        this->NeighboorhoodPointerMap[name] = d;
        dim.elements = sum;
        this->NeighboohoodDimensions[name] = dim;
        NeighboorhoodList.push_back(name);
    }
    //Init Rules;

    for(std::map<std::string, std::string>::iterator it = param.rules.begin(); it != param.rules.end(); ++it) {
        std::string Name = it->first;
        QString rule = QString::fromStdString(it->second);
        Parser * p = new Parser();
        mu::addCorineConstants(p);
        p->DefineFun("nov", numberOfValues);
        p->DefineFun("rand", random);
        //Replace Names
        BOOST_FOREACH(std::string neigh, NeighboorhoodList) {
            int elements = NeighboohoodDimensions[neigh].elements;
            std::stringstream ss;
            for (int i = 0; i < elements; i++) {
                if (i > 0)
                    ss << ",";
                ss << neigh << i;

                std::stringstream varname;
                varname << neigh << i;
                double ** d =  NeighboorhoodPointerMap[neigh];
                p->DefineVar(varname.str(), d[i]);
            }
            rule.replace(QString::fromStdString(neigh),QString::fromStdString(ss.str()));
        }
        p->SetExpr(rule.toStdString());
        Rules.push_back( p );
        RulesResults.push_back(new double);

    }
    //Define Descision
    Desicion = new Parser();
    Desicion->DefineFun("nov", numberOfValues);
    Desicion->DefineFun("rand", random);
    mu::addCorineConstants(Desicion);
    int i = 0;
    for(std::map<std::string, std::string>::iterator it = param.rules.begin(); it != param.rules.end(); ++it) {
        std::string Name = it->first;
        Desicion->DefineVar(Name, RulesResults.at(i));
        i++;
    }
    Desicion->SetExpr(param.Desicion);
     //Logger(Debug) << "Descision"  ;
}


