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
import pyvibe
from numpy import *
from os import  *
#from scipy import *

class WhiteNoise2(Module):
        def __init__(self):
            Module.__init__(self)
            self.Height = 200
            self.Width = 200
            self.CellSize = 20
            self.RasterData = RasterDataIn
            self.addParameter(self, "Height", VIBe2.LONG)
            self.addParameter(self, "Width", VIBe2.LONG)
            self.addParameter(self, "CellSize", VIBe2.DOUBLE)
            self.addParameter(self, "RasterData", VIBe2.RASTERDATA_OUT)

    
        def run(self):
            pyvibe.log("Run White Noise")
            r = self.RasterData.getItem()
            a = random.random((self.Width,self.Height))
            r.setSize(self.Width, self.Height, self.CellSize)
            for i in range(self.Width):
                for j in range(self.Height):
                    r.setValue(i,j,a[i,j])

