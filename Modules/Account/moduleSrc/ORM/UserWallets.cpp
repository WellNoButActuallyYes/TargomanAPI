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

#include "UserWallets.h"
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserWalletStatus);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;
using namespace DBManager;

QVariant UserWallets::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblUserWallets::wal_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool UserWallets::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    TAPI::ORMFields_t ExtraFilters;

    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false){
        ExtraFilters.insert(tblUserWallets::walDefault, 0);
        this->setSelfFilters({{tblUserWallets::wal_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);
    }
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, ExtraFilters);
}

bool UserWallets::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint64 UserWallets::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false){
        _createInfo.insert(tblUserWallets::walDefault, 0);
        this->setSelfFilters({{tblUserWallets::wal_usrID, clsJWT(_JWT).usrID()}}, _createInfo);
    }

    return this->create(CREATE_METHOD_CALL_ARGS).toULongLong();
}

bool UserWallets::apiUPDATEdefaultWallet(TAPI::JWT_t _JWT, quint64 _walID){
    bool IsPrivileged = Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    clsDACResult Result = this->execQuery(
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET " + tblUserWallets::walDefault + "= 1, " + tblUserWallets::walUpdatedBy_usrID + " = ?"
                              + QUERY_SEPARATOR
                              + "WHERE "
                              + (IsPrivileged ? "TRUE" : (tblUserWallets::wal_usrID + QStringLiteral(" = ? ")))
                              + " AND walID = ?",
                              (IsPrivileged ?
                                   QVariantList({ clsJWT(_JWT).usrID(), _walID }) :
                                   QVariantList({ clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID(), _walID })
                                   )
                              );

    return Result.numRowsAffected() > 0;
}

bool UserWallets::apiCREATEtransfer(TAPI::JWT_t _JWT,
                                    const QString& _destLogin,
                                    quint32 _amount,
                                    const TAPI::MD5_t& _pass,
                                    const QString& _salt,
                                    quint64 _walID){

    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_destLogin, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP("AAA.sp_CREATE_transfer", {
                     {"iFromUserID", clsJWT(_JWT).usrID()},
                     {"iFromWalID", _walID},
                     {"iToUserLogin", _destLogin},
                     {"iAmount", _amount},
                     {"iPass", _pass},
                     {"iSalt", _salt},
                 });
    return true;
}

UserWallets::UserWallets() :
    clsTable(AAASchema,
              tblUserWallets::Name,
              { ///<ColName                           Type                        Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblUserWallets::walID,               ORM_PRIMARY_KEY64},
                {tblUserWallets::wal_usrID,           S(quint64),                 QFV.integer().minValue(1),          QRequired,  UPNone},
                {tblUserWallets::walName,             S(QString),                 QFV.unicodeAlNum().maxLenght(100),  "default",  UPOwner},
                {tblUserWallets::walDefault,          S(bool),                    QFV,                                false,      UPOwner},
                {tblUserWallets::walMinBalance,       S(qint64),                  QFV,                                0,          UPAdmin,false,false},
                {tblUserWallets::walNotTransferable,  S(qint64),                  QFV,                                0,          UPAdmin,false,false},
                {tblUserWallets::walMaxTransferPerDay,S(qint64),                  QFV,                                10000000,   UPAdmin,false,false},
                {tblUserWallets::walLastBalance,      S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblUserWallets::walSumIncome,        S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblUserWallets::walSumExpenses,      S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblUserWallets::walSumCredit,        S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblUserWallets::walSumDebit,         S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblUserWallets::walCreatedBy_usrID,  ORM_CREATED_BY},
                {tblUserWallets::walCreationDateTime, ORM_CREATED_ON},
                {tblUserWallets::walUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblUserWallets::walStatus,           S(TAPI::enuUserWalletStatus::Type), QFV,                        TAPI::enuUserWalletStatus::Active, UPStatus},
              },
              { ///< Col                              Reference Table               ForeignCol          Rename   LeftJoin
                {tblUserWallets::wal_usrID,           R(AAASchema,tblUser::Name),   tblUser::usrID},
                {tblUserWallets::walCreatedBy_usrID,  ORM_JOIN_CREATOR},
                {tblUserWallets::walUpdatedBy_usrID,  ORM_JOIN_UPDATER},
              })
{
}

}
}
}
