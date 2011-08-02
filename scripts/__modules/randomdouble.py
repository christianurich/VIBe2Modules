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
Created on Fri Jun  4 09:51:37 2010

@author: c8451045
"""

from pyvibe import *
import pyvibe
import random

class RandomDouble(Module):
    def __init__(self):
        Module.__init__(self)
        self.DoubleData = 0
        self.addParameter(self, "DoubleData", VIBe2.DOUBLEDATA_OUT)
    def run(self):
        self.DoubleData = random.randint(0,100)
        
        
