#ifndef TEST_ACCOUNT_ORM_IPBin_HPP
#define TEST_ACCOUNT_ORM_IPBin_HPP

#include "testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "defs.hpp"

using namespace Targoman::API::AAA;

class testIPBin: public clsBaseTest
{
    Q_OBJECT

private slots:
    void IPBin_CREATE_Unprivileged(){
        QVERIFY(callAPI(PUT, QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                             }) == gInvalid);
    }

    void IPBin_CREATE_Admin(){
        QVERIFY(callAdminAPI(PUT, QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                                 {"rolPrivileges", QJsonArray({"12"})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                                 {"rolPrivileges", QJsonObject()}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                                 {"rolPrivileges", QJsonObject({{"ALL",1}})}
                             }) == gInvalid);
        QVERIFY((gServiceRoleID = callAdminAPI(PUT, 
QString("Account/IPBin/"),{},{
                                {"rolName", UT_RoleName},
                                 {"rolPrivileges", 
QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }).toUInt()) > 0);
    }

    void IPBin_UPDATE_Unprivileged(){
        QVERIFY(callAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", 
QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }) == gInvalid);
    }

    void IPBin_UPDATE_Admin(){
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonArray({"12"})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject()}
                             }) .toBool());
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject({{"ALL",1}})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PATCH, 
QString("Account/IPBin/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", 
QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }).toBool());
    }
