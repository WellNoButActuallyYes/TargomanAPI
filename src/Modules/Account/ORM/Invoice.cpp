/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Invoice.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void Invoice::init()
{

}

QVariant Invoice::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Invoice::Invoice() :
    intfTable("AAA",
              "tblIPBin",
              "ipb",
              { ///<ColName             Validation                                  Sort   Filter AS  RO    PK
                {"invID",               QFV.integer().minValue(1),                  true,  true,  "", true, true},
                {"invCreationDateTime", QFV.dateTime(),                             true,  true,  "", true},
                {"invServiceCode",      QFV.asciiAlNum().minLenght(4).maxLenght(4)},
                {"inv_usrID",           QFV.integer().minValue(1),                  true,  true,  "", true, true},
                {"invDesc",             QFV.allwaysInvalid(),                       false, false},
                {"invPaymentType",      QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuInvoiceType::options().join("|"))))},
                {"invStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuInvoiceStatus::options().join("|"))))},
              },
              { ///< Col       Reference Table   ForeignCol
                {"inv_usrID",  "AAA.tblUser",    "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceStatus);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceType);

    this->registerMyRESTAPIs();
}


