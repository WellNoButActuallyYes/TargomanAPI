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

#ifndef TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H
#define TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H

#include "QHttp/intfRESTAPIHolder.h"

namespace Targoman {
namespace API {

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

class FormalityChecker : private QHttp::intfRESTAPIHolder
{
    Q_OBJECT
public:
    void init();

private slots:
    QString API(, Check, (const QString _text, const QHttp::ISO639_2_t& _lang),
                    "Normalizes input text")

    private:
        FormalityChecker();
    TARGOMAN_DEFINE_SINGLETON_MODULE(FormalityChecker);
};

}
}

#endif // TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H