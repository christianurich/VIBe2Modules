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
from numpy import *
from scipy import *
from matplotlib.pyplot import *
from guiplotraster import *
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import os
import tempfile
class PlotRaster(Module):        

        def __init__(self):
                Module.__init__(self)
                self.vmin = 0
                self.vmax = 0
                self.RasterDataMap = RasterDataMap()
                self.addParameter(self, "RasterDataMap", VIBe2.USER_DEFINED_RASTERDATA_IN)
                   
        def run(self):
                mapNames = self.RasterDataMap.getNames()
                numberofFig = int((len(mapNames)+1)/2)
                fig = figure()
                index = 1
                if len(mapNames) > 1:
                        f = fig.add_subplot(numberofFig,2 ,1)
                        index = 2
                else:
                        f = fig.add_subplot(numberofFig,1 ,1)  
                
                for n in range(len(mapNames)):    
                        r = self.RasterDataMap.get(mapNames[n])
                        f = fig.add_subplot(numberofFig,index,n+1)
                        f.set_title(mapNames[n])
                        a = array([])    
                        b = [] 
                        nameMap = ""
                        
                        PlotStyle = ""
#                        mapName_r = "Rule_"+str(mapNames[n])
#                        if self.params.has_key(mapName_r) == True:
#                            PlotStyle = self.params[mapName_r].split()[0]
#                        print PlotStyle
                        width = r.getWidth()
                        height = r.getHeight()                            
                       
                        val = []
                        cval = array([])
                        
                        a.resize(height, width)
                        for i in range(width):
                            for j in range(height): 
                                    a[j,i] = r.getValue(i,j) * 1
                        
                        #imshow(a, origin='lower', vmin=self.vmin, vmax=self.vmax, extent=[0,width,0,height], interpolation='nearest')
                        imshow(a, origin='lower', extent=[0,width,0,height], interpolation='nearest')  
                        colorbar(ax = f,   orientation='horizontal')
                filename = "plot" 
                        #filename+=str(self.getT())
                filename+=".svg"
                savefig(tempfile.gettempdir()+'/'+filename)
                close()
                self.sendImageToResultViewer(tempfile.gettempdir()+'/'+filename)
                
                            
#                        if (PlotStyle == 'Path'):
#                            val.append (0)
#                            val.append (1)
#                            val.append (2)
#                            val.append (3)
#                            cval.resize(4,3)
#                            cval[0,0] = 1
#                            cval[0,1] = 1
#                            cval[0,2] = 1
#                            cval[1,0] = 0
#                            cval[1,1] = 0
#                            cval[1,2] = 0  
#                            cval[2,0] = 0
#                            cval[2,1] = 0
#                            cval[2,2] = 0  
#                            cval[3,0] = 0
#                            cval[3,1] = 0
#                            cval[3,2] = 0  
#                        if (PlotStyle == 'Landuse'):
#                            val.append (3)
#                            val.append (2)
#                            val.append (4)
#                            val.append (5)
#                            val.append (6)
#                            val.append (7)
#                            
#                            cval.resize(6,3)
#                            
#                            cval[0,0] = 255./255.
#                            cval[0,1] = 0
#                            cval[0,2] = 0       
#                            
#                            cval[1,0] = 139./255.
#                            cval[1,1] = 0
#                            cval[1,2] = 0
#                            
#                            cval[2,0] = 0
#                            cval[2,1] = 0
#                            cval[2,2] = 0  
#                            
#                            cval[3,0] = 0
#                            cval[3,1] = 128./255.
#                            cval[3,2] = 0  
#                            
#                            cval[4,0] = 0
#                            cval[4,1] = 100./255.
#                            cval[4,2] = 0 
#                            
#                            cval[5,0] = 0
#                            cval[5,1] = 0
#                            cval[5,2] = 255./255.  
#                        
#                        if len(val) > 0:
#                            a.resize(height, width,3)
#                            for i in range(width):
#                                    for j in range(height): 
#                                        inside = False
#                                        for v in val:
#                                            if r.getValue(i,j) ==  v:
#                                                inside = True
#                                        if (inside == True ):
#                                            a[j,i] = cval[val.index(r.getValue(i,j))]
#                                        else: 
#                                            a[j,i] = [0,0,0]
#                                           
#                   
#  
#                            x,y = ogrid[0:width:1, 0:height:1]
#                            imshow(a, origin='lower',vmin=self.vmin, vmax=self.vmax, extent=[0,width,0,height], interpolation='nearest')   
#                            colorbar(ax = f,  orientation='horizontal')
                
                                            
#                    if ( self.SaveToFile == "true"):
#                            filename = "plot" 
#                            filename+=str(self.getT())
#                            #filename+=".svg"
#                            #plt.savefig(filename+".svg")
#                            #savefig(filename+".png")
#                            #savefig(filename+".svg")
#                             plt.savefig(filename+".eps")
#                            #plt.savefig(filename+".pdf")
#                            #plt.savefig(filename+".emf")
#                
#                    if ( self.ShowFigure == "true"):
#                            show()
#                    else:
#                            close()


                        
        def createInputDialog(self):
            form = GUIPlotRaster(self, QApplication.activeWindow())
            form.show()
            return True   


