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
import math
class TotalLengthOfLines(Module):
    def __init__(self):
        Module.__init__(self)
        self.vec = VectorDataIn
        self.TotalLength = 0
        self.addParameter(self, "vec", VIBe2.VECTORDATA_IN)
        self.addParameter(self, "TotalLength", VIBe2.DOUBLEDATA_OUT)
    def run(self):
        vec = self.vec.getItem()
        self.TotalLength = 0
        names = vec.getEdgeNames()
        for i in range(len(names)):
            edges = vec.getEdges(names[i])
            points = vec.getPoints(names[i])
            for j in range(len(edges)):
                e = edges[i]
                id1 = e.getID1()
                id2 = e.getID2()
                p1 = points[id1]
                p2 = points[id2]
                dx = p2.getX() - p1.getX()
                dy = p2.getY() - p1.getY()
                length = math.sqrt(dx*dx+dy*dy)
                self.TotalLength += length
        log(str(self.TotalLength), LogLevel.standard)
        
