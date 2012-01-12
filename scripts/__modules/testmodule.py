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
import pyvibehelper
class Test(Module):
        def __init__(self):
            Module.__init__(self)
            self.Vec = pyvibe.VectorDataIn
            self.Vecout = pyvibe.VectorDataIn
            self.addParameter(self, "Vec", VIBe2.VECTORDATA_IN)
            self.addParameter(self, "Vecout", VIBe2.VECTORDATA_OUT)
            
    
        def run(self):
            vec = self.Vec.getItem()            
            vecout = self.Vecout.getItem()
            vecout.addVectorData(vec)
            attr = pyvibe.Attribute()
            attr.setAttribute("test", 1)
            vecout.setAttributes("huh", attr)
            names = vec.getAttributeNames()
            print names
            for i in range(len(names)):
                print names[i]
            names = pyvibehelper.findElementsWithIdentifier("GRID_", vec.getAttributeNames())
            for i in range(len(names)):
                print names[i]
                


