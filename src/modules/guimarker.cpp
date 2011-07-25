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
#include "guimarker.h"
#include "ui_guimarker.h"
#include "module.h"
#include <QInputDialog>
#include <guiequationeditor.h>


void GUIMarker::fromOutSide() {
    std::cout << "From Outside" << std::endl;
    QCheckBox * b = (QCheckBox *)QObject::sender();
    QString s =b->objectName();
    QStringList s1 = s.split("_");
    QString name;
    name = "DoubleIn_" + s1[1];
    if (b->checkState ()) {
            this->m->appendToUserDefinedParameter("InputDouble",name.toStdString());
    } else {
        //this->m->de("InputDouble","DoubleIn_Width");
    }
}


bool GUIMarker::checkIfFromOutSide(QString name) {
    QString s2 = "DoubleIn_" + name;
    std::map<std::string, double> doublemap = m->getParameter<std::map<std::string, double> >("InputDouble");
    for (std::map<std::string, double>::iterator it = doublemap.begin(); it != doublemap.end(); ++it) {
        if (s2.toStdString().compare(it->first) == 0)
            return true;
    }
   return false;

}

GUIMarker::GUIMarker(vibens::Module * m, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::GUIMarker)
{
    this->m = m;
    ui->setupUi(this);
    ui->lineEdit_Height->setText( QString::fromStdString(m->getParameterAsString("Height")) );
    ui->lineEdit_Width->setText( QString::fromStdString(m->getParameterAsString("Width")) );
    ui->lineEdit_CellSize->setText( QString::fromStdString(m->getParameterAsString("CellSize")) );
    ui->lineEdit_RExpression->setText( QString::fromStdString(m->getParameterAsString("RExpression")) );
    ui->lineEdit_rExpression->setText( QString::fromStdString(m->getParameterAsString("rExpression")) );
    ui->lineEdit_maxExpression->setText( QString::fromStdString(m->getParameterAsString("maxExpression")) );
    ui->checkBox_Points->setChecked(QString::fromStdString(m->getParameterAsString("Points")).toInt());
    ui->checkBox_Edges->setChecked(QString::fromStdString(m->getParameterAsString("Edges")).toInt());
    ui->lineEdit_Idnetifier->setText( QString::fromStdString(m->getParameterAsString("Identifier")) );
    //Check for Parameters from Outside
    if (checkIfFromOutSide("Height")) {
        ui->checkBox_Height->setChecked(true);
    }
    if (checkIfFromOutSide("Width")) {
        ui->checkBox_Width->setChecked(true);
    }
    if (checkIfFromOutSide("CellSize")) {
        ui->checkBox_CellSize->setChecked(true);
    }


    QStringList optionList;
    optionList << "Replace" << "KeepValue" << "KeepLowerValue" << "KeepHigherValue" << "Add";
    foreach(QString s, optionList)
        ui->comboBox_option->addItem(s);


    if (!QString::fromStdString(m->getParameterAsString("PlacementOption")).isEmpty())
        ui->comboBox_option->setCurrentIndex(optionList.indexOf(QString::fromStdString(m->getParameterAsString("PlacementOption"))));
    std::map<std::string ,RasterData * >  maps = m->getParameter<std::map<std::string, RasterData * > >("InputRasterData");
    for (std::map<std::string ,RasterData * >::iterator it = maps.begin(); it != maps.end(); ++it)
        ui->listWidget_InputRasterData->addItem(QString::fromStdString(it->first));
    QObject::connect(ui->pushButton_addInputRasterData, SIGNAL(clicked()), this, SLOT(addRasterData()));
    QObject::connect(ui->pushButton_RExpression, SIGNAL(clicked()), this, SLOT(addR()));
    QObject::connect(ui->pushButton_rExpression, SIGNAL(clicked()), this, SLOT(addr()));
    QObject::connect(ui->pushButton_maxExpression, SIGNAL(clicked()), this, SLOT(addMax()));

    QObject::connect(ui->checkBox_Height, SIGNAL(clicked()), this, SLOT(fromOutSide()));
    QObject::connect(ui->checkBox_Width, SIGNAL(clicked()), this, SLOT(fromOutSide()));
    QObject::connect(ui->checkBox_CellSize, SIGNAL(clicked()), this, SLOT(fromOutSide()));


}
void GUIMarker::addR() {
    GUIEquationEditor * dlg = new GUIEquationEditor(this->FormulaR, QStringList(), this);    
    QObject::connect(dlg, SIGNAL(values(QStringList)), this, SLOT(addRtoVIBe(QStringList)));
    dlg->addString(ui->lineEdit_RExpression->text());
    dlg->show();
}
void GUIMarker::addRtoVIBe(QStringList list) {
    m->setParameterValue("RExpression", list[1].toStdString());
    ui->lineEdit_RExpression->setText(list[1]);
}
void GUIMarker::addr() {
    GUIEquationEditor * dlg = new GUIEquationEditor(this->Formular, QStringList(), this);
    QObject::connect(dlg, SIGNAL(values(QStringList)), this, SLOT(addrtoVIBe(QStringList)));
    dlg->addString(ui->lineEdit_rExpression->text());
    dlg->show();
}
void GUIMarker::addrtoVIBe(QStringList list) {
    m->setParameterValue("rExpression", list[1].toStdString());
    ui->lineEdit_rExpression->setText(list[1]);
}
void GUIMarker::addMax() {
    GUIEquationEditor * dlg = new GUIEquationEditor(this->FormulaMax, QStringList(), this);
    QObject::connect(dlg, SIGNAL(values(QStringList)), this, SLOT(addMaxtoVIBe(QStringList)));
    dlg->show();
}
void GUIMarker::addMaxtoVIBe(QStringList list) {
    m->setParameterValue("maxExpression", list[1].toStdString());
    ui->lineEdit_maxExpression->setText(list[1]);
}
void GUIMarker::addRasterData() {
    bool ok;
    std::cout << "AddRasterData" << std::endl;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Name:"), QLineEdit::Normal, "" , &ok);
    if (ok && !text.isEmpty()) {
        m->appendToUserDefinedParameter("InputRasterData", text.toStdString());
        std::cout << text.toStdString() << std::endl;
        ui->listWidget_InputRasterData->addItem(text);
    }


}
GUIMarker::~GUIMarker()
{
    delete ui;
}
void GUIMarker::accept() {
    this->m->setParameterValue("Height", ui->lineEdit_Height->text().toStdString());
    this->m->setParameterValue("Width", ui->lineEdit_Width->text().toStdString());
    this->m->setParameterValue("CellSize", ui->lineEdit_CellSize->text().toStdString());
    this->m->setParameterValue("Identifier", ui->lineEdit_Idnetifier->text().toStdString());
    this->m->setParameterValue("rExpression", ui->lineEdit_rExpression->text().toStdString());
    this->m->setParameterValue("RExpression", ui->lineEdit_RExpression->text().toStdString());
    this->m->setParameterValue("maxExpression", ui->lineEdit_maxExpression->text().toStdString());
    this->m->setParameterValue("PlacementOption", ui->comboBox_option->currentText().toStdString());
    this->m->setParameterValue("Edges", QString::number(ui->checkBox_Edges->isChecked()).toStdString());
    this->m->setParameterValue("Points", QString::number(ui->checkBox_Points->isChecked()).toStdString());


    QDialog::accept();
}
