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
#include "vectordataaddition.h"
#include <QThread>
#include <boost/foreach.hpp>
#include "vectordata.h"
VIBe_DECLARE_NODE_NAME( VectorDataAddition,Modules )
VectorDataAddition::VectorDataAddition()
{
}
VectorDataAddition::~VectorDataAddition()
{
}

void VectorDataAddition::run() {
    std::cout << "Running VectorData_in RasterMap" << std::endl;
    VectorData out_vec;

    for ( std::vector<std::string>::const_iterator it=param.VectorData_in.begin(); it != param.VectorData_in.end(); ++it) {
        std::cout << *it << std::endl;

        VectorData add_vec;

        if ( paramRaw.find(*it)!= paramRaw.end() ) {
            try {
                add_vec = this->getVectorData(paramRaw.at(*it));
            }  catch(int e) {
                std::cout << "Error: No Input Data" << std::endl;

            }

        }  else {
            std::cout << "Error: parameter " << *it << " not set" << std::endl;
            QThread::currentThread()->exit();;
        }

        BOOST_FOREACH (std::string name, add_vec.getAttributeNames()) {
            out_vec.setAttributes(name, add_vec.getAttributes(name));
        }
        BOOST_FOREACH (std::string name, add_vec.getPointsNames()) {
            out_vec.setPoints(name, add_vec.getPoints(name));
        }
        BOOST_FOREACH (std::string name, add_vec.getEdgeNames()) {
            out_vec.setEdges(name, add_vec.getEdges(name));
        }
        BOOST_FOREACH (std::string name, add_vec.getFaceNames()) {
            out_vec.setFaces(name, add_vec.getFaces(name));
        }
        BOOST_FOREACH (std::string name, add_vec.getLinkNames()) {
            out_vec.setLinks(name, add_vec.getLinks(name));
        }

    }

    if(param.Flat == true) {
        VectorData new_vec;
        std::vector<Point> new_points;
        std::vector<Edge> new_edges;
        BOOST_FOREACH (std::string name, out_vec.getEdgeNames()) {
            std::vector<Edge> edges = out_vec.getEdges(name);
            std::vector<Point> points = out_vec.getPoints(name);

            if (QString::fromStdString(name).contains("Agent") == false || edges.size()==1) {
                BOOST_FOREACH (Edge e, edges) {
                    new_points.push_back(points.at(e.getID1()));
                    new_points.push_back(points.at(e.getID2()));
                    new_edges.push_back(Edge(new_points.size()-2, new_points.size()-1));
                }
            } else {
                if(edges.size() > 1) {
                    Edge e1;
                    Edge e2;
                    e1 = edges.at(0);
                    e2 = edges.at(edges.size()-1);
                    new_points.push_back(points.at(e1.getID1()));
                    new_points.push_back(points.at(e2.getID2()));
                    new_edges.push_back(Edge(new_points.size()-2, new_points.size()-1));

                }
            }
        }

        new_vec.setPoints(param.Name, new_points);
        new_vec.setEdges(param.Name, new_edges);

        out_vec = new_vec;
    }
    this->setVectorData(param.VectorData_out, out_vec);



}
void VectorDataAddition::init(const parameter_type &parameters) {
    paramRaw = parameters;
    if (parameters.find("Output") != parameters.end()) {
        param.VectorData_out = parameters.at("Output");
    } else {
        std::cout << "Error: parameter VectorData_out not set" << std::endl;
        QThread::currentThread()->exit();
    }
    if (parameters.find("Name") != parameters.end()) {
        param.Name = parameters.at("Name");
    } else {
        std::cout << "Error: parameter VectorData_out not set" << std::endl;
        QThread::currentThread()->exit();
    }
    if (parameters.find("Input") != parameters.end()) {
        std::string buf;
        std::stringstream ss(parameters.at("Input"));

        while (ss >> buf)
            param.VectorData_in.push_back(buf);
    }
    if (parameters.find("Flat") != parameters.end()) {
        if (parameters.at("Flat").compare("true") == 0) {
            param.Flat = true;
        } else if  (parameters.at("Flat").compare("false") == 0) {
            param.Flat = false;
        } else {
            std::cout << "Parameter: Flat true|false not set" << std::endl;
            QThread::currentThread()->exit();;
        }

    } else {
        param.Flat = true;
    }

}
boost::shared_ptr<Module> VectorDataAddition::clone() const {
    return boost::shared_ptr<Module>(new VectorDataAddition);
}

