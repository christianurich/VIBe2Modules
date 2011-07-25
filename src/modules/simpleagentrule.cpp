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
#include "userdefinedfunctions.h"
#include <sstream>
#include <boost/foreach.hpp>
#include "simpleagent.h"
#include "simpleagentrule.h"

SimpleAgentRule::SimpleAgentRule(const std::string & rule, AgentStatusMap * ag ,PointerMap * NeighbourhoodPointerMap, int NeighbourhoodSize , PointerMap * StatusPointerMap )
{
    mu::Parser * p = new mu::Parser;

    p->DefineConst("death",  -1);
    p->DefineConst("alive",  0);
    p->DefineConst("unsuccessful",  -1);
    p->DefineConst("successful",  0);
    p->DefineVar("dir", ag->dir);
    p->DefineVar("age", ag->age);
    p->DefineVar("gen", ag->gen);
    p->DefineVar("siblingID", ag->siblingID);
    p->DefineVar("parentID", ag->parentID);
    p->DefineVar("ID", ag->id);
    p->DefineVar("success", ag->success);
    p->DefineFun("random", mu::random, false);
    p->DefineFun("printValue", mu::printValue, false);
    for (PointerMap::const_iterator it = NeighbourhoodPointerMap->begin(); it != NeighbourhoodPointerMap->end(); ++it) {
        for (int i = 0; i < NeighbourhoodSize; i++ ) {
            std::stringstream ss;
            ss << it->first << i;
            p->DefineVar(ss.str(),  & it->second[i]);
        }
    }

    for (PointerMap::const_iterator it = StatusPointerMap->begin(); it != StatusPointerMap->end(); ++it) {
        //std::cout << it->first << std::endl;
        p->DefineVar(it->first,  it->second);
    }
    p->SetExpr(rule);
    this->expression = p;
}

double SimpleAgentRule::evaluateExpression() {
    try {
        return  expression->Eval();

    }catch (mu::Parser::exception_type &e) {
        std::cout << e.GetMsg() << std::endl;
    }
}

SimpleAgentRule::~SimpleAgentRule() {  
    delete expression;
}

