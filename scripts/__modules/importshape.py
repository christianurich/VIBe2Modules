"""
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
from osgeo import ogr, osr
from shapely import wkb, geometry, coords
import itertools
from shapely.geometry import asShape

class ImportShapeFile(Module):
	"""Import Shapefiles
	
	Import for Shapfiles. All data and attributes are imported. The data are saved in the VIBe VectorData format. 
	The elements are saved with Identifier_ID
	@ingroup Modules
	@author Christian Urich
	"""
        def __init__(self):
            Module.__init__(self)
            self.FileName = ""
	    self.Identifier = ""
            #self.AsLines  = True
            #self.AsFaces = False
            
            self.output = pyvibe.VectorDataIn()
            #self.addParameter(self, "AsLines", VIBe2.BOOL)
            #self.addParameter(self, "AsFaces", VIBe2.BOOL)
            self.addParameter(self, "FileName", VIBe2.FILENAME)
            self.addParameter(self, "output", VIBe2.VECTORDATA_OUT)
	    self.addParameter(self, "Identifier", VIBe2.STRING)
            
            
            
        def run(self):
            vec = self.output.getItem()
            shapelyGeometries, fieldPacks, fieldDefinitions = [], [], []
            sourcePath =  self.FileName
            dataSource = ogr.Open(sourcePath)
            layer = dataSource.GetLayer()
            featureDefinition = layer.GetLayerDefn()
            fieldIndices = xrange(featureDefinition.GetFieldCount())
            #print fieldIndices
            for fieldIndex in fieldIndices:
                    fieldDefinition = featureDefinition.GetFieldDefn(fieldIndex)
                    fieldDefinitions.append((fieldDefinition.GetName(), fieldDefinition.GetType()))
            feature = layer.GetNextFeature()
            #print layer.GetFeatureCount()
            #while there are more features,
            while feature:
                    shapelyGeometries.append(wkb.loads(feature.GetGeometryRef().ExportToWkb()))
                    fieldPacks.append([feature.GetField(x) for x in fieldIndices])
                    # Get the next feature
                    feature = layer.GetNextFeature()
         
            counter = 1
            for i in range(len(shapelyGeometries)):
                shapelyGeometry = shapelyGeometries[i]
                geoms =  []
		if shapelyGeometry.type == 'Point':
                    pl = PointList()
                    pl.append(Point(shapelyGeometry.x ,shapelyGeometry.y,0))
   		    
                    counter+=1
                    vec.setPoints(self.Identifier + str(counter), pl)
                    attr = Attribute()
                    for j in range(len(fieldDefinitions)):      
		                attr.setAttribute(fieldDefinitions[j][0], str(fieldPacks[i][j]))
                    attr.setAttribute(self.Identifier + 'ID', counter)
                    vec.setAttributes(self.Identifier + str(counter), attr)		
                elif shapelyGeometry.type == 'Polygon':
                    geoms.append(shapelyGeometry)
                else:
                    geoms = list(shapelyGeometry.geoms)
		if shapelyGeometry.type != 'Point':
		        for geo in geoms:
		            r = geo.exterior
		            coordinates = list(r.coords)
		            pl = PointList()
		            el = EdgeList()
		            numberOfPoints = 0
		            for coords in coordinates:                                        
		                pl.append(Point(coords[0], coords[1],0))
		                if numberOfPoints > 0:
		                    el.append(Edge(numberOfPoints-1, numberOfPoints))
		                numberOfPoints += 1     
		            counter+=1
		            #print counter
		            vec.setPoints(self.Identifier + str(counter), pl)
		            vec.setEdges(self.Identifier + str(counter), el)
		            attr = Attribute()
		            
		            for j in range(len(fieldDefinitions)):
				#print fieldDefinitions[j][0] + " " + str(fieldPacks[i][j])              
		                attr.setAttribute(fieldDefinitions[j][0], str(fieldPacks[i][j]))
		            attr.setAttribute(self.Identifier + 'ID', counter)
			    vec.setAttributes(self.Identifier + str(counter), attr)
		            #Create Faces
		            f = LongList()
		            for j in range(len(pl)):
		                f.append(j)
		            fl = FaceList()
		            fl.append(Face(f))
		            vec.setFaces(self.Identifier + str(counter), fl)
		            elements = list(geo.interiors)     
		            #print elements                      
		            for element in elements:
		                coordinates = list(element.coords)
		                pl = PointList()
		                el = EdgeList()
		                numberOfPoints = 0
		                for coords in coordinates:                                        
		                    pl.append(Point(coords[0], coords[1],0))
		                    if numberOfPoints > 0:
		                        el.append(Edge(numberOfPoints-1, numberOfPoints))
		                    numberOfPoints += 1     
		                counter+=1
		                #print counter
		                vec.setPoints(self.Identifier + str(counter), pl)
		                vec.setEdges(self.Identifier+ str(counter), el)
		                #Create Faces
		                f = LongList()
		                for j in range(len(pl)):
		                    f.append(j)
		                fl = FaceList()
		                fl.append(Face(f))
		                vec.setFaces(self.Identifier + str(counter), fl)
            print "Importet elements: " + str(counter)
                            
                    
                        
