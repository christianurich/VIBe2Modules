"""
@file
@author  Chrisitan Urich <christian.urich@gmail.com>
@version 1.0
@section LICENSE

This file is part of VIBe2
Copyright (C) 2011  Christian Urich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
"""

# -*- coding: utf-8 -*-
"""
Created on Wed Jul 14 14:18:45 2010

@author: c8451045
"""

from pyvibe import *


class AppendCatchment(Module):
        def __init__(self):
            Module.__init__(self)  

            self.topoElevation = RasterData
            self.Population = RasterData
            self.Catchments = VectorData()
            self.MidPoints= VectorData()
            self.addParameter(self, "topoElevation", VIBe2.RASTERDATA_IN)
            self.addParameter(self, "Population", VIBe2.RASTERDATA_IN)
            self.addParameter(self, "MidPoints", VIBe2.VECTORDATA_IN)
            self.addParameter(self, "Catchments", VIBe2.VECTORDATA_OUT)
            
        def run(self):
            names = self.MidPoints.getPointsNames()
            for i in range(len(names)):
                print names[i]
                
            
            
