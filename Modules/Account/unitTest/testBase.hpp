/******************************************************************************
 * Targoman: A robust Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2018 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef UNITTEST_H
#define UNITTEST_H

#include <QtTest/QtTest>

#include "Modules/Account/libsrc/Account.h"

class testBase: public QObject
{
    Q_OBJECT
public:
    testBase() :
        Instance(Account::instance())
    {
        Instance().init();
    }
private slots:
    void signup(){

    }

    void login(){
        QVERIFY()
        /*QVERIFY(VERIFY_NORMALIZE("fa",
        "خانه ی",
        "خانه"
        ));*/
    };

private:
    Account& Instance;
};

#endif // UNITTEST_H