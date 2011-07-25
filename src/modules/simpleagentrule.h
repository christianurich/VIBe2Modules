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

#ifndef SIMPLEAGENTREACTINGRULE_H
#define SIMPLEAGENTREACTINGRULE_H

#include "vectordata.h"
#include "rasterdata.h"
#include <vector>
#include <map>
#include "muParser.h"


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/unordered_map.hpp>

using namespace boost;
namespace   ublas = boost::numeric::ublas;
class SimpleAgent;
struct AgentStatusMap;
typedef unordered_map<std::string, double * >  PointerMap;


class VIBE_HELPER_DLL_EXPORT SimpleAgentRule
{
private:
    mu::Parser* expression;

public:
    SimpleAgentRule(const std::string & rule ,  AgentStatusMap * ag, PointerMap * NeighbourhoodPointerMap, int NeighbourhoodSize, PointerMap * StatusPointerMap );
    double evaluateExpression();
    ~SimpleAgentRule();
};

#endif // SIMPLEAGENTREACTINGRULE_H
