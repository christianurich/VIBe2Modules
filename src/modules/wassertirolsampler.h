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
#ifndef WASSERTIROLSAMPLER_H
#define WASSERTIROLSAMPLER_H
#include "module.h"
using namespace vibens;
class VIBE_HELPER_DLL_EXPORT WassertirolSampler : public  Module {
VIBe_DECLARE_NODE(WassertirolSampler)
private:
    VectorData * SampleArea;
    VectorData * Sample;
    VectorData * Result;
    double cellSize;
    double NumberOfFaces;
    std::vector<int> buildRandomIndices(int size);
public:
    WassertirolSampler();
void run();
};

#endif // WASSERTIROLSAMPLER_H
