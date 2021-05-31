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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblBlockingRules {
constexpr char Name[] = "tblBlockingRules";
TARGOMAN_CREATE_CONSTEXPR(blrID);
TARGOMAN_CREATE_CONSTEXPR(blr_ipbIP);
TARGOMAN_CREATE_CONSTEXPR(blr_ipIPReadable);
TARGOMAN_CREATE_CONSTEXPR(blrStartingTime);
TARGOMAN_CREATE_CONSTEXPR(blrEndingTime);
TARGOMAN_CREATE_CONSTEXPR(blrCause);
TARGOMAN_CREATE_CONSTEXPR(blrCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(blrCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(blrUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(blrStatus);
}
#pragma GCC diagnostic pop

class BlockingRules : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get BlockingRules information")
    bool ORMDELETE("Delete a BlockingRules")
    bool ORMUPDATE("Update blocking rule info by priviledged user")
    quint64 ORMCREATE("Create a new BlockingRule by priviledged user")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,BlockingRules)
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H