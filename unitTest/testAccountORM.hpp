/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */
#ifndef TESTACCOUNTORM_HPP
#define TESTACCOUNTORM_HPP

#include "testCommon.hpp"
#include "Helpers/AAA/clsJWT.hpp"

using namespace Targoman::API::Helpers::AAA;

class testAccountORM: public clsBaseTest
{
    Q_OBJECT

private slots:
    void initTestCase(){

    }

    void cleanupTestCase(){

    }

    void unprivActionLogs(){
        QVERIFY(callAPI(GET, "Account/ActionLogs").toString().isEmpty());
    }

    void ActiveSessions(){
        QVERIFY(callAPI(GET, QString("Account/ActiveSessions/")).toString().isEmpty());
        QVERIFY(callAPI(GET, QString("Account/ActiveSessions/%1").arg(clsJWT(gJWT).session()), {{"ssn_usrID",gUserID}, {"cols", "ssnKey"}}).toMap().value("ssnKey") == clsJWT(gJWT).session());
        QVERIFY(callAPI(GET,
                        QString("Account/ActiveSessions/"),
                        {
                            {"filters",QString("ssn_usrID=%1 + ssnKey='%2'").arg(gUserID).arg(clsJWT(gJWT).session())},
                            {"cols", "ssnKey"}
                        }
                        ).toMap().value("rows").toList().value(0).toMap().value("ssnKey")== clsJWT(gJWT).session());
    }
};


#endif // TESTACCOUNTORM_HPP