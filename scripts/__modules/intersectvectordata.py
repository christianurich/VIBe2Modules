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
import Polygon
from Polygon.Utils import pointList
import random
import math 

class IntersectVectorData(Module):
    def __init__(self):
        Module.__init__(self)
        self.vecIn = VectorDataIn
        self.vecOut = VectorDataIn
        self.X1 = 0.0
        self.Y1 = 0.0
        self.X2 = 0.0
        self.Y2 = 0.0
        self.addParameter(self, "X1", VIBe2.DOUBLE)
        self.addParameter(self, "Y1", VIBe2.DOUBLE)
        self.addParameter(self, "X2", VIBe2.DOUBLE)
        self.addParameter(self, "Y2", VIBe2.DOUBLE)
        self.addParameter(self, "vecIn", VIBe2.VECTORDATA_IN)
        self.addParameter(self, "vecOut", VIBe2.VECTORDATA_OUT)

    def run(self):
        print "Run IntersectVectorData"
        xmin = float(self.X1)
        ymin = float(self.Y1)
        xmax = float(self.X2)
        ymax = float(self.Y2)        
        v = self.vecIn.getItem()
        vec = self.vecOut.getItem()
        vec.clean()
        names = v.getFaceNames()
        
        for i in range(len(names)):
            #print names[i]
            faces = v.getFaces(names[i])
            edges = v.getEdges(names[i])
            points = v.getPoints(names[i])
	    attributs = v.getAttributes(names[i])
            #attributes = v.getDoubleAttributes(names[i])
            for j in range(len(faces)):
                pointsInside = True
                f = faces[j]
                ids = f.getIDs()
                for k in range(len(ids)):
                    id = ids[k]
                    point = points[id]
                    if point.getX() < xmin or point.getX() > xmax or  point.getY() < ymin or point.getY() > ymax:
                        pointsInside = False
                if pointsInside == True:
                    #print "Place"
                    vec.setPoints(names[i], points)
                    vec.setFaces(names[i], faces)
                    vec.setEdges(names[i], edges)
                    vec.setAttributes(names[i], attributs)
                    #vec.setDoubleAttributes(names[i], attributes)
                
        names = v.getEdgeNames()
       
        for i in range(len(names)):
            #print names[i]
            edges = v.getEdges(names[i])
            points = v.getPoints(names[i])
            #attributes = v.getDoubleAttributes(names[i])

            el = EdgeList()
            vl = PointList()
            
            #Check if Line is Inside
            counter = 0            

            for j in range(len(edges)):
                pointsInside = True
                e = edges[j]
                id1 = e.getID1()
                id2 = e.getID2()
                point = points[id1]
                if point.getX() < xmin or point.getX() > xmax or  point.getY() < ymin or point.getY() > ymax:
                    pointsInside = False
                point = points[id2]
                if point.getX() < xmin or point.getX() > xmax or  point.getY() < ymin or point.getY() > ymax:
                    pointsInside = False

                if pointsInside == True:
                    #e = Edge(counter*2, counter*2+1)
                    el.append(e)
                    #vl.append(points[id1])
                    #vl.append(points[id2])
                    
                    counter +=1
            
            vec.setPoints(names[i], points)
            vec.setEdges(names[i], el)
                #vec.setDoubleAttributes(names[i], attributes)
                
            
