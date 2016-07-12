#ifndef MAIN_H_
#define MAIN_H_

#include <libeng/Global.h>  // Include this file first

////// Check valid libeng definition here
#ifndef LIBENG_VERSION_1_1_106
#error "Invalid libeng version: Expected 1.1.106 version"
#endif

#if defined(LIBENG_PORT_AS_LAND) || \
    !defined(LIBENG_ENABLE_STORAGE) || \
    !defined(LIBENG_ENABLE_PROGRESS) || \
    !defined(LIBENG_ENABLE_CAMERA) || \
    !defined(LIBENG_ENABLE_MIC) || \
    !defined(LIBENG_DB_CALC) || \
    !defined(LIBENG_ENABLE_DELTA)
#error "Wrong libeng configuration"
#endif

#if defined(LIBENG_USE_RTTI) || \
    defined(LIBENG_ENABLE_SCROLLING) || \
    defined(LIBENG_ENABLE_TEXT) || \
    defined(LIBENG_ENABLE_SPRITE) || \
    defined(LIBENG_ENABLE_GAMEPAD) || \
    defined(LIBENG_ENABLE_INTERNET) || \
    defined(LIBENG_ENABLE_ADVERTISING) || \
    defined(LIBENG_ENABLE_SOCIAL)
#warning "Unused feature(s) defined"
#endif

//////
#include <libeng/PlatformOS.h>
#include <libeng/Game/Game.h>
#include "CrystalBall.h"

using namespace eng;

#define PROJECT_NAME            "CrystalBall"
#ifdef __ANDROID__
#define JAVA_PROJECT_NAME       "com/studio/artaban/crystalball"

#else // !__ANDROID__
#import "EngResources.h"

BOOL engGetFontGrayscale();
unsigned char engLoadTexture(EngResources* resources, unsigned char Id);
void engLoadSound(EngResources* resources, unsigned char Id);

#ifdef LIBENG_ENABLE_ADVERTISING
#import "EngAdvertising.h"

BOOL engGetAdType();
void engLoadAd(EngAdvertising* ad, GADRequest* request);
void engDisplayAd(EngAdvertising* ad, unsigned char Id);
void engHideAd(EngAdvertising* ad, unsigned char Id);

#endif

#ifdef LIBENG_ENABLE_SOCIAL
typedef enum {

    FIELD_NAME = 0,
    FIELD_GENDER,
    FIELD_BIRTHDAY,
    FIELD_LOCATION

} SocialField;

BOOL engReqInfoField(SocialField field, unsigned char socialID);
#endif

#endif

inline void init(PlatformData& data) {

    data.project = PROJECT_NAME;
    data.file = FILE_NAME;
    data.dimension = false;
    data.game = static_cast<Game*>(CrystalBall::getInstance());
}

#endif // MAIN_H_
