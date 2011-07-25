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
#include "guisinglevalue.h"
#include "ui_guisinglevalue.h"
#include "QInputDialog"
#include "module.h"
#include <guiequationeditor.h>

GUISingleValue::GUISingleValue(vibens::Module * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUISingleValue)
{
    this->m = m;
    ui->setupUi(this);
    std::map<std::string ,double >  maps = m->getParameter<std::map<std::string,double > >("VaraiblesFromOutSide");
    for (std::map<std::string ,double >::iterator it = maps.begin(); it != maps.end(); ++it) {
        ui->listWidget->addItem(QString::fromStdString(it->first));
        values <<     QString::fromStdString(it->first);
    }

    ui->lineEdit_Formula->setText( QString::fromStdString(m->getParameterAsString("Value")) );
    QObject::connect(ui->pushButton_addVariable, SIGNAL(clicked()), this, SLOT(addVariable()));
    QObject::connect(ui->pushButton_Formula, SIGNAL(clicked()), this, SLOT(addFormula()));

}

GUISingleValue::~GUISingleValue()
{
    delete ui;
}

void GUISingleValue::addVariable() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Name:"), QLineEdit::Normal, "" , &ok);
    if (ok && !text.isEmpty()) {
        m->appendToUserDefinedParameter("VaraiblesFromOutSide", text.toStdString());
        std::cout << text.toStdString() << std::endl;
        ui->listWidget->addItem(text);
        values <<  text;
    }

}
void GUISingleValue::accept() {
    this->m->setParameterValue("Value", ui->lineEdit_Formula->text().toStdString());
    QDialog::accept();

}

void GUISingleValue::addFormula() {
    GUIEquationEditor * dlg = new GUIEquationEditor(values, QStringList(), this);
    dlg->hideName();
    dlg->setFormula(QString::fromStdString(m->getParameterAsString("Value")));
    QObject::connect(dlg, SIGNAL(values(QStringList)), this, SLOT(addFormulaToLineEdit(QStringList)));
    dlg->show();
}
void GUISingleValue::addFormulaToLineEdit(QStringList list) {
    ui->lineEdit_Formula->setText(list[1]);
}
