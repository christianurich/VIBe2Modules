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

from pyvibe import *
import pyvibehelper
class NumberOfFaces(Module):
    def __init__(self):
        Module.__init__(self)
        self.NumberOfHouses = 0
        self.roofAreaPercent = 0
        self.vec = VectorDataIn()
        self.addParameter(self, "NumberOfHouses",  VIBe2.DOUBLEDATA_OUT)
        self.addParameter(self, "vec", VIBe2.VECTORDATA_IN)
        self.addParameter(self, "roofAreaPercent", VIBe2.DOUBLEDATA_OUT)
    def run(self):
        roofArea = 0
        self.NumberOfHouses = 0
        vec = self.vec.getItem()
        names = vec.getFaceNames()
        for i in range(len(names)):
             faces = vec.getFaces(names[i])
             for j in range(len(faces)):
                 area = pyvibehelper.calculateArea(faces[j], vec.getPoints(names[i]))
                 log(str(roofArea), LogLevel.standard)
                 roofArea += area
                 if (area > 60):
                     self.NumberOfHouses+=1
        log(str(roofArea), LogLevel.standard)
        self.roofAreaPercent = roofArea
