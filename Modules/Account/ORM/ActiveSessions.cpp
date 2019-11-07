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

#include "ActiveSessions.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void ActiveSessions::init()
{;}

QVariant ActiveSessions::apiGET(GET_METHOD_ARGS_IMPL)
{
    bool IsSelf = _EXTRAPATH.split(',').size() == 2 && clsJWT(_JWT).usrID() != _EXTRAPATH.split(',').last().toUInt();
    if(IsSelf == false)
        Authorization::checkPriv(_JWT,{"Account:ActiveSessions:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {},
                                 IsSelf ? "" : QString("+ssn_usrID=%1").arg(clsJWT(_JWT).usrID()),
                                 GET_METHOD_CALL_ARGS);
}

ActiveSessions::ActiveSessions() :
    clsTable("AAA",
              "tblActiveSessions",
              { ///<ColName            Validation                    Sort   Filter RO   PK
                {"ssnKey",              QFV.md5(),                   true,  true, true, true},
                {"ssn_usrID",           QFV.integer().minValue(1),   true,  true, true, true},
                {"ssnCreationDateTime", QFV.dateTime(),              true,  true, true},
                {"ssnIP",               QFV.integer().minValue(1),   true,  true, true},
                {"ssnIPReadable",       QFV.allwaysInvalid(),        false,false, true},
                {"ssnInfo",             QFV.json(),                  false,false, true},
                {"ssnLastActivity",     QFV.integer().minValue(1),   true,  true, true},
                {"ssnRemember",         QFV.boolean(),               true,  true, true},
                {"ssnUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"ssnStatus",           QFV_Enum(enuSessionStatus)},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"ssn_usrID",          "AAA.tblUser",      "usrID",     "Owner_"},
                {"ssnUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuSessionStatus);
    this->registerMyRESTAPIs();
}

}
}
}
