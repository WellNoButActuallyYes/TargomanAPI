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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuAPRStatus,
                     New      = 'N',
                     Sent     = 'S',
                     Applied  = 'A',
                     FirstTry = '1',
                     SecondTry= '2',
                     Expired  = 'E',
                     Removed  = 'R',
                     )

TARGOMAN_DEFINE_ENUM(enuApprovalType,
                     Email      = 'E',
                     Mobile     = 'M',
                     )
}

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblApprovalRequest {
constexpr char Name[] = "tblApprovalRequest";
TARGOMAN_CREATE_CONSTEXPR(aprID);
TARGOMAN_CREATE_CONSTEXPR(apr_usrID);
TARGOMAN_CREATE_CONSTEXPR(aprRequestedFor);
TARGOMAN_CREATE_CONSTEXPR(aprApprovalCode);
TARGOMAN_CREATE_CONSTEXPR(aprApprovalValue);
TARGOMAN_CREATE_CONSTEXPR(aprRequestDate);
TARGOMAN_CREATE_CONSTEXPR(aprApplyDate);
TARGOMAN_CREATE_CONSTEXPR(aprStatus);
}
#pragma GCC diagnostic pop

class ApprovalRequest : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get ApprovalRequest information.")
    bool ORMDELETE("Delete an ApprovalRequest")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,ApprovalRequest)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuAPRStatus::Type);
TAPI_DECLARE_METATYPE(TAPI::enuApprovalType::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H