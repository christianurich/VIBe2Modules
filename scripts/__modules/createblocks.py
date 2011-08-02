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


class CreateBlocks(Module):
        def __init__(self):
            Module.__init__(self)  
            self.CellSize = 40            
            self.addParameter(self, "CellSize", VIBe2.DOUBLE)
            self.RasterData = RasterDataIn
            self.RasterDataIn = RasterDataIn
            self.Centers = VectorDataIn
            self.addParameter(self, "RasterData", VIBe2.RASTERDATA_OUT)
            self.addParameter(self, "RasterDataIn", VIBe2.RASTERDATA_IN)
            self.addParameter(self, "Centers", VIBe2.VECTORDATA_OUT)
            
        def run(self):
            width =  self.RasterDataIn.getItem().getWidth() * self.RasterDataIn.getItem().getCellSize()
            height =  self.RasterDataIn.getItem().getHeight() * self.RasterDataIn.getItem().getCellSize()
            
            widhtnew = int(width/self.CellSize)
            heightnew = int(height/self.CellSize)
            self.RasterData.getItem().setSize(widhtnew, heightnew, self.CellSize)
            
            Blocker = RasterData(widhtnew, heightnew, self.CellSize)
            counter = 0
            for i in range(widhtnew):
                for j in range(heightnew):
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
                    value = self.RasterDataIn.getItem().meanOverArea(pl)
                                    
                    self.Centers.getItem().setPoints("Positions"+str(counter), centers)
                    attr = Attribute()
                    attr.setAttribute("Value", value)
                    self.Centers.getItem().setAttributes("Positions"+str(counter), attr)
                    
                    
                    self.RasterData.getItem().setValue(i,j,value)    
                    counter += 1
            
