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
#include "guicellularautomata_neighbourhood.h"
#include "ui_guicellularautomata_neighbourhood.h"
#include <module.h>
#include <sstream>
GUICellularAutomata_Neighbourhood::GUICellularAutomata_Neighbourhood(GUICellularAutomata * GUICellular, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::GUICellularAutomata_Neighbourhood)
{
    gui = GUICellular;
    ui->setupUi(this);
    this->m = GUICellular->getModule();
    std::map<std::string ,RasterData * >  maps = m->getParameter<std::map<std::string, RasterData * > >("Landscapes");
    for (std::map<std::string ,RasterData * >::iterator it = maps.begin(); it != maps.end(); ++it)
        ui->comboBox_landscapes->addItem(QString::fromStdString(it->first));
    ui->comboBox_neigh->addItem("Moore");
    ui->comboBox_neigh->addItem("Neumann");
    ui->comboBox_neigh->addItem("CurrentValue");

}

GUICellularAutomata_Neighbourhood::~GUICellularAutomata_Neighbourhood()
{
    delete ui;
}

void GUICellularAutomata_Neighbourhood::accept() {

    std::cout << "huh" << std::endl;
    std::map<std::string, std::string> neighs = m->getParameter< std::map<std::string, std::string> >("Neighs");
    std::string name = ui->lineEdit_name->text().toStdString();
    std::stringstream ss;
    ss << ui->comboBox_landscapes->currentText().toStdString();
    ss << "+|+";
    ss << ui->comboBox_neigh->currentText().toStdString();
    ss << "+|+";
    ss << this->getNeighbourhood( ui->comboBox_neigh->currentText().toStdString());
    neighs[name] =ss.str();
    m->setParameterNative< std::map<std::string, std::string> >("Neighs", neighs);
   emit valuesChanged();
    QDialog::accept();

}

std::string GUICellularAutomata_Neighbourhood::getNeighbourhood(std::string name) {
    std::stringstream ss;
    if (name.compare("Moore") == 0) {
        ss << "|1,1,1;";
        ss << "1,0,1;";
        ss << "1,1,1|";
    }
    if (name.compare("Neumann") == 0) {
        ss << "|0,1,0;";
        ss << "1,0,1;";
        ss << "0,1,0|";
    }
    if (name.compare("CurrentValue") == 0) {
        ss << "|1|";
    }
    return ss.str();
}
