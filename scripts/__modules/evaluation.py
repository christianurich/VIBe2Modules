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
Created on Fri Jun  4 09:48:00 2010

@author: c8451045
"""

from pyvibe import *

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.path as path
import matplotlib.mlab as mlab
from scipy.stats.kde import gaussian_kde

class Evaluation(Module):
        def __init__(self):
            Module.__init__(self)
            self.DoubleData = 0
            self.RessVector=[]
            self.addParameter(self, "DoubleData", VIBe2.DOUBLEDATA_IN)
        def run(self):
            self.RessVector.append(self.DoubleData)
            if (len(self.RessVector) > 2):
                fig = plt.figure()
                ax = fig.add_subplot(111)
                '''n, bins = np.histogram(self.RessVector, 50)
                # get the corners of the rectangles for the histogram
                left = np.array(bins[:-1])
                right = np.array(bins[1:])
                bottom = np.zeros(len(left))
                top = bottom + n
                XY = np.array([[left,left,right,right], [bottom,top,top,bottom]]).T
                barpath = path.Path.make_compound_path_from_polys(XY)
                
                patch = patches.PathPatch(barpath, facecolor='blue', edgecolor='gray', alpha=0.8)
                ax.add_patch(patch)
                
                
                ax.set_xlim(left[0], right[-1])
                ax.set_ylim(bottom.min(), top.max())'''
                n, bins, patches = ax.hist( self.RessVector, normed = True)   
                approximate_pdf = gaussian_kde(self.RessVector)
                c = np.array(self.RessVector)
                x = np.linspace(c.min(), c.max(), 200)
                ax.plot(x, approximate_pdf(x), 'r')

                filename = "plot" 
                filename+=str(self.getT())
                plt.savefig(filename+".png")
                #plt.savefig(filename+".svg")
                plt.close()
            
