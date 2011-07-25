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
#ifndef LOOPGROUP_H
#define LOOPGROUP_H
#include "compilersettings.h"
#include "group.h"
#include <iostream>
#include <vector>
using namespace vibens;
class VIBE_HELPER_DLL_EXPORT LoopGroup : public  Group {
VIBe_DECLARE_GROUP(LoopGroup)

    public:
        LoopGroup();
        virtual ~LoopGroup(){}
        void run();
        std::vector<std::string> InRasterData;
        std::vector<std::string> OutRasterData;
        std::vector<std::string> InVectorData;
        std::vector<std::string> OutVectorData;
        std::vector<std::string> InDoubleData;
        std::vector<std::string> OutDoubleData;
        int Runs;
        int i;
};

#endif // LOOPGROUP_H
