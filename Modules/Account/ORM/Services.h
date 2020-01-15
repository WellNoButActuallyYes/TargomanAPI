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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

class Services : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get Service information")
    bool ORMDELETE("Delete a Service")
    bool ORMUPDATE("Update Service info by priviledged user")
    quint64 ORMCREATE("Create a new Service by priviledged user")

private:
    Services();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,Services);
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H