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


class CreateRasterGrid(Module):
	"""Creates a cell grid as vectordata
	
	Two tables are created tables
	- GRID_ID
		- Area
		- relative_x
		- relative_y
	- Center_ID
	@ingroup DAnCE4Water
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self)  
            self.CellSize = 20
            self.Width = 420
            self.Height = 200      
            self.Identifier_Centers = "Center_"
            self.Identifier_Grid = "GRID_"
            self.Grid = VectorDataIn
            self.addParameter(self, "Grid", VIBe2.VECTORDATA_OUT)
            self.addParameter(self, "CellSize", VIBe2.DOUBLE)
            self.addParameter(self, "Width", VIBe2.LONG)
            self.addParameter(self, "Height", VIBe2.LONG)
            self.addParameter(self, "Identifier_Centers", VIBe2.STRING, "Identifier of the Center Points ")
            self.addParameter(self, "Identifier_Grid", VIBe2.STRING, "Identifier of the Grids ")
            
        def run(self):
            vec = self.Grid.getItem()
            counter = 1
            g = Attribute()
	    g.setAttribute("Datatype", "GridData")
            vec.setAttributes("Globals", g)
            for j in range(int(self.Height/self.CellSize)):
                for i in range(int(self.Width/self.CellSize)):
                    centers = PointList()
                    center = Point((i+0.5) * self.CellSize, (j+0.5) * self.CellSize, 0)
                    centers.append(center)
                    x1 = Point(i*self.CellSize,j*self.CellSize,0)
                    x2 = Point( (i+1)*self.CellSize,j*self.CellSize,0)
                    x3 = Point((i+1)*self.CellSize,(j+1)*self.CellSize,0)
                    x4 = Point(i*self.CellSize,(j+1)*self.CellSize,0)
                    pl = PointList()
                    pl.append(x1)
                    pl.append(x2)
                    pl.append(x3)
                    pl.append(x4)       
                    el = EdgeList()
                    el.append(Edge(0,1))       
                    el.append(Edge(1,2))       
                    el.append(Edge(2,3)) 
                    el.append(Edge(3,0))     
                    fl = FaceList()
                    longL  = LongList()
                    longL.append(0)
                    longL.append(1)
                    longL.append(2)
                    longL.append(3)
                    longL.append(0)
                    fl.append(Face(longL))
                    vec.setPoints(self.Identifier_Centers+str(counter), centers)
                    vec.setPoints(self.Identifier_Grid+str(counter), pl)
                    
                    attr = Attribute()
                    attr.setAttribute(self.Identifier_Grid+"ID",counter)
                    vec.setAttributes(self.Identifier_Centers+str(counter), attr)    
                    
                    vec.setEdges(self.Identifier_Grid+str(counter), el)   
                    vec.setFaces(self.Identifier_Grid+str(counter), fl)
                    
                    attr = Attribute()
                    attr.setAttribute(self.Identifier_Grid+"ID", counter)    
                    attr.setAttribute('Area',self.CellSize*self.CellSize)   
                    attr.setAttribute('relative_x', i+1)
                    attr.setAttribute('relative_y', j+1)
                    vec.setAttributes(self.Identifier_Grid+str(counter), attr)          
                    counter += 1
            
