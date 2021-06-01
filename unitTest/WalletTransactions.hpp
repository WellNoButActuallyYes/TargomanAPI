#ifndef TEST_ACCOUNT_ORM_WalletTransactions_HPP
#define TEST_ACCOUNT_ORM_WalletTransactions_HPP

#include "testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "defs.hpp"

using namespace Targoman::API::AAA;

class testWalletTransactions: public clsBaseTest
{
    Q_OBJECT

private slots:
    void WalletTransactions_CREATE_Unpriviledged(){
        QVERIFY(callAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                {"aptToken", UT_NormalToken},
                             }) == gInvalid);
    }

    void WalletTransactions_CREATE_Admin(){
        QVERIFY(callAdminAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                               {"aptToken", UT_NormalToken},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", gServiceID},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", gServiceID},
                                 {"apt_usrID", 0},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/WalletTransactions/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", 0},
                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                             }) == gInvalid);


        QVERIFY((gAPITokenID =  callAdminAPI(PUT, 
QString("Account/WalletTransactions/"),{},{
                                                 {"aptToken", UT_NormalToken},
                                                 {"apt_svcID", gServiceID},
                                                 {"apt_usrID", 
clsJWT(gJWT).usrID()},
                                             }).toUInt()) > 0);
        QVERIFY((gAPIAdminTokenID =  callAdminAPI(PUT, 
QString("Account/WalletTransactions/"),{},{
                                                 {"aptToken", UT_AdminToken},
                                                 {"apt_svcID", gServiceID},
                                                 {"apt_usrID", 
clsJWT(gAdminJWT).usrID()},
                                             }).toUInt()) > 0);
    }

    void WalletTransactions_UPDATE_Unprivileged(){
        QVERIFY(callAPI(PATCH, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID),{},{
                                 {"aptStatus", "Removed"}
                             })== gInvalid);
        QVERIFY(callAPI(PATCH, 
QString("Account/WalletTransactions/%1").arg(gAPIAdminTokenID),{},{
                                 {"aptStatus", "Removed"}
                             })== gInvalid);
    }

    void WalletTransactions_UPDATE_Admin(){
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/WalletTransactions/%1").arg(gAPIAdminTokenID),{},{
                                 {"aptStatus", "Removed"}
                             }).toBool());
    }

    void WalletTransactions_DELETE_Unprivileged(){
        QVERIFY(callAPI(DELETE, 
QString("Account/WalletTransactions/%1").arg(gAPIAdminTokenID)) == gInvalid);
        QVERIFY(callAPI(DELETE, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID)).toBool());
    }

    void WalletTransactions_DELETE_Admin(){
        QVERIFY(callAdminAPI(DELETE, QString("Account/WalletTransactions/")) == 
gInvalid);
        QVERIFY(callAdminAPI(DELETE, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID)).toBool() == false);
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
        QVERIFY(callAdminAPI(DELETE, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID)).toBool());
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/WalletTransactions/%1").arg(gAPITokenID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
    }

