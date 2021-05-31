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

#include "Accounting.h"
#include "Defs.hpp"
#include "Locations.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;
using namespace AAA;
using namespace AAA::Accounting;
using namespace Targoman::API::AAA::Accounting;

/******************************************************/
AccountProducts::AccountProducts()
    : intfAccountProducts(AdvertSchema,
        {///< ColName                                    Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
            { tblAccountProducts::prd_locID,             S(quint32), QFV.integer().minValue(1),  QRequired, UPOwner },
            { tblAccountProducts::prdShowPerDay,         S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { tblAccountProducts::prdShowTotal,          S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
            { tblAccountProducts::prdClicksPerDay,       S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { tblAccountProducts::prdClicksPerMonth,     S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { tblAccountProducts::prdClicksTotal,        S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
        },
        {///< Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            { tblAccountProducts::prd_locID,   R(AdvertSchema, tblLocations::Name), tblLocations::locID },
        }
    )
{;}

/******************************************************/
AccountSaleables::AccountSaleables()
    : intfAccountSaleables(AdvertSchema)
{;}

/******************************************************/
AccountUserAssets::AccountUserAssets()
    : intfAccountUserAssets(AdvertSchema,
        {///< ColName                                 Type        Validation                  Default  UpBy    Sort   Filter Self  Virt   PK
            ADVERT_DEFINE_ASSET_FIELDS("uas")
        })
{;}

/******************************************************/
AccountAssetUsage::AccountAssetUsage()
    : intfAccountAssetUsage(AdvertSchema,
        {///< ColName                                 Type        Validation                  Default  UpBy    Sort   Filter Self  Virt   PK
            ADVERT_DEFINE_ASSET_FIELDS("usg")
        }
    )
{;}

/******************************************************/
AccountCoupons::AccountCoupons()
    : intfAccountCoupons(AdvertSchema)
{;}

}
}
}