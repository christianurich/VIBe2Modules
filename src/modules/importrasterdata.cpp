/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#include "importrasterdata.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QThread>
VIBe_DECLARE_NODE_NAME( ImportRasterData,Modules )
ImportRasterData::ImportRasterData(){
    param = Parameters();
    param.Multiplier = 1;
    param.flip = false;
    this->addParameter("r", VIBe2::RASTERDATA_OUT, &param.r);
    this->addParameter("FileName", VIBe2::FILENAME, &param.FileName);
    this->addParameter("Flip", VIBe2::BOOL, &param.flip);
    this->addParameter("Multiplier", VIBe2::DOUBLE, &param.Multiplier);
}
ImportRasterData::~ImportRasterData()
{
}

void ImportRasterData::run() {



    QFile file(QString::fromStdString(param.FileName));

    
    QTextStream stream(&file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         Logger(Error) << "warning, read input file ";
         return;
    }

    QString line("NULL");

    int LineCounter  = 0;
    int rowCounter = 0;
    int ncols = 0;
    int nrows = 0;
    double cellsize = 0;
    double NoDataValue = -9999; //default

    //Read Header
    while (!line.isNull() && LineCounter < 6 ) {
        LineCounter++;
        line =stream.readLine();
        if (LineCounter == 1) {
            QStringList list = line.split(QRegExp("\\s+"));
            QString s = QString(list[1]);
            s.replace(",", ".");
            ncols = s.toInt();
        }
        if (LineCounter == 2) {
            QStringList list = line.split(QRegExp("\\s+"));
            QString s = QString(list[1]);
            s.replace(",", ".");
            nrows = s.toInt();
        }
        if (LineCounter == 5) {
            QStringList list = line.split(QRegExp("\\s+"));
            QString s = QString(list[1]);
            s.replace(",", ".");
            cellsize = s.toDouble() * param.Multiplier;
        }
        if (LineCounter == 6) {
            QStringList list = line.split(QRegExp("\\s+"));
            QString s = QString(list[1]);
            s.replace(",", ".");
            NoDataValue = s.toDouble();
        }
    }
    std::cout <<" Cols " << ncols << std::endl;
    std::cout <<" Rows " << nrows << std::endl;
    std::cout <<" Cellsize " << cellsize << std::endl;
    param.r->setNoValue(NoDataValue);
    param.r->setSize(ncols, nrows, cellsize);
    while (!line.isNull()) {
        LineCounter++;
        line =stream.readLine();
        if (LineCounter >= 6 && rowCounter < nrows) {
            QStringList list = line.split(QRegExp("\\s+"));
            for ( int i = 0; i < list.size(); i++ ) {
                QString s = QString(list[i]);
                s.replace(",", ".");
                if (param.flip)
                    param.r->setValue(i, nrows-rowCounter-1, s.toDouble());
                else
                    param.r->setValue(i, rowCounter, s.toDouble());
            }
            rowCounter++;

        }
    }
    file.close();
}
