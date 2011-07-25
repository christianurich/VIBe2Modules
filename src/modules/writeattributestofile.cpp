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
#include "writeattributestofile.h"
#include "vectordatahelper.h"
#include "fstream"

VIBe_DECLARE_NODE_NAME(WriteAttributesToFile, Evaluation)

WriteAttributesToFile::WriteAttributesToFile()
{
    this->Identifier = "";
    this->Filename = "ExportAttributes.dat";
    this->addParameter("Identifier", VIBe2::STRING,& this->Identifier);
    this->addParameter("Vec", VIBe2::VECTORDATA_IN, &this->vecin);
    this->addParameter("Filename", VIBe2::STRING, &this->Filename);

}

void WriteAttributesToFile::run() {


    std::fstream inp;

    std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(this->Identifier, vecin->getAttributeNames());
    int controller =0;
    inp.open(this->Filename.c_str(), ios::out);
    foreach (std::string n, names) {

        Attribute attr = vecin->getAttributes(n);
        std::vector<std::string> att_names = attr.getAttributeNames();
        if (controller == 0) {
            foreach(std::string att_n, att_names) {
                inp << att_n  <<"\t";
            }
            inp << "\n";
        }

        controller++;
        foreach(std::string att_n, att_names) {
             inp << attr.getAttribute(att_n)<<"\t";
        }
        inp << "\n";
    }
    inp.close();
}
