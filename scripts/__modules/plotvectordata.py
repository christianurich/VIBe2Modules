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

import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import pylab
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection
from matplotlib.collections import LineCollection
import numpy as np
import numpy as np

import os
import tempfile
import pyvibehelper

class PlotVectorData(Module):       
    """Plots VectorData
    @ingroup Modules
    @author Christian Urich
    """
    def __init__(self):
            Module.__init__(self)            
            self.VectorDataPlot = VectorDataMap()
            self.Rules = StringMap()
            self.PlotStyles = StringMap()
	    self.Identifier = ""
            self.addParameter(self, "VectorDataPlot", VIBe2.USER_DEFINED_VECTORDATA_IN)
            self.addParameter(self, "Rules", VIBe2.STRING_MAP)        
	    self.addParameter(self, "Identifier", VIBe2.STRING)   
            self.addParameter(self, "PlotStyles", VIBe2.STRING_MAP)               
                

    def run(self):
        patches = []
        linescol = []
        linescolWithC = []
        linewidths = []
        linecolors = []
        print "Run Plot"
        fig = plt.figure()
        fig.set_size_inches(46.8, 33.1)
        ax = fig.add_subplot(111,  aspect='equal')
        mapNames = self.VectorDataPlot.getNames()
        index = 0
        for i in range(len(mapNames)):
            mapName = mapNames[i]
            v =  self.VectorDataPlot.get(mapName)

            Type = "none"
            rule_keys = self.Rules.keys()
            for r in rule_keys:
                if r == mapName:
                    Type = self.Rules[mapName].split()[0]
            print Type
                
            #find LayerNames
            names = []
            if Type == "Edge":
                names =  pyvibehelper.findElementsWithIdentifier(self.Identifier, v.getEdgeNames()) 
            if Type == "Face":
                names = pyvibehelper.findElementsWithIdentifier(self.Identifier, v.getFaceNames())
            
            vectornames_tmp = v.getPointsNames()
            vectornames= []
            for i in range(len(vectornames_tmp)):
                vectornames.append(str(vectornames_tmp[i]))
            for n in range(len(names)):  
                    name = names[n]
                    points = v.getPoints(name)
                    if (len(points)== 0):
                        break
                    if index == 0:
                        xmax = points[0].getX()
                        xmin = points[0].getX()
                        ymax = points[0].getY()
                        ymin = points[0].getY() 
                        index+=1
                    if str(Type) == "Edge":
                        edges = v.getEdges(name)   
                        poly = []
                        for i in range(len(edges)):
                            x = points[edges[i].getID1()].getX()
                            y = points[edges[i].getID1()].getY()
                            poly.append([x,y])
                        
                
                                
                        if str(name).find("CityBlock") > -1 and self.ColorCityBlocks == "true":  
                            fc = [0,0,0]
                            alhp = 1
                            attr = v.getAttributes(name)
                            print self.ColorMapCityBlock
                            if(self.ColorMapCityBlock == 'Landuse'):                           
                                val = []
                                val.append (3)
                                val.append (2)
                                val.append (4)
                                val.append (5)
                                val.append (6)
                                val.append (7)
                                cval = np.array([])
                                cval.resize(6,3)
                                
                                cval[0,0] = 255./255.
                                cval[0,1] = 0
                                cval[0,2] = 0       
                                
                                cval[1,0] = 139./255.
                                cval[1,1] = 0
                                cval[1,2] = 0
                                
                                cval[2,0] = 0
                                cval[2,1] = 0
                                cval[2,2] = 0  
                                
                                cval[3,0] = 0
                                cval[3,1] = 128./255.
                                cval[3,2] = 0  
                                
                                cval[4,0] = 0
                                cval[4,1] = 100./255.
                                cval[4,2] = 0 
                                
                                cval[5,0] = 0
                                cval[5,1] = 0
                                cval[5,2] = 255./255.  

                                a = attr.getAttribute('Landuse')
                                print "ATTRIBUTE"
                                print a
                                if (a >= 2 and a <=7):
                                    fc = cval[val.index(attr.getAttribute('Landuse'))]
                                polygon = Polygon(poly, True,  facecolor=fc, alpha = alhp, linewidth=0.001)
                                #ax.add_patch(polygon) 
                                patches.append(polygon)
                            
                            if(self.ColorMapCityBlock == 'Population'):
                                pop = attr.getAttribute('PopDens')
                                print pop
                                fc = [0,0,0]
                                alhp = 1
                                attr = v.getAttributes(name)
                                val = []
                                val.append (0)
                                val.append (50)
                                val.append (100)
                                val.append (150)
                                val.append (200)
                                val.append (250)
                                val.append (300)
                                val.append (350)
                                
                                cval = np.array([])
                                cval.resize(8,3)
                                for i in range(8):
                                    cval[i,0] = ((i+1) * 255./8.)/255.
                                    print ((i+1) * 255./8.)/255.
                                    cval[i,1] = 0
                                    cval[i,2] = 0  
                                code = 0
                                for vals in val:                                    
                                    if vals > pop:
                                        break
                                    code = code+1
                                if(code == 8):
                                    code = code -1
                                print code
                                fc = cval[code]
                                polygon = Polygon(poly, True,  facecolor=fc, alpha = alhp, linewidth=0.001)
                                #ax.add_patch(polygon)   
                                patches.append(polygon)                             

       
                                
                        if str(name).find("House") > -1 and self.ColorHouse == "true":
                                if self.ColorHouse == 'true':
                                    if(self.ColorMapHouse == 'House'):
                                        fc = 'gray'
                                        alhp = 0.4
                                    attr = v.getAttributes(name)
                                    if(self.ColorMapHouse == 'Age'):
                                        print "Age"
                                        alhp = 1.0                                        
                                    
                                        age = attr.getAttribute('CA')
                                        if(age < 1980):
                                            fc = 'red'
                                        elif(age < 2000):
                                            fc = 'blue'  
                                        elif(age < 2010):
                                            fc = 'green'  
                                        elif(age >= 2010):
                                            fc = 'green'  
                                    if(self.ColorMapHouse == 'Stories'):
                                        attr = v.getAttributes(name)
                                        pop = attr.getAttribute('Stories')
                                        print "Stories"
                                        print pop
                                        fc = [0,0,0]
                                        alhp = 1.0
                                        attr = v.getAttributes(name)
                                        val = []
                                        val.append (0)
                                        val.append (1)
                                        val.append (2)
                                        val.append (3)
                                        val.append (4)
                                        val.append (5)
                                        val.append (6)
                                        cval = np.array([])
                                        
                                        cval.resize(7,3)
                                        cval[0,0] = 1
                                        cval[0,1] = 1
                                        cval[0,2] = 1     
                                        
                                        cval[1,0] = 255./255.
                                        cval[1,1] = 0
                                        cval[1,2] = 0       
                                        
                                        cval[2,0] = 139./255.
                                        cval[2,1] = 0
                                        cval[2,2] = 0
                                        
                                        cval[3,0] = 0
                                        cval[3,1] = 255./255.
                                        cval[3,2] = 0  
                                        
                                        cval[4,0] = 0
                                        cval[4,1] = 128./255.
                                        cval[4,2] = 0  
                                        
                                        cval[5,0] = 0
                                        cval[5,1] = 255./255.
                                        cval[5,2] = 0 
                                        
                                        cval[6,0] = 0
                                        cval[6,1] = 0
                                        cval[6,2] = 100./255.                                     
                                    if(self.ColorMapHouse == 'Demand'):            
                                        pop = attr.getAttribute('HeatDemand')/1000
                                        fc = [0,0,0]
                                        alhp = 1
                                        attr = v.getAttributes(name)
                                        val = range(200)
        
                                        
                                        cval = np.array([])
                                        cval.resize(len(val),3)
                                        for i in range(len(val)):
                                            cval[i,0] = 1-((i+1) * 255./len(val))/255.
                                            cval[i,2] = 0  
                                    if(self.ColorMapHouse == 'Err'):            
                                        pop = attr.getAttribute('Err')*100
                                        fc = [0,0,0]
                                        alhp = 1
                                        attr = v.getAttributes(name)
                                        val = range(-100,100)
        
                                        
                                        cval = np.array([])
                                        cval.resize(len(val),3)
                                        counter = 0
                                        for i in range(len(val)):   
                                            if (i <= len(val)/2):
                                                val1 = (((i*2+1) * 255./len(val))/255.)
                                                if val1 > 1:
                                                    val1 = 1                                                    
                                                cval[i,0] = val1
                                                cval[i,1] = val1
                                                cval[i,2] = 1
                                            else:
                                                val1 = (((i-len(val)/2)*2+1) * 255./len(val))/255.
                                                if val1 > 1:
                                                    val1 = 1                                               
                                                cval[i,0] = 1
                                                cval[i,1] = val1
                                                cval[i,2] = val1                                                

                                            
                                    code = 0                                    
                                    if (self.ColorMapHouse != 'Age' and self.ColorMapHouse != 'House' ):
                                        for vals in val:                                    
                                            if vals >= pop:
                                                break
                                            code = code+1
                                        if(code == len(val)):
                                            code = code -1
                                        fc = cval[code]
                                
                                    polygon = Polygon(poly, True,  facecolor=fc, alpha = alhp, linewidth=0.001)
                                    #ax.add_patch(polygon)
                                    patches.append(polygon)
                           
                        for i in range(len(edges)):
                            x1 = points[edges[i].getID1()].getX()
                            x2 = points[edges[i].getID2()].getX()
                            x = [x1, x2]

                            y1 = points[edges[i].getID1()].getY()
                            y2 = points[edges[i].getID2()].getY()
                            y = [y1, y2]    

                            xmax = max( x1, x2, xmax )
                            ymax = max( y1, y2, ymax )
                            xmin = min( x1, x2, xmin )                    
                            ymin = min( y1, y2, ymin )                        
                            
                            definedStyle = False
                            for i in range(len(self.PlotStyles)):
                                plotStyle = str(self.PlotStyles[i])
                                lw = 1
                                ec= ""
                                if self.params.has_key(plotStyle) == True:
                                    lw = float(self.params[plotStyle].split()[0])
                                    ec = str(self.params[plotStyle].split()[1])                                    
                                if str(name).find(plotStyle) > -1:
                                    line = lines.Line2D(x,y,color=ec, linewidth=lw)
                                    linecolors.append(ec)
                                    linewidths.append(lw)
                                    linescolWithC.append(((x[0], y[0]),(x[1], y[1])) )
                                    #ax.add_line(line)
                                    definedStyle = True
                                    
                            if definedStyle == False:
                                line = lines.Line2D(x,y)                                
                                linescol.append(((x[0], y[0]),(x[1], y[1])) )
                                #ax.add_line(line)

                    if str(Type) == "Face":                        
                        faces = v.getFaces(name)
                        
                        colorname = "COLOR_" + str(name)
			#print colorname
                        try:
                            vectornames.index(colorname)
                            hascolor = True
                            #print "Color Found"
                        except:
                            hascolor = False
                            #print "Color Not Found"
                        if hascolor == True:
                            col = v.getPoints(colorname)
                        for f in range (len(faces)):
                                face = faces[f]                                
                                ids = face.getIDs()
                                poly = []  
                                for i in range(len(ids)):   
				    id = ids[i]
				    p = points[id]
                                    x1 = p.getX()
                                    y1 = p.getY()
                               	    xmax = max( x1, xmax )
		                    ymax = max( y1, ymax )
		                    xmin = min( x1, xmin )                    
		                    ymin = min( y1, ymin )  
                                    poly.append([x1,y1])
                                if hascolor == True: 
                                    polygon = Polygon(poly,True, edgecolor=(col[ids[0]].getX(),col[ids[0]].getY(),col[ids[0]].getZ(),1), facecolor=(col[ids[0]].getX(),col[ids[0]].getY(),col[ids[0]].getZ(),1) ,  linewidth=0.1, )
                                else:
                                    polygon = Polygon(poly,True,  linewidth=0.001)
                                #patches.append(polygon)
                                #p = PatchCollection(patches,  linewidths=0,   facecolor='red' ,alpha=0.4 )
                                #ax.add_collection(p)
                                #ax.add_patch(polygon)
                                patches.append(polygon)
#                                    if i < len(ids)-1:
#                                        x2 = points[ids[i+1]].getX()
#                                        y2 = points[ids[i+1]].getY()
#                                    else:
#                                        x2 = points[ids[0]].getX()
#                                        y2 = points[ids[0]].getY()
#                                    x = [x1, x2]
#                                    
#                                    
#                                    y = [y1, y2]   
#                                    xmax = max( x1, x2, xmax )
#                                    ymax = max( y1, y2, ymax )
#                                    xmin = min( x1, x2, xmin )                    
#                                    ymin = min( y1, y2, ymin )                        
#                                    line = lines.Line2D(x,y)
#                                    ax.add_line(line)
        if(len(patches) > 0):
            p = PatchCollection(patches, True)
            ax.add_collection(p)  
        if(len(linescol) > 0):
            pl = LineCollection(linescol)
            ax.add_collection(pl)
        if(len(linescolWithC) > 0):
            pl = LineCollection(linescolWithC, linewidths, linecolors)
            ax.add_collection(pl)      
        '''
        if self.xmin != -1 and self.xmax != -1:
            xmin = self.xmin 
            xmax = self.xmax
        if self.ymin != -1 and self.ymax != -1:
            ymin = self.ymin 
            ymax = self.ymax
        '''
        print xmax
        print ymax
        plt.xlim(xmin, xmax)
        plt.ylim(ymin, ymax)
        '''   
        if ( self.SaveToFile == "true"):
            filename = "plot" 
            filename+=str(self.getT())+"_"+str(self.getID())
            #filename+=".svg"
            #plt.savefig(filename+".svg")
            plt.savefig(filename+".png")
            #plt.savefig(filename+".eps")
            plt.savefig(filename+".pdf")
            #plt.savefig(filename+".emf")
            
        if ( self.ShowFigure == "true"):
            plt.show()
        else:
            plt.close()
        '''
        filename = "plot" 
        filename+=".svg"
        plt.savefig(tempfile.gettempdir()+'/'+filename)
        plt.close()
        self.sendImageToResultViewer(tempfile.gettempdir()+'/'+filename)   
        
          
    def clone(self):
        return PlotVectorData()

    '''def init(self, params):
        self.paramComplete = True   
        if params.has_key("Input") == True:        
            self.mapNames = params["Input"].split()        
        if params.has_key("Options") == True:      
                self.Options = params["Options"].split() 
        else:
                self.Options = ""
        if params.has_key("SaveToFile") == True:      
                self.SaveToFile = params["SaveToFile"].split()[0]
        else:
                self.SaveToFile = "false"
        if params.has_key("ColorHouse") == True:      
                self.ColorHouse = params["ColorHouse"].split()[0]
        else:
                self.ColorHouse = "false"
        if params.has_key("ColorCityBlocks") == True:      
                self.ColorCityBlocks = params["ColorCityBlocks"].split()[0]
        else:
                self.ColorCityBlocks = "false"
        if params.has_key("ColorMapHouse") == True:      
                self.ColorMapHouse = params["ColorMapHouse"]
        else:
                self.ColorMapHouse = ""
        if params.has_key("ColorMapCityBlock") == True:      
                self.ColorMapCityBlock = params["ColorMapCityBlock"]
        else:
                self.ColorMapCityBlock = ""
        if params.has_key("ShowFigure") == True:      
                self.ShowFigure = params["ShowFigure"].split()[0]
        else:
                self.ShowFigure = "false"
        if params.has_key("xlim") == True:      
               ress = params["xlim"].split() 
               self.xmin = float(ress[0])
               self.xmax = float(ress[1])
        else:
                self.xmin = -1
                self.xmax = -1
        if params.has_key("ylim") == True:      
               ress = params["ylim"].split() 
               self.ymin = float(ress[0])
               self.ymax = float(ress[1])
        else:
                self.ymin = -1
                self.ymax = -1
                
        if params.has_key("vmax") == True:      
                self.vmax = float(params["vmax"])
        else:
                self.vmax = None
                
        if params.has_key("Many") == True:      
                self.Many = params["Many"]
        else:
                self.Many = "false"
        if params.has_key("PlotStyles") == True:      
                self.PlotStyles = params["PlotStyles"].split() 
        else:
                self.PlotStyles = []     
      
        self.params = params
        '''
