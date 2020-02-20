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

#include "Locations.h"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Locations::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool Locations::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool Locations::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 Locations::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

Locations::Locations() :
    clsTable(AdvertSchema,
              tblLocations::Name,
              { ///<ColName                         Type                 Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
                {tblLocations::locID,               S(quint32),          QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {tblLocations::locURL,              S(TAPI::URL_t),      QFV.integer().minValue(1),      QInvalid, UPAdmin},
                {tblLocations::locPlaceCode,        S(TAPI::URL_t),      QFV.integer().minValue(1),      QInvalid, UPAdmin},
                {tblLocations::locCreatedBy_usrID,  ORM_CREATED_BY},
                {tblLocations::locCreationDateTime, ORM_CREATED_ON},
                {tblLocations::locUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblLocations::locStatus,           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active},
              },
              { ///< Col                            Reference Table                ForeignCol        Rename      LeftJoin
                {tblLocations::locCreatedBy_usrID,  ORM_JOIN_CREATOR},
                {tblLocations::locUpdatedBy_usrID,  ORM_JOIN_UPDATER},
              })
{
}

}
}
}