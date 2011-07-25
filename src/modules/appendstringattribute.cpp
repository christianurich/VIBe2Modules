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
#include "appendstringattribute.h"
#include "vectordatahelper.h"

VIBe_DECLARE_NODE_NAME(AppendStringAttribute, Modules)


AppendStringAttribute::AppendStringAttribute()
{
    this->addParameter("Name", VIBe2::STRING, &this->Name);
    this->addParameter("Text", VIBe2::STRING, &this->Text);
    this->addParameter("Identifier", VIBe2::STRING, & this->Identifier);
    this->addParameter("VectorDataIn", VIBe2::VECTORDATA_IN, & this->VecIn);
    this->addParameter("VectorDataOut", VIBe2::VECTORDATA_OUT, & this->VecOut);
}

void AppendStringAttribute::run()
{
    *(VecOut) = *(VecIn);

    std::vector<std::string> names = VectorDataHelper::findElementsWithIdentifier(this->Identifier, VecIn->getAttributeNames());
    foreach (std::string name, names) {
        Attribute attr = VecOut->getAttributes(name);
        attr.setAttribute(Name, Text);
        this->VecOut->setAttributes(name, attr);
    }
}
