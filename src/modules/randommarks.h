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

#ifndef RANDOMMARKS_H
#define RANDOMMARKS_H

#include "module.h"
using namespace vibens;
class VIBE_HELPER_DLL_EXPORT RandomMarks : public  Module {
VIBe_DECLARE_NODE( RandomMarks)

    struct Parameters {
        unsigned long Width;
        unsigned long Height;
        double CellSize;
        std::string Map;
        long NumberOfPoints;
    };
private:
    Parameters param;
public:
    RandomMarks();
    virtual ~RandomMarks(){}
    void init(const parameter_type &parameters);
    void run();
    boost::shared_ptr<Module> clone() const;
};

#endif // RANDOMMARKS_H
