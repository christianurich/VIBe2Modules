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
#include "writedatatofile.h"
#include "fstream"

VIBe_DECLARE_NODE_NAME(WriteDataToFile, Evaluation)

WriteDataToFile::WriteDataToFile()
{
    firsttime = true;
    this->FileName = "export.dat";
    this->addParameter("Data", VIBe2::USER_DEFINED_DOUBLEDATA_IN, &double_data);
    this->addParameter("Filename", VIBe2::FILENAME, &this->FileName);
}

void WriteDataToFile::run() {

    std::fstream inp;
    if (firsttime) {
        firsttime = false;

        inp.open(this->FileName.c_str(), fstream::out | fstream::app);
        for(std::map<std::string, double>::const_iterator it = double_data.begin(); it != double_data.end(); ++it) {
            inp << it->first  <<"\t";
        }
        inp << "\n";

        inp.close();

    }


    inp.open(this->FileName.c_str(), fstream::out | fstream::app);

    for(std::map<std::string, double>::const_iterator it = double_data.begin(); it != double_data.end(); ++it) {
        inp << it->second <<"\t";
    }
    inp << "\n";

    inp.close();

}
