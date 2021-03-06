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

import osgeo.ogr, osgeo.osr
import os
from pyvibe import *

class ExportToShapeFile(Module):
            def __init__(self):
                Module.__init__(self)
                
                self.addDescription("Exports VIBe2 VectorData to ESRI Shapefiles")                
                
                self.vecin = VectorDataIn();
                self.Points = True
                self.Lines = True
                self.Faces = True
                self.FileName = "ShapeFile"
                self.Name = ""
                self.addParameter(self, "Name", VIBe2.STRING, "Name that is Exported e.g. Path_ all elements that start with Path_ are exported")
                self.addParameter(self, "FileName", VIBe2.STRING, "Filename the Filename is extended by the type e.g. Filename_points")
                self.addParameter(self, "Points", VIBe2.BOOL, "Option if points are exported")
                self.addParameter(self, "Lines", VIBe2.BOOL, "Option if lines are exported")
                self.addParameter(self, "Faces", VIBe2.BOOL, "Option if faces are exported")
                self.addParameter(self, "vecin", VIBe2.VECTORDATA_IN)
                
            def run(self):
		#print "StartExporting" 
		#print self.FileName 
                if self.Points:
                    self.exportPoints()  
                if self.Faces:                       
                    self.exportFaces()
                if self.Lines:
                    self.exportPolyline()             

            def exportFaces(self):
                spatialReference = osgeo.osr.SpatialReference()
                spatialReference.ImportFromProj4('+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs')
                
                #Init Shape Files
                driver = osgeo.ogr.GetDriverByName('ESRI Shapefile')
                if os.path.exists(str(self.getInternalCounter()) + "_" +self.FileName+'_faces.shp'): os.remove(str(self.getInternalCounter()) + "_" +self.FileName+'_faces.shp')
                shapeData = driver.CreateDataSource(str(self.getInternalCounter())+"_" +self.FileName+'_faces.shp')
                
                layer = shapeData.CreateLayer('layer1', spatialReference, osgeo.ogr.wkbPolygon)
                layerDefinition = layer.GetLayerDefn()               
                AttributeList = []
                attr = []
                hasAttribute = False
                #Get Data 

                 
                names_tot = self.vecin.getItem().getFaceNames()
                
                names = []
                for i in range(len(names_tot)): 
                    n = str(names_tot[i])
                    if n.find(str(self.Name)) == 0 or self.Name == "":
                        names.append(n)
                        
                attr_tmp = self.vecin.getItem().getAttributeNames()
                for i in range(len( attr_tmp)):
                    attr.append(attr_tmp[i])
                for i in range(len(names)): 
                    #Append Attributes
                    if names[i] in attr:
                        alist = self.vecin.getItem().getAttributes(names[i]).getAttributeNames()   
                        #Check if attribute exists             
                        for j in range(len(alist)):
                            hasAttribute = True                                  
                            if (alist[j] in AttributeList) == False:
                                fielddef = osgeo.ogr.FieldDefn(alist[j], osgeo.ogr.OFTString)
                                layer.CreateField(fielddef)
                                layerDefinition = layer.GetLayerDefn()  
                                log(alist[j])
                                AttributeList.append(alist[j]) 
                    
                    plist = self.vecin.getItem().getPoints(names[i])
                    flist = self.vecin.getItem().getFaces(names[i])

                    for j in range(len(flist)):
                            
                        line = osgeo.ogr.Geometry(osgeo.ogr.wkbPolygon)
                        ring = osgeo.ogr.Geometry(osgeo.ogr.wkbLinearRing)
                        pointIDs = flist[j].getIDs()
                        for k in range(len(pointIDs)):
                            p = Point
                            p = plist[pointIDs[k]];
                            ring.AddPoint(p.getX(),p.getY())
                        line.AddGeometry(ring)

                        featureIndex = 0
                        feature = osgeo.ogr.Feature(layerDefinition)
                        feature.SetGeometry(line)
                        feature.SetFID(featureIndex)  
                        #Append Attributes
                        if hasAttribute:        
                            for k in range(len(alist)):
				#print alist[k]
				#print self.vecin.getItem().getAttributes(names[i]).getStringAttribute(alist[k])
                                value = self.vecin.getItem().getAttributes(names[i]).getStringAttribute(alist[k])
                                feature.SetField(alist[k],value)
                        layer.CreateFeature(feature)    
                shapeData.Destroy()               
               
            def exportPoints(self):
                spatialReference = osgeo.osr.SpatialReference()
                spatialReference.ImportFromProj4('+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs')
                
                #Init Shape Files
                driver = osgeo.ogr.GetDriverByName('ESRI Shapefile')
                if os.path.exists(str(self.getInternalCounter())+"_" +self.FileName+'_points.shp'): os.remove(str(self.getInternalCounter())+"_" +self.FileName+'_points.shp')
                shapeData = driver.CreateDataSource(str(self.getInternalCounter())+"_" +self.FileName+'_points.shp')
                
                layer = shapeData.CreateLayer('layer1', spatialReference, osgeo.ogr.wkbPoint)
                #osgeo.ogr.wkbPolygon()
                layerDefinition = layer.GetLayerDefn()               
                AttributeList = []
                attr = []
                hasAttribute = False
                
                fielddef = osgeo.ogr.FieldDefn("Z", osgeo.ogr.OFTString)
                layer.CreateField(fielddef)
                layerDefinition = layer.GetLayerDefn()  
                AttributeList.append("Z") 
                
                #Get Data 
                names_tot = self.vecin.getItem().getPointsNames()
                names = []
                for i in range(len(names_tot)): 
                    n = str(names_tot[i])
                    if n.find(str(self.Name)) == 0 or self.Name == "": 
                        names.append(n)
                attr_tmp = self.vecin.getItem().getAttributeNames()
                for i in range(len( attr_tmp)):
                    attr.append(attr_tmp[i])
                for i in range(len(names)): 
                    #Append Attributes
                    if names[i] in attr:
                        alist = self.vecin.getItem().getAttributes(names[i]).getAttributeNames()   
                        #Check if attribute exists             
                        for j in range(len(alist)):
                            hasAttribute = True                            
                            if (alist[j] in AttributeList) == False:
                                fielddef = osgeo.ogr.FieldDefn(alist[j], osgeo.ogr.OFTString)
                                layer.CreateField(fielddef)
                                layerDefinition = layer.GetLayerDefn()  
                                log(alist[j])
                                AttributeList.append(alist[j])
                          
                        
                            
                    #Addend Points
                    plist = self.vecin.getItem().getPoints(names[i])
                    for j in range(len(plist)):    
                        point = osgeo.ogr.Geometry(osgeo.ogr.wkbPoint)
                        point.SetPoint(0, plist[j].getX(),  plist[j].getY())
                        featureIndex = 0
                        feature = osgeo.ogr.Feature(layerDefinition)
                        feature.SetGeometry(point)
                        feature.SetFID(featureIndex) 
                        feature.SetField("Z",str( plist[j].getZ()))
                        #Append Attributes
                        if hasAttribute:      
                            for k in range(len(alist)):
				#print alist[k]
				#print self.vecin.getItem().getAttributes(names[i]).getStringAttribute(alist[k])
                                value = self.vecin.getItem().getAttributes(names[i]).getStringAttribute(alist[k])
                                feature.SetField(alist[k],value)
                        layer.CreateFeature(feature)    
                shapeData.Destroy()
                
            def exportPolyline(self):
                spatialReference = osgeo.osr.SpatialReference()
                spatialReference.ImportFromProj4('+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs')
                
                #Init Shape Files
                driver = osgeo.ogr.GetDriverByName('ESRI Shapefile')
                if os.path.exists(str(self.getInternalCounter())+"_" +self.FileName+'_lines.shp'): os.remove(str(self.getInternalCounter())+"_" +self.FileName+'_lines.shp')
                shapeData = driver.CreateDataSource(str(self.getInternalCounter())+"_" + self.FileName+'_lines.shp')
                
                layer = shapeData.CreateLayer('layer1', spatialReference, osgeo.ogr.wkbLineString)
                layerDefinition = layer.GetLayerDefn()               
                AttributeList = []
                attr = []
                hasAttribute = False
                #Get Data 
                names_tot = self.vecin.getItem().getEdgeNames()
                names = []
                for i in range(len(names_tot)): 
                    n = str(names_tot[i])
                    if n.find(str(self.Name)) == 0 or self.Name == "":
                        names.append(n)
                attr_tmp = self.vecin.getItem().getAttributeNames()
                for i in range(len( attr_tmp)):
                    attr.append(attr_tmp[i])
                for i in range(len(names)): 
                    #Append Attributes
                    if names[i] in attr:
                        alist = self.vecin.getItem().getAttributes(names[i]).getAttributeNames()   
                        #Check if attribute exists             
                        for j in range(len(alist)):
                            hasAttribute = True                                  
                            if (alist[j] in AttributeList) == False:
                                fielddef = osgeo.ogr.FieldDefn(alist[j], osgeo.ogr.OFTString)
                                layer.CreateField(fielddef)
                                layerDefinition = layer.GetLayerDefn()  
                                log(alist[j])
                                AttributeList.append(alist[j]) 
                    
                    plist = self.vecin.getItem().getPoints(names[i])
                    flist = self.vecin.getItem().getEdges(names[i])

                    for j in range(len(flist)):
                            
                        line = osgeo.ogr.Geometry(osgeo.ogr.wkbLineString)
                        edge = flist[j]
                        p1 = Point
                        p1 = plist[edge.getID1()];
                        p2 = Point
                        p2 = plist[edge.getID2()];
                        line.AddPoint(p1.getX(),p1.getY())
                        line.AddPoint(p2.getX(),p2.getY())

                        featureIndex = 0
                        feature = osgeo.ogr.Feature(layerDefinition)
                        feature.SetGeometry(line)
                        feature.SetFID(featureIndex)  
                        #Append Attributes
                        if hasAttribute:        
                            for k in range(len(alist)):
                                value = self.vecin.getItem().getAttributes(names[i]).getStringAttribute(alist[k])
                                feature.SetField(alist[k],value)
                        layer.CreateFeature(feature)    
                shapeData.Destroy()               
               
