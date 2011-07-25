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
#include "examplemodule.h"
#include <iostream>
//#include <omp.h>
#include <cmath>
VIBe_DECLARE_NODE_NAME( ExampleModule,Modules )
ExampleModule::ExampleModule() {
}

ExampleModule::~ExampleModule() {
    std::cout << "Remove ExampleModule" << std::endl;
}

boost::shared_ptr<Module> ExampleModule::clone() const {
    return boost::shared_ptr<Module>(new ExampleModule);
}

void ExampleModule::run() {
   // int tnum = omp_get_thread_num();
    //std::cout << "[" << tnum << "]" << "running ExampleModule" << std::endl;
    for (long i = 0; i < 9999999; i++) {
        for (long j  = 0; i < 9999999; i++) {
            long x = i*j;
            (void) x;
        }
    }
}
