#ifndef TEST_ACCOUNT_ORM_IPSTATS_HPP
#define TEST_ACCOUNT_ORM_IPSTATS_HPP

#include "testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "defs.hpp"

using namespace Targoman::API::AAA;

class testIPStats: public clsBaseTest
{
    Q_OBJECT

private slots:
    void IPStats_CREATE_Unpriviledged(){
        QVERIFY(callAPI(PUT, QString("Account/IPStats/"),{},{
                                {"svcName", UT_IPStatsName},
                             }) == gInvalid);
    }

    void IPStats_CREATE_Admin(){
        QVERIFY(callAdminAPI(PUT, QString("Account/IPStats/"),{},{
                                               {"svcName", UT_IPStatsName},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/IPStats/"),{},{
                                 {"svcName", UT_IPStatsName},
                                 {"svc_rolID", 0},
                             }) == gInvalid);
        QVERIFY((gIPStatsID =  callAdminAPI(PUT, 
QString("Account/IPStats/"),{},{
                                               {"svcName", UT_IPStatsName},
                                               {"svc_rolID", gIPStatsRoleID},
                             }).toUInt()) > 0);
    }

    void IPStats_UPDATE_Unprivileged(){
        QVERIFY(callAPI(PATCH, 
QString("Account/IPStats/%1").arg(gIPStatsID),{},{
                                 {"svcStatus", "Removed"}
                             }) == gInvalid);
    }


    void IPStats_GET_Unpriviledged(){
        QVERIFY(callAPI(GET, QString("Account/IPStats/")).toMap().isEmpty());
        QVERIFY(callAPI(GET,
                        QString("Account/IPStats/%1").arg(gIPStatsID), {
                            {"rolName",UT_IPStatsName}, {"cols", "rolName"}
                        }) == gInvalid);
    }

    void IPStats_GET_Admin(){
        QVERIFY(callAdminAPI(GET, QString("Account/IPStats/"),{{"cols", 
"svcName"}}).toMap().size());
        QVERIFY(callAdminAPI(GET,
                        QString("Account/IPStats/%1").arg(gIPStatsID), {
                            {"cols", "svcName"}
                        }).toMap().value("svcName").toString() == 
UT_IPStatsName);
    }
};


#endif // TEST_ACCOUNT_ORM_IPSTATS_HPP
 
