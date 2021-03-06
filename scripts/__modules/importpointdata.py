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
Created on Fri May 21 09:26:01 2010

@author: c8451045
"""
from pyvibe import *

import csv
from math import * 


def rotate2d(degrees,point,origin):
    """
    A rotation function that rotates a point around a point
    to rotate around the origin use [0,0]
    """
    x = point[0] - origin[0]
    yorz = point[1] - origin[1]
    newx = (x*cos(radians(degrees))) - (yorz*sin(radians(degrees)))
    newyorz = (x*sin(radians(degrees))) + (yorz*cos(radians(degrees)))
    newx += origin[0]
    newyorz += origin[1] 
    
    return newx,newyorz

class ImportPointData(Module):        

    def __init__(self):
            Module.__init__(self)

    def run(self):
        spamReader = csv.reader(open('/home/c8451045/Documents/vibe2/data/maps/adresspunkte_wohnbau.csv'), delimiter='\t', quotechar='"')
        points = PointList()
        stories = DoubleAttributesList()
        heatd = DoubleAttributesList()
        BGF = DoubleAttributesList()
        ID = DoubleAttributesList()
        GWR = DoubleAttributesList()
        counter = 0
        for row in spamReader:
            if counter != 0:
                x = (float(row[6])-77208.7376)
                y = (float(row[7])-234840.0635)
                x, y = rotate2d(-2.2088,[x,y], [0,0])    
                GWR.append(int(float(row[4])))
                points.append(Point(x,y,0))
                heatd.append(float(row[5]))
                BGF.append(float(row[3]))
                stories.append(float(row[2]))
                ID.append(float(row[0]))
            counter = counter +1 
        v = VectorData()
                    
        v.setPoints("Points", points)
        v.setDoubleAttributes("Stories", stories)
        v.setDoubleAttributes("HeatDemands", heatd)
        v.setDoubleAttributes("BGFs", BGF)
        v.setDoubleAttributes("IDs", ID)
        v.setDoubleAttributes("GWR", GWR)
        self.setPoints(self.mapName, v)

        
    def init(self, params):
        if params.has_key("VectorData") == True:        
            self.mapName = params["VectorData"].split()[0]
