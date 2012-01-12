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
#include "singlevalue.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QThread>
#include "guisinglevalue.h"
#include "userdefinedfunctions.h"

VIBe_DECLARE_NODE_NAME ( SingleValue, Modules )
SingleValue::SingleValue()
{

    param.Value = "";
    this->addParameter("Value" , VIBe2::STRING, & param.Value);
    this->addParameter("DoubleData", VIBe2::DOUBLEDATA_OUT, & param.DoubleData);
    this->addParameter("VaraiblesFromOutSide", VIBe2::USER_DEFINED_DOUBLEDATA_IN, & param.VariablesFromOutside);
    this->repeater = new double(0);
    this->FirstTime = true;


}

void SingleValue::run() {
     *this->repeater = this->getInternalCounter();
    //init Runtime
    if (this->FirstTime) {
        this->FirstTime = false;
        this->Expression = mu::Parser();
        this->Expression.DefineVar("repeater", this->repeater);
        this->Expression.DefineFun("rand", mu::random, false);
        //Create Varaibles
        for (std::map<std::string, double>::iterator it = param.VariablesFromOutside.begin(); it != param.VariablesFromOutside.end(); ++it) {
            //Check if Parameter Exists
            bool exists = false;
            std::string name = it->first;
            for (std::map<std::string, double*>::iterator it1 = variables.begin(); it1 != variables.end(); ++it1) {
                std::string n = it1->first;
                if (n.compare(name) == 0) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                this->variables[name] = new double;

                this->Expression.DefineVar(name,  this->variables[name]);
            }
        }


        this->Expression.SetExpr(param.Value);
    }




    //Update Values
     for (std::map<std::string, double*>::iterator it1 = variables.begin(); it1 != variables.end(); ++it1) {
        std::string name = it1->first;
        double val = param.VariablesFromOutside[name];
        *(variables[name]) = param.VariablesFromOutside[name];


     }



    param.DoubleData = this->Expression.Eval();
}


bool SingleValue::createInputDialog() {
    QWidget * w = new GUISingleValue(this);
    w->show();
    return true;
}
