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

#ifndef TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP
#define TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"
#include "libTargomanDBM/clsDAC.h"

namespace Targoman {
namespace API {

TARGOMAN_DEFINE_ENHANCED_ENUM (enuAdvertOrder,
                               First  = '1',
                               Second = '2',
                               Third  = '3',
                               HugeRequest = 'H',
                               Normal = 'R'
                               );

TARGOMAN_DEFINE_ENHANCED_ENUM (enuAdvertType,
                               Text,
                               Image
                               );

TARGOMAN_DEFINE_ENHANCED_ENUM (enuBannerSizes,
                               MedimRectangle_300x250   = 'A',
                               LargeRectangle_336x280   = 'B',
                               LeaderBoard_728x90       = 'C',
                               HalfPage_300x600         = 'D',
                               WideSkyscrapper_160x600  = 'E',
                               Skyscrapper_120x600      = 'F',
                               Square_250x250           = 'G',
                               SmallSquare_200x200      = 'H',
                               MainBanner_468x60        = 'I',
                               Portrait_300x1050        = 'J',
                               Billboard_970x250        = 'K',
                               LargeLeaderBoard_970x90  = 'L',
                               HalfBanner_234x60        = 'M',
                               VerticalBanner_120x240   = 'N',
                               SmallRectangle_180x150   = 'O',

                               Mobile_320x50            = 'P',
                               LargeMobile_320x100      = 'Q',
                               );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
using namespace DBManager;
static clsDAC& AdvertDACInstance(){static clsDAC* Instance = nullptr; return *(Q_LIKELY(Instance) ? Instance : (Instance = new clsDAC("Advert")));}
#pragma GCC diagnostic pop

}
}

Q_DECLARE_METATYPE(Targoman::API::enuAdvertOrder::Type);
Q_DECLARE_METATYPE(Targoman::API::enuAdvertType::Type);
Q_DECLARE_METATYPE(Targoman::API::enuBannerSizes::Type);

#endif // TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP
