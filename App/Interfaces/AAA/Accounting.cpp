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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#include "Authorization.h"
#include "PrivHelpers.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

void checkCredit(const QJsonObject& _privs, const QString& _selector, double _value)
{
    QVariant Credit = PrivHelpers::getPrivValue(_privs, _selector);
    if(Credit.isValid() &&
       Credit.canConvert<double>() &&
       Credit.toDouble() >= 0 &&
       Credit.toDouble() < _value)
        throw exAccounting("You have exceeded your "+_selector+" quota.");
}

}
}
}
}
