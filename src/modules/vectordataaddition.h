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

#ifndef VECTORDATAADDITION_H
#define VECTORDATAADDITION_H
#include "module.h"
using namespace vibens;
class VIBE_HELPER_DLL_EXPORT VectorDataAddition : public  Module {
VIBe_DECLARE_NODE( VectorDataAddition)


    struct Parameters {
        std::vector<std::string> VectorData_in;
        std::string VectorData_out;
        std::string Name;
        bool Flat;

    };
private:
    Parameters param;
    parameter_type paramRaw;
public:
    VectorDataAddition();
    boost::shared_ptr<Module> clone() const;
    virtual ~VectorDataAddition();
    void run();
    void init(const parameter_type &parameters);
};

#endif // VECTORDATAADDITION_H
