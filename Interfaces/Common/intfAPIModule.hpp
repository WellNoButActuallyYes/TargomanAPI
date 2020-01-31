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

#ifndef TARGOMAN_API_INTFAPIMODULE_HPP
#define TARGOMAN_API_INTFAPIMODULE_HPP

#include <QObject>
#include <QtPlugin>

#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"

#include "QHttp/qhttpfwd.hpp"

#include "Interfaces/Common/HTTPExceptions.h"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/ORM/clsORMField.h"

namespace Targoman {
namespace API {

/**********************************************************************/
/** @TODO document QT_NO_CAST_FROM_ASCII */
/**********************************************************************/
/**
  * @brief CACHEABLE macros are predefined macros in order to mark each API cache TTL. You can add more cache time as you wish while
  *        following cache definition pattern "\d+(S|M|H)" where last character means S: Seconds, M: Minutes, H: Hours and digits must be between 0 to 16384
  */
#ifndef Q_MOC_RUN
#  define CACHEABLE_1S
#  define CACHEABLE_3S
#  define CACHEABLE_5S
#  define CACHEABLE_10S
#  define CACHEABLE_30S
#  define CACHEABLE_1M
#  define CACHEABLE_5M
#  define CACHEABLE_10M
#  define CACHEABLE_1H
#  define CACHEABLE_3H
#  define CACHEABLE_6H
#  define CACHEABLE_12H
#  define CACHEABLE_24H
#  define CACHEABLE_INF
#endif

/**
  * @brief CENTRALCACHE macros are predefined macros in order to mark each API central cache TTL. You can add more cache time as you wish while
  *        following cache definition pattern "\d+(S|M|H)" where last character means S: Seconds, M: Minutes, H: Hours and digits must be between 0 to 16384
  */
#ifndef Q_MOC_RUN
#  define CENTRALCACHE_1S
#  define CENTRALCACHE_3S
#  define CENTRALCACHE_5S
#  define CENTRALCACHE_10S
#  define CENTRALCACHE_30S
#  define CENTRALCACHE_1M
#  define CENTRALCACHE_5M
#  define CENTRALCACHE_10M
#  define CENTRALCACHE_1H
#  define CENTRALCACHE_3H
#  define CENTRALCACHE_6H
#  define CENTRALCACHE_12H
#  define CENTRALCACHE_24H
#endif

#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return #_doc; }
#define ASYNC_API(_method, _name, _sig, _doc) asyncApi##_method##_name _sig;QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return #_doc; }

#define INTFAPIMODULE_IID "TARGOMAN.API.INTFAPIMODULE/1.0.0"

class intfAPIModule : public Targoman::Common::Configuration::intfModule
{
public:
    struct stuDBInfo{
        QString Host;
        quint16 Port;
        QString User;
        QString Pass;
        QString Schema;

        stuDBInfo(QString _schema = "", quint16 _port = 0, QString _host = "", QString _user = "", QString _pass = "") :
            Host(_host),
            Port(_port),
            User(_user),
            Pass(_pass),
            Schema(_schema)
        {;}

        QString toConnStr(bool _noSchema = false){
            return QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5").arg(
                                                  this->Host).arg(
                                                  this->Port).arg(
                                                  this->User).arg(
                                                  this->Pass).arg(
                                                  _noSchema ? "" : this->Schema);
        }
    };

public:
    intfAPIModule(Targoman::Common::Configuration::intfModule *_parent = nullptr) :
        Targoman::Common::Configuration::intfModule(_parent)
    {;}
    virtual ~intfAPIModule() {}
    virtual QList<ORM::clsORMField> filterItems(qhttp::THttpMethod _method = qhttp::EHTTP_ACL) {Q_UNUSED(_method) return {};}
    virtual void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID) {Q_UNUSED(_fieldTypeName) Q_UNUSED(_typeID)}
    virtual QList<QMetaMethod> listOfMethods() const = 0;
    virtual stuDBInfo requiredDB() const {return {};}
    virtual bool requiresTextProcessor() const {return false;}
    virtual bool requiresFormalityChecker() const {return false;}

    virtual void init() = 0;

    /**
     * @brief createSignedJWT creates an string containing HEADER.PAYLOAD.SIGNATURE as described by JWT standard.
     * @param _payload The payload to include in JWT. The payload object must not include enteries with following keys:
     *        - iat: reserved for initial time
     *        - exp: reserved for expiration time
     *        - jti: reserved for JWT session
     *        - prv: reserved for private payload
     * @param _privatePayload Optinally private object that will be included in JWT encrypted. There will be no restriction on key values
     * @param _expiry optinally a time in seconds for max life time
     * @param _sessionID optinally a session key for each user to be stored in `jti`
     * @return a base64 encoded string in form of HEADER.PAYLOAD.SIGNATURE
     */
    //TAPI::EncodedJWT_t createSignedJWT(QJsonObject _payload, QJsonObject _privatePayload = QJsonObject(), const qint32 _expiry = -1, const QString& _sessionID = QString());
};

}
}
Q_DECLARE_INTERFACE(Targoman::API::intfAPIModule, INTFAPIMODULE_IID)

#define TARGOMAN_DEFINE_API_MODULE(_name) \
public: \
    static QString moduleFullNameStatic(){return Targoman::Common::demangle(typeid(_name).name());}\
    QString moduleFullName(){return _name::moduleFullNameStatic();}\
    static _name& instance() {return *(reinterpret_cast<_name*>(_name::moduleInstance()));} \
    static Targoman::Common::Configuration::intfModule* moduleInstance(){static _name* Instance = NULL; return Q_LIKELY(Instance) ? Instance : (Instance = new _name);} \
    static QString moduleName(){return QStringLiteral(TARGOMAN_M2STR(_name));}  \
    QString moduleBaseName(){return _name::moduleName();} \
    QList<QMetaMethod> listOfMethods() const { \
        QList<QMetaMethod> Methods; \
        for (int i=0; i<this->metaObject()->methodCount(); ++i) \
            Methods.append(this->metaObject()->method(i)); \
        return Methods; \
    } \
private: \
    Q_DISABLE_COPY(_name) \

#define TARGOMAN_DEFINE_API_SUBMODULE(_module, _name) \
public: \
    static QString moduleFullNameStatic(){return Targoman::Common::demangle(typeid(_name).name());}\
    virtual QString moduleFullName(){return _name::moduleFullNameStatic();}\
    static _name& instance() {return *(reinterpret_cast<_name*>(_name::moduleInstance()));} \
    static Targoman::Common::Configuration::intfModule* moduleInstance(){static _name* Instance = NULL; return Q_LIKELY(Instance) ? Instance : (Instance = new _name);} \
    static QString moduleName(){return QStringLiteral(TARGOMAN_M2STR(TARGOMAN_CAT_BY_SLASH(_module,_name)));}  \
    QString moduleBaseName(){return _name::moduleName();} \
    QList<QMetaMethod> listOfMethods() const { \
        QList<QMetaMethod> Methods; \
        for (int i=0; i<this->metaObject()->methodCount(); ++i) \
            Methods.append(this->metaObject()->method(i)); \
        return Methods; \
    } \
private: \
    Q_DISABLE_COPY(_name) \

#define TARGOMAN_API_MODULE_DB_CONFIGS(_module) \
    struct DB{ \
        static inline QString makeConfig(const QString& _name){return TARGOMAN_M2STR(_module) + _name;} \
        static Common::Configuration::tmplConfigurable<QString>      Host;   \
        static Common::Configuration::tmplRangedConfigurable<quint16>Port;   \
        static Common::Configuration::tmplConfigurable<QString>      User;   \
        static Common::Configuration::tmplConfigurable<QString>      Pass;   \
        static Common::Configuration::tmplConfigurable<QString>      Schema; \
    };

#define TARGOMAN_API_MODULE_DB_CONFIG_IMPL(_module)     \
    using namespace Targoman::Common::Configuration;    \
    tmplConfigurable<QString> _module::DB::Host(        \
            _module::DB::makeConfig("Host"),            \
            "Database Host address", "127.0.0.1");      \
    tmplRangedConfigurable<quint16> _module::DB::Port(  \
            _module::DB::makeConfig("Port"),            \
            "Database port", 10, 65000, 3306);          \
    tmplConfigurable<QString> _module::DB::User(        \
            _module::DB::makeConfig("User"), "Database username for connection", \
            "root");                                   \
    tmplConfigurable<QString> _module::DB::Pass(        \
            _module::DB::makeConfig("Pass"),            \
            "Database password", "");                   \
    tmplConfigurable<QString> _module::DB::Schema(      \
            _module::DB::makeConfig("Schema"),          \
            "Database schema");

#endif // TARGOMAN_API_INTFAPIMODULE_HPP