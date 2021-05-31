/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "IPBin.h"
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuIPBinStatus);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant IPBin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

IPBin::IPBin() :
    clsTable(AAASchema,
              tblIPBin::Name,
              { ///<ColName                     Type                 Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblIPBin::ipbIP,               ORM_PRIMARY_KEY32},
                {tblIPBin::ipbReadable,         S(TAPI::IPv4_t),     QFV,                                QInvalid,  UPNone},
                {tblIPBin::ipbFirstAccess,      S(TAPI::DateTime_t), QFV,                                QAuto,     UPNone},
                {tblIPBin::ipbAccessCount,      S(quint64),          QFV.integer().minValue(0),          0,         UPNone},
                {tblIPBin::ipbLastAccess,       S(TAPI::DateTime_t), QFV,                                QAuto,     UPNone},
                {tblIPBin::ipbBlockedBy_usrID,  S(quint64),          QFV.integer().minValue(1),          QNull,     UPNone},
                {tblIPBin::ipbBlockingTime,     S(TAPI::DateTime_t), QFV,                                QNull,     UPNone},
                {tblIPBin::ipbStatus,           S(TAPI::enuIPBinStatus::Type), QFV,                      TAPI::enuIPBinStatus::Active,UPStatus},
              },
              { ///< Col                        Reference Table             ForeignCol Rename      LeftJoin
                {tblIPBin::ipbBlockedBy_usrID,  R(AAASchema,tblUser::Name), tblUser::usrID,   "Blocker_", true},
              })
{
}

}
}
}
