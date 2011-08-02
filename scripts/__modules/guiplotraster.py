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
Created on Sat Sep 11 16:50:53 2010

@author: c8451045
"""

from PyQt4 import QtCore, QtGui
from pyvibe import *
from guiplotraster_ui import Ui_Dialog


class GUIPlotRaster(QtGui.QDialog):
    def __init__(self, m, parent=None):
        self.module = Module
        self.module = m
        QtGui.QDialog.__init__(self, parent)
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        QtCore.QObject.connect(self.ui.pushButton_addRaster,QtCore.SIGNAL("clicked()"), self.addRasterGUI)
        self.updateRasterDataList()
    def updateRasterDataList(self):
        maps = self.module.getParameterAsString("RasterDataMap")
        qmap = QtCore.QString(maps)
        l = qmap.split("*|*")
        print l
        for s in l:
            if (len(str(s)) != 0):
                self.ui.listWidget_rasterdata.addItem(str(s))
            

    def addRasterGUI(self):
        text, ok = QtGui.QInputDialog.getText(self, "Add RasterData", "Name:")
        textAsString = str(text)
        if len(textAsString) != 0:
            self.module.appendToUserDefinedParameter("RasterDataMap", textAsString)
            self.ui.listWidget_rasterdata.addItem(text)
            
