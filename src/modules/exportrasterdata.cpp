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
#include "exportrasterdata.h"

#include <QString>
#include <fstream>
#include <sstream>
#include <QThread>

VIBe_DECLARE_NODE_NAME( ExportRasterData,Modules )
ExportRasterData::ExportRasterData()
{
    this->FileName = "export";
    this->flip_h = false;
    this->addParameter("FileName", VIBe2::STRING, & this->FileName);
    this->addParameter("RasterData", VIBe2::RASTERDATA_IN, & this->rData);
    this->addParameter("FlipHorizontal", VIBe2::BOOL, &this->flip_h);


}
ExportRasterData::~ExportRasterData()
{
}

void ExportRasterData::run() {
    std::cout << "Runnning export" << std::endl;
    QString extension=".txt";
    std::stringstream s;

    QString fullFileName =   QString::fromStdString(FileName)+  QString::fromStdString(s.str()) +extension;

    std::cout << fullFileName.toStdString() << std::endl;
    std::fstream txtout;

    txtout.open(fullFileName.toAscii(),std::ios::out);

    //header for ARCGIS import

    txtout<<"ncols "<< rData->getWidth() <<"\n";
    txtout<<"nrows "<< rData->getHeight() <<"\n";
    txtout<<"xllcorner 0"<<"\n";
    txtout<<"yllcorner 0"<<"\n";
    txtout<<"cellsize "<<rData->getCellSize()<<"\n";
    txtout<<"nodata_value "<< rData->getNoValue()<<"\n";


    if (!flip_h) {
        for (int j=0; j<rData->getHeight() ; j++)
        {
            for (int i=0; i<rData->getWidth(); i++)
            {
                txtout<<rData->getValue(i,j)<< " ";
            }
            txtout<<"\n";

        }
    }
    if (flip_h) {
        for (int j=rData->getHeight() -1 ; j > -1 ; j--)
        {
            for (int i=0; i<rData->getWidth(); i++)
            {
                txtout<<rData->getValue(i,j)<< " ";
            }
            txtout<<"\n";

        }
    }
    txtout.close();
}

