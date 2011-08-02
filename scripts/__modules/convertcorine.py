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


class ConvertCorine(Module):        

    def __init__(self):
        Module.__init__(self)
        self.rIn = RasterDataIn()
        self.rOut = RasterDataIn()
        self.Width = 400
        self.Height = 250
        self.CellSize = 20
            
        self.addParameter(self, "Height", VIBe2.LONG)
        self.addParameter(self, "Width", VIBe2.LONG)
        self.addParameter(self, "CellSize", VIBe2.DOUBLE)
        self.addParameter(self, "rIn", VIBe2.RASTERDATA_IN)
        self.addParameter(self, "rOut", VIBe2.RASTERDATA_OUT)
            
  

   
    def run(self):
        print "Run RasterDataAddition"
        ress = self.rOut.getItem()
        ress.setSize(self.Width, self.Height, self.CellSize)
        rdata = self.rIn.getItem()
        
        for x in range(self.Width):            
            for y in range(self.Height):   
                val = rdata.getValue(x,y)
                ress.setValue(x,y,val)  
                if val == 313 or val== 321 or val== 311: 
                    ress.setValue(x,y,6)
                if val == 312: 
                    ress.setValue(x,y,6)
                if val == 231 or val ==242 or val == 211: 
                    ress.setValue(x,y,5)
                if val == 111 : 
                    ress.setValue(x,y,2)     
                if val == 112 or val == 124 or val == 122: 
                    ress.setValue(x,y,3)      
                if val == 511: 
                    ress.setValue(x,y,7)                        

 



